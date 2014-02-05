/*************** <auto-copyright.rb BEGIN do not edit this line> **************
 *
 * Copyright 2012-2013 by Ames Laboratory
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.rb END do not edit this line> ***************/


// --- AutoData Includes --- //
#include <autodata/dynamic/Struct.h>

// --- Poco Includes --- //
#include <Poco/UUIDGenerator.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/ParseHandler.h>

using namespace Poco::Dynamic;

namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
Struct::Struct()
    :
    m_struct( Poco::Dynamic::Struct< std::string >() )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Var& Struct::operator [](
    std::string const& name )
{
    return m_struct[ name ];
}
////////////////////////////////////////////////////////////////////////////////
Var const& Struct::operator [](
    std::string const& name ) const
{
    return m_struct[ name ];
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Struct< std::string >& Struct::GetStruct()
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Struct< std::string > const& Struct::GetStruct() const
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Struct::GetTypename() const
{
    return m_typename;
}
////////////////////////////////////////////////////////////////////////////////
void Struct::SetTypename(
    std::string name )
{
    m_typename = std::move( name );
}
////////////////////////////////////////////////////////////////////////////////
Var const& Struct::GetID() const
{
    return m_struct[ "id" ];
}
////////////////////////////////////////////////////////////////////////////////
void Struct::SetID(
    Var id )
{
    m_struct[ "id" ] = std::move( id );
}
////////////////////////////////////////////////////////////////////////////////
/*void Struct::CreateID()
{
    Poco::UUIDGenerator gen;
    Poco::UUID id( gen.createOne() );
    SetID( id.toString() );
}
////////////////////////////////////////////////////////////////////////////////
std::string Struct::ToJSON()
{
    std::string json("{ \"typename\" : \"");
    json += m_typename + "\", \"data\" : "
            + Var::toString( m_struct ) + " }";
    return json;
}
////////////////////////////////////////////////////////////////////////////////
void Struct::FromJSON(const std::string& json)
{
    Poco::JSON::ParseHandler::Ptr handler = new Poco::JSON::ParseHandler( true );
    Poco::JSON::Parser parser( handler );
    Var result = parser.parse( json );

    Poco::JSON::Object::Ptr set;
    if ( result.type() == typeid( Poco::JSON::Object::Ptr ) )
    {
        set = result.extract<Poco::JSON::Object::Ptr>();
    }

    // Require a matching typename
    if( !set->has( "typename" ) )
    {
        throw std::runtime_error("JSON contains no typename");
    }
    if( set->getValue<std::string>( "typename" ) != m_typename )
    {
        throw std::runtime_error("Typename mismatch");
    }

    // This seems like the obvious way, but unfortunately it results in
    // a Var that isn't really a DynamicStruct, and our [] operator fails.
    // m_struct = set->getObject( "data" );
    // And so we have to do it the longer way.
    Var tmp = set->getObject( "data" );
    m_struct = Var::parse( Var::toString( tmp ) );
}
////////////////////////////////////////////////////////////////////////////////
void Struct::Load( Poco::Data::Session& session )
{
    //Build statement of form:
    //select col1, col2, etc. from TYPENAME where id = MYID
    std::string stmt( "select " );
    Poco::Dynamic::Struct<std::string>::ConstIterator it = begin();
    Poco::Dynamic::Struct<std::string>::ConstIterator stop = end();
    while( it != stop )
    {
        stmt += it->first + ",";
        ++it;
    }
    stmt.erase( stmt.end() - 1 );
    stmt += " from " + m_typename + " where id = '" + m_id + "'";

    Poco::Data::Statement s( session );
    s << stmt;
    it = begin();
    while( it != stop )
    {
        s, Poco::Data::Keywords::into( m_struct[it->first] );
        ++it;
    }
    s.execute();
}
////////////////////////////////////////////////////////////////////////////////
void Struct::Save( Poco::Data::Session& session )
{
    //Lazy ID creation
    if( m_id.empty() )
    {
        CreateID();
    }

    // Since CreateTable sets m_check_table to false, the expensive part of
    // this conditional should only happen once.
    if( m_check_table && !TableExists( session ) )
    {
        CreateTable( session );
    }

    //Build statement of form:
    //insert or replace into TYPENAME (col1, col2, etc..) values (:0, :1, etc.)
    std::string stmt = "insert or replace into " + m_typename + " (";
    Poco::Dynamic::Struct<std::string>::ConstIterator it = begin();
    Poco::Dynamic::Struct<std::string>::ConstIterator stop = end();
    while( it != stop )
    {
        stmt += it->first + ",";
        ++it;
    }
    stmt.erase( stmt.end() - 1 );
    stmt += ") values(";
    Var index = 0;
    size_t size =  m_struct.extract< Poco::Dynamic::Struct< std::string > >().size();
    for( size_t ct = 0; ct < size; ++ct )
    {
        index = ct;
        stmt += ":" + index.convert<std::string>() + ",";
    }
    stmt.erase( stmt.end() - 1 );
    stmt += ")";

    Poco::Data::Statement s( session );
    s << stmt;
    it = begin();
    while( it != stop )
    {
        s, Poco::Data::Keywords::useRef( it->second );
        ++it;
    }
    s.execute();
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Struct<std::string>::ConstIterator Struct::begin() const
{
    return m_struct.extract< Poco::Dynamic::Struct< std::string > >().begin();
}
////////////////////////////////////////////////////////////////////////////////
Poco::Dynamic::Struct<std::string>::ConstIterator Struct::end() const
{
    return m_struct.extract< Poco::Dynamic::Struct< std::string > >().end();
}
////////////////////////////////////////////////////////////////////////////////
void Struct::CreateTable( Poco::Data::Session& session )
{
    Poco::Data::Statement stmt( session );
    stmt << "CREATE TABLE IF NOT EXISTS " << m_typename << "(";
    //'id' TEXT, 'Col1' TEXT, 'Col2' REAL, 'Col3' INTEGER, 'Col4' BOOLEAN)"

    // Var can only distinguish String, Numeric, and Integer
    // POD-ish types. No boolean, since it internally stores booleans as
    // integers. This limits what we can handle here.
    Poco::Dynamic::Struct<std::string>::ConstIterator it = begin();
    Poco::Dynamic::Struct<std::string>::ConstIterator stop = end();
    while( it != stop )
    {
        stmt << "'" << it->first << "' ";
        Var v = it->second;
        if( v.isNumeric() )
        {
            if( v.isInteger() )
            {
                stmt << "INTEGER";
            }
            else
            {
                stmt << "REAL";
            }
        }
        else if( v.isString() )
        {
            stmt << "TEXT";
        }

        ++it;

        if( it != stop )
        {
            stmt << ", ";
        }
        else
        {
            stmt << ")";
        }
    }

    stmt.execute();

    m_check_table = false;
}
////////////////////////////////////////////////////////////////////////////////
bool Struct::TableExists( Poco::Data::Session& session )
{
    bool exists = false;

    Poco::Data::Statement stmt( session );
    stmt << "SELECT 1 FROM sqlite_master WHERE type='table' AND name=:name",
            Poco::Data::Keywords::into( exists ),
            Poco::Data::Keywords::useRef( m_typename );

    stmt.execute();

    return exists;
}*/
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
