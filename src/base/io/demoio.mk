ABCROOT=/home/VMCentOS-00/abc
ABCEXTROOT=/home/VMCentOS-00/abcExt
EXTROOT=$(PWD)/../../../
IFLAG=-I${ABCROOT}/src -I${ABCEXTROOT}/src
all:
	cd ../../../;\
	make libabcExt.a;\
	cd $(EXTROOT);
	#gcc -DLIN64 -I${ABCROOT}/src -c verCoreExt.c -o verCoreExt.o
	gcc -DLIN64 ${IFLAG} -c demoio.c -o demoio.o
	gcc -DLIN64 ${IFLAG} -c ioWriteVerilogExt.c -o ioWriteVerilogExt.o
	gcc -L${ABCROOT} -L${EXTROOT} -g -o demoio *.o -labc -labcExt -lm -ldl -lrt -rdynamic -lreadline -ltermcap -lpthread