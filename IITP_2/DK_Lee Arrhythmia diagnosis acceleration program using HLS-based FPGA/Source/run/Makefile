
help:
	@echo "Makefile Usage:"
	@echo "  make build TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to generate the design for specified Target and Device."
	@echo ""
	@echo "  make run TARGET=<sw_emu/hw_emu/hw> PLATFORM=<FPGA platform>"
	@echo "      Command to run for specified Target."
	@echo ""
	@echo "  make exe "
	@echo "      Command to generate host."
	@echo ""
	@echo "  make xclbin "
	@echo "      Command to generate hardware platform files(xo,xclbin)."
	@echo ""
	@echo "  make clean "
	@echo "      Command to remove the generated files."
	@echo ""

TARGET   := sw_emu
PLATFORM := xilinx_u200_xdma_201830_2
HOST_EXE := host

DB		 := ../data/DB.csv
PEAK	 := ../data/R_peak.csv

KRNL_NAME:= geterr
XO       := $(KRNL_NAME).$(TARGET).$(PLATFORM).xo
XCLBIN   := $(KRNL_NAME).$(TARGET).$(PLATFORM).xclbin

DESIGN   := design.cfg

# Instructions
exe: $(HOST_EXE)

build: $(XCLBIN) emconfig

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O0 -g -std=c++11 -L$(XILINX_XRT)/lib/ -lpthread -lrt -lstdc++
CXXFLAGS2 := -lOpenCL

# Kernel compiler & linker global settings
KRNL_COMPILE_OPTS := -t $(TARGET) --config $(DESIGN) 
KRNL_LINK_OPTS := -t $(TARGET) --config $(DESIGN)

# Building kernel
$(XO): ../src/geterr.cpp
	v++ $(KRNL_COMPILE_OPTS) -c -k $(KRNL_NAME) -I'$(<D)' -o'$@' '$<'

$(XCLBIN): $(XO)
	v++ $(KRNL_LINK_OPTS) -l -o'$@' $(+)
	

# Building Host
$(HOST_EXE): ../src/host.cpp ../src/LLIST_ECG.c
	g++ $(CXXFLAGS) -o $@ $^ $(CXXFLAGS2)

emconfig:
	emconfigutil --platform $(PLATFORM)

run:
ifeq ($(TARGET),$(filter $(TARGET),sw_emu hw_emu))
	XCL_EMULATION_MODE=$(TARGET) ./$(HOST_EXE) $(XCLBIN) $(DB) $(PEAK)
else
	./$(HOST_EXE) $(XCLBIN)	$(DB) $(PEAK)
endif
	
# Clean
clean:
	rm -rf $(HOST_EXE) *geterr.*.$(PLATFORM).* *.log *.json *.xo _x

cleanup: clean
	rm -rf *.csv *.jou xsim.dir .run .Xil

