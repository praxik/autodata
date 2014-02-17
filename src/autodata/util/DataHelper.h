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
#include <autodata/CompilerGuards.h>

// --- POCO Includes --- //
#include <Poco/Nullable.h>
#include <Poco/SharedPtr.h>

#include <Poco/Data/Binding.h>
#include <Poco/Data/Extraction.h>
#include <Poco/Data/Limit.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SessionFactory.h>
#include <Poco/Data/SessionPool.h>

#ifdef POCO_MYSQL_API
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#endif //MySQL_API
#ifdef POCO_ODBC_API
#ifndef SWIG
DIAG_OFF( deprecated-declarations )
#endif
#include <Poco/Data/ODBC/Connector.h>
#ifndef SWIG
DIAG_ON( deprecated-declarations )
#endif
#include <Poco/Data/ODBC/ODBCException.h>
#endif //POCO_ODBC_API
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/SQLite/SQLiteException.h>

#include <Poco/Dynamic/Struct.h>

// --- Boost Includes --- //
#include <boost/array.hpp>
#include <boost/function.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>

#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <boost/math/special_functions.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include <boost/thread/once.hpp>

#ifndef SWIG
boost::once_flag const BOOST_ONCE_INIT_CONST = BOOST_ONCE_INIT;
#endif //SWIG

// --- Ix++ Includes --- //
#include <cpplinq/linq.hpp>

// --- Standard Includes --- //
#include <limits>

#ifndef SWIG
#ifndef isnan
AUTODATA_EXPORTS
bool isnan(
    double v );
#endif
#endif //SWIG

double const NaN = std::numeric_limits< double >::quiet_NaN();

#ifdef _MSC_VER
#ifndef SWIG
namespace std
{

///
AUTODATA_EXPORTS
double round(
    double const v );

}
#endif //SWIG
#endif

namespace autodata
{
namespace util
{

#ifndef SWIG
///
template< typename T >
std::string to_string_p(
    T const t,
    int const n )
{
    std::ostringstream oss;
    oss << std::setprecision( n ) << std::fixed << t;
    return oss.str();
}

///std::initializer_list alternative for non c++11 compliant
///ex.
///#define foo( x, ... ) FOO( ( varargls< std::string >( x ), __VA_ARGS__ ) )
///void FOO( varargls< std::string > args )
template< typename T, typename A = std::allocator< T > >
struct varargls : public std::vector< T, A >
{
    ///
    varargls(
        T const& v )
        :
        std::vector< T, A > ( 1, v )
    {
        ;
    }

    ///
    varargls& operator ,(
        T const& v )
    {
        push_back( v );
        return *this;
    }
};

///
class AUTODATA_EXPORTS StmtObj
{
public:
    ///
    StmtObj(
        Poco::Data::Session const& session );

    ///destructor
    ~StmtObj();

    ///
    Poco::Data::Session m_session;

    ///
    Poco::Data::Statement m_statement;

    ///
    Poco::Data::StatementImpl* m_statementImpl;

protected:

private:
    ///constructor
    StmtObj();

    ///copy constructor
    StmtObj( StmtObj const& );// = delete;

    ///assignment operator
    StmtObj& operator =( StmtObj );// = delete;

};

///
AUTODATA_EXPORTS
void ExecuteRetry(
    util::StmtObj& stmtObj,
    bool const& reset = true,
    unsigned int const& maxRetryAttempts = 100,
    unsigned int const& retrySleep = 100 );

///
template< typename T >
bool Convert(
    Poco::Dynamic::Var const& var,
    T& val )
{
    try
    {
        var.convert< T >( val );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}

///
template< typename T >
bool Convert(
    Poco::Nullable< T > const& nullable,
    T& val )
{
    if( nullable.isNull() ) return false;
    val = nullable.value();
    return true;
}

///
template< typename T >
bool Convert(
    Poco::Nullable< Poco::Dynamic::Var > const& nullable,
    T& val )
{
    return nullable.isNull() ?
        false : Convert< T >( nullable.value(), val );
}
#endif //SWIG

///
template< typename T >
T Convert(
    Poco::Dynamic::Var const& var,
    T const& defVal = T() )
{
    try
    {
        try
        {
            return Convert( var.extract< Poco::Nullable< T > >(), defVal );
        }
        catch( ... ){}
        return var.convert< T >();
    }
    catch( ... )
    {
        return defVal;
    }
}

///
template< typename T >
T Convert(
    Poco::Nullable< T > const& nullable,
    T const& defVal = T() )
{
    return nullable.isNull() ? defVal : nullable.value();
}

///
template< typename T >
T Convert(
    Poco::Nullable< Poco::Dynamic::Var > const& nullable,
    T const& defVal = T() )
{
    return nullable.isNull() ?
        defVal : Convert< T >( nullable.value(), defVal );
}

#ifndef SWIG
///
template< typename T, typename U >
T Convert(
    Poco::Nullable< U > const& nullable,
    T const& defVal = T() )
{
    return nullable.isNull() ?
        defVal : Convert< T >( Var( nullable.value() ), defVal );
}

///
template< typename T >
struct less_than
{
    bool operator ()( Poco::Dynamic::Var const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Left.isEmpty() || _Right.isEmpty() ) return false;
        return _Left.convert< T >() < _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() || _Right.isNull() ) return false;
        return operator ()( _Left.value(), _Right.value() );
    }

    bool operator ()( Poco::Dynamic::Var const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isEmpty() ) return false;
        return _Left.convert< T >() < _Right;
    }

    bool operator ()( T const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Right.isEmpty() ) return false;
        return _Left < _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        return operator ()( _Left.value(), _Right );
    }

    bool operator ()( T const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Right.isNull() ) return false;
        return operator ()( _Left, _Right.value() );
    }

};

///
template< typename T >
struct less_than_or_equal_to
{
    bool operator ()( Poco::Dynamic::Var const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Left.isEmpty() || _Right.isEmpty() ) return false;
        return _Left.convert< T >() <= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() || _Right.isNull() ) return false;
        return operator ()( _Left.value(), _Right.value() );
    }

    bool operator ()( Poco::Dynamic::Var const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isEmpty() ) return false;
        return _Left.convert< T >() <= _Right;
    }

    bool operator ()( T const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Right.isEmpty() ) return false;
        return _Left <= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        return operator ()( _Left.value(), _Right );
    }

    bool operator ()( T const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Right.isNull() ) return false;
        return operator ()( _Left, _Right.value() );
    }

};

///
template< typename T >
struct greater_than
{
    bool operator ()( Poco::Dynamic::Var const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Left.isEmpty() || _Right.isEmpty() ) return false;
        return _Left.convert< T >() > _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() || _Right.isNull() ) return false;
        return operator ()( _Left.value(), _Right.value() );
    }

    bool operator ()( Poco::Dynamic::Var const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isEmpty() ) return false;
        return _Left.convert< T >() > _Right;
    }

    bool operator ()( T const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Right.isEmpty() ) return false;
        return _Left > _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        return operator ()( _Left.value(), _Right );
    }

    bool operator ()( T const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Right.isNull() ) return false;
        return operator ()( _Left, _Right.value() );
    }

};

///
template< typename T >
struct greater_than_or_equal_to
{
    bool operator ()( Poco::Dynamic::Var const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Left.isEmpty() || _Right.isEmpty() ) return false;
        return _Left.convert< T >() >= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() || _Right.isNull() ) return false;
        return operator ()( _Left.value(), _Right.value() );
    }

    bool operator ()( Poco::Dynamic::Var const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isEmpty() ) return false;
        return _Left.convert< T >() >= _Right;
    }

    bool operator ()( T const& _Left,
                      Poco::Dynamic::Var const& _Right ) const
    {
        if( _Right.isEmpty() ) return false;
        return _Left >= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      T const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        return operator ()( _Left.value(), _Right );
    }

    bool operator ()( T const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Right.isNull() ) return false;
        return operator ()( _Left, _Right.value() );
    }

};

} //end util
} //end autodata

namespace Poco
{
namespace Data
{

///
template< typename J >
class Extraction< boost::shared_ptr< J > > : public AbstractExtraction
{
public:
    Extraction(
        boost::shared_ptr< J >& result,
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
        boost::shared_ptr< J >& result,
        J const& def,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default( def ),
        _extracted( false )
    {
        ;
    }

    virtual ~Extraction()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler< J >::size();
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
        if( _rResult == boost::shared_ptr< J >() )
        {
            _rResult = boost::make_shared< J >();
        }
        TypeHandler< J >::extract( pos, *_rResult, _default, pExt );
        _null = isValueNull( *_rResult, pExt->isNull( pos ) );
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
        return new Preparation< J >( pPrep, pos, _default );
    }

private:
    boost::shared_ptr< J >& _rResult;

    J _default;

    bool _extracted;

    bool _null;
};

///
template< typename J >
class Extraction< std::vector< boost::shared_ptr< J > > > :
    public AbstractExtraction
{
public:
    Extraction(
        std::vector< boost::shared_ptr< J > >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        std::vector< boost::shared_ptr< J > >& result,
        J const& def,
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
        return TypeHandler< J >::size();
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
        boost::shared_ptr< J > tmp = boost::make_shared< J >();
        TypeHandler< J >::extract( pos, *tmp, _default, pExt );
        _rResult.push_back( tmp );
        _nulls.push_back( isValueNull( *tmp, pExt->isNull( pos ) ) );
        return 1u;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep,
        std::size_t pos )
    {
        return new Preparation< J >( pPrep, pos, _default );
    }

protected:
    std::vector< boost::shared_ptr< J > > const& result() const
    {
        return _rResult;
    }

private:
    std::vector< boost::shared_ptr< J > >&  _rResult;

    J _default;

    std::deque< bool > _nulls;

};

///
template< typename J, typename K >
class Extraction< boost::multi_index_container< J, K > > :
    public AbstractExtraction
{
public:
    Extraction(
        boost::multi_index_container< J, K >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        boost::multi_index_container< J, K >& result,
        J const& def,
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
        return TypeHandler< J >::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    std::size_t extract(
        std::size_t pos )
    {
        J tmp;
        TypeHandler< J >::extract( pos, tmp, _default, getExtractor() );
        _rResult.insert( tmp );

        return 1u;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep, std::size_t pos )
    {
        return new Preparation< J >( pPrep, pos, _default );
    }

protected:

private:
    boost::multi_index_container< J, K >& _rResult;

    ///Copy the default
    J _default;

};

///
template< typename J, typename K >
class Extraction< boost::multi_index_container< J const*, K > > :
    public AbstractExtraction
{
public:
    Extraction(
        boost::multi_index_container< J const*, K >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        boost::multi_index_container< J const*, K >& result,
        J const& def,
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
        return TypeHandler< J >::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    std::size_t extract(
        std::size_t pos )
    {
        J* tmp = new J();
        TypeHandler< J >::extract( pos, *tmp, _default, getExtractor() );
        _rResult.insert( tmp );

        return 1u;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep, std::size_t pos )
    {
        return new Preparation< J >( pPrep, pos, _default );
    }

protected:

private:
    boost::multi_index_container< J const*, K >& _rResult;

    ///Copy the default
    J _default;

};

///
template< typename J, typename K >
class Extraction< boost::multi_index_container< boost::shared_ptr< J >, K > > :
    public AbstractExtraction
{
public:
    Extraction(
        boost::multi_index_container< boost::shared_ptr< J >, K >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        boost::multi_index_container< boost::shared_ptr< J >, K >& result,
        J const& def,
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
        return TypeHandler< J >::size();
    }

    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    std::size_t extract(
        std::size_t pos )
    {
        boost::shared_ptr< J > tmp = boost::make_shared< J >();
        TypeHandler< J >::extract( pos, *tmp, _default, getExtractor() );
        _rResult.insert( tmp );

        return 1u;
    }

    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep, std::size_t pos )
    {
        return new Preparation< J >( pPrep, pos, _default );
    }

protected:

private:
    boost::multi_index_container< boost::shared_ptr< J >, K >& _rResult;

    ///Copy the default
    J _default;

};

///
/*template<>
class Extraction< Dynamic::Struct< std::string > > : public AbstractExtraction
{
public:
    typedef Dynamic::Struct< std::string > ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef Extraction< ValType > Type;
    typedef SharedPtr< Type > Ptr;

    ///
    Extraction(
        Dynamic::Struct< std::string >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        ;
    }

    ///
    Extraction(
        Dynamic::Struct< std::string >& result,
        Dynamic::Var const& def,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default( def )
    {
        ;
    }

    ///
    ~Extraction()
    {
        ;
    }

    ///
    std::size_t numOfColumnsHandled() const
    {
        return TypeHandler< Dynamic::Var >::size();
    }

    ///
    std::size_t numOfRowsHandled() const
    {
        return static_cast< std::size_t >( _rResult.size() );
    }

    ///
    std::size_t numOfRowsAllowed() const
    {
        return getLimit();
    }

    ///
    std::size_t extract(
        std::size_t pos )
    {
        Dynamic::Var tmp;
        for( auto const& kv : _rResult )
        {
            TypeHandler< Dynamic::Var >::extract(
                pos, tmp, _default, getExtractor() );
            _rResult[ kv.first ] = tmp;
        }
        return _rResult.size();
    }

    ///
    AbstractPreparation::Ptr createPreparation(
        AbstractPreparator::Ptr& pPrep,
        std::size_t pos )
    {
        return new Preparation< Dynamic::Var >( pPrep, pos, _default );
    }

private:
    ///
    Dynamic::Struct< std::string >& _rResult;

    ///
    Dynamic::Var _default;

};*/

} //end Data

namespace Dynamic
{

///
AUTODATA_EXPORTS
std::ostream& operator <<(
    std::ostream& os,
    Var const& o );

///
/*template<>
class VarHolderImpl< boost::shared_ptr< T > > : public VarHolder
{
public:
    VarHolderImpl(
        boost::shared_ptr< T > const& val )
        :
        _val( val )
    {
        ;
    }

    ~VarHolderImpl()
    {
        ;
    }

    const std::type_info& type() const
    {
        return typeid( T );
    }

    VarHolder* clone() const
    {
        return new VarHolderImpl( _val );
    }

    boost::shared_ptr< T > const& value() const
    {
        return _val;
    }

private:
    VarHolderImpl();

    VarHolderImpl(
        VarHolderImpl const& );

    VarHolderImpl& operator =(
        VarHolderImpl const& );

    boost::shared_ptr< T > _val;
};*/

///
template< typename J, typename K >
class VarHolderImpl< boost::multi_index_container<
    boost::shared_ptr< J >, K > > : public VarHolder
{
public:
    VarHolderImpl(
        boost::multi_index_container< boost::shared_ptr< J >, K > const& val )
        :
        _val( val )
    {
        ;
    }

    ~VarHolderImpl()
    {
        ;
    }

    const std::type_info& type() const
    {
        return typeid(
            boost::multi_index_container< boost::shared_ptr< J >, K > );
    }

    void convert(
        std::string& val ) const
    {
        // Serialize in JSON format: note: although this is a vector<T>, the code only
        // supports vector<Var>. Total specialization is not possible
        // because of the cyclic dependency between Var and VarHolder

        // JSON format definition: [ n times: elem ',' ], no ',' for last elem
        /*val.append("[ ");
        typename std::vector<T>::const_iterator it = _val.begin();
        typename std::vector<T>::const_iterator itEnd = _val.end();
        if (!_val.empty())
        {
            appendJSONString(val, *it);
            ++it;
        }
        for (; it != itEnd; ++it)
        {

            val.append(", ");
            appendJSONString(val, *it);
        }
        val.append(" ]");*/
    }

    VarHolder* clone() const
    {
        return new VarHolderImpl( _val );
    }

    boost::multi_index_container< boost::shared_ptr< J >, K > const&
    value() const
    {
        return _val;
    }

    bool isArray() const
    {
        return true;
    }

    bool isStruct() const
    {
        return false;
    }

    bool isInteger() const
    {
        return false;
    }

    bool isSigned() const
    {
        return false;
    }

    bool isNumeric() const
    {
        return false;
    }

    bool isString() const
    {
        return false;
    }

    //boost::shared_ptr< J >& operator[](
        //typename std::vector<T>::size_type n )
    //{
        //return _val.operator[]( n );
    //}

    boost::shared_ptr< J > const& operator[](
        typename boost::multi_index_container<
            boost::shared_ptr< J >, K >::size_type n ) const
    {
        typename boost::multi_index_container< boost::shared_ptr< J >, K >::
            template nth_index< 0 >::type::const_iterator itr;
        itr = _val.template get< 0 >().begin();
        std::advance( itr, ( n == 0 ) ? 0 : n - 1 );
        return *itr;
    }

private:
    VarHolderImpl();

    VarHolderImpl(
        VarHolderImpl const& );

    VarHolderImpl& operator =(
        VarHolderImpl const& );

    boost::multi_index_container< boost::shared_ptr< J >, K > _val;

};
#endif //SWIG

} //end Dynamic
} //end Poco
