AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -g

all : test
	@echo -n "test\n"

clean :
	rm *.o test

test : main.cpp read_verilog.o current_design.o graph.o
	$(CXX) $(CPPFLAGS) $^ -o $@

read_verilog.o : read_verilog.cpp read_verilog.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

current_design.o : current_design.cpp current_design.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

graph.o : graph.cpp graph.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
