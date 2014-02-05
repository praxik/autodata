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
using namespace Poco::Dynamic;

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
void Struct::Into(
    Poco::Data::Statement& s )
{
    for( auto& kv : m_struct ) s, Keywords::into( kv.second );
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
void Struct::UseRef(
    Poco::Data::Statement& s )
{
    for( auto const& kv : m_struct ) s, Keywords::useRef( kv.second );
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
