Author : Luc Legeard



Information
-----------

This package MFMlib is a complete  C++ library to decode or encode MFM frame.


Installation
------------

do a "make" 
- to generate a libMFM.a which can be linked to your own code
- to generate a stand-alone example "MFMtest.exe"  which generate MFM frames in a file
  and can reuse this MFM file.

If tinyxml library is not installed
MFM can be compiled without tinyxml  (make  NO_MFMNXML=YES)   
of download tynixml from  http://www.grinninglizard.com/tinyxml/.
In tinyxml directory
edit Makefile and for adding -fPIC option change lines 
-----------------------------------------------------------
# Rules for compiling source files to object files
%.o : %.cpp
	${CXX}  -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC}  -c ${CFLAGS} ${INCS} $< -o $@
----------------------------------------------------------------
to
-----------------------------------------------------------
# Rules for compiling source files to object files
%.o : %.cpp
	${CXX} -fPIC -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -fPIC -c ${CFLAGS} ${INCS} $< -o $@
----------------------------------------------------------------
compile with a "make" tynixml .




In MFM directory , so do a  "make TINYXMLLIBDIR=/where/is/tinyxml/"

  
Usage
-----
 
 usage "MFMtest.exe help"       for help
 
 
           

 
