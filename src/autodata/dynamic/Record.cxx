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
#include <autodata/db/Query.h>

#include <autodata/dynamic/Record.h>

using namespace autodata::db;
using namespace autodata::dynamic;
using namespace autodata::util;

// --- Poco Includes --- //
#include <Poco/UUIDGenerator.h>

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>

using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;
using namespace Poco::JSON;

namespace autodata
{
namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
Record load(
    std::string const& typeName,
    Var const& id,
    Session& session )
{
    poco_assert( !typeName.empty() && !id.isEmpty() );

    //
    Query query = ( session
        << "select\n"
        << "  *\n"
        << "from " << typeName << "\n"
        << "where\n"
        << "  id = ?",
        useRef( id ) );
    query.Execute();
    Records records = query.ToRecords();
    return cpplinq::from( records ).first_or_default();
}
////////////////////////////////////////////////////////////////////////////////
Record::Record()
    :
    Struct< std::string >(),
    m_typename()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::Record(
    Struct< std::string > const& o )
    :
    Struct< std::string >( o ),
    m_typename()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::Record(
    Record&& o )
    :
    Struct< std::string >( std::move( o ) ),
    m_typename( std::move( o.m_typename ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::~Record()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record::iterator Record::begin()
{
    return Struct< std::string >::begin();
}
////////////////////////////////////////////////////////////////////////////////
Record::const_iterator Record::begin() const
{
    return Struct< std::string >::begin();
}
////////////////////////////////////////////////////////////////////////////////
void Record::CreateId()
{
    SetId( Poco::UUIDGenerator::defaultGenerator().createOne().toString() );
}
////////////////////////////////////////////////////////////////////////////////
void Record::CreateTable(
    Session& session )
{
    //
    if( !contains( "id" ) ) CreateId();
    Poco::Data::Statement statement( session );
    statement << "create table if not exists " << m_typename << "(\n";
    for( auto& kv : *this )
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
                statement << "real";
            }
        }
        else if( var.isString() )
        {
            statement << "text";
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
    if( empty() ) return std::string( "*" ).insert( 0, n, ' ' );

    std::string out;
    Poco::Dynamic::Struct< std::string >::ConstIterator kv;
    for( kv = begin(); kv != --end(); ++kv )
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
    return Struct< std::string >::end();
}
////////////////////////////////////////////////////////////////////////////////
Record::const_iterator Record::end() const
{
    return Struct< std::string >::end();
}
////////////////////////////////////////////////////////////////////////////////
void Record::FromJson(
    std::string const& json )
{
    Parser parser;
    Var result = parser.parse( json );
    poco_assert( result.type() == typeid( Object::Ptr ) );
    Struct< std::string >::operator =( *result.extract< Object::Ptr >() );
}
////////////////////////////////////////////////////////////////////////////////
Var const& Record::GetId() const
{
    return operator []( "id" );
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
    poco_assert(
        !m_typename.empty() &&
        contains( "id" ) &&
        !operator []( "id" ).isEmpty() );

    //
    Statement statement( session );
    statement
        << "select\n"
        <<    columns( 2 ) << "\n"
        << "from " << m_typename << "\n"
        << "where\n"
        << "  id = ?",
        into( *this ),
        useRef( operator []( "id" ) );
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
    if( !contains( "id" ) ) CreateId();
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
void Record::SetId(
    Var id )
{
    operator []( "id" ) = std::move( id );
}
////////////////////////////////////////////////////////////////////////////////
void Record::SetTypename(
    std::string name )
{
    m_typename = std::move( name );
}
////////////////////////////////////////////////////////////////////////////////
std::string Record::ToJson() const
{
    return const_cast< Record& >( *this ).toString();
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
    Var tmp( o );
    swap( tmp );
#else
    construct( o );
#endif
    return *this;
}
////////////////////////////////////////////////////////////////////////////////

} //end Dynamic
} //end Poco
