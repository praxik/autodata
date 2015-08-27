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

#include <autodata/util/DataHelper.h>

// --- Poco Includes --- //
#ifndef SWIG
#include <Poco/Data/SessionPool.h>
#endif //SWIG

namespace autodata
{
namespace db
{

///
enum ConnectorEnum
{
    CONN_NONE = ( 0 << 0 ),
#ifdef POCO_MYSQL_API
    CONN_MYSQL = ( 1 << 0 ),
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    CONN_ODBC = ( 1 << 1 ),
#endif //POCO_ODBC_API
    CONN_SQLITE = ( 1 << 2 ),
#if defined( POCO_MYSQL_API ) && defined( POCO_ODBC_API )
    CONN_ALL = CONN_MYSQL | CONN_ODBC | CONN_SQLITE,
#elif defined( POCO_MYSQL_API ) && !defined( POCO_ODBC_API )
    CONN_ALL = CONN_MYSQL | CONN_SQLITE,
#elif !defined( POCO_MYSQL_API ) && defined( POCO_ODBC_API )
    CONN_ALL = CONN_ODBC | CONN_SQLITE,
#else
    CONN_ALL = CONN_SQLITE
#endif //POCO_MYSQL_API && POCO_ODBC_API
};

///
class AUTODATA_EXPORTS ConnectionTuple
{
public:
    ///default constructor...
    ConnectionTuple() = default;

    ///constructor
    ConnectionTuple(
        std::string const& name,
        ConnectorEnum connector,
        std::string const& value );

    ///copy constructor
    ConnectionTuple(
        ConnectionTuple const& ) = default;

#ifndef SWIG
    ///move constructor
    ConnectionTuple(
        ConnectionTuple&& rhs );
#endif

    ///assignment operator
    ConnectionTuple& operator =(
        ConnectionTuple rhs );

    ///destructor
    ~ConnectionTuple() = default;

    std::string const& Name() const;
    ConnectorEnum Connector() const;
    std::string const& Value() const;

private:
    std::string m_name;
    ConnectorEnum m_connector;
    std::string m_value;
};
typedef std::vector< ConnectionTuple > ConnectionVector;

///
AUTODATA_EXPORTS
void Drivers(
    unsigned int connectors = CONN_ALL );

///
AUTODATA_EXPORTS
void DataSources(
    unsigned int connectors = CONN_ALL );

///
AUTODATA_EXPORTS
std::string GetConnString(
    std::string const& name );

///
AUTODATA_EXPORTS
ConnectorEnum GetConnector(
    std::string const& name );

#ifndef SWIG
///
AUTODATA_EXPORTS
Poco::Data::Session GetSession(
    std::string const& name,
    unsigned int maxRetryAttempts = 100,
    unsigned int retrySleep = 100 );

#endif //SWIG

///
AUTODATA_EXPORTS
void RegisterConnectors(
    ConnectionVector const& connections );

///
///\param maxFldSize set the maximum field size in bytes
/*\note
 * When connecting via the psqlodbc driver on *nix os,
 * make sure to specify the connection option "MaxLongVarcharSize=maxFldSize;"
 */
AUTODATA_EXPORTS
void SetMaxFieldSize(
    std::string const& name,
    std::size_t maxFldSize );

///
AUTODATA_EXPORTS
void UnregisterConnectors();

#ifndef SWIG
///For internal use only
class Connection
{
public:
    ///
    Connection(
        Connection&& o );

    ///destructor
    ~Connection();

    ///
    ConnectorEnum Connector() const;

    ///
    std::string const& Value() const;

    ///
    std::shared_ptr< Poco::Data::SessionPool > const& SessPool() const;

private:
    ///
    friend AUTODATA_EXPORTS void RegisterConnectors( ConnectionVector const& );

    ///constructor
    Connection(
        ConnectorEnum connector,
        std::string value,
        std::shared_ptr< Poco::Data::SessionPool > sessionPool );

    ///
    Connection( Connection const& ); //= delete;

    ///
    Connection& operator =( Connection ); //= delete

    ///
    ConnectorEnum m_connector;

    ///
    std::string m_value;

    ///
    std::shared_ptr< Poco::Data::SessionPool > m_sessionPool;

};
#endif //SWIG

} //end db
} //end autodata
