
// --- App Includes --- //
#include <cppiter/collapse.hpp>

// --- STL Includes --- //
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
void Main()
{
    //Test data
    using maptype = std::map< int, std::string >;
    maptype map =
    {
        { 0, "SWG" },
        { 1, "CG" }, { 2, "CG" },
        { 3, "OT" },
        { 4, "CG" }, { 5, "CG" },
        { 6, "SB" },
        { 7, "SWG" }
    };
    std::vector< std::string > vec;
    std::transform( map.begin(), map.end(), std::back_inserter( vec ),
        []( maptype::value_type const& kv )
        {
            return kv.second;
        } );

    //Test a map
    std::cout << "Testing map w/ custom key function and "
        "custom binary predicate" << std::endl;
    for( auto const& group : iter::collapse( map,
        []( maptype::value_type const& kv ) -> std::string const&
        {
            return kv.second;
        },
        []( std::string const& lhs, std::string const& rhs ) -> bool
        {
            return ( lhs == rhs ) && ( lhs == "SWG" );
        } ) )
    {
        std::cout << "key: ";
        std::cout << group.value() << std::endl;
        //std::cout << &group.value() << std::endl;

        std::cout << "  items(" << group.size() << "): ";
        for( auto const& item : group )
        {
            std::cout << item.first << "  ";
            //std::cout << &item.second << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    //
    std::cout << "Testing vector w/ default key function" << std::endl;
    for( auto const& group : iter::collapse( vec ) )
    {
        std::cout << "key: ";
        std::cout << group.value() << std::endl;
        //std::cout << &group.value() << std::endl;

        std::cout << "  items(" << group.size() << "): ";
        for( auto const& item : group )
        {
            std::cout << item << "  ";
            //std::cout << &item << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    //
    std::cout << "Testing rvalue vector w/ default key function" << std::endl;
    for( auto const& group : iter::collapse( std::move( vec ) ) )
    {
        std::cout << "key: ";
        std::cout << group.value() << std::endl;
        //std::cout << &group.value() << std::endl;

        std::cout << "  items(" << group.size() << "): ";
        for( auto const& item : group )
        {
            std::cout << item << "  ";
            //std::cout << &item << "  ";
        }
        std::cout << std::endl;

        auto&& subcollapse = iter::collapse( group,
            iter::DefKeyFunc(),
            []( std::string const& lhs, std::string const& rhs )
            {
                return ( lhs == rhs ) && lhs[ 0 ] == 'S';
            } );
        for( auto const& subgrp : subcollapse )
        {
            std::cout << "  key: ";
            std::cout << subgrp.value() << std::endl;
            //std::cout << &subgrp.value() << std::endl;

            std::cout << "    items(" << subgrp.size() << "): ";
            for( auto const& item : subgrp )
            {
                std::cout << item << "  ";
                //std::cout << &item << "  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
////////////////////////////////////////////////////////////////////////////////
int main(
    int argc,
    char** argv)
{
    try
    {
        Main();
    }
    catch( std::exception const& e )
    {
        std::cerr << "*** std::exception caught: "
            << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
