

hardware:
	vivado -source -mode tcl -source ./scripts/project.tcl

clean:
	rm -rf iot .Xil
	rm -f *.log *.jou
