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