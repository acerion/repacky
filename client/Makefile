ALL_CFLAGS = $(CFLAGS) -I../rpcgen -I../
LDFLAGS = $(shell pkg-config --libs libtirpc) -L../client_lib -lclientlib -Wl,-rpath=$(shell pwd)/../client_lib/




program=client




all:
	cc -c $(ALL_CFLAGS) $(WARNINGS) main.c
	cc *.o -o $(program) $(LDFLAGS)




clean:
	rm -rf *.o $(program)

