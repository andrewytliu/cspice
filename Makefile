LEX       = flex
YACC      = bison
LEX_FLAG  = -Pparse
YACC_FLAG = -d -p parse

CUDA      = nvcc
CXX       = g++

#CFLAGS    = -g -Iinclude
CFLAGS    = -O3 -Iinclude -Wall $(EXTRA)
CUDAFLAGS = -O3 -Iinclude $(EXTRA)#--device-emulation
CSRCS     = $(wildcard src/*.cpp)
CHDRS     = $(wildcard include/*.h)
#COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

COBJS   = obj/main.o obj/simulator.o obj/circuit.o obj/utils.o obj/parseLEX.o obj/parseYY.o
CUOBJS  = $(COBJS) obj/integral.o

default : original
cuda : EXTRA = -D__CUDA__
parallel : EXTRA = -D__CUDA__ -D__PARALLEL__ -lpthread -lrt

ifdef threadcnt
pthread : EXTRA = -D__PARALLEL__ -lpthread -lrt -Dthreadcnt=$(threadcnt)
else
pthread : EXTRA = -D__PARALLEL__ -lpthread -lrt
endif

original : bin/cspice
parallel : bin/cspice-parallel
pthread : bin/cspice-pthread$(threadcnt)
cuda : bin/cspice-cuda

src/parseLEX.cpp: src/parser.l src/parseYY.hpp
	@echo "> lexing: $<"
	@$(LEX) $(LEX_FLAG) -o$@ $<

src/parseYY.cpp src/parseYY.hpp: src/parser.y
	@echo "> yaccing: $<"
	@$(YACC) $(YACC_FLAG) -o parseYY.cpp $<
	@mv parseYY.cpp src/parseYY.cpp
	@mv parseYY.hpp src/parseYY.hpp
	@ln -sf src/parseYY.hpp include/parseYY.hpp

obj/parseYY.o : src/parser.cpp

bin/cspice : $(COBJS)
	$(CXX) $(CFLAGS) -o $@ $(COBJS)

bin/cspice-pthread$(threadcnt) : $(COBJS)
	$(CXX) $(CFLAGS) -o $@ $(COBJS)

bin/cspice-parallel : $(CUOBJS)
	$(CUDA) $(CUDAFLAGS) -o $@ $(CUOBJS)

bin/cspice-cuda : $(CUOBJS)
	$(CUDA) $(CUDAFLAGS) -o $@ $(CUOBJS)

obj/%.o : src/%.c
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/%.o : src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/%.o : src/%.cu
	$(CUDA) $(CUDAFLAGS) -c -o $@ $<

$(COBJS) : $(CHDRS)

clean:
	-rm -f obj/* src/parseYY.cpp src/parseYY.hpp src/parseLEX.cpp

