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

namespace autodata
{

namespace dynamic
{
class Record;
}

namespace db
{

#ifndef SWIG

///
class AUTODATA_EXPORTS Query : private Poco::Data::Statement
{
public:
    ///constructor
    Query(
        Poco::Data::Statement const& statement );

    ///copy constructor
    Query(
        Query const& o );

    ///assignment operator
    Query& operator =(
        Query const& o );

    ///destructor
    ~Query();

    ///
    std::size_t Execute();

    ///
    int MaxRetry;

    ///
    int Sleep;

    ///
    std::vector< dynamic::Record > ToRecords();

    ///
    std::string const& ToString() const;

protected:

private:
    ///Returns the reference to column at specified position
    template< typename C >
    Poco::Data::Column< C > const& column(
        std::size_t pos ) const
    {
        if( isBulkExtraction() )
        {
            typedef Poco::Data::InternalBulkExtraction< C > E;
            return columnImpl< C, E >( pos );
        }
        else
        {
            typedef Poco::Data::InternalExtraction< C > E;
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

#endif //SWIG

} //end db
} //end autodata
