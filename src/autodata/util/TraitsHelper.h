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

// --- AutoData Includes --- //
#include <autodata/AutoDataConfig.h>
#include <autodata/CompilerGuards.h>

// --- Standard Includes --- //
#include <type_traits>
#include <functional>
#include <utility>

namespace autodata
{
namespace util
{

///
template< typename T, typename... Ts >
struct are_arithmetic : std::conditional<
    std::is_arithmetic< T >::value,
    are_arithmetic< Ts... >, std::false_type >::type{};

///
template< typename T >
struct are_arithmetic< T > : std::is_arithmetic< T >{};

///Check if type U is a specialization of the incomplete type T
template< template< typename... > class T, typename U >
struct is_specialization_of : std::false_type{};

///Check if type U is a specialization of the incomplete type T
template< template< typename... > class T, typename... Ts >
struct is_specialization_of< T, T< Ts... > > : std::true_type{};

///Variadic is_specialization_of
template< template< typename... > class T, typename U, typename... Ts >
struct are_specialization_of : std::conditional<
    is_specialization_of< T, U >::value,
    are_specialization_of< T, Ts... >, std::false_type >::type{};

///Variadic is_specialization_of
template< template< typename... > class T, typename U >
struct are_specialization_of< T, U > : is_specialization_of< T, U >{};

} //end util
} //end autodata
