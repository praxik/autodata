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

#include <autodata/dynamic/Record.h>

// --- Poco Includes --- //
#include <Poco/JSON/Parser.h>

namespace autodata
{
namespace dynamic
{

class DefaultLoad;
typedef DefaultLoad DefaultPolicy;
class DefaultSave;

///
template<
    typename LoadPolicy = DefaultLoad,
    typename SavePolicy = DefaultSave >
class Table : public LoadPolicy, SavePolicy
{
public:
    ///
    typedef Records::iterator iterator;
    typedef Records::const_iterator const_iterator;
    typedef Records::size_type size_type;
    typedef Records::value_type value_type;

    ///
    Table()
        :
        LoadPolicy(),
        SavePolicy(),
        m_records()
    {
        ;
    }

    ///
    template< typename T = LoadPolicy, typename =
        typename std::enable_if< std::is_same< T, DefaultLoad >::value >::type >
    Table(
        Records::size_type cnt,
        Records::value_type const& val )
        :
        LoadPolicy(),
        SavePolicy(),
        m_records( cnt, val )
    {
        ;
    }

    ///
    template< typename T = LoadPolicy, typename =
        typename std::enable_if< std::is_same< T, DefaultLoad >::value >::type >
    Table(
        Records records )
        :
        LoadPolicy(),
        SavePolicy(),
        m_records( std::move( records ) )
    {
        ;
    }

    ///copy constructor
    Table(
        Table const& o )
        :
        LoadPolicy( o ),
        SavePolicy( o ),
        m_records( o.m_records )
    {
        ;
    }

    ///move constructor
    Table(
        Table&& o )
        :
        LoadPolicy( std::move( o ) ),
        SavePolicy( std::move( o ) ),
        m_records( std::move( o.m_records ) )
    {
        ;
    }

    ///assignment operator
    //Table& operator =( Table ) = default;

    ///
    ~Table()
    {
        ;
    }

    ///
    operator Records&()
    {
        return m_records;
    }

    ///
    operator Records const&() const
    {
        return m_records;
    }

    ///
    Record& operator [](
        unsigned int pos )
    {
        return m_records.at( pos );
    }

    ///
    Record const& operator [](
        unsigned int pos ) const
    {
        return m_records.at( pos );
    }

    ///
    iterator begin()
    {
        return m_records.begin();
    }

    ///
    const_iterator begin() const
    {
        return m_records.begin();
    }

    ///
    void clear()
    {
        m_records.clear();
    }

    ///
    template< typename T >
    std::vector< T > col(
        std::string const& name,
        T const& defVal = T() ) const
    {
        return cpplinq::
            from( m_records ).
            select( [ &name, &defVal ]( Record const& record )
            {
                return util::Convert< T >( record[ name ], defVal );
            } ).to_vector();
    }

    ///
    bool empty() const
    {
        return m_records.empty();
    }

    ///
    iterator end()
    {
        return m_records.end();
    }

    ///
    const_iterator end() const
    {
        return m_records.end();
    }

    ///
    void FromJson(
        std::string const& json )
    {
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse( json );
        poco_assert( result.type() == typeid( Poco::JSON::Array::Ptr ) );
        m_records = *result.extract< Poco::JSON::Array::Ptr >();
    }

    ///
    template< typename T >
    void Load(
        T&& t )
    {
        LoadPolicy::Load( std::forward< T >( t ), m_records );
    }

    ///
    template< typename T >
    void Save(
        T&& t,
        std::string const& tableName = "" )
    {
        SavePolicy::Save( std::forward< T >( t ), tableName, m_records );
    }

    ///
    void push_back(
        Record record )
    {
        m_records.push_back( std::move( record ) );
    }

    ///
    size_type size() const
    {
        return m_records.size();
    }

    ///
    std::string ToJson() const
    {
        std::string json;
        Var( *this ).template convert< std::string >( json );
        return json;
    }

protected:

private:
    ///
    friend LoadPolicy;
    friend SavePolicy;

    ///
    Records m_records;

};

///For cpplinq use
template< typename LoadPolicy, typename SavePolicy > inline
auto begin(
    Table< LoadPolicy, SavePolicy >& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy, typename SavePolicy > inline
auto begin(
    Table< LoadPolicy, SavePolicy > const& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy, typename SavePolicy > inline
auto end(
    Table< LoadPolicy, SavePolicy >& o ) -> decltype( o.end() )
{
    return o.end();
}

///For cpplinq use
template< typename LoadPolicy, typename SavePolicy > inline
auto end(
    Table< LoadPolicy, SavePolicy > const& o ) -> decltype( o.end() )
{
    return o.end();
}

///
class AUTODATA_EXPORTS DefaultLoad
{
public:
    ///
    DefaultLoad();

    ///
    void Load(
        Records from,
        Records& to );

protected:
    ///
    ~DefaultLoad();

private:

};

///
class AUTODATA_EXPORTS DefaultSave
{
public:
    ///
    DefaultSave();

protected:
    ///
    ~DefaultSave();

private:

};

///
class AUTODATA_EXPORTS DbLoad
{
public:
    ///
    DbLoad();

    ///
    void Load(
        Poco::Data::Statement& statement,
        Records& records );

protected:
    ///
    ~DbLoad();

private:

};
typedef DbLoad DBPolicy;

///
class AUTODATA_EXPORTS DbSave
{
public:
    ///
    DbSave();

    ///
    void Save(
        Poco::Data::Session& session,
        std::string const& tableName,
        Records& records );

protected:
    ///
    ~DbSave();

private:

};

///
class AUTODATA_EXPORTS FlatFilePolicy
{
public:
    ///
    FlatFilePolicy();

    ///
    void Load(
        std::ifstream& ifs,
        Records& records );

    ///
    std::vector< std::string > const& GetHeader() const;

    ///
    void SetHeader(
        std::vector< std::string > header );

    ///Specify whether to try conversion from string to a numerical value
    bool ConvertType;

    ///Specify if the file has a header with column values
    bool HasHeader;

    ///Specify the delimiter character for data rows
    char ColumnDelimiter;

    ///Specify the character used for escaping in text fields
    char EscapeCharacter;

    ///Specify the delimiter character for the header row
    char HeaderDelimiter;

    ///Specify the character used for quoting text fields
    char TextQualifier;

    ///Specify the number of nonblank header rows to skip
    unsigned int HeaderRowsToSkip;

    ///Specify the number of nonblank data rows to skip
    unsigned int DataRowsToSkip;

    ///Specify characters to ignore when looking for empty lines
    std::string EmptyLineCharacters;

protected:
    ///
    Poco::Dynamic::Var TryCast(
        std::string const& s );

    ///
    ~FlatFilePolicy();

private:
    ///
    char RowDelimiter;

    ///
    std::vector< std::string > m_header;

};

} //end dynamic
} //end autodata

namespace Poco
{
namespace Data
{

template<>
class Binding< autodata::dynamic::Records > : public AbstractBinding
{
public:
    typedef autodata::dynamic::Records ValType;
    typedef SharedPtr< ValType > ValPtr;
    typedef SharedPtr< Binding< ValType > > Ptr;
    typedef ValType::const_iterator Iterator;

    explicit Binding(
        ValType const& val,
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
        return _val.back().size();
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
        ValType::value_type const& def,
        Position const& pos = Position( 0 ) )
        :
        AbstractExtraction( Limit::LIMIT_UNLIMITED, pos.value() ),
        _rResult( result ),
        _default( def )
    {
        _rResult.clear();
    }

    ~Extraction()
    {
        ;
    }

    std::size_t numOfColumnsHandled() const
    {
        //This is probably bad - need to fix this
        return _rResult.back().size();
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
        return new Preparation< ValType::value_type >(
            pPrep, pos, _default );
    }

protected:
    ValType const& result() const
    {
        return _rResult;
    }

private:
    ValType& _rResult;
    ValType::value_type _default;
    std::deque< bool > _nulls;
};

} //end Data

namespace Dynamic
{

template< typename T, typename U >
class VarHolderImpl< autodata::dynamic::Table< T, U > > : public VarHolder
{
public:
    VarHolderImpl(
        autodata::dynamic::Table< T, U > const& val )
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
        return typeid( autodata::dynamic::Table< T, U > );
    }

    void convert( Int8& ) const
    {
        throw BadCastException( "Cannot cast Table type to Int8" );
    }

    void convert( Int16& ) const
    {
        throw BadCastException( "Cannot cast Table type to Int16" );
    }

    void convert( Int32& ) const
    {
        throw BadCastException( "Cannot cast Table type to Int32" );
    }

    void convert( Int64& ) const
    {
        throw BadCastException( "Cannot cast Table type to Int64" );
    }

    void convert( UInt8& ) const
    {
        throw BadCastException( "Cannot cast Table type to UInt8" );
    }

    void convert( UInt16& ) const
    {
        throw BadCastException( "Cannot cast Table type to UInt16" );
    }

    void convert( UInt32& ) const
    {
        throw BadCastException( "Cannot cast Table type to UInt32" );
    }

    void convert( UInt64& ) const
    {
        throw BadCastException( "Cannot cast Table type to UInt64" );
    }

    void convert( bool& ) const
    {
        throw BadCastException( "Cannot cast Table type to bool" );
    }

    void convert( float& ) const
    {
        throw BadCastException( "Cannot cast Table type to float" );
    }

    void convert( double& ) const
    {
        throw BadCastException( "Cannot cast Table type to double" );
    }

    void convert( char& ) const
    {
        throw BadCastException( "Cannot cast Table type to char" );
    }

    void convert( std::string& val ) const
    {
        val.append( "[ " );
        typename autodata::dynamic::Table< T, U >::const_iterator it = _val.begin();
        typename autodata::dynamic::Table< T, U >::const_iterator itEnd = _val.end();
        if( !_val.empty() )
        {
            val.append( it->ToJson() );
            ++it;
        }
        for( ; it != itEnd; ++it )
        {
            val.append( ", " );
            val.append( it->ToJson() );
        }
        val.append( " ]" );
    }

    void convert( Poco::DateTime& ) const
    {
        throw BadCastException( "Table -> Poco::DateTime" );
    }

    void convert( Poco::LocalDateTime& ) const
    {
        throw BadCastException( "Table -> Poco::LocalDateTime" );
    }

    void convert( Poco::Timestamp& ) const
    {
        throw BadCastException( "Table -> Poco::Timestamp" );
    }

    VarHolder* clone( Placeholder< VarHolder >* pVarHolder = 0 ) const
    {
        return cloneHolder( pVarHolder, _val );
    }

    autodata::dynamic::Table< T, U > const& value() const
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

    std::size_t size() const
    {
        return _val.size();
    }

    T& operator[](
        typename autodata::dynamic::Table< T, U >::size_type n )
    {
        if( n < size() ) return _val.operator []( n );

        throw RangeException( "Vector index out of range" );
    }

    T const& operator[](
        typename autodata::dynamic::Table< T, U >::size_type n ) const
    {
        if( n < size() ) return _val.operator []( n );

        throw RangeException( "Vector index out of range" );
    }

private:
    VarHolderImpl();
    VarHolderImpl( VarHolderImpl const& );
    VarHolderImpl& operator =( VarHolderImpl const& );

    ///
    autodata::dynamic::Table< T, U > _val;

};

} // end Dynamic
} // end Poco
