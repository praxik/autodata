
// --- AutoData Includes --- //
#include <autodata/db/Connection.h>

#include <autodata/dynamic/Record.h>
#include <autodata/dynamic/Table.h>

using namespace autodata::db;
using namespace autodata::dynamic;
using namespace autodata::util;

// --- Boost Includes --- //
#include <boost/assign.hpp>

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
    std::string lString;
    std::cin >> lString;

    try
    {
        std::string const TEST_DB = "TEST_DB";
        ConnectionVector connections = boost::assign::list_of
            ( ConnectionTuple( TEST_DB, CONN_SQLITE, "test.db" ) );
        RegisterConnectors( connections );

        std::string tableName = "table1";
        Record astruct;// = load( tableName,
            //"13dad76e-9844-11e3-8ee5-3c970eb4a232", GetSession( TEST_DB ) );
        if( astruct.empty() )
        {
            astruct.SetTypename( tableName );
            astruct[ "zero" ] = 5.4;
            astruct[ "one" ] = 7.32;
            astruct[ "two" ] = "twamy";
            astruct[ "three" ] = 7;
            astruct[ "four" ] = 3.0;
            astruct[ "five" ] = 8;
            Session session = GetSession( TEST_DB );
            astruct.CreateTable( session );
            astruct.Save( session );
        }

        Statement statement( GetSession( TEST_DB ) << "select * from table1" );
        Table< DBPolicy, DbSave > table;
        table.Load( statement );
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

        std::vector< double > doubleCol = table.col< double >( "zero" );
        auto var2 = cpplinq::
            from( doubleCol ).
            aggregate( std::plus< double >() );
        std::cout << "Summation of col 'zero': " << var2 << std::endl;

        table.Save( GetSession( TEST_DB ), "table2" );

        UnregisterConnectors();
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
