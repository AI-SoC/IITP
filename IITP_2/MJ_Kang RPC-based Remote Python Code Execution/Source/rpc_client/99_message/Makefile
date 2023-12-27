RPC = msg

CLNT_SRC = client.cpp
CLNT = client

SERV_SRC = server.cpp
SERV = server

CC = g++

rpc: ${RPC}.x
	rpcgen ${RPC}.x

${SERV}: ${SERV_SRC} ${RPC}.h ${RPC}_svc.c
	${CC} -o $@ $^

${CLNT}: ${CLNT_SRC} ${RPC}.h ${RPC}_clnt.c
	${CC} -o $@ $^

all: rpc ${CLNT} ${SERV}

clean:
	rm -rf ${CLNT} ${SERV} ${RPC}_clnt.c ${RPC}_svc.c ${RPC}.h *.log

run_server:
	sudo ./${SERV}

run_client:
	./${CLNT} 155.230.17.83 ${MSG}
