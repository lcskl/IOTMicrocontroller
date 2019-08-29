#!/usr/bin/tclsh

# Generates the example Hello World project in SDK

source scripts/utils.tcl

puts $sdkprojpath
puts $sdkprojpath/$hardware_hdf

setws $sdkprojpath

set ws_projects [getprojects]
puts $ws_projects

# Simple routine to force creation of new SDK projects.
#if {0} {
#if {$ws_projects > 0} {
#	foreach proj $ws_projects {
#		deleteprojects -name $proj;
#	}
#}
#set ws_projects [getprojects]

createhw  -name hw_0 -hwspec $sdkprojpath/$hardware_hdf
createbsp -name bsp_0 -hwproject hw_0 -proc microblaze_0 -os standalone
createapp -name helloworld -hwproject hw_0 -proc microblaze_0 -os standalone -lang C -app {Hello World} -bsp bsp_0

projects -build
