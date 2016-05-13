# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	:	bin/parallel_computing
	@echo -n ""

# optimized version
bin/parallel_computing	:	parallel_computing.o lib
			$(CC) $(OPTFLAGS) parallel_computing.o -ltm_usage -Llib -o bin/parallel_computing
parallel_computing.o	:	src/parallel_computing.cc lib/tm_usage.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@


lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

