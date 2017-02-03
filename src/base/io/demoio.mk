ABCROOT=/home/VMCentOS-00/abc
EXTROOT=$(PWD)/../../../
all:
	cd ../../../;\
	make libabcExt.a;\
	cd $(EXTROOT);
	#gcc -DLIN64 -I${ABCROOT}/src -c verCoreExt.c -o verCoreExt.o
	gcc -DLIN64 -I${ABCROOT}/src -c demoio.c -o demoio.o
	gcc -DLIN64 -I${ABCROOT}/src -c ioWriteVerilogExt.c -o ioWriteVerilogExt.o
	gcc -L${ABCROOT} -L${EXTROOT} -g -o demoio *.o -labc -labcExt -lm -ldl -lrt -rdynamic -lreadline -ltermcap -lpthread