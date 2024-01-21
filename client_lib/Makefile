ALL_CFLAGS = $(CFLAGS) -I../rpcgen/  -fPIC
LDFLAGS = $(shell pkg-config --libs libtirpc)




lib=libclientlib.so




all:
	cc -c $(ALL_CFLAGS) $(WARNINGS) functions.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) test_functions.c
	cc -c $(ALL_CFLAGS) $(WARNINGS) rpc.c
	cc -c $(ALL_CFLAGS) ../rpcgen/rpc_services_clnt.c
	cc -c $(ALL_CFLAGS) ../rpcgen/rpc_services_xdr.c
	cc *.o -o $(lib) $(LDFLAGS) -shared -fPIC



clean:
	rm -rf *.o $(lib)

