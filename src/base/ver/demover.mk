ABCROOT=/home/VMCentOS-00/abc
ABCEXTROOT=/home/VMCentOS-00/abcExt
EXTROOT=$(PWD)/../../../
IFLAG=-I${ABCROOT}/src -I${ABCEXTROOT}/src
all:
	gcc -DLIN64 ${IFLAG} -c verCoreExt.c -o verCoreExt.o
	gcc -DLIN64 ${IFLAG} -c demover.c -o demover.o
	gcc -DLIN64 ${IFLAG} -c ioReadVerilogExt.c -o ioReadVerilogExt.o
	#gcc -L${ABCROOT} -g -o demover *.o -labc -lm -ldl -lrt -rdynamic -lreadline -ltermcap -lpthread