AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2

all : test
	@echo -n "test\n"

clean :
	rm *.o test

test : main.cpp read_verilog.o
	$(CXX) $(CPPFLAGS) $^ -o $@

read_verilog.o : read_verilog.cpp read_verilog.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
