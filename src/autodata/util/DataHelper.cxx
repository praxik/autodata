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
std::istream& operator >>(
    std::istream& is,
    Var& o )
{
    std::string s;
    is >> s;

    try{ int i = boost::lexical_cast< int >( s ); o = i; return is; }
    catch( boost::bad_lexical_cast& ){;}

    try{ double d = boost::lexical_cast< double >( s ); o = d; return is; }
    catch( boost::bad_lexical_cast& ){;}

    o = std::move( s );
    return is;
}
////////////////////////////////////////////////////////////////////////////////

} //end Dynamic
} //end Poco
