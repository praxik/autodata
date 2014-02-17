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
#include <autodata/dynamic/Table.h>

// --- Poco Includes --- //
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;

namespace autodata
{
namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
Table::Table()
    :
    Statement( NULL ),
    m_records()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Table::Table(
    Session& session )
    :
    Statement( session ),
    m_records()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Table::Table(
    Statement const& statement )
    :
    Statement( statement ),
    m_records()
{
    execute();
}
////////////////////////////////////////////////////////////////////////////////
Table::Table(
    Table const& o )
    :
    Statement( o.impl() )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Table::~Table()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Record& Table::operator [](
    unsigned int pos )
{
    return m_records.at( pos );
}
////////////////////////////////////////////////////////////////////////////////
Record const& Table::operator [](
    unsigned int pos ) const
{
    return m_records.at( pos );
}
////////////////////////////////////////////////////////////////////////////////
Table::iterator Table::begin()
{
    return m_records.begin();
}
////////////////////////////////////////////////////////////////////////////////
Table::const_iterator Table::begin() const
{
    return m_records.begin();
}
////////////////////////////////////////////////////////////////////////////////
MetaColumn::ColumnDataType Table::columnType(
    std::size_t pos ) const
{
    return metaColumn( static_cast< Poco::UInt32 >( pos ) ).type();
}
////////////////////////////////////////////////////////////////////////////////
void Table::clear()
{
    m_records.clear();
}
////////////////////////////////////////////////////////////////////////////////
Table::iterator Table::end()
{
    return m_records.end();
}
////////////////////////////////////////////////////////////////////////////////
Table::const_iterator Table::end() const
{
    return m_records.end();
}
////////////////////////////////////////////////////////////////////////////////
std::size_t Table::execute(
    bool reset )
{
    m_records.clear();
    std::size_t size = Statement::execute( reset );

    size_t colcnt = columnsExtracted();
    size_t rowcnt = rowsExtracted();
    m_records.reserve( rowcnt );
    for( std::size_t rowidx = 0; rowidx < rowcnt; ++rowidx )
    {
        Record record;
        for( std::size_t colidx = 0; colidx < colcnt; ++colidx )
        {
            MetaColumn const& mc =
                metaColumn( static_cast< Poco::UInt32 >( colidx ) );
            record[ mc.name() ] = value( colidx, rowidx );
        }
        m_records.push_back( std::move( record ) );
    }

    Statement::reset( Statement::session() );
    return size;
}
////////////////////////////////////////////////////////////////////////////////
void Table::push_back(
    Record record )
{
    m_records.push_back( std::move( record ) );
}
////////////////////////////////////////////////////////////////////////////////
Var Table::value(
    std::size_t col,
    std::size_t row ) const
{
    if( isNull( col, row ) ) return Var();

    switch( columnType( col ) )
    {
        case MetaColumn::FDT_BOOL: return value< bool >( col, row );
        case MetaColumn::FDT_INT8: return value< Poco::Int8 >( col, row );
        case MetaColumn::FDT_UINT8: return value< Poco::UInt8 >( col, row );
        case MetaColumn::FDT_INT16: return value< Poco::Int16 >( col, row );
        case MetaColumn::FDT_UINT16: return value< Poco::UInt16 >( col, row );
        case MetaColumn::FDT_INT32: return value< Poco::Int32 >( col, row );
        case MetaColumn::FDT_UINT32: return value< Poco::UInt32 >( col, row );
        case MetaColumn::FDT_INT64: return value< Poco::Int64 >( col, row );
        case MetaColumn::FDT_UINT64: return value< Poco::UInt64 >( col, row );
        case MetaColumn::FDT_FLOAT: return value< float >( col, row );
        case MetaColumn::FDT_DOUBLE: return value< double >( col, row );
        case MetaColumn::FDT_STRING: return value< std::string >( col, row );
        case MetaColumn::FDT_BLOB: return value< BLOB >( col, row );
        case MetaColumn::FDT_CLOB: return value< CLOB >( col, row );
        case MetaColumn::FDT_DATE: return value< Date >( col, row );
        case MetaColumn::FDT_TIME: return value< Time >( col, row );
        case MetaColumn::FDT_TIMESTAMP: return value< Poco::DateTime >( col, row );
        default: throw UnknownTypeException( "Data type not supported." );
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
