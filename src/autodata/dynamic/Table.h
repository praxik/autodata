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

namespace autodata
{
namespace dynamic
{

///
template< typename LoadPolicy >
class Table : public LoadPolicy
{
public:
    ///
    typedef Records::iterator iterator;
    typedef Records::const_iterator const_iterator;
    typedef Records::value_type value_type;

    ///
    template< typename T >
    Table(
        T&& t )
        :
        LoadPolicy( std::forward< T >( t ) ),
        m_records()
    {
        ;
    }

    ///assignment operator
    Table& operator =(
        Records records )
    {
        m_records = std::move( records );
        return *this;
    }

    ///
    ~Table()
    {
        ;
    }

    ///
    inline
    Record& operator [](
        unsigned int pos )
    {
        return m_records.at( pos );
    }

    ///
    inline
    Record const& operator [](
        unsigned int pos ) const
    {
        return m_records.at( pos );
    }

    ///
    inline
    iterator begin()
    {
        return m_records.begin();
    }

    ///
    inline
    const_iterator begin() const
    {
        return m_records.begin();
    }

    ///
    inline
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
    inline
    iterator end()
    {
        return m_records.end();
    }

    ///
    inline
    const_iterator end() const
    {
        return m_records.end();
    }

    ///
    inline
    void Load()
    {
        In();
    }

    ///
    inline
    void push_back(
        Record record )
    {
        m_records.push_back( std::move( record ) );
    }

protected:

private:
    ///
    friend LoadPolicy;

    ///copy constructor
    Table( Table const& );// = delete;

    ///
    Records m_records;

};

///For cpplinq use
template< typename LoadPolicy > inline
auto begin(
    autodata::dynamic::Table< LoadPolicy >& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto begin(
    autodata::dynamic::Table< LoadPolicy > const& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto end(
    autodata::dynamic::Table< LoadPolicy >& o ) -> decltype( o.end() )
{
    return o.end();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto end(
    autodata::dynamic::Table< LoadPolicy > const& o ) -> decltype( o.end() )
{
    return o.end();
}

///
class AUTODATA_EXPORTS LoadPolicyDB : private Poco::Data::Statement
{
public:
    ///
    void In();

protected:
    ///
    explicit LoadPolicyDB(
        Poco::Data::Statement const& statement );

    ///
    ~LoadPolicyDB();

private:
    ///copy constructor
    LoadPolicyDB( LoadPolicyDB const& );// = delete;

    ///assignment operator
    LoadPolicyDB& operator =( LoadPolicyDB );// = delete;

    ///Returns the reference to column at specified position
    template< typename C >
    Poco::Data::Column< C > const& column(
        std::size_t pos ) const
    {
        if( isBulkExtraction() )
        {
            typedef InternalBulkExtraction< C > E;
            return columnImpl< C, E >( pos );
        }
        else
        {
            typedef InternalExtraction< C > E;
            return columnImpl< C, E >( pos );
        }
    }

    ///Returns the reference to column at specified position
    template< typename C, typename E >
    Poco::Data::Column< C > const& columnImpl(
        std::size_t pos ) const
    {
        typedef typename C::value_type T;
        typedef E const* ExtractionVecPtr;

        Poco::Data::AbstractExtractionVec const& rExtractions = extractions();
        std::size_t s = rExtractions.size();
        if( 0 == s || pos >= s )
        {
            throw Poco::RangeException(
                Poco::format( "Invalid column index: %z", pos ) );
        }

        ExtractionVecPtr pExtraction =
            dynamic_cast< ExtractionVecPtr >( rExtractions[ pos ].get() );
        if( pExtraction )
        {
            return pExtraction->column();
        }
        else
        {
            throw Poco::BadCastException( Poco::format(
                "Type cast failed!\nColumn: %z\nTarget type:\t%s",
                pos,
                std::string( typeid( T ).name() ) ) );
        }
    }

    ///Returns the type for the column at specified position
    Poco::Data::MetaColumn::ColumnDataType columnType(
        std::size_t pos ) const;

    ///
    std::size_t execute();

    ///Returns the data value at column, row location
    Poco::Dynamic::Var value(
        std::size_t col,
        std::size_t row ) const;

    ///Returns the reference to data value at [col, row] location
    template< typename T >
    T const& value(
        std::size_t col,
        std::size_t row ) const
    {
        switch( storage() )
        {
            case STORAGE_VECTOR:
            {
                typedef typename std::vector< T > C;
                return column< C >( col ).value( row );
            }
            case STORAGE_LIST:
            {
                typedef typename std::list< T > C;
                return column< C >( col ).value( row );
            }
            case STORAGE_DEQUE:
            case STORAGE_UNKNOWN:
            {
                typedef typename std::deque< T > C;
                return column< C >( col ).value( row );
            }
            default:
                throw Poco::IllegalStateException( "Invalid storage setting." );
        }
    }

};

///
class AUTODATA_EXPORTS LoadPolicyIStream
{
public:
    ///
    void In()
    {
        ;
    }

protected:
    ///
    LoadPolicyIStream(
        std::istream& is )
        :
        m_is( is )
    {
        ;
    }

private:
    ///
    std::istream& m_is;

};

} //end dynamic
} //end autodata
