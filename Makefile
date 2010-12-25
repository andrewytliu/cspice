CXX       = g++
CFLAGS    = -g -Iinclude
CSRCS     = $(wildcard src/*.cpp)
CHDRS     = $(wildcard include/*.h)
#COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

COBJS     = obj/main.o obj/parser.o obj/simulator.o obj/circuit.o obj/utils.o

all : bin/cspice

bin/cspice : $(COBJS)
	$(CXX) $(CFLAGS) -o $@ $(COBJS)

obj/%.o : src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(COBJS) : $(CHDRS)

clean:
	-rm -f obj/* bin/*

