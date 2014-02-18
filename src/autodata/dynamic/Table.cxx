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
#include <fstream>
#include <sstream>

namespace autodata
{
namespace dynamic
{

////////////////////////////////////////////////////////////////////////////////
void DBPolicy::Load(
    Statement& statement )
{
    //
    Table< DBPolicy >& table =
        static_cast< Table< DBPolicy >& >( *this );

    //
    Query query( statement );
    query.Execute();
    table.m_records = std::move( query.ToRecords() );
}
////////////////////////////////////////////////////////////////////////////////
void IFStreamPolicy::Load(
    std::ifstream& ifs )
{
    //
    Table< IFStreamPolicy >& table =
        static_cast< Table< IFStreamPolicy >& >( *this );

    //
    poco_assert( ifs.is_open() );

    //Line characters to ignore
    std::string const iggy = " \t\v\r\n";

    //Read until we find the first line with something...
    std::string line;
    while( std::getline( ifs, line ) )
    {
        if( line.find_first_not_of( iggy ) != std::string::npos ) break;
    }

    //Strip off the column names
    std::string name;
    std::istringstream iss( line );
    std::vector< std::string > columns;
    while( iss >> name ) columns.push_back( name );

    //Get the values
    Records records; records.reserve( columns.size() );
    while( std::getline( ifs, line ) )
    {
        //Skip blank lines
        if( line.find_first_not_of( iggy ) == std::string::npos ) continue;

        iss.str( line ); iss.clear();
        Var value;
        Record record;
        unsigned int idx = 0;
        while( iss >> value ) record[ columns.at( idx++ ) ] = value;
        records.push_back( std::move( record ) );
    }
    table.m_records = std::move( records );

    ifs.close();
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
