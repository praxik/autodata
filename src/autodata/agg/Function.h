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

// --- Autodata Includes --- //
#include <autodata/AutoDataConfig.h>
#include <autodata/CompilerGuards.h>

// --- Standard Includes --- //
#include <utility>

namespace autodata
{
namespace agg
{

///
AUTODATA_EXPORTS
int vcount();

///
template< typename T, typename... Ts >
int vcount(
    T&& v,
    Ts&&... vs )
{
  return 1 + vcount( std::forward< Ts >( vs )... );
}

///
template< typename T, typename =
    typename std::enable_if< std::is_arithmetic< T >::value >::type >
T vsum(
    T v )
{
    return v;
}

///
template< typename T, typename... Ts, typename =
    typename std::enable_if< std::is_arithmetic< T >::value >::type >
typename std::common_type< T, Ts... >::type vsum(
    T v,
    Ts... vs )
{
    return v + vsum( vs... );
}

///
template< typename T, typename =
    typename std::enable_if< std::is_arithmetic< T >::value >::type >
T vavg(
    T v )
{
    return v;
}

///
template< typename T, typename... Ts, typename =
    typename std::enable_if< std::is_arithmetic< T >::value >::type >
double vavg(
    T v,
    Ts... vs )
{
    return vsum( v, vs... ) / (double)vcount( v, vs... );
}

///
template< typename T >
T&& vmin(
    T&& v )
{
    return std::forward< T >( v );
}

///c++14
/*template< typename T0, typename T1, typename... Ts >
auto vmin(
    T0&& v0,
    T1&& v1,
    Ts&&... vs )
{
    return ( v0 < v1 ) ?
        vmin( v0, std::forward< Ts >( vs )... ) :
        vmin( v1, std::forward< Ts >( vs )... );
}*/

///
template< typename T >
T&& vmax(
    T&& v )
{
    return std::forward< T >( v );
}

///c++14
/*template< typename T0, typename T1, typename... Ts >
auto vmax(
    T0&& v0,
    T1&& v1,
    Ts&&... vs )
{
    return ( v0 > v1 ) ?
        vmax( v0, std::forward< Ts >( vs )... ) :
        vmax( v1, std::forward< Ts >( vs )... );
}*/

} // end agg
} // end autodata
