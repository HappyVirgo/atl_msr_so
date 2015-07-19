This information is as of July 19, 2015.

This folder, Opos_Includes, contains the OPOS include files
used with building an OPOS Service Object or using OPOS in
an application.

There are two kinds of files.
 - files with an extension of .h
 - files with an extension of .hi

The files with an extension of .h are used by both applications
using OPOS controls and by Service Object projects.

The files with an extension of .hi are used only by Service Object
projects. The .hi files contain various constants and prototypes
intended for Service Object functionality only.

Typically a Service Object source would do an include of the
appropriate .hi file, for instance an MSR Service Object would
include OposMsr.hi, and the .hi file has an include directive
for the proper .h file that is needed for that Service Object
type.

To obtain the latest version of these files you will need to go
to the Monroe Consulting Services web site where the OPOS Common
Controls are located.

Monroe Consulting Services are located at http://monroecs.com/

To access a copy of the OPOS Common Controls go to
the URL http://monroecs.com/oposccos.htm

The actual file containing the include files used are at URL:
  http://monroecs.com/oposccos_current.htm

And the file to download from the page is the CCO Runtime zip file
which will have name that includes the version such as 
  http://www.monroecs.com/files/OposCCOs-1_14_001.zip

