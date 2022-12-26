help:
	@echo "fcws test"
	@echo "make clean"
	@echo "make help"
	@echo "make all"

clean:
ifeq ($(OS), Windows_NT)
	(cd src & make clean)
	(cd bin & make clean)
else
	(cd src; make clean)
	(cd bin; make clean)
endif

all:
ifeq ($(OS), Windows_NT)	
	@echo "Wondows" $(OS)
	(cd src 	    & make all)
	(cd src		    & move *.exe "../bin")
	(cd lib/windows/bin & copy *.dll "../../../bin")
	(cd bin		    & fcws)
else
	@echo "Linux" $(OS)
	(cd lib/linux/bin; make all)
	(cd src; 	  	   make all)
	(cd src; 	   	   mv fcws ../bin)
	@echo "Please export LD_LIBRARY_PATH=../lib/linux/bin"
	(cd bin;	   	   taskset 0x01 ./fcws)
endif

