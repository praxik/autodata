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
template< typename S0, typename S1 >
struct concat{};

template< std::size_t... I0, std::size_t... I1 >
struct concat< seq< I0... >, seq< I1... > > :
    seq< I0..., ( sizeof...( I0 ) + I1 )... >{};

///
template< std::size_t N >
struct gens;

template<>
struct gens< 0 > : seq<>{};

template<>
struct gens< 1 > : seq< 0 >{};

template< std::size_t N >
struct gens : concat<
    typename gens< N / 2 >::type,
    typename gens< N - N / 2 >::type >::type{};

///Closed interval sequence
template< std::size_t C, std::size_t P, std::size_t... Ns >
struct cintvl_helper : cintvl_helper< C - 1, P + 1, Ns..., P >{};

template< std::size_t P, std::size_t... Ns >
struct cintvl_helper< 0, P, Ns... > : seq< Ns... >{};

template< std::size_t S, std::size_t E >
struct cigens : cintvl_helper< E - S + 1, S >{};

///
template< typename Callable, typename Tuple, std::size_t... Ns >
auto CallWithTuple(
    Callable&& fn,
    Tuple&& tup,
    seq< Ns... > ) -> decltype(
        fn( std::get< Ns >( std::forward< Tuple >( tup ) )... ) )
{
    return fn( std::get< Ns >( std::forward< Tuple >( tup ) )... );
}

///
template< typename Callable, typename Tuple >
auto Invoke(
    Callable&& fn,
    Tuple&& tup ) -> decltype(
        CallWithTuple(
            std::forward< Callable >( fn ),
            std::forward< Tuple >( tup ),
            typename gens< std::tuple_size<
                typename std::remove_reference< Tuple >::type >::value >::type() ) )
{
    return CallWithTuple(
        std::forward< Callable >( fn ),
        std::forward< Tuple >( tup ),
        typename gens< std::tuple_size<
            typename std::remove_reference< Tuple >::type >::value >::type() );
}

///Helper function
template< typename PocoTuple, std::size_t... Ns >
auto Convert(
    PocoTuple&& pocotuple,
    seq< Ns... > ) -> decltype(
        std::make_tuple(
            std::forward< PocoTuple >( pocotuple ).template get< Ns >()... ) )
{
    return std::make_tuple(
        std::forward< PocoTuple >( pocotuple ).template get< Ns >()... );
}

///Poco::Tuple is not a true tuple w/ NullTypeList template params
template< typename PocoTuple >
auto Convert(
    PocoTuple&& pocotuple ) -> decltype(
        Convert(
            std::forward< PocoTuple >( pocotuple ),
            typename gens<
                std::remove_reference< PocoTuple >::type::length >::type() ) )
{
    return Convert(
        std::forward< PocoTuple >( pocotuple ),
        typename gens<
            std::remove_reference< PocoTuple >::type::length >::type() );
}

} //end util
} //end autodata
