*****************************************************************************
*  Pickering Linux Client Bridge 
* --------------------------------------------------------------------------- 
*  Original Release: 2009-04                                          
*  Copyright:        2011, Pickering Interfaces Ltd. - www.pickeringtest.com
*  Description:      High level C/C++ driver for LXI, PCI and PXI PI cards.
*
*  Note:	     It requires kernel driver for PXI or PCI cards. Version
*		           of the driver should be 2.95 or higher.
*****************************************************************************

Driver's folders and files 
=============================================================================
Note: Folder "[CBROOT]" represents root directory of the installation. Default
root directory is: 
	                           "/usr/local/lib". 

This root directory contains shared libraries of the Client Bridge and its
symbolic links:
- libpicmlx.so.1.8.1: shared library of a communication module of the Client Bridge.
- libpiplx.so.1.6.0: shared library of a standard module of the Client Bridge.                 


Folder "[CBROOT]/clientbridge-{version}"
----------------------------------------
Contains another part of the Client Bridge installation and these files:
- LICENSE:  License of the Pickering Linux Client Bridge
- README:   This file. 


Folder "[CBROOT]/clientbridge-{version}/examples"
-------------------------------------------------
Contains series of examples in two directories. "picmlx" directory contains 
examples of communication module and "piplx" direcotry contains examples of
standard module.

Every example has its own makefile. All makefiles 
can be compiled in one type by script "Compile.sh" which is included in 
the directory "picmlx" or "piplx". "Clean.sh" script cleans output of all makefiles.
					

Folder "[CBROOT]/clientbridge-{version}/doc"
--------------------------------------------
This directory contains documentation for the Client Bridge.

Folder "[CBROOT]/clientbridge-{version}/doc/picmlx"
---------------------------------------------------
This directory contains documentation for the communication module of 
the Client Bridge in HTML format. A main page is index.html.

Folder "[CBROOT]/clientbridge-{version}/doc/piplx"
--------------------------------------------------
This directory contains documentation for the standard (switch) module of 
the Client Bridge in HTML format. A main page is index.html.

Folder "/usr/local/include"
---------------------------
- Picmlx.h: header file of communication API.
- Piplx.h   header file of standard module API.

Post actions
============================================================================= 
At the end of the installation is updated cache which contains paths to the required
shared libraries.  


Enhanced Security
============================================================================= 
If the Linux system is running SELinux (enhanced scurity) then the libraries
will fail to load, producing an error message:

	'Cannot restore segment prot after reloc: Permission denied'

To resolve this, either turn off enhanced security, or add the libraries to
be added to the security module. To do this:

chcon -t textrel_shlib_t /lib/libpicmlx.so

and so on for each library used.




Support
=============================================================================
For further assistance, please contact:

Pickering Interfaces Ltd.
Stephenson Road
Clacton-on-Sea
Essex
CO15 4NL
UK

Tel. +44 (0)1255-687900
Fax. +44 (0)1255-425349
WWW: http://www.pickeringtest.com
Email (sales): sales@pickeringtest.com
Email (technical support): support@pickeringtest.com      
