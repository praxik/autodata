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


#pragma once

// --- AutoData Includes --- //
#include <autodata/AutoDataConfig.h>

// --- POCO Includes --- //
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/Session.h>

namespace autodata
{
namespace dynamic
{

///
class AUTODATA_EXPORTS Struct
{
public:
    ///
    Struct();

    ///
    ~Struct();

    ///
    operator Poco::Dynamic::Struct< std::string >&();

    ///
    operator Poco::Dynamic::Struct< std::string > const&() const;

    ///
    Poco::Dynamic::Var& operator [](
        std::string const& name );

    ///
    Poco::Dynamic::Var const& operator [](
        std::string const& name ) const;

    ///
    Poco::Dynamic::Var const& GetID() const;

    ///
    Poco::Dynamic::Struct< std::string >& GetStruct();

    ///
    Poco::Dynamic::Struct< std::string > const& GetStruct() const;

    ///
    std::string const& GetTypename() const;

    ///
    void SetID(
        Poco::Dynamic::Var id );

    ///
    void SetTypename(
        std::string name );


private:
    ///
    std::string m_typename;

    ///
    Poco::Dynamic::Struct< std::string > m_struct;

};

} //end dynamic
} //end autodata

namespace Poco
{
namespace Data
{
namespace Keywords
{

///
inline AbstractExtraction::Ptr into(
    autodata::dynamic::Struct& s )
{
    for( auto& kv : s.GetStruct() ) s, Keywords::into( kv.second );
}

///
/*void UseRef(
    Poco::Data::Statement& s )
{
    for( auto const& kv : m_struct ) s, Keywords::useRef( kv.second );
}*/

} //end Keywords
} //end Data
} //end Poco
