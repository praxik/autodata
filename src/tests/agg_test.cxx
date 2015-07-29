
// --- App Includes --- //
#include <autodata/agg/Function.h>

using namespace autodata;

// --- STL Includes --- //
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
void Main()
{
    auto sum1 = agg::vsum( 3, 4.6f );
    auto sum2 = agg::vsum( 4, 5.2f, 7.0 );
    std::cout
        << "sum1: " << sum1 << " | type1: "
        << typeid( decltype( sum1 ) ).name() << std::endl;
    std::cout
        << "sum2: " << sum2 << " | type2: "
        << typeid( decltype( sum2 ) ).name() << std::endl;

    auto avg1 = agg::vavg( 3, 4.6f );
    auto avg2 = agg::vavg( 4, 5.2f, 7.0 );
    std::cout
        << "avg1: " << avg1 << " | type1: "
        << typeid( decltype( avg1 ) ).name() << std::endl;
    std::cout
        << "avg2: " << avg2 << " | type2: "
        << typeid( decltype( avg2 ) ).name() << std::endl;
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
