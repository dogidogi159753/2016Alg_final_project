AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -g

all : test
	@echo -n "test\n"

clean :
	rm *.o test

test : src/main.o src/read_verilog.o src/current_design.o src/graph.o lib
		$(CXX) $(CPPFLAGS) src/main.o src/read_verilog.o src/current_design.o src/graph.o -ltm_usage -Llib -o test
#	$(CXX) $(CPPFLAGS) $^ -o $@

main.o:	src/main.cpp lib/tm_usage.h
	$(CXX) $(CPPFLAGS) $< -Ilib -o $@

read_verilog.o : src/read_verilog.cpp src/read_verilog.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

current_design.o : src/current_design.cpp src/current_design.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

graph.o : src/graph.cpp src/graph.h
	$(CXX) $(CPPFLAGS) -c $< -o $@
lib:	lib/libtm_usage.a

lib/libtm_usage.a	: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o :	lib/tm_usage.cpp lib/tm_usage.h 
	$(CXX) $(CPPFLAGS) $<

