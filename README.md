# atl_msr_so
a sample OPOS Service Object example for an MSR device

The purpose of this project is to investigate what is required to implement
an OPOS Service Object. As part of this we are investigating the following:
 - OPOS Common Control objects and interfaces
 - OPOS Service Control objects and interfaces
 - Microsoft COM technology
 - Microsfot ATL for development of COM objects and User Interfaces

See Monroe Consulting Service web site at http://monroecs.com/ for an explanation
of the OPOS standards along with quite a few materials and links to OPOS related
information.

The Monroe Consulting Services web site has this explantion for OPOS:
  The first widely-adopted POS device standard is OPOS. OPOS was initiated
  by Microsoft, NCR, Epson, and Fujitsu-ICL to help integrate POS hardware
  into applications for the Windows™ family of operating systems. OPOS uses
  COM technology, and is therefore language independent. The acronym stands
  for "OLE for POS", which is somewhat dated due to the renaming of "OLE" to
  "ActiveX" and "COM". But OPOS has been retained for historical and
  recognition reasons.
  
In order to test OPOS Common Control objects and OPOS Service Objects you will
need a test container. We have been using a combination of the NCR Retail Services
Manager, RSM, and the Microsoft POS .NET example UI whose source code is part
of the Microsoft POS .NET 1.12 installation.

Both of these allow us to exercise the OPOS Service Object that we are developing.
The Microsoft POS .NET utility is especially useful since it has separate buttons
which correspond to the various OPOS Common Control object methods such as Claim(),
Open(), etc.
