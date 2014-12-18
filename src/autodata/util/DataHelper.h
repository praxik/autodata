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
#include <Poco/SharedPtr.h>

#include <Poco/Data/Binding.h>
#include <Poco/Data/Extraction.h>
#include <Poco/Data/Limit.h>
#include <Poco/Data/RecordSet.h>

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
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

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
#include <cmath>
#include <iomanip>
#include <limits>
#include <memory>

#ifdef WIN32
#define MAKE_SHARED_FRIENDS( Class ) \
friend class std::_Ref_count_obj< Class >;
#elif __APPLE__
#define MAKE_SHARED_FRIENDS( Class ) \
template< typename _T1, typename _T2, unsigned > \
friend class std::__1::__libcpp_compressed_pair_imp;
#elif __linux__
#define MAKE_SHARED_FRIENDS( Class ) \
friend class __gnu_cxx::new_allocator< Class >;
#endif

#if _MSC_VER >= 1800 || defined __linux__
#define ISNAN_EXISTS 1
#endif

#if !defined SWIG && !defined isnan && !defined ISNAN_EXISTS
AUTODATA_EXPORTS
bool isnan(
    double v );
#endif

double const NaN = std::numeric_limits< double >::quiet_NaN();

#if !defined SWIG && defined _MSC_VER
namespace std
{
#if _MSC_VER < 1800
///
AUTODATA_EXPORTS
double round(
    double const v );
#endif
///
template< typename T >
auto begin(
    pair< T, T > const& range ) -> T
{
    return range.first;
}

///
template <typename T>
auto end(
    pair< T, T > const& range ) -> T
{
    return range.second;
}
}
#endif

namespace autodata
{
namespace util
{

#ifndef SWIG
///
AUTODATA_EXPORTS
bool only_1_bit(
    int x );

///
AUTODATA_EXPORTS
std::string get_file_contents(
    std::string const& filename );

///
AUTODATA_EXPORTS
std::string escape_json(
    std::string const& json );

///
template< typename T >
std::string to_string_p(
    T const t,
    int const n )
{
    static_assert( std::is_arithmetic< T >::value,
        "to_string_p: requires arithmetic type" );
    std::ostringstream oss;
    oss << std::setprecision( n ) << std::fixed << t;
    return oss.str();
}

///
struct AUTODATA_EXPORTS hexchar
{
    hexchar();
    hexchar( unsigned char in );
    unsigned char c;

#ifndef SWIG
    ///
    AUTODATA_EXPORTS
    friend std::ostream& operator <<(
        std::ostream& os,
        hexchar const& c );

    ///
    AUTODATA_EXPORTS
    friend std::istream& operator >>(
        std::istream& is,
        hexchar& c );
#endif //SWIG
};

///
AUTODATA_EXPORTS
std::string hex_to_bin(
    std::string const& s );

///
AUTODATA_EXPORTS
std::vector< unsigned char > hex_to_bytes(
    std::string const& hexstr );

///
template< typename T > struct to_unsigned;
template<> struct to_unsigned< char >{ typedef unsigned char type; };
template<> struct to_unsigned< signed char >{ typedef unsigned char type; };
template<> struct to_unsigned< short >{ typedef unsigned short type; };
template<> struct to_unsigned< int >{ typedef unsigned int type; };
template<> struct to_unsigned< long >{ typedef unsigned long type; };
template<> struct to_unsigned< long long >{ typedef unsigned long long type; };

///
template< typename A, typename B,
    typename = typename std::enable_if< std::is_arithmetic< A >::value >::type,
    typename = typename std::enable_if< std::is_arithmetic< B >::value >::type >
struct safe_comparison
{
public:
    ///
    typedef typename std::is_integral< A >::type a_integral;
    typedef typename std::is_integral< B >::type b_integral;
    typedef typename std::is_signed< A >::type a_signed;
    typedef typename std::is_signed< B >::type b_signed;

    ///
    typedef typename std::integral_constant< bool,
        a_integral::value && b_integral::value >::type both_integral;
    typedef typename std::integral_constant< bool,
        a_signed::value && b_signed::value >::type both_signed;
    typedef typename std::integral_constant< bool,
        !a_signed::value && !b_signed::value >::type both_unsigned;
    typedef typename std::integral_constant< bool,
        both_integral::value &&
        !( both_signed::value || both_unsigned::value ) >::type need_check;

    ///
    static bool gt(
        A a,
        B b )
    {
        return gt_check( a, b, need_check() );
    }

    ///
    static bool lt(
        A a,
        B b )
    {
        return lt_check( a, b, need_check() );
    }

private:
    ///
    static bool gt_check(
        A a,
        B b,
        std::false_type )
    {
        return ( a > b );
    }

    ///
    static bool gt_check(
        A a,
        B b,
        std::true_type )
    {
        return gt_different_signed( a, b, a_signed(), b_signed() );
    }

    ///
    static bool gt_different_signed(
        A a,
        B b,
        std::true_type,
        std::false_type )
    {
        return ( a >= 0 ) &&
            ( static_cast< typename to_unsigned< A >::type >( a ) > b );
    }

    ///
    static bool gt_different_signed(
        A a,
        B b,
        std::false_type,
        std::true_type )
    {
        return ( b < 0 ) ||
            ( a > static_cast< typename to_unsigned< B >::type >( b ) );
    }

    ///
    static bool lt_check(
        A a,
        B b,
        std::false_type )
    {
        return ( a < b );
    }

    ///
    static bool lt_check(
        A a,
        B b,
        std::true_type )
    {
        return lt_different_signed( a, b, a_signed(), b_signed() );
    }

    ///
    static bool lt_different_signed(
        A a,
        B b,
        std::true_type,
        std::false_type )
    {
        return ( a < 0 ) ||
            ( static_cast< typename to_unsigned< A >::type >( a ) < b );
    }

    ///
    static bool lt_different_signed(
        A a,
        B b,
        std::false_type,
        std::true_type )
    {
        return ( b >= 0 ) &&
            ( a < static_cast< typename to_unsigned< B >::type >( b ) );
    }
};

///
template< typename A, typename B >
bool safe_less_than(
    A a,
    B b )
{
    return safe_comparison< A, B >::lt( a, b );
}

///
template< typename A, typename B >
bool safe_greater_than(
    A a,
    B b )
{
    return safe_comparison< A, B >::gt( a, b );
}

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
        if( _Left.isEmpty() ) return false;
        if( _Right.isEmpty() ) return true;
        return _Left.convert< T >() < _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        if( _Right.isNull() ) return true;
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
        if( _Left.isEmpty() ) return false;
        if( _Right.isEmpty() ) return true;
        return _Left.convert< T >() <= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        if( _Right.isNull() ) return true;
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
        if( _Left.isEmpty() ) return false;
        if( _Right.isEmpty() ) return true;
        return _Left.convert< T >() > _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        if( _Right.isNull() ) return true;
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
        if( _Left.isEmpty() ) return false;
        if( _Right.isEmpty() ) return true;
        return _Left.convert< T >() >= _Right.convert< T >();
    }

    bool operator ()( Poco::Nullable< Poco::Dynamic::Var > const& _Left,
                      Poco::Nullable< Poco::Dynamic::Var > const& _Right ) const
    {
        if( _Left.isNull() ) return false;
        if( _Right.isNull() ) return true;
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
class Extraction< std::shared_ptr< J > > : public AbstractExtraction
{
public:
    Extraction(
        std::shared_ptr< J >& result,
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
        std::shared_ptr< J >& result,
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
        if( _rResult == std::shared_ptr< J >() )
        {
            _rResult = std::make_shared< J >();
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
    std::shared_ptr< J >& _rResult;

    J _default;

    bool _extracted;

    bool _null;
};

///
template< typename J >
class Extraction< std::vector< std::shared_ptr< J > > > :
    public AbstractExtraction
{
public:
    Extraction(
        std::vector< std::shared_ptr< J > >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        std::vector< std::shared_ptr< J > >& result,
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
        std::shared_ptr< J > tmp = std::make_shared< J >();
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
    std::vector< std::shared_ptr< J > > const& result() const
    {
        return _rResult;
    }

private:
    std::vector< std::shared_ptr< J > >&  _rResult;

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
class Extraction< boost::multi_index_container< std::shared_ptr< J >, K > > :
    public AbstractExtraction
{
public:
    Extraction(
        boost::multi_index_container< std::shared_ptr< J >, K >& result,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default()
    {
        _rResult.clear();
    }

    Extraction(
        boost::multi_index_container< std::shared_ptr< J >, K >& result,
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
        std::shared_ptr< J > tmp = std::make_shared< J >();
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
    boost::multi_index_container< std::shared_ptr< J >, K >& _rResult;

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
class VarHolderImpl< std::shared_ptr< T > > : public VarHolder
{
public:
    VarHolderImpl(
        std::shared_ptr< T > const& val )
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

    std::shared_ptr< T > const& value() const
    {
        return _val;
    }

private:
    VarHolderImpl();

    VarHolderImpl(
        VarHolderImpl const& );

    VarHolderImpl& operator =(
        VarHolderImpl const& );

    std::shared_ptr< T > _val;
};*/

///
template< typename J, typename K >
class VarHolderImpl< boost::multi_index_container<
    std::shared_ptr< J >, K > > : public VarHolder
{
public:
    VarHolderImpl(
        boost::multi_index_container< std::shared_ptr< J >, K > const& val )
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
            boost::multi_index_container< std::shared_ptr< J >, K > );
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

    boost::multi_index_container< std::shared_ptr< J >, K > const&
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

    //std::shared_ptr< J >& operator[](
        //typename std::vector<T>::size_type n )
    //{
        //return _val.operator[]( n );
    //}

    std::shared_ptr< J > const& operator[](
        typename boost::multi_index_container<
            std::shared_ptr< J >, K >::size_type n ) const
    {
        typename boost::multi_index_container< std::shared_ptr< J >, K >::
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

    boost::multi_index_container< std::shared_ptr< J >, K > _val;

};
#endif //SWIG

} //end Dynamic
} //end Poco
