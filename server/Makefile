ALL_CFLAGS = $(CFLAGS)  -I../rpcgen/
LDFLAGS = $(shell pkg-config --libs libtirpc)

program=rpc_server

all:
	cc -c $(ALL_CFLAGS) $(WARNINGS) freeresult.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) fn_gettimeofday.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) fn_add3.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) fn_state_machine.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) fn_transport_test.c
	cc -c $(ALL_CFLAGS) ../rpcgen/rpc_services_xdr.c
	cc -c $(ALL_CFLAGS) ../rpcgen/rpc_services_svc.c
	cc *.o -o $(program) $(LDFLAGS)




clean:
	rm -rf *.o $(program)



