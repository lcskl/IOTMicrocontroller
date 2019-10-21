#!/usr/bin/tclsh

source scripts/utils.tcl

#Create Report Output Directory
if ![file exists $reportdir]  {file mkdir $reportdir}

open_project "$projpath/$projname.xpr"
open_run synth_1 -name synth_1

report_timing_summary -file $reportdir/timing.rpt

exit