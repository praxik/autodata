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
    Record(
        Record const& o );

    ///copy constructor
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
    void clear();

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

template<>
class Preparation< autodata::dynamic::Record > : public AbstractPreparation
{
public:
    Preparation(
        AbstractPreparator::Ptr& pPreparator,
        std::size_t pos,
        autodata::dynamic::Record& val )
        :
        AbstractPreparation( pPreparator ),
        _pos( pos ),
        _val( val )
    {
        ;
    }

    ~Preparation()
    {
        ;
    }

    void prepare()
    {
        AbstractPreparator::Ptr pPrep = preparation();
        for( auto const& kv : _val )
        {
            Poco::Data::TypeHandler< Poco::Dynamic::Var >::prepare(
                _pos++, kv.second, pPrep );
        }
    }

private:
    std::size_t _pos;
    autodata::dynamic::Record& _val;
};

template<>
class Extraction< autodata::dynamic::Records > : public AbstractExtraction
{
public:
    typedef autodata::dynamic::Records ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef Extraction< ValType > Type;
    typedef SharedPtr< Type > Ptr;

    Extraction(
        ValType& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        ValType& result,
        autodata::dynamic::Record const& def,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default( def )
    {
        _rResult.clear();
    }

    virtual ~Extraction()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        return 1u;
    }

    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    bool isNull(
        std::size_t row ) const
    {
        try
        {
            return _nulls.at( row );
        }
        catch( std::out_of_range& ex )
        {
            throw RangeException( ex.what() );
        }
    }

    std::size_t extract(
        std::size_t pos )
    {
        AbstractExtractor::Ptr pExt = getExtractor();
        _rResult.push_back( _default );
        for( auto& kv : _rResult.back() )
        {
            TypeHandler< Dynamic::Var >::extract(
                pos++, kv.second, _default, pExt );
        }
        _nulls.push_back( isValueNull( _rResult.back(), pExt->isNull( pos ) ) );
        return 1u;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep,
        std::size_t pos )
    {
        return new Preparation< autodata::dynamic::Record >(
            pPrep, pos, _default );
    }

protected:
    ValType const& result() const
    {
        return _rResult;
    }

private:
    ValType& _rResult;
    autodata::dynamic::Record _default;
    std::deque< bool > _nulls;
};

template<>
class Binding< autodata::dynamic::Records > : public AbstractBinding
{
public:
    typedef autodata::dynamic::Records ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef SharedPtr< Binding< ValType > > Ptr;
    typedef ValType::const_iterator Iterator;

    explicit Binding(
        ValType& val,
        std::string const& name = "",
        Direction direction = PD_IN )
        :
        AbstractBinding( name, direction ),
        _val( val ),
        _begin(),
        _end()
    {
        if( PD_IN == direction && numOfRowsHandled() == 0 )
            throw BindingException(
                "It is illegal to bind to an empty data collection" );
        reset();
    }

    ~Binding()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        return 1u;
    }

    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _val.size() );
    }

    bool canBind() const
    {
        return _begin != _end;
    }

    void bind(
        std::size_t pos )
    {
        poco_assert_dbg( !getBinder().isNull() );
        poco_assert_dbg( canBind() );
        for( auto const& kv : *_begin )
        {
            TypeHandler< Dynamic::Var >::bind(
                pos++, kv.second, getBinder(), getDirection() );
        }
        ++_begin;
    }

    void reset()
    {
        _begin = _val.begin();
        _end = _val.end();
    }

private:
    ValType const& _val;
    Iterator _begin;
    Iterator _end;
};

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
