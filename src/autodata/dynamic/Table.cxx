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

// --- Boost Includes --- //
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/spirit/include/qi.hpp>

#include <boost/timer/timer.hpp>

using namespace boost::spirit;
using namespace boost::timer;

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
    Query query( statement );
    query.Execute();

    //
    Table< DBPolicy >& table =
        static_cast< Table< DBPolicy >& >( *this );
    table.m_records = std::move( query.ToRecords() );
}
////////////////////////////////////////////////////////////////////////////////
void IFStreamPolicy::Load(
    std::ifstream& ifs )
{
    auto_cpu_timer timer;

    //
    Table< IFStreamPolicy >& table =
        static_cast< Table< IFStreamPolicy >& >( *this );
    table.m_records.clear();

    //
    poco_assert( ifs.is_open() );

    //Declare variables
    bool DelimWS = ( DelimChar == ' ' );
    std::string line;
    typedef boost::escaped_list_separator< char > Sep;
    typedef boost::tokenizer< Sep > Toker;
    Sep const sep( EscapeChar, DelimChar, QuoteChar );
    Toker tok( line, sep );

    if( HasHeader && m_header.empty() )
    {
        //Read until we find the first line with something...
        while( std::getline( ifs, line ) )
        {
            if( line.find_first_not_of( EmptyLineChars ) == std::string::npos )
                continue;

            //Strip off the column names
            if( DelimWS ) boost::trim( line );
            tok.assign( line.begin(), line.end() );
            for( auto const& s : tok )
            {
                if( DelimWS && s.empty() ) continue;
                m_header.push_back( s );
            }
            break;
        }
    }

    //Get the values
    while( std::getline( ifs, line ) )
    {
        //Skip blank lines
        if( line.find_first_not_of( EmptyLineChars ) == std::string::npos )
            continue;

        //Push back new record
        Record record;
        unsigned int idx = 0;
        if( DelimWS ) boost::trim( line );
        tok.assign( line.begin(), line.end() );
        for( auto const& s : tok )
        {
            if( DelimWS && s.empty() ) continue;
            record[ m_header.at( idx++ ) ] = ( ConvertType ) ? TryCast( s ) : s;
        }
        table.m_records.push_back( std::move( record ) );
    }

    ifs.close();
}
////////////////////////////////////////////////////////////////////////////////
Var IFStreamPolicy::TryCast(
    std::string const& s )
{
    static qi::real_parser< double, qi::strict_real_policies< double > > real_;
    std::string::const_iterator b = s.begin();
    std::string::const_iterator e = s.end();
    double d; if( qi::parse( b, e, real_, d ) && ( b == e ) ) return d;
    b = s.begin(); //reset
    int i; if( qi::parse( b, e, qi::int_, i ) && ( b == e ) ) return i;
    return s;
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
