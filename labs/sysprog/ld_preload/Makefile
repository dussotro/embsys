SONAME = libhook.so
GCC = gcc

all : hook

hook:
	$(GCC) -g -c -fPIC hook.c -o hook.o
	gcc -g -shared -Wl,-soname,$(SONAME) -o $(SONAME) ptmx.o
	
	

