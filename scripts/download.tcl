#!/usr/bin/tclsh

# Download elf to FPGA.

source scripts/utils.tcl

if { $argc < 1} {
	puts "No elf file passed!"
	exit
}

puts "argc: $argc";
puts "argv: [lindex $argv 0]";

# Creater server.
connect

# Download hardware bitstream.
targets 1
fpga -file $sdkprojpath/hw_0/microcontroller_wrapper.bit

# Connect to Microblaze inside the FPGA.
targets 3

# Download Elf.
dow $sdkprojpath/helloworld/Debug/helloworld.elf

# Run program.
con