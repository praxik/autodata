
// --- AutoData Includes --- //
#include <autodata/dynamic/Record.h>
#include <autodata/dynamic/Table.h>

using namespace autodata::dynamic;
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
        Table table( statement );
        RecordVec rows = table.ToRecordVec();
        for( auto& row : rows )
        {
            std::cout << row.ToJson() << std::endl;
            auto var = cpplinq::
                from( row ).
                select( []( Record::value_type const& var ) -> double
                {
                    return Convert< double >( var.second, 0.0 );
                } ).
                aggregate( std::plus< double >() );
            std::cout << "Summation of cols: " << var << std::endl;
        }

        auto var = cpplinq::
            from( rows ).
            select( []( Record const& row ) -> double
            {
                return row[ "zero" ];
            } ).
            aggregate( std::plus< double >() );
        std::cout << "Summation of col 'zero': " << var << std::endl;

        /*Record astruct;
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

        Record bstruct;
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
