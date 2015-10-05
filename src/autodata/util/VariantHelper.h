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

// --- Boost Includes --- //
#include <boost/variant.hpp>

// --- Standard Includes --- //
#include <type_traits>
#include <functional>
#include <utility>

namespace autodata
{
namespace util
{

///
template< typename R, typename Func >
struct GenericVisitor : public boost::static_visitor< R >
{
    ///
    GenericVisitor(
        Func&& func )
        :
        m_func( std::move( func ) )
    {
        ;
    }

    ///
    template< typename T >
    R operator()(
        T&& val ) const
    {
        return m_func( std::forward< T >( val ) );
    }

private:
    ///
    Func m_func;
};

///
template< typename R, typename Func >
GenericVisitor< R, Func > generic_visitor(
    Func&& func )
{
    return { std::forward< Func >( func ) };
}

///
template< template< typename T > class M, typename... Ts >
struct MemberVisitor : boost::static_visitor<>
{
    ///
    using Params = std::tuple< Ts... >;

    ///
    template< typename T >
    using member_type = decltype( M< T >::value );

    template< typename T >
    void operator()(
        T const& val )
    {
        return ( ( val ).*( M< T >::value ) );
    }
};

///
/*template< template< typename T > class M, typename R >
struct MemberVisitor : boost::static_visitor< R >
{
    template< typename T >
    using member_type = decltype( M< T >::value );

    template< typename T >
    typename std::enable_if<
        std::is_member_object_pointer< member_type< T > >::value, R >::type
    operator()(
        T const& val )
    {
        return ( ( val ).*( M< T >::value ) );
    }

    template< typename T >
    typename std::enable_if<
        std::is_member_function_pointer< member_type< T > >::value, R >::type
    operator()(
        T const& val )
    {
        return ( ( val ).*( M< T >::value ) )();
    }
};*/

///
template< typename Visitor, typename Variant >
void CallWithVariant(
    Variant& v )
{
    boost::apply_visitor( Visitor(), v );
}

} //end util
} //end autodata
