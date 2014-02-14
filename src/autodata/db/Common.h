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

namespace autodata
{
namespace db
{

#ifndef SWIG
///
enum ConnectorEnum
{
    CONN_NONE = 0x0,
#ifdef POCO_MYSQL_API
    CONN_MYSQL = 0x1,
#endif //POCO_MYSQL_API
#ifdef POCO_ODBC_API
    CONN_ODBC = 0x2,
#endif //POCO_ODBC_API
    CONN_SQLITE = 0x4,
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
class AUTODATA_EXPORTS Connection
{
public:
    ///
    Connection();

    ///
    std::string Name;
    std::string Value;
    ConnectorEnum Connector;
};

///
typedef std::vector< Connection > ConnectionVector;

///
AUTODATA_EXPORTS
void RegisterConnectors(
    ConnectionVector const& connections );
#endif //SWIG

///
AUTODATA_EXPORTS
void UnregisterConnectors();

#ifndef SWIG
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
Poco::Data::Session GetSession(
    std::string const& dbEnum,
    unsigned int maxRetryAttempts = 100,
    unsigned int retrySleep = 100 );

///
AUTODATA_EXPORTS
ConnectorEnum GetConnector(
    std::string const& dbEnum );

///
///\param maxFldSize set the maximum field size in bytes
AUTODATA_EXPORTS
void SetMaxFieldSize(
    std::string const& dbEnum,
    std::size_t maxFldSize );
#endif //SWIG

} //end db
} //end autodata
