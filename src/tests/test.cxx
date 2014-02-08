
// --- AutoData Includes --- //
#include <autodata/dynamic/Struct.h>
#include <autodata/dynamic/Table.h>

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
        statement.execute(); statement.reset( session );

        statement
            << "select * from table1",
            now;
        autodata::dynamic::Table table( statement );
        autodata::dynamic::StructVec rows = table.ToStructVec();
        for( auto& row : rows )
        {
            std::cout << "zero: " << Convert< std::string >( row[ "zero" ], "null" ) << std::endl;
            std::cout << "one: " << Convert< std::string >( row[ "one" ], "null" ) << std::endl;
            std::cout << "two: " << Convert< std::string >( row[ "two" ], "null" ) << std::endl;
            std::cout << "three: " << Convert< std::string >( row[ "three" ], "null" ) << std::endl;
            std::cout << "four: " << Convert< std::string >( row[ "four" ], "null" ) << std::endl;
            std::cout << "five: " << Convert< std::string >( row[ "five" ], "null" ) << std::endl;
            std::cout << row.ToJson() << std::endl;
        }

        /*autodata::dynamic::Struct astruct;
        astruct.SetTypename( "table1" );
        astruct[ "zero" ] = 7;
        astruct[ "one" ] = 7;
        astruct[ "two" ] = "twamy";
        astruct[ "three" ] = 7;
        astruct[ "four" ] = 7;
        astruct[ "five" ] = 7;

        std::cout << astruct.ToJson() << std::endl;

        astruct.Load( session );*/

        /*statement
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
