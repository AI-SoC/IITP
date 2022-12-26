#!/bin/csh -f

if (!(-e cds.lib)) then
	echo "define worklib ./INCA_libs/worklib" > cds.lib
	mkdir INCA_libs
	mkdir INCA_libs/worklib
endif

if(!(-e INCA_libs)) then
	mkdir INCA_libs
	mkdir INCA_libs/worklib
endif
if(!(-e hdl.var)) then
	touch hdl.var
endif

ncvlog -MESSAGES -linedebug -f v_list.v -work worklib tb_TPU.v
ncelab -MESSAGES -ACCESS +RWC -SNAP worklib.tb_TPU tb_TPU
ncsim worklib.tb_TPU $1 $2 
