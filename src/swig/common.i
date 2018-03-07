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

#ifdef SWIGPYTHON
//Workaround for undefined SWIGPY_SLICE_ARG with swig 2.0.3 and 2.0.4
//If removed, fails also with swig 3.0, so this has not been fixed?
//Needs to be investigated
#if( 1 ) //( PY_VERSION_HEX >= 0x03020000 )
#define SWIGPY_SLICE_ARG( obj ) ( (PyObject*) (obj) )
#else
#define SWIGPY_SLICE_ARG( obj ) ( (PySliceObject*) (obj) )
#endif
#endif //SWIGPYTHON

%import <operator_overload.i>
