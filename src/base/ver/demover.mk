ABCROOT=/home/VMCentOS-00/abc
all:
	gcc -DLIN64 -I${ABCROOT}/src -c verCoreExt.c -o verCoreExt.o
	gcc -DLIN64 -I${ABCROOT}/src -c demover.c -o demover.o
	gcc -DLIN64 -I${ABCROOT}/src -c ioReadVerilogExt.c -o ioReadVerilogExt.o
	gcc -L${ABCROOT} -g -o demover *.o -labc -lm -ldl -lrt -rdynamic -lreadline -ltermcap -lpthread