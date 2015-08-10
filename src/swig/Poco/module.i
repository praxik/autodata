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
Poco_ruby
#else
Poco
#endif
%{
    #include <Poco/Nullable.h>
%}

%import <std/module.i>

%import <Poco/Foundation.h>
%import <Poco/UTFString.h>

%ignore operator NullType&;
%ignore operator bool&;
%ignore operator const bool&;
%ignore operator int&;
%ignore operator const int&;
%ignore operator long&;
%ignore operator const long&;
%ignore operator double&;
%ignore operator const double&;
%ignore operator std::string&;
%ignore operator const std::string&;
%ignore Poco::Nullable::Nullable( NullType const& );
%ignore Poco::Nullable::assign( NullType );
%ignore Poco::Nullable::value() const;
%ignore Poco::Nullable::operator =( NullType );
%ignore Poco::Nullable::operator ==( NullType const& ) const;
%ignore Poco::Nullable::operator !=( NullType const& ) const;
%include <Poco/Nullable.h>
%template( NullableBool ) Poco::Nullable< bool >;
%template( NullableInt ) Poco::Nullable< int >;
%template( NullableLong ) Poco::Nullable< long >;
%template( NullableDouble ) Poco::Nullable< double >;
%template( NullableString ) Poco::Nullable< std::string >;
