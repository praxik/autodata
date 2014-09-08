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


// --- Boost Includes --- //
//These headers need to be first or the clang compiler errors out
#include <boost/tokenizer.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/spirit/include/qi.hpp>

// --- AutoData Includes --- //
#include <autodata/db/Query.h>

#include <autodata/dynamic/Table.h>

using namespace autodata::db;
using namespace autodata::util;

// --- Poco Includes --- //
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;

//These declerations must be here otherwise the clang compiler gets
//confused by the _1 in the spirit parser and the _1 in the multi-index headers
//included through autodata/dynamic/Table.h back to data helper
using namespace boost::spirit;

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
DefaultPolicy::DefaultPolicy()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DefaultPolicy::~DefaultPolicy()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DefaultSave::DefaultSave()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DefaultSave::~DefaultSave()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DBPolicy::DBPolicy()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DBPolicy::~DBPolicy()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void DBPolicy::Load(
    Statement& statement,
    Records& records )
{
    //
    Query query( statement );
    query.Execute();

    //
    Table< DBPolicy >& table =
        static_cast< Table< DBPolicy >& >( *this );
    records = query.ToRecords();
}
////////////////////////////////////////////////////////////////////////////////
DbSave::DbSave()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
DbSave::~DbSave()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void DbSave::Save(
    Session& session,
    std::string const& tableName,
    Records& records )
{
    Record& r = records.back();
    std::string tmp;
    if( tableName.empty() )
    {
        tmp = r.GetTypename();
    }
    else
    {
        tmp = tableName;
        r.SetTypename( tmp );
    }
    poco_assert( !tmp.empty() );
    r.CreateTable( session );

    //
    Query query = ( session
        << "insert into \"" << tmp << "\"(\n"
        <<    r.columns( 2 ) << " )\n"
        << "values(\n"
        <<    r.columns( 2, true ) << " )",
        useRef( records ) );
    query.Execute();
}
////////////////////////////////////////////////////////////////////////////////
FlatFilePolicy::FlatFilePolicy()
    :
    ConvertType( true ),
    HasHeader( true ),
    ColumnDelimiter( ' ' ),
    EscapeCharacter( '\\' ),
    HeaderDelimiter( ColumnDelimiter ),
    RowDelimiter( '\n' ),
    TextQualifier( '\"' ),
    HeaderRowsToSkip( 0 ),
    DataRowsToSkip( 0 ),
    EmptyLineCharacters( " \t\v\r\n" ),
    m_header()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
FlatFilePolicy::~FlatFilePolicy()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
void FlatFilePolicy::Load(
    std::ifstream& ifs,
    Records& records )
{
    //
    records.clear();

    //
    poco_assert( ifs.is_open() );

    //Declare variables
    bool const hDelimWS = ( HeaderDelimiter == ' ' );
    bool const DelimWS = ( ColumnDelimiter == ' ' );
    std::string line;
    typedef boost::escaped_list_separator< char > Sep;
    typedef boost::tokenizer< Sep > Toker;
    Sep const hsep( EscapeCharacter, HeaderDelimiter, TextQualifier );
    Sep const sep( EscapeCharacter, ColumnDelimiter, TextQualifier );
    Toker tok( line );

    if( m_header.empty() )
    {
        //
        unsigned int skip = 0;
        std::streampos spos = ifs.tellg();
        while( std::getline( ifs, line ) )
        {
            //Skip user defined blank lines
            if( line.find_first_not_of( EmptyLineCharacters ) ==
                std::string::npos ) continue;

            //Skip user defined number of header rows
            if( skip++ < HeaderRowsToSkip ) continue;

            //Strip off the column names
            unsigned int idx = 0;
            if( hDelimWS ) boost::trim( line );
            tok.assign( line.begin(), line.end(), hsep );
            for( auto const& t : tok )
            {
                std::string s = t; //Don't like this copy
                if( hDelimWS )
                {
                    if( s.empty() ) continue;
                }
                else
                {
                    boost::trim( s );
                }
                m_header.push_back( ( HasHeader ) ?
                    s : ( "Column_" + std::to_string( idx++ ) ) );
            }
            break;
        }
        //Rewind a line if we have no header
        if( !HasHeader ) ifs.seekg( spos );
        poco_assert( !m_header.empty() );
    }

    //Get the values
    unsigned int skip = 0;
    while( std::getline( ifs, line ) )
    {
        //Skip blank lines
        if( line.find_first_not_of( EmptyLineCharacters ) == std::string::npos )
            continue;

        //Skip user defined number of data rows
        if( skip++ < DataRowsToSkip ) continue;

        //Push back new record
        Record record;
        unsigned int idx = 0;
        if( DelimWS ) boost::trim( line );
        tok.assign( line.begin(), line.end(), sep );
        for( auto const& t : tok )
        {
            std::string s = t; //Don't like this copy
            if( DelimWS )
            {
                if( s.empty() ) continue;
            }
            else
            {
                boost::trim( s );
            }
            record[ m_header.at( idx++ ) ] =
                ( ConvertType ) ? TryCast( s ) : Var( s );
        }
        records.push_back( std::move( record ) );
    }

    ifs.close();
}
////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > const& FlatFilePolicy::GetHeader() const
{
    return m_header;
}
////////////////////////////////////////////////////////////////////////////////
void FlatFilePolicy::SetHeader(
    std::vector< std::string > header )
{
    m_header = std::move( header );
}
////////////////////////////////////////////////////////////////////////////////
Var FlatFilePolicy::TryCast(
    std::string const& s )
{
    typedef boost::variant< bool, int, double, std::string > A;
    auto f( std::begin( s ) ), l( std::end( s ) );
    static auto const qichar = qi::char_( "0-9a-fA-F" );

    static qi::rule< decltype( f ), std::string() > const guid_rule  = qi::raw[
        qi::repeat( 8 )[ qichar ] > '-' >
        qi::repeat( 4 )[ qichar ] > '-' >
        qi::repeat( 4 )[ qichar ] > '-' >
        qi::repeat( 4 )[ qichar ] > '-' >
        qi::repeat( 12 )[ qichar ] ];
    static qi::real_parser< double,
        qi::strict_real_policies< double > > const double_rule;
    static auto const int_rule = qi::int_;
    static auto const bool_rule = qi::bool_;
    static qi::rule< decltype( f ), A() > const p =
        guid_rule | double_rule | int_rule | bool_rule;

    A a;
    if( !qi::parse( f, l, p, a ) ) return s;
    switch( a.which() )
    {
        case 0: return boost::get< bool >( a );
        case 1: return boost::get< int >( a );
        case 2: return boost::get< double >( a );
        case 3: return boost::get< std::string >( a );
        default: throw std::runtime_error( "" );
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end dynamic
} //end autodata
