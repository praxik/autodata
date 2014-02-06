
// --- AutoData Includes --- //
#include <autodata/dynamic/Struct.h>

using namespace autodata::util;

// --- Poco Includes --- //
#include <Poco/Data/SQLite/SQLite.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/SQLite/SQLiteException.h>
#include <Poco/Data/DataException.h>

using namespace Poco::Dynamic;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;

// --- Standard Includes --- //
#include <fstream>

////////////////////////////////////////////////////////////////////////////////
int main(
    int argc,
    char* argv[] )
{
    std::cin >> std::string();

    try
    {
        SQLite::Connector::registerConnector();
        Session session( "SQLite", "test.db" );
        Statement statement( session );
        statement
            << "create table if not exists table1(\n"
            << "  zero text,\n"
            << "  one text,\n"
            << "  two text,\n"
            << "  three text,\n"
            << "  four text,\n"
            << "  five text )";
        //RecordSet rs( statement );
        statement.execute(); statement.reset( session );

        autodata::dynamic::Struct astruct;
        astruct[ "zero" ] = 7;
        astruct[ "one" ] = 7;
        astruct[ "two" ] = "twamy";
        astruct[ "three" ] = 7;
        astruct[ "four" ] = 7;
        astruct[ "five" ] = 7;

        std::string json = Poco::Dynamic::Var::toString( astruct.GetStruct() );
        std::cout << json << std::endl;

        autodata::dynamic::Struct bstruct;
        bstruct[ "zero" ] = 6;
        bstruct[ "one" ] = 8;
        bstruct[ "two" ] = "cool";
        bstruct[ "three" ] = 5.46;
        bstruct[ "four" ] = "somestringy";
        bstruct[ "five" ] = 9.0;

        astruct[ "three" ] = bstruct;

        json = astruct.GetStruct().toString();
        std::cout << json << std::endl;

        /*statement
            << "select\n"
            << "  7 as zero,\n"
            << "  'awesome' as one,\n"
            << "  6.66 as two,\n"
            << "  'sauce' as three,\n"
            << "  5.0 as four,\n"
            << "  'sweet' as five",
            into( astruct );
        statement.execute(); statement.reset( session );

        for( auto const& kv : astruct.GetStruct() )
        {
            std::cout << "aName: " << kv.first << "\taValue: "
                << Convert< std::string >( kv.second ) << std::endl;
        }

        std::string json = Poco::Dynamic::Var::toString( astruct.GetStruct() );
        std::cout << json << std::endl;

        statement
            << "insert into table1 values( ?, ?, ?, ?, ?, ? )",
            useRef( astruct );
        statement.execute(); statement.reset( session );

        autodata::dynamic::Struct bstruct;
        bstruct[ "zero" ];
        bstruct[ "one" ];
        bstruct[ "two" ];
        bstruct[ "three" ];
        bstruct[ "four" ];
        bstruct[ "five" ];

        statement
            << "select ?, ?, ?, ?, ?, ?",
            into( bstruct ),
            useRef( astruct );
        statement.execute(); statement.reset( session );

        for( auto const& kv : bstruct.GetStruct() )
        {
            std::cout << "bName: " << kv.first << "\tbValue: "
                << Convert< std::string >( kv.second ) << std::endl;
        }

        json = Poco::Dynamic::Var::toString( bstruct.GetStruct() );
        std::cout << json << std::endl;*/

        SQLite::Connector::unregisterConnector();
    }
    catch( SQLite::SQLiteException const& e )
    {
        std::cerr << "*** Poco::Data::SQLite::SQLiteException caught: "
                  << e.what() << std::endl;
    }
    catch( DataException const& e )
    {
        std::cerr << "*** Poco::Data::DataException caught: "
                  << e.what() << std::endl;
    }
    catch( Poco::Exception const& e )
    {
        std::cerr << "*** Poco::Exception caught: "
                  << e.what() << std::endl;
    }
    catch( std::exception const& e )
    {
        std::cerr << "*** std::exception caught: "
                  << e.what() << std::endl;
    }

    return 0;
}
////////////////////////////////////////////////////////////////////////////////
