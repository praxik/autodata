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


// --- AutoData Includes --- //
#include <autodata/db/Query.h>

#include <autodata/dynamic/Record.h>

using namespace autodata::dynamic;
using namespace autodata::util;

// --- POCO Includes --- //
using namespace Poco::Data;
using namespace Poco::Dynamic;

namespace autodata
{
namespace db
{

////////////////////////////////////////////////////////////////////////////////
Query::Query(
    Statement const& statement )
    :
    Statement( statement ),
    MaxRetry( 100 ),
    Sleep( 100 )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Query::Query(
    Query const& o )
    :
    Statement( o.impl() ),
    MaxRetry( o.MaxRetry ),
    Sleep( o.Sleep )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Query& Query::operator =(
    Query const& o )
{
    Statement::operator =( o );
    MaxRetry = o.MaxRetry;
    Sleep = o.Sleep;
    return *this;
}
////////////////////////////////////////////////////////////////////////////////
Query::~Query()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
MetaColumn::ColumnDataType Query::columnType(
    std::size_t pos ) const
{
    return metaColumn( static_cast< Poco::UInt32 >( pos ) ).type();
}
////////////////////////////////////////////////////////////////////////////////
std::size_t Query::Execute()
{
    //
    if( isAsync() )
    {
        throw std::runtime_error(
            "autodata::util::StmntObj does not support asynchronous queries" );
    }

    unsigned int cnt( 0 );
    std::size_t size( 0 );
    while( ++cnt <= MaxRetry )
    {
        try
        {
            if( cnt == 1 )
            {
                size = Statement::execute();
            }
            else
            {
                impl()->reset();
                size = impl()->execute( true );
            }
            return size;
        }
        catch( SQLite::DBLockedException const& ){;}
        catch( SQLite::TableLockedException const& ){;}
        catch( Poco::Exception const& ex ){ ex.rethrow(); }
        Poco::Thread::sleep( Sleep );
    }

    throw std::runtime_error( "autodata::util::Statement::execute() failed" );
}
////////////////////////////////////////////////////////////////////////////////
Records Query::ToRecords()
{
    size_t colcnt = columnsExtracted();
    size_t rowcnt = rowsExtracted();
    Records records; records.reserve( rowcnt );
    for( std::size_t rowidx = 0; rowidx < rowcnt; ++rowidx )
    {
        Record record;
        for( std::size_t colidx = 0; colidx < colcnt; ++colidx )
        {
            MetaColumn const& mc =
                metaColumn( static_cast< Poco::UInt32 >( colidx ) );
            record[ mc.name() ] = value( colidx, rowidx );
        }
        records.push_back( std::move( record ) );
    }
    return records;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Query::ToString() const
{
    return toString();
}
////////////////////////////////////////////////////////////////////////////////
Var Query::value(
    std::size_t col,
    std::size_t row ) const
{
    if( isNull( col, row ) ) return Var();

    switch( columnType( col ) )
    {
        case MetaColumn::FDT_BOOL:
            return value< bool >( col, row );
        case MetaColumn::FDT_INT8:
            return value< Poco::Int8 >( col, row );
        case MetaColumn::FDT_UINT8:
            return value< Poco::UInt8 >( col, row );
        case MetaColumn::FDT_INT16:
            return value< Poco::Int16 >( col, row );
        case MetaColumn::FDT_UINT16:
            return value< Poco::UInt16 >( col, row );
        case MetaColumn::FDT_INT32:
            return value< Poco::Int32 >( col, row );
        case MetaColumn::FDT_UINT32:
            return value< Poco::UInt32 >( col, row );
        case MetaColumn::FDT_INT64:
            return value< Poco::Int64 >( col, row );
        case MetaColumn::FDT_UINT64:
            return value< Poco::UInt64 >( col, row );
        case MetaColumn::FDT_FLOAT:
            return value< float >( col, row );
        case MetaColumn::FDT_DOUBLE:
            return value< double >( col, row );
        case MetaColumn::FDT_STRING:
            return value< std::string >( col, row );
        case MetaColumn::FDT_BLOB:
            return value< BLOB >( col, row );
        case MetaColumn::FDT_CLOB:
            return value< CLOB >( col, row );
        case MetaColumn::FDT_DATE:
            return value< Date >( col, row );
        case MetaColumn::FDT_TIME:
            return value< Time >( col, row );
        case MetaColumn::FDT_TIMESTAMP:
            return value< Poco::DateTime >( col, row );
        default:
            throw UnknownTypeException( "Data type not supported" );
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end db
} //end autodata
