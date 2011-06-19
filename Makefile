LEX		= flex
YACC		= bison
LEX_FLAG  = -Pparse
YACC_FLAG = -d -p parse

CXX       = g++ -D__ELIMINATION__ -D__PARALLEL__ -lpthread
#CFLAGS    = -g -Iinclude
CFLAGS	 = -O3 -Iinclude -Wall
CSRCS     = $(wildcard src/*.cpp)
CHDRS     = $(wildcard include/*.h)
#COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

COBJS     = obj/main.o obj/simulator.o obj/circuit.o obj/utils.o obj/parseLEX.o obj/parseYY.o

all : bin/cspice

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

obj/%.o : src/%.c
	$(CXX) $(CFLAGS) -c -o $@ $<

obj/%.o : src/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(COBJS) : $(CHDRS)

clean:
	-rm -f obj/* bin/* src/parseYY.cpp src/parseYY.hpp src/parseLEX.cpp

