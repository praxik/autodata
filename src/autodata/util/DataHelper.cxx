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

// --- Standard Includes --- //
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
bool isnan(
    double v )
{
    return v != v;
}
////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
namespace std
{

////////////////////////////////////////////////////////////////////////////////
double round(
    double const v )
{
    return v < 0.0 ? ceil( v - 0.5 ) : floor( v + 0.5 );
}
////////////////////////////////////////////////////////////////////////////////

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
    std::ifstream ifs( filename, std::ios::in | std::ios::binary );
    if( !ifs ) throw( errno );

    std::string contents;
    ifs.seekg( 0, std::ios::end );
    contents.resize( static_cast< std::size_t >( ifs.tellg() ) );
    ifs.seekg( 0, std::ios::beg );
    ifs.read( &contents[ 0 ], contents.size() );
    ifs.close();
    return contents;
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
