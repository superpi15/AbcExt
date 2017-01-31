ABCROOT=/home/VMCentOS-00/abc
all:
	g++ -DLIN64 -I${ABCROOT}/src -c verCore.c -o verCore.o
	g++ -DLIN64 -I${ABCROOT}/src -c demover.c -o demover.o
	g++ -DLIN64 -I${ABCROOT}/src -c ioReadVerilogExt.c -o ioReadVerilogExt.o
	g++ -L${ABCROOT} -g -o demover *.o -labc -lm -ldl -lrt -rdynamic -lreadline -ltermcap -lpthread