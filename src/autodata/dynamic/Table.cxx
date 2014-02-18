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

#include <autodata/dynamic/Table.h>

using namespace autodata::db;
using namespace autodata::util;

// --- Poco Includes --- //
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;

// --- Standard Includes --- //
#include <iomanip>
#include <iostream>
#include <sstream>

namespace autodata
{
namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
LoadPolicyDB::LoadPolicyDB(
    Statement const& statement )
    :
    m_statement( statement )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
LoadPolicyDB::~LoadPolicyDB()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void LoadPolicyDB::In()
{
    Query query( m_statement );
    query.Execute();
    static_cast< Table< LoadPolicyDB >& >( *this ) =
        std::move( query.ToRecords() );
}
////////////////////////////////////////////////////////////////////////////////
void LoadPolicyIStream::In()
{
    //
    //poco_assert( m_is.is_open() );

    //Strip off the column names
    //DailyDayCent_list100/Table.f names contain no spaces, so this is safe
    unsigned int idx = 0;
    std::string col;
    std::string line; std::getline( m_is, line );
    std::istringstream iss( line );
    std::map< unsigned int, std::string > columns;
    while( iss >> col )
    {
        columns.insert( std::make_pair( idx++, col ) );
    }
    //Skip the blank line between column names and values
    std::getline( m_is, line );
    Records records; records.reserve( columns.size() );
    while( std::getline( m_is, line ) )
    {
        iss.str( line ); iss.clear();
        float value;
        Record record;
        idx = 0;
        while( iss >> value )
        {
            record[ columns.at( idx++ ) ] = value;
        }
        records.push_back( std::move( record ) );
    }
    static_cast< Table< LoadPolicyIStream >& >( *this ) =
        std::move( records );
    //m_is.close();
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
