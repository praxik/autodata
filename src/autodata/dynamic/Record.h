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
#ifndef SWIG
#include <Poco/Data/Session.h>
#endif //SWIG
#include <Poco/Dynamic/Struct.h>

namespace autodata
{
namespace dynamic
{

///
class Record;
typedef std::vector< Record > Records;

#ifndef SWIG
///
AUTODATA_EXPORTS
Record load(
    std::string const& typeName,
    Poco::Dynamic::Var const& id,
    Poco::Data::Session& session );
#endif //SWIG

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
    Record& operator =( Record const& ) = default;

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

#ifndef SWIG
    ///
    void CreateTable(
        Poco::Data::Session& session );
#endif //SWIG

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

#ifndef SWIG
    ///
    void Load(
        Poco::Data::Session& session );

    ///
    void Save(
        Poco::Data::Session& session );
#endif //SWIG

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

#ifndef SWIG
namespace Poco
{
namespace Data
{

template<>
class Binding< autodata::dynamic::Record > : public AbstractBinding
{
public:
    typedef autodata::dynamic::Record ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef Binding< ValType > Type;
    typedef SharedPtr< Type > Ptr;

    explicit Binding(
        ValType const& val,
        std::string const& name = "",
        Direction direction = PD_IN )
        :
        AbstractBinding( name, direction ),
        _val( val ),
        _bound( false )
    {
        ;
    }

    ~Binding()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        return static_cast< std::size_t >( _val.size() );
    }

    std::size_t numOfRowsHandled() const
    {
        return 1u;
    }

    bool canBind() const
    {
        return !_bound;
    }

    void bind(
        std::size_t pos )
    {
        poco_assert_dbg( !getBinder().isNull() );
        for( auto const& kv : _val )
        {
            TypeHandler< Dynamic::Var >::bind(
                pos++, kv.second, getBinder(), getDirection() );
        }
        _bound = true;
    }

    void reset()
    {
        _bound = false;
        AbstractBinder::Ptr pBinder = getBinder();
        poco_assert_dbg( !pBinder.isNull() );
        pBinder->reset();
    }

private:
    ValType const& _val;
    bool _bound;
};

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
class Extraction< autodata::dynamic::Record > : public AbstractExtraction
{
public:
    typedef autodata::dynamic::Record ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef Extraction< ValType > Type;
    typedef SharedPtr< Type > Ptr;

    Extraction(
        ValType& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default(),
        _extracted( false )
    {
        ;
    }

    Extraction(
        ValType& result,
        ValType const& def,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default( def ),
        _extracted( false )
    {
        ;
    }

    ~Extraction()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    std::size_t numOfRowsHandled() const
    {
        return _extracted ? 1u : 0;
    }

    std::size_t numOfRowsAllowed() const
    {
        return 1u;
    }

    bool isNull(
        std::size_t row = 0 ) const
    {
        return _null;
    }

    std::size_t extract(
        std::size_t pos )
    {
        if( _extracted ) throw ExtractException( "value already extracted" );
        _extracted = true;
        AbstractExtractor::Ptr pExt = getExtractor();
        for( auto& kv : _rResult )
        {
            TypeHandler< Dynamic::Var >::extract(
                pos++, kv.second, _default, pExt );
        }
        _null = isValueNull< ValType >( _rResult, pExt->isNull( pos ) );
        return 1u;
    }

    void reset()
    {
        _extracted = false;
    }

    bool canExtract() const
    {
        return !_extracted;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep,
        std::size_t pos )
    {
        return new Preparation< ValType >( pPrep, pos, _rResult );
    }

private:
    ValType& _rResult;
    ValType _default;
    bool _extracted;
    bool _null;
};

} //end Data

namespace Dynamic
{

///
template<>
class AUTODATA_EXPORTS VarHolderImpl< autodata::dynamic::Record > :
    public VarHolderImpl< Struct< std::string > >
{
public:
    VarHolderImpl(
        autodata::dynamic::Record const& val )
        :
        VarHolderImpl< Struct< std::string > >( val )
    {
        ;
    }

    ~VarHolderImpl()
    {
        ;
    }
};

} //end Dynamic
} //end Poco
#endif //SWIG
