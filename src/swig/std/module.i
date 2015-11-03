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
std_ruby
#else
std
#endif
%{
    ;
%}

%import <common.i>

%include <stl.i>
%include <stdint.i>
#ifdef SWIGRUBY
namespace std
{
template< typename T >
class shared_ptr
{
public:
    T* operator ->();
    T* get();
};
}
#else
%include <std_shared_ptr.i>
#endif

%template( PairDouble ) std::pair< double, double >;
%template( PairString ) std::pair< std::string, std::string >;

%template( VectorInt ) std::vector< int >;
%template( VectorUInt ) std::vector< unsigned int >;
%template( VectorSizeT ) std::vector< size_t >;
%template( VectorDouble ) std::vector< double >;
%template( VectorString ) std::vector< std::string >;
%template( VectorPairString ) std::vector< std::pair< std::string, std::string > >;
