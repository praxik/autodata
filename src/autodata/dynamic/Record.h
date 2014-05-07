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
#include <Poco/Data/Session.h>

#include <Poco/Dynamic/Struct.h>

namespace autodata
{
namespace dynamic
{

///
class Record;
typedef std::vector< Record > Records;

///
AUTODATA_EXPORTS
Record load(
    std::string const& typeName,
    Poco::Dynamic::Var const& id,
    Poco::Data::Session& session );

///
class AUTODATA_EXPORTS Record : public Poco::Dynamic::Struct< std::string >
{
public:
    ///
    typedef Poco::Dynamic::Struct< std::string >::Iterator iterator;
    typedef Poco::Dynamic::Struct< std::string >::ConstIterator const_iterator;
    typedef Poco::Dynamic::Struct< std::string >::Data::value_type value_type;

    ///constructor
    Record();

    ///copy constructor
    //Record( Record const& ) = default;
    Record(
        Poco::Dynamic::Struct< std::string > const& o );

    ///move constructor
    Record(
        Record&& o );

    ///assignment operator
    //Record& operator =( Record ) = default;

    ///
    virtual ~Record();

    ///
    iterator begin();

    ///
    const_iterator begin() const;

    ///
    std::string columns(
        unsigned int n,
        bool usePH = false ) const;

    ///
    void CreateId();

    ///
    void CreateTable(
        Poco::Data::Session& session );

    ///
    iterator end();

    ///
    const_iterator end() const;

    ///
    void FromJson(
        std::string const& json );

    ///
    Poco::Dynamic::Var const& GetId() const;

    ///
    std::string const& GetTypename() const;

    ///
    void Load(
        Poco::Data::Session& session );

    ///
    void Save(
        Poco::Data::Session& session );

    ///
    void SetId(
        Poco::Dynamic::Var id );

    ///
    void SetTypename(
        std::string name );

    ///
    std::string ToJson() const;

#ifndef SWIG
    ///
    AUTODATA_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        Record const& record );
#endif //SWIG

private:
    ///
    std::string ph(
        std::string const& s ) const;

    ///
    std::string m_typename;

};

///For cpplinq use
inline
auto begin(
    Record& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
inline
auto begin(
    Record const& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
inline
auto end(
    Record& o ) -> decltype( o.end() )
{
    return o.end();
}

///For cpplinq use
inline
auto end(
    Record const& o ) -> decltype( o.end() )
{
    return o.end();
}

} //end dynamic
} //end autodata

namespace Poco
{
namespace Data
{
namespace Keywords
{

///
inline
AbstractExtractionVec into(
    autodata::dynamic::Record& o )
{
    AbstractExtractionVec extVec;
    for( auto& kv : o ) extVec.push_back( into( kv.second ) );
    return extVec;
}

///
inline
AbstractBindingVec useRef(
    autodata::dynamic::Record& o )
{
    AbstractBindingVec bindVec;
    for( auto& kv : o ) bindVec.push_back( useRef( kv.second ) );
    return bindVec;
}

} //end Keywords
} //end Data

namespace Dynamic
{

///Assignment operator for assigning autodata::dynamic::Record to Var
template<>
Var& Var::operator =(
    autodata::dynamic::Record const& o );

} //end Dynamic
} //end Poco
