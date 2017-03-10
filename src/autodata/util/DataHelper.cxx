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
#include <autodata/util/DataHelper.h>

using namespace autodata::util;

// --- POCO Includes --- //
using namespace Poco::Data;

// --- Stl Includes --- //
#include <cctype>
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
#if !defined isnan && !defined ISNAN_EXISTS
bool isnan(
    double v )
{
    return v != v;
}
#endif
////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
namespace std
{
#if _MSC_VER < 1800
////////////////////////////////////////////////////////////////////////////////
double round(
    double const v )
{
    return v < 0.0 ? ceil( v - 0.5 ) : floor( v + 0.5 );
}
////////////////////////////////////////////////////////////////////////////////
#endif
} //end std
#endif

namespace autodata
{
namespace util
{

////////////////////////////////////////////////////////////////////////////////
bool only_1_bit(
    int x )
{
    return ( x > 0 ) && !( x & ( x - 1 ) );
}
////////////////////////////////////////////////////////////////////////////////
std::string get_file_contents(
    std::string const& filename )
{
    std::ifstream ifs;
    //Prepare to throw if failbit gets set
    ifs.exceptions( ifs.exceptions() | std::ios::failbit );
    try
    {
        ifs.open( filename, std::ios::in | std::ios::binary );
    }
    catch( std::ios_base::failure const& e )
    {
        std::ostringstream oss;
        oss << "autodata::util::get_file_contents: " << std::endl
            << "  " << std::strerror( errno ) << std::endl
            << "  " << e.what();
        throw std::runtime_error( oss.str() );
    }

    std::string contents;
    ifs.seekg( 0, std::ios::end );
    contents.resize( static_cast< std::size_t >( ifs.tellg() ) );
    ifs.seekg( 0, std::ios::beg );
    ifs.read( &contents[ 0 ], contents.size() );
    ifs.close();
    return contents;
}
////////////////////////////////////////////////////////////////////////////////
std::string escape_json(
    std::string const& json )
{
    if( json.empty() ) return json;
    std::ostringstream oss;
    for( auto itr = json.cbegin(); itr != json.cend(); ++itr )
    {
        switch( *itr )
        {
            case '\\': oss << "\\\\"; break;
            case '"': oss << "\\\""; break;
            //case '/': oss << "\\/"; break;
            case '\b': oss << "\\b"; break;
            case '\t': oss << "\\t"; break;
            case '\n': oss << "\\n"; break;
            case '\f': oss << "\\f"; break;
            case '\r': oss << "\\r"; break;
            default:
            {
                if( *itr < ' ' )
                {
                    oss << "\\u"
                        << std::uppercase
                        << std::setfill( '0' )
                        << std::setw( 4 )
                        << std::hex
                        << static_cast< unsigned int >( *itr );
                }
                else
                {
                    oss << *itr;
                }
                break;
            }
        }
    }
    return oss.str();
}
////////////////////////////////////////////////////////////////////////////////
bool icase::operator ()(
    std::string const& s1,
    std::string const& s2 ) const
{
    return std::lexicographical_compare(
        s1.begin(), s1.end(), s2.begin(), s2.end(),
        []( char c1, char c2 ) -> bool
        {
            return std::tolower( c1 ) < std::tolower( c2 );
        } );
}
////////////////////////////////////////////////////////////////////////////////
bool icase::compare(
    std::string const& s1,
    std::string const& s2 )
{
    return ( ( s1.size() == s2.size() ) &&
        std::equal( s1.begin(), s1.end(), s2.begin(),
        []( char c1, char c2 ) -> bool
        {
            return std::tolower( c1 ) == std::tolower( c2 );
        } ) );
}
////////////////////////////////////////////////////////////////////////////////
std::string hex_to_bin(
    std::string const& s )
{
    std::string tmp;
    if( s.empty() ) return tmp;

    int x = 0;
    if( s.at( 0 ) == '0' && ( s.at( 1 ) == 'x' || s.at( 1 ) == 'X' ) ) x = 2;
    tmp.reserve( 4 * ( s.size() - x ) );
    for( auto itr( s.cbegin() + x ); itr != s.cend(); ++itr )
    {
        switch( *itr )
        {
            case '0': tmp.append( "0000" ); break;
            case '1': tmp.append( "0001" ); break;
            case '2': tmp.append( "0010" ); break;
            case '3': tmp.append( "0011" ); break;
            case '4': tmp.append( "0100" ); break;
            case '5': tmp.append( "0101" ); break;
            case '6': tmp.append( "0110" ); break;
            case '7': tmp.append( "0111" ); break;
            case '8': tmp.append( "1000" ); break;
            case '9': tmp.append( "1001" ); break;
            case 'a': tmp.append( "1010" ); break;
            case 'A': tmp.append( "1010" ); break;
            case 'b': tmp.append( "1011" ); break;
            case 'B': tmp.append( "1011" ); break;
            case 'c': tmp.append( "1100" ); break;
            case 'C': tmp.append( "1100" ); break;
            case 'd': tmp.append( "1101" ); break;
            case 'D': tmp.append( "1101" ); break;
            case 'e': tmp.append( "1110" ); break;
            case 'E': tmp.append( "1110" ); break;
            case 'f': tmp.append( "1111" ); break;
            case 'F': tmp.append( "1111" ); break;
            default: throw std::runtime_error( "autodata::util::hex_to_bin: "
                "input hex string contains invalid characters" ); break;
        }
    }
    return tmp;
}
////////////////////////////////////////////////////////////////////////////////
std::vector< unsigned char > hex_to_bytes(
    std::string const& hexstr )
{
    if( hexstr.size() % 2 != 0 )
    {
        throw std::runtime_error( "autodata::util::hex_to_bytes: "
            "input hex string contains incomplete bytes" );
    }

    std::vector< unsigned char > bytes;
    bytes.reserve( hexstr.size() / 2 );
    for( auto citr = hexstr.cbegin(); citr != hexstr.cend(); citr += 2 )
    {
        std::string tmp( citr, citr + 2 );
        unsigned int n = std::stoul( tmp, nullptr, 16 );
        bytes.push_back( static_cast< unsigned char >( n ) );
    }
    return bytes;
}
////////////////////////////////////////////////////////////////////////////////
hexchar::hexchar()
    :
    c()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
hexchar::hexchar(
    unsigned char in )
    :
    c( in )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    hexchar const& c )
{
    return os << std::hex << std::setw( 2 ) << std::setfill( '0' )
        << static_cast< unsigned int >( c.c );
}
////////////////////////////////////////////////////////////////////////////////
std::istream& operator >>(
    std::istream& is,
    hexchar& c )
{
    unsigned int n;
    is >> std::hex >> n;
    c.c = static_cast< unsigned char >( n );
    return is;
}
////////////////////////////////////////////////////////////////////////////////

} //end util
} //end autodata

namespace Poco
{
namespace Dynamic
{

////////////////////////////////////////////////////////////////////////////////
std::ostream& operator <<(
    std::ostream& os,
    Var const& o )
{
    if( o.isString() ) return os << o.extract< std::string >();
    return os << Convert< std::string >( o, "null" );
}
////////////////////////////////////////////////////////////////////////////////

} //end Dynamic
} //end Poco
