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


%include <typemaps.i>
#ifdef SWIGWIN
%include <windows.i>
#define __in
#endif

#ifdef SWIGCSHARP
SWIG_CSBODY_PROXY( public, public, SWIGTYPE )
SWIG_CSBODY_TYPEWRAPPER( public, protected, public, SWIGTYPE )
#define SWIG_SHARED_PTR_TYPEMAPS( CONST, TYPE... ) \
    SWIG_SHARED_PTR_TYPEMAPS_IMPLEMENTATION( public, public, CONST, TYPE )

%include <enums.swg>
%csconst( 1 );
#endif //SWIGCSHARP

%import <operator_overload.i>
