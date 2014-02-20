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
#include <autodata/CompilerGuards.h>

#include <autodata/dynamic/Record.h>

namespace autodata
{
namespace dynamic
{

///
template< typename LoadPolicy = void >
class Table : public LoadPolicy
{
public:
    ///
    typedef Records::iterator iterator;
    typedef Records::const_iterator const_iterator;
    typedef Records::value_type value_type;

    ///
    Table()
        :
        m_records()
    {
        ;
    }

    ///move constructor
    Table(
        Table&& o )
        :
        m_records( std::move( o.m_records ) )
    {
        ;
    }

    ///
    ~Table()
    {
        ;
    }

    ///
    inline
    operator Records&()
    {
        return m_records;
    }

    ///
    inline
    operator Records const&() const
    {
        return m_records;
    }

    ///
    inline
    Record& operator [](
        unsigned int pos )
    {
        return m_records.at( pos );
    }

    ///
    inline
    Record const& operator [](
        unsigned int pos ) const
    {
        return m_records.at( pos );
    }

    ///
    inline
    iterator begin()
    {
        return m_records.begin();
    }

    ///
    inline
    const_iterator begin() const
    {
        return m_records.begin();
    }

    ///
    inline
    void clear()
    {
        m_records.clear();
    }

    ///
    template< typename T >
    std::vector< T > col(
        std::string const& name,
        T const& defVal = T() ) const
    {
        return cpplinq::
            from( m_records ).
            select( [ &name, &defVal ]( Record const& record )
            {
                return util::Convert< T >( record[ name ], defVal );
            } ).to_vector();
    }

    ///
    inline
    iterator end()
    {
        return m_records.end();
    }

    ///
    inline
    const_iterator end() const
    {
        return m_records.end();
    }

    ///
    template< typename T > inline
    void Load(
        T&& t )
    {
        LoadPolicy::Load( std::forward< T >( t ) );
    }

    ///
    inline
    void push_back(
        Record record )
    {
        m_records.push_back( std::move( record ) );
    }

protected:

private:
    ///
    friend LoadPolicy;

    ///copy constructor
    Table( Table const& ); // = delete;

    ///assignment operator
    Table& operator =( Table ); // = delete;

    ///
    Records m_records;

};

///For cpplinq use
template< typename LoadPolicy > inline
auto begin(
    autodata::dynamic::Table< LoadPolicy >& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto begin(
    autodata::dynamic::Table< LoadPolicy > const& o ) -> decltype( o.begin() )
{
    return o.begin();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto end(
    autodata::dynamic::Table< LoadPolicy >& o ) -> decltype( o.end() )
{
    return o.end();
}

///For cpplinq use
template< typename LoadPolicy > inline
auto end(
    autodata::dynamic::Table< LoadPolicy > const& o ) -> decltype( o.end() )
{
    return o.end();
}

///
class AUTODATA_EXPORTS DBPolicy
{
public:
    ///
    void Load(
        Poco::Data::Statement& statement );

protected:
    ///
    ~DBPolicy(){;}

private:

};

///
class AUTODATA_EXPORTS IFStreamPolicy
{
public:
    ///
    void Load(
        std::ifstream& ifs );

    ///Specify whether to try conversion to a numerical value
    bool ConvertType;

    ///Specify the delimiter character for a row
    char DelimChar;

    ///Specify if the file has a header with column values
    bool HasHeader;

    ///Specify characters to ignore when looking for empty lines
    std::string EmptyLineChars;

    ///Specify the character used for escaping in text fields
    char EscapeChar;

    ///Specify the character used for quoting text fields
    char QuoteChar;

protected:
    ///
    IFStreamPolicy()
        :
        ConvertType( true ),
        DelimChar( ' ' ),
        HasHeader( true ),
        EmptyLineChars( " \t\v\r\n" ),
        EscapeChar( '\\' ),
        QuoteChar( '\"' ),
        m_header()
    {
        ;
    }

    ///
    Poco::Dynamic::Var TryCast(
        std::string const& s );

    ///
    ~IFStreamPolicy(){;}

private:
    ///
    std::vector< std::string > m_header;

};

} //end dynamic
} //end autodata
