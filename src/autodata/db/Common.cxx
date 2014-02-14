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


// --- AutoData Includes --- //
#include <autodata/db/Common.h>

using namespace autodata::util;

// --- Boost Includes --- //
#include <boost/assign.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

// --- POCO Includes --- //
#ifdef POCO_ODBC_API
#include <Poco/Data/ODBC/Utility.h>
#endif //POCO_ODBC_API

using namespace Poco::Data;
using namespace Poco::Data::Keywords;

namespace autodata
{
namespace db
{

////////////////////////////////////////////////////////////////////////////////
typedef std::function< void () > RegisterFunction;
typedef std::map< ConnectorEnum, RegisterFunction > RegisterMap;
RegisterMap Registers = boost::assign::map_list_of
#ifdef POCO_MYSQL_API
    ( CONN_MYSQL, &MySQL::Connector::registerConnector )
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    ( CONN_ODBC, &ODBC::Connector::registerConnector )
#endif //POCO_ODBC_API
    ( CONN_SQLITE, &SQLite::Connector::registerConnector );
////////////////////////////////////////////////////////////////////////////////
typedef std::function< void () > UnregisterFunction;
typedef std::map< ConnectorEnum, UnregisterFunction > UnregisterMap;
UnregisterMap Unregisters = boost::assign::map_list_of
#ifdef POCO_MYSQL_API
    ( CONN_MYSQL, &MySQL::Connector::unregisterConnector )
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    ( CONN_ODBC, &ODBC::Connector::unregisterConnector )
#endif //POCO_ODBC_API
    ( CONN_SQLITE, &SQLite::Connector::unregisterConnector );
////////////////////////////////////////////////////////////////////////////////
typedef std::map< ConnectorEnum, std::string > ConnectorStringMap;
ConnectorStringMap ConnectorStrings = boost::assign::map_list_of
#ifdef POCO_MYSQL_API
    ( CONN_MYSQL, MySQL::Connector::KEY )
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    ( CONN_ODBC, ODBC::Connector::KEY )
#endif //POCO_ODBC_API
    ( CONN_SQLITE, SQLite::Connector::KEY );
////////////////////////////////////////////////////////////////////////////////
Connection::Connection(
    ConnectorEnum connector,
    std::string value,
    std::shared_ptr< SessionPool > sessionPool )
    :
    m_connector( connector ),
    m_value( std::move( value ) ),
    m_sessionPool( std::move( sessionPool ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Connection::Connection(
    Connection&& o )
    :
    m_connector( o.m_connector ),
    m_value( std::move( o.m_value ) ),
    m_sessionPool( std::move( o.m_sessionPool ) )
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
    ;
}
////////////////////////////////////////////////////////////////////////////////
ConnectorEnum Connection::Connector() const
{
    return m_connector;
}
////////////////////////////////////////////////////////////////////////////////
std::string const& Connection::Value() const
{
    return m_value;
}
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr< SessionPool > const& Connection::SessPool() const
{
    return m_sessionPool;
}
////////////////////////////////////////////////////////////////////////////////
typedef std::map< std::string, Connection > ConnectionMap;
ConnectionMap Connections;
////////////////////////////////////////////////////////////////////////////////
void Drivers(
    unsigned int connectors )
{
#ifdef POCO_ODBC_API
    if( connectors & CONN_ODBC )
    {
        typedef ODBC::Utility::DriverMap Drivers;
        Drivers drivers;
        ODBC::Utility::drivers( drivers );
        Drivers::const_iterator itr = drivers.begin();
        std::cout << "Available ODBC Drivers:" << std::endl;
        for ( ; itr != drivers.end(); ++itr )
        {
            std::string const& name = itr->first;
            std::cout << "  " << name << std::endl;
        }
    }
#endif //POCO_ODBC_API
}
////////////////////////////////////////////////////////////////////////////////
void DataSources(
    unsigned int connectors )
{
#ifdef POCO_ODBC_API
    if( connectors & CONN_ODBC )
    {
        typedef ODBC::Utility::DSNMap DataSources;
        DataSources dataSources;
        ODBC::Utility::dataSources( dataSources );
        DataSources::const_iterator itr = dataSources.begin();
        std::cout << "Available ODBC Data Sources:" << std::endl;
        for ( ; itr != dataSources.end(); ++itr )
        {
            std::string const& name = itr->first;
            std::cout << "  " << name << std::endl;
        }
    }
#endif //POCO_ODBC_API
}
////////////////////////////////////////////////////////////////////////////////
ConnectorEnum GetConnector(
    std::string const& name )
{
    return Connections.at( name ).Connector();
}
////////////////////////////////////////////////////////////////////////////////
Session GetSession(
    std::string const& name,
    unsigned int maxRetryAttempts,
    unsigned int retrySleep )
{
    unsigned int cnt( 0 );
    while( ++cnt <= maxRetryAttempts )
    {
        try
        {
            Connection const& connection = Connections.at( name );
            Session session = connection.SessPool()->get();
            if( !session.isConnected() ) session.reconnect();
            return session;
        }
        catch( ExecutionException const& ){;}
        catch( Poco::Exception const& ex ){ ex.rethrow(); }
        Poco::Thread::sleep( retrySleep );
    }

    throw std::runtime_error( "leaf::open::GetSession failed" );
}
////////////////////////////////////////////////////////////////////////////////
void RegisterConnectors(
    ConnectionVector const& connections )
{
    ConnectionVector::const_iterator citr = connections.begin();
    for( ; citr != connections.end(); ++citr )
    {
        std::string const& name = std::get< 0 >( *citr );
        ConnectorEnum connector = std::get< 1 >( *citr );
        std::string const& value = std::get< 2 >( *citr );

        Registers[ connector ]();
        Connections.insert( ConnectionMap::value_type( name,
            Connection( connector, value, std::make_shared< SessionPool >(
                ConnectorStrings[ connector ], value, 1, 16, 0 ) ) ) );
        Connection const& connection = Connections.at( name );
        switch( connector )
        {
#ifdef POCO_ODBC_API
            case CONN_ODBC:
            {
                connection.SessPool()->setFeature( "autoBind", true );
                connection.SessPool()->setFeature( "autoExtract", true );
                break;
            }
#endif //POCO_ODBC_API
            case CONN_SQLITE:
            {
                //Foreign key support is not enabled in SQLite by default
                //This may need to be moved to GetSession() function below
                StmtObj stmtObj( GetSession( name ) );
                stmtObj.m_statement << "PRAGMA foreign_keys = ON;";
                ExecuteRetry( stmtObj );
                break;
            }
            default:
            {
                break;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
void SetMaxFieldSize(
    std::string const& name,
    std::size_t maxFldSize )
{
    Connection const& connection = Connections.at( name );
    ConnectorEnum connector = connection.Connector();
    if( connector == CONN_ODBC )
    {
        connection.SessPool()->setProperty(
            "maxFieldSize", Poco::Any( maxFldSize ) );
    }
}
////////////////////////////////////////////////////////////////////////////////
void UnregisterConnectors()
{
    for( auto const& kv : Connections )
    {
        Connection const& connection = kv.second;
        connection.SessPool()->shutdown();
        Unregisters[ connection.Connector() ]();
    }
}
////////////////////////////////////////////////////////////////////////////////

} //end db
} //end autodata