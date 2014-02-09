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

using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;
using namespace Poco::JSON;

namespace autodata
{
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
Struct::~Struct()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Struct::operator Poco::Dynamic::Struct< std::string >&()
{
    return m_struct;
}
////////////////////////////////////////////////////////////////////////////////
Struct::operator Poco::Dynamic::Struct< std::string >const&() const
{
    return m_struct;
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
Struct::iterator Struct::begin()
{
    return m_struct.begin();
}
////////////////////////////////////////////////////////////////////////////////
Struct::const_iterator Struct::begin() const
{
    return m_struct.begin();
}
////////////////////////////////////////////////////////////////////////////////
Struct::iterator Struct::end()
{
    return m_struct.end();
}
////////////////////////////////////////////////////////////////////////////////
Struct::const_iterator Struct::end() const
{
    return m_struct.end();
}
////////////////////////////////////////////////////////////////////////////////
void Struct::FromJson(
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
Var const& Struct::GetID() const
{
    return m_struct[ "id" ];
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
void Struct::Load(
    Session& session )
{
    //Build statement of form:
    //select col1, col2, etc. from TYPENAME where id = ?
    Statement statement( session );
    statement
        << "select\n"
        <<  colstr()
        << "from " << m_typename << "\n"
        << "where\n"
        << "  id = ?",
        into( *this ),
        useRef( m_struct[ "id" ] );
    std::cout << statement.toString() << std::endl;
    //statement.execute();
}
////////////////////////////////////////////////////////////////////////////////
void Struct::SetID(
    Var id )
{
    m_struct[ "id" ] = std::move( id );
}
////////////////////////////////////////////////////////////////////////////////
void Struct::SetTypename(
    std::string name )
{
    m_typename = std::move( name );
}
////////////////////////////////////////////////////////////////////////////////
std::string Struct::ToJson()
{
    return m_struct.toString();
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata

namespace Poco
{
namespace Dynamic
{

///////////////////////////////////////////////////////////////////////////////////
template<>
Var& Var::operator =(
    autodata::dynamic::Struct const& o )
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
