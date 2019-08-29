HELLO_ELF := ./iot/iot.sdk/helloworld/Debug/helloworld.elf 

all: hardware software

hardware:
	vivado -source -mode tcl -source ./scripts/hardware.tcl

software:
	xsdk -batch -source ./scripts/sdk_hello_world.tcl

download:
	xsdk -batch -source ./scripts/download.tcl $(HELLO_ELF)

hello-world: all download

clean:
	rm -rf iot .Xil
	rm -f *.log *.jou
