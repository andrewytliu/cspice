CXX       = g++
CFLAGS    = -g
CSRCS     = $(wildcard *.cpp)
CHDRS     = $(wildcard *.h)
COBJS     = $(addsuffix .o, $(basename $(CSRCS)))

all : cspice

cspice : $(COBJS)
	$(CXX) $(CFLAGS) -o cspice $(COBJS)

%.o : %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

$(COBJS) : $(CHDRS)

clean:
	rm -f $(COBJS)

