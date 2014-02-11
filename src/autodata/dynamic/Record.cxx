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
#include <autodata/dynamic/Record.h>

// --- Poco Includes --- //
#include <Poco/UUIDGenerator.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Handler.h>
#include <Poco/JSON/ParseHandler.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;
using namespace Poco::JSON;

namespace autodata
{
namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
Record::Record()
    :
    m_struct( Struct< std::string >() )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::~Record()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::operator Struct< std::string >&()
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
Record::operator Struct< std::string >const&() const
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
Var& Record::operator [](
    std::string const& name )
{
    return m_struct[ name ];
}
////////////////////////////////////////////////////////////////////////////////
Var const& Record::operator [](
    std::string const& name ) const
{
    return m_struct[ name ];
}
////////////////////////////////////////////////////////////////////////////////
Record::iterator Record::begin()
{
    return m_struct.begin();
}
////////////////////////////////////////////////////////////////////////////////
Record::const_iterator Record::begin() const
{
    return m_struct.begin();
}
////////////////////////////////////////////////////////////////////////////////
void Record::CreateTable(
    Session& session )
{
    //
    Poco::Data::Statement statement( session );
    statement << "create table if not exists " << m_typename << "(\n";
    for( auto& kv : m_struct )
    {
        statement << "  '" << kv.first << "' ";
        Var const& var = kv.second;
        if( var.isNumeric() )
        {
            if( var.isInteger() )
            {
                statement << "integer";
            }
            else
            {
                statement << "float";
            }
        }
        else if( var.isString() )
        {
            std::string const& str = var.extract< std::string >();
            std::size_t size = str.size();
            if( size > 255 )
            {
                statement << "text";
            }
            else
            {
                statement << "varchar( " << size << " )";
            }
        }
        statement << ",\n";
    }
    statement
        << "constraint " << m_typename << "_ukey_1\n"
        << "  unique(\n"
        << "    id ) );";

    statement.execute();
}
////////////////////////////////////////////////////////////////////////////////
std::string Record::columns(
    unsigned int n,
    bool usePH ) const
{
    std::string out;
    Poco::Dynamic::Struct< std::string >::ConstIterator kv;
    for( kv = m_struct.begin(); kv != --m_struct.end(); ++kv )
    {
        out += ( (
            usePH ? ph( kv->first ) : kv->first ) + ",\n" ).insert( 0, n, ' ' );
    }
    out += std::string(
        usePH ? ph( kv->first ) : kv->first ).insert( 0, n, ' ' );

    return out;
}
////////////////////////////////////////////////////////////////////////////////
Record::iterator Record::end()
{
    return m_struct.end();
}
////////////////////////////////////////////////////////////////////////////////
Record::const_iterator Record::end() const
{
    return m_struct.end();
}
////////////////////////////////////////////////////////////////////////////////
void Record::FromJson(
    std::string const& json )
{
    ParseHandler::Ptr handler = new ParseHandler( true );
    Parser parser( handler );
    Var result = parser.parse( json );

    Object::Ptr set;
    if( result.type() == typeid( Object::Ptr ) )
    {
        set = result.extract< Object::Ptr >();
    }

    //First we check for a typename and make sure it matches
    /*if( !set->has( "typename" ) )
    {
        // throw exception?
        std::cout << "Error: No typename in json. Aborting FromJSON operation!" << std::endl;
        return;
    }
    if( set->getValue< std::string >( "typename" ) != T::type() )
    {
        //throw exception?
        std::cout << "Error: Typename specified in JSON does not match this object's typename." << std::endl;
        return;
    }*/

    //Pull the data and load it into m_struct
    Object::Ptr data = set->getObject( "data" );
    m_struct = *data;
}
////////////////////////////////////////////////////////////////////////////////
Var const& Record::GetID() const
{
    return m_struct[ "id" ];
}
////////////////////////////////////////////////////////////////////////////////
Struct< std::string >& Record::GetStruct()
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
Struct< std::string > const& Record::GetStruct() const
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Record::GetTypename() const
{
    return m_typename;
}
////////////////////////////////////////////////////////////////////////////////
void Record::Load(
    Session& session )
{
    //
    Statement statement( session );
    statement
        << "select\n"
        <<    columns( 2 ) << "\n"
        << "from " << m_typename << "\n"
        << "where\n"
        << "  id = ?",
        into( *this ),
        useRef( m_struct[ "id" ] );
    statement.execute();
}
////////////////////////////////////////////////////////////////////////////////
std::string Record::ph(
    std::string const& s ) const
{
    return "? /*" + s + "*/";
}
////////////////////////////////////////////////////////////////////////////////
void Record::Save(
    Session& session )
{
    //
    Statement statement( session );
    statement
        << "insert or replace into " << m_typename << "(\n"
        <<    columns( 2 ) << " )\n"
        << "values(\n"
        <<    columns( 2, true ) << " )",
        useRef( *this );
    statement.execute();
}
////////////////////////////////////////////////////////////////////////////////
void Record::SetID(
    Var id )
{
    m_struct[ "id" ] = std::move( id );
}
////////////////////////////////////////////////////////////////////////////////
void Record::SetTypename(
    std::string name )
{
    m_typename = std::move( name );
}
////////////////////////////////////////////////////////////////////////////////
std::string Record::ToJson()
{
    return m_struct.toString();
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata

////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream &os,
    Row const& record )
{
    return os;
}
////////////////////////////////////////////////////////////////////////////////

namespace Poco
{
namespace Dynamic
{

///////////////////////////////////////////////////////////////////////////////////
template<>
Var& Var::operator =(
    autodata::dynamic::Record const& o )
{
#ifdef POCO_NO_SOO
    Var tmp( o.GetStruct() );
    swap( tmp );
#else
    construct( o.GetStruct() );
#endif
    return *this;
}
////////////////////////////////////////////////////////////////////////////////

} //end Dynamic
} //end Poco
