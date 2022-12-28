.PHONY:run

TARGET=sw_emu

help:
	@echo "Makefile usage:"
	@echo "	make run TARGET=<TARGET>"
	@echo "		Command to run for specified Target"
	@echo "	make exe"
	@echo "		Command to generate host"
	@echo "	make build TARGET=<TARGET>"
	@echo "		Command to generate hardware"
	@echo "	make clean"
	@echo "		Command to remove the generated files"

run:
	./run_me.sh $(TARGET)

exe:
	cd scripts; ./exe.sh
	
build:
	cd scripts; ./build.sh $(TARGET)

clean:
	./cleanup.sh
