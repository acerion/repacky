CFLAGS  = $(shell pkg-config --cflags libtirpc)
CFLAGS += -std=c99
# CFLAGS += -g -O0

WARNINGS = -Wall -Wextra -pedantic



all:
	make -C rpcgen
	make -C server CFLAGS="$(CFLAGS)" WARNINGS="$(WARNINGS)"
	make -C client_lib CFLAGS="$(CFLAGS)" WARNINGS="$(WARNINGS)"
	make -C client CFLAGS="$(CFLAGS)" WARNINGS="$(WARNINGS)"



clean:
	make -C rpcgen clean
	make -C server clean
	make -C client_lib clean
	make -C client clean
	rm -rf *.html




html:
	pandoc README.md -f markdown -t html > README.html


