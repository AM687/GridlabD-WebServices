WebServices for GRIDLAB-D

DISCLAIMER:
This is an implementation of webservices for parts of GRIDLABD simulation software. I do not own or have created any of the core software provided.
The implementation was made on the premises of the undergraduate class Web Information Systems for the Departement of Electrical and computer engineering, at the Univeristy of Thessaly.
Professor was Vavalis Emmanouil, with the (much appreciated) technical help of Antonia Nasiakou

Web service client and service skeletons were created, using the Staff Framework:
https://code.google.com/p/staff/ 

You can download the latest version of GRIDLABD here:
http://sourceforge.net/projects/gridlab-d/

Instructions

The parts of interest are the following:

Residential, where the WHeater has been implemented as a web service. 
  Test with test_waterheater_explicit_2.glm
Market, where the controller has been implement as a service.
  Test with test_controller.glm
Generators, where microturbine,solar and windturbine were implemented as services
  Test with:
  test_microturbine.glm
  test_Vout_PV.glm
  test_windturbine_NR.glm

If you wish to recompile the Visual Studio Projects:

project->properties->c++->general->additional libraries->new folder ->STAFF_HOME\include
project->properties->linker->general->additional libraries->new folder->STAFF_HOME\lib
linker->input-> additional dependencies ->staff_common.lib
staff_client.lib
staff_utils.li

The service list, can be found at:

http://10.96.12.46:9090/axis2/services
 
