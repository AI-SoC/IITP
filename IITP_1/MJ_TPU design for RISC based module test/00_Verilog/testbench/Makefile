help:
	@echo "make help"
	@echo "make sim"
	@echo "make gui"
	@echo "make clean"

vlog:
	ncvlog -MESSAGES -linedebug -f v_list.v -work worklib tb_TPU.v

elab:
	ncelab -MESSAGES -ACCEESS +RWC -SNAP worklib.tb_TPU tb_TPU

all : sim

sim:
	./sim.sh
gui:
	./sim.sh -gui -run
clean:
	rm -rf *.log
	rm -rf ncsim.key
	rm -rf INCA_libs
	rm -rf cds.lib
	rm -rf hdl.var
	rm -rf *.shm
	rm -rf simvision.sv

cleanup: clean
