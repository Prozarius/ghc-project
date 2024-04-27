#makefile
ghc: ghc.o ghcontrol.o led2472g.o hts221.o lps25h.o
	gcc -g -o ghc ghc.o ghcontrol.o led2472g.o hts221.o lps25h.o -li2c
ghc.o: ghc.c ghcontrol.h
	gcc -g -c ghc.c
ghcontrol.o: ghcontrol.c ghcontrol.h
	gcc -g -c ghcontrol.c
led2472g.o: led2472g.c led2472g.h
	gcc -g -c led2472g.c -li2c
hts221.o: hts221.c hts221.h
	gcc -g -c hts221.c -li2c
lps25h.o: lps25h.c lps25h.h
	gcc -g -c lps25h.c -lic2c
.PHONY: clean
clean:
	rm -f *.o
	touch *
