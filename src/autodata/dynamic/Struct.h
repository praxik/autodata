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

#include <autodata/util/DataHelper.h>

// --- POCO Includes --- //
#include <Poco/Dynamic/Struct.h>
#include <Poco/Data/Session.h>

namespace autodata
{
namespace dynamic
{

class Struct;
typedef std::vector< Poco::Dynamic::Struct< std::string > > Table;

///
class AUTODATA_EXPORTS Struct
{
public:
    ///
    Struct();

    ///
    ~Struct();

    ///Creates the Struct from the given value
    Struct(
        Poco::Dynamic::Struct< std::string >::Data const& val )
        :
        m_struct( val )
    {
        ;
    }

    ///
    template< typename T >
    Struct(
        std::map< std::string, T > const& val )
    {
        for( auto const& kv : val ) m_struct[ kv.first ] = kv.second;
    }

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
    void FromJson(
        std::string const& json );

    ///
    Poco::Dynamic::Var const& GetID() const;

    ///
    Poco::Dynamic::Struct< std::string >& GetStruct();

    ///
    Poco::Dynamic::Struct< std::string > const& GetStruct() const;

    ///
    std::string const& GetTypename() const;

    ///
    void Load(
        Poco::Data::Session& session );

    ///
    std::string colstr(
        unsigned int n = 2 ) const
    {
        std::string out;
        Poco::Dynamic::Struct< std::string >::ConstIterator kv;
        for( kv = m_struct.begin(); kv != --m_struct.end(); ++kv )
        {
            out += ( kv->first + ",\n" ).insert( 0, n, ' ' );
        }
        out += ( kv->first + "\n" ).insert( 0, n, ' ' );

        return out;
    }

    ///
    void SetID(
        Poco::Dynamic::Var id );

    ///
    void SetTypename(
        std::string name );

    ///
    std::string ToJson();

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
inline AbstractExtractionVec into(
    autodata::dynamic::Struct& o )
{
    AbstractExtractionVec extVec;
    for( auto& kv : o.GetStruct() )
    {
        extVec.push_back( into( kv.second ) );
    }
    return extVec;
}

///
inline AbstractBindingVec useRef(
    autodata::dynamic::Struct& o )
{
    AbstractBindingVec bindVec;
    for( auto& kv : o.GetStruct() )
    {
        bindVec.push_back( useRef( kv.second ) );
    }
    return bindVec;
}

} //end Keywords
} //end Data

namespace Dynamic
{

///Assignment operator for assigning autodata::dynamic::Struct to Var
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

} //end Dynamic
} //end Poco
