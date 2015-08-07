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

// --- Poco Includes --- //
#include <Poco/Tuple.h>

// --- Standard Includes --- //
#include <functional>
#include <tuple>
#include <utility>

namespace autodata
{
namespace util
{

///c++14: std::integer_sequence/std::index_sequence

///
template< std::size_t... >
struct seq{ using type = seq; };

///
template< std::size_t N, std::size_t... Ns >
struct gens : gens< N - 1, N - 1, Ns... >{};

///
template< std::size_t... Ns >
struct gens< 0, Ns... > : seq< Ns... >{};

///Reversed
template< std::size_t N, std::size_t... Ns >
struct rgens : rgens< N - 1, Ns..., N - 1 >{};

///Reversed
template< std::size_t... Ns >
struct rgens< 0, Ns... > : seq< Ns... >{};

///
struct tuple_unpacker
{
    ///Value returning function
    template< typename Callable, typename Tuple, std::size_t... Ns >
    static auto apply(
        Callable&& fn,
        Tuple&& tup,
        seq< Ns... > ) -> decltype(
            fn( std::get< Ns >( std::forward< Tuple >( tup ) )... ) )
    {
        return fn( std::get< Ns >( std::forward< Tuple >( tup ) )... );
    }
};

///
template< typename Unpacker, typename Callable, typename Tuple >
auto apply_tuple(
    Callable&& fn,
    Tuple&& tup ) -> decltype(
        Unpacker::apply(
            std::forward< Callable >( fn ),
            std::forward< Tuple >( tup ),
            typename gens< std::tuple_size<
                typename std::remove_reference< Tuple >::type >::value >::type() ) )
{
    return Unpacker::apply(
        std::forward< Callable >( fn ),
        std::forward< Tuple >( tup ),
        typename gens< std::tuple_size<
            typename std::remove_reference< Tuple >::type >::value >::type() );
}

///Helper function
template< typename PocoTuple, std::size_t... Ns >
auto Convert(
    PocoTuple&& pocotuple,
    seq< Ns... > ) -> decltype( std::make_tuple( pocotuple.get< Ns >()... ) )
{
    return std::make_tuple( pocotuple.get< Ns >()... );
}

///Must manually specify template params
///Poco::Tuple is not a true tuple w/ NullTypeList template params
template< typename... Ts >
std::tuple< Ts... > Convert(
    Poco::Tuple< Ts... > const& pocotuple )
{
    return Convert( pocotuple, typename gens< sizeof...( Ts ) >::type() );
}

} //end util
} //end autodata
