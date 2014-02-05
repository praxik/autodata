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

// --- Ix++ Includes --- //
#include <cpplinq/linq.hpp>

// --- Standard Includes --- //
#include <limits>

#ifndef SWIG
#ifndef isnan
AUTODATA_EXPORTS bool isnan(
    double v );
#endif
#endif //SWIG

double const NaN = std::numeric_limits< double >::quiet_NaN();

#ifdef _MSC_VER
#ifndef SWIG
namespace std
{

///
AUTODATA_EXPORTS double round(
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

template<>
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
        TypeHandler< Dynamic::Var >::extract( pos, tmp, _default, getExtractor() );
        //_rResult[ pos ] = tmp;
        return 1u;
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

};
#endif //SWIG

} //end Data
} //end Poco
