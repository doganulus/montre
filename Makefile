PACKAGE = montre
LIBPACKAGE = libmontre
VERSION = 0.3.0

PREFIX = /usr/local

CXX      ?= g++
CXXFLAGS += -std=c++0x -Iinclude -DVERSION=\"$(VERSION)\" -O3
LDFLAGS  +=

lib_hdr    = $(wildcard src/libmontre/*.h)
lib_src    = $(wildcard src/libmontre/*.cpp)

dist_files = $(lib_hdr) $(lib_src)

LIBEXT    = .so

.PHONY: all clean install uninstall

all: $(PACKAGE)

$(PACKAGE): $(patsubst %.cpp, %.o, $(lib_src))
	$(CXX) -shared -fPIC $(CXXFLAGS) $(LDFLAGS) $^ $(lib_libs) -o $(LIBPACKAGE)$(LIBEXT)
	pure -c src/main.pure -o $(PACKAGE)

clean:
	rm -f src/libmontre/*.o src/libmontre/*.d $(PACKAGE) $(LIBPACKAGE)$(LIBEXT)

install: $(PACKAGE)
	mkdir -p $(prefix)/lib
	cp $(LIBPACKAGE)$(LIBEXT) $(PREFIX)/lib
	mkdir -p $(prefix)/bin
	cp $(PACKAGE) $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/lib/$(LIBPACKAGE)$(LIBEXT)
	rm $(PREFIX)/bin/$(PACKAGE)

%.o : %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -MD -c $< -o $(patsubst %.cpp, %.o, $<)
