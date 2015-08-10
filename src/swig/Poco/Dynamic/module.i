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


%module
#ifdef SWIGRUBY
Poco_Dynamic_ruby
#else
Poco_Dynamic
#endif
%{
    #include <Poco/Nullable.h>

    #include <Poco/Dynamic/Var.h>
    #include <Poco/Dynamic/Struct.h>
%}

%ignore operator Poco::Dynamic::Var&;
%ignore operator const Poco::Dynamic::Var&;
%import <Poco/module.i>
%template( NullableVar ) Poco::Nullable< Poco::Dynamic::Var >;

%import <Poco/Dynamic/VarHolder.h>

%ignore Poco::Dynamic::operator ==( char const*, Var const& );
%ignore Poco::Dynamic::operator ==( UTF16String const&, Var const& );
%ignore Poco::Dynamic::operator !=( char const*, Var const& );
%ignore Poco::Dynamic::operator !=( UTF16String const&, Var const& );
%ignore Poco::Dynamic::Var::operator []( std::string const& ) const;
%ignore Poco::Dynamic::Var::operator []( char const* ) const;
%ignore Poco::Dynamic::Var::operator []( char const* );
%ignore Poco::Dynamic::Var::type() const;
%ignore Poco::Dynamic::Var::convert< bool >( bool& ) const;
%ignore Poco::Dynamic::Var::convert< int >( int& ) const;
%ignore Poco::Dynamic::Var::convert< double >( double& ) const;
%ignore Poco::Dynamic::Var::convert< std::string >( std::string& ) const;
%ignore Poco::Dynamic::Var::begin() const;
%ignore Poco::Dynamic::Var::end() const;
%ignore Poco::Dynamic::Var::begin();
%ignore Poco::Dynamic::Var::end();
%ignore Poco::Dynamic::Var::toString() const;
%include <Poco/Dynamic/Var.h>
//Create conversion constructors
%template( Var ) Poco::Dynamic::Var::Var< bool >;
%template( Var ) Poco::Dynamic::Var::Var< int >;
%template( Var ) Poco::Dynamic::Var::Var< double >;
%template() Poco::Dynamic::Var::operator =< bool >;
%template() Poco::Dynamic::Var::operator =< int >;
%template() Poco::Dynamic::Var::operator =< double >;
%template() Poco::Dynamic::Var::operator =< std::string >;
%template( ExtractBool ) Poco::Dynamic::Var::extract< bool >;
%template( ExtractInt ) Poco::Dynamic::Var::extract< int >;
%template( ExtractDouble ) Poco::Dynamic::Var::extract< double >;
%template( ExtractString ) Poco::Dynamic::Var::extract< std::string >;
%template( ConvertBool ) Poco::Dynamic::Var::convert< bool >;
%template( ConvertInt ) Poco::Dynamic::Var::convert< int >;
%template( ConvertDouble ) Poco::Dynamic::Var::convert< double >;
%template( ConvertString ) Poco::Dynamic::Var::convert< std::string >;

%ignore Poco::Dynamic::Struct< std::string >::operator []( std::string const& ) const;
%ignore Poco::Dynamic::Struct< std::string >::find( std::string const& ) const;
%ignore Poco::Dynamic::Struct< std::string >::end() const;
%ignore Poco::Dynamic::Struct< std::string >::begin() const;
%include <Poco/Dynamic/Struct.h>
%template( DynamicStruct ) Poco::Dynamic::Struct< std::string >;
%template( StructData ) std::map< std::string, Poco::Dynamic::Var >;
//%template( StructNameSet ) std::set< std::string >;
