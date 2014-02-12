
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

        Record astruct;
        astruct.SetTypename( "table1" );
        astruct[ "zero" ] = 5.4;
        astruct[ "one" ] = 7.32;
        astruct[ "two" ] = "twamy";
        astruct[ "three" ] = 7;
        astruct[ "four" ] = 3.0;
        astruct[ "five" ] = 8;
        astruct.CreateTable( session );
        astruct.Save( session );

        Table table( session );
        table << "select * from table1", now;
        for( auto& record : table )
        {
            Record bstruct; bstruct.FromJson( record.ToJson() );
            std::cout << bstruct.ToJson() << std::endl;
            auto var = cpplinq::
                from( record ).
                select( []( Record::value_type const& var ) -> double
                {
                    return Convert< double >( var.second, 0.0 );
                } ).
                aggregate( std::plus< double >() );
            std::cout << "Summation of cols: " << var << std::endl;
        }

        auto var2 = cpplinq::
            from( table.col< double >( "zero" ) ).
            aggregate( std::plus< double >() );
        std::cout << "Summation of col 'zero': " << var2 << std::endl;

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
