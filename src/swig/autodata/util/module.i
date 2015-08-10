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
autodata_util_ruby
#else
autodata_util
#endif
%{
    #include <autodata/util/DataHelper.h>
    #include <autodata/util/Version.h>
%}

%import <Poco/Data/module.i>

%import <autodata/AutoDataConfig.h>

#ifdef SWIGCSHARP
%typemap( csimports ) SWIGTYPE
%{
    using System;
    using System.Runtime.InteropServices;

    using std;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;
%}
%pragma( csharp ) moduleimports=
%{
    using System;
    using System.Runtime.InteropServices;

    using std;

    using Poco;
    using Poco.Data;
    using Poco.Dynamic;
%}
#endif

%include <autodata/util/DataHelper.h>
%template( ConvertInt ) autodata::util::Convert< int >;
%template( ConvertDouble ) autodata::util::Convert< double >;
%template( ConvertString ) autodata::util::Convert< std::string >;

%include <autodata/util/Version.h>
