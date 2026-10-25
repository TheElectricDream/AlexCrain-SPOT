# Makefile

LINK.o = $(LINK.cc)

CFLAGS = -fPIC -Wall -g -O3 -funroll-loops
CXXFLAGS = -fPIC -Wall -g -O3 -funroll-loops

LDFLAGS = -Wl,-rpath ./
LDLIBS = -lrt

REV = 410

libowlsock.so: owl.o libowl.o owl_rpd.o libowl_socket.o socket.o
	$(LINK.cc) -shared $^ $(LDLIBS) $(PROFILE) -o $@

libowl.o libowl_socket.o: CXXFLAGS += -DLIBOWL_REV=${REV}

clean:
	rm -f libowlsock.so *.o

dep:
	-$(CXX) $(DEPFLAGS) -MM *.cc *.h > .depend

-include .depend
