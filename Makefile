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

# platform-specific setup
LIBEXT    = $(shell pkg-config pure --variable DLL)
PIC       = $(shell pkg-config pure --variable PIC)
shared    = $(shell pkg-config pure --variable shared)

ifeq ($(LIBEXT),.dylib)
LDPATH = DYLD_LIBRARY_PATH
else
LDPATH = LD_LIBRARY_PATH
endif

.PHONY: all clean install uninstall

all: $(PACKAGE)

$(PACKAGE): $(patsubst %.cpp, %.o, $(lib_src))
	$(CXX) $(shared) $(PIC) $(soname) $(CXXFLAGS) $(LDFLAGS) $^ $(lib_libs) -o $(LIBPACKAGE)$(LIBEXT)
# We need to set LD_LIBRARY_PATH here so that the library is found without
# hard-coding its path into the executable.
	$(LDPATH)=. pure -c src/main.pure -o $(PACKAGE)

clean:
	rm -f src/libmontre/*.o src/libmontre/*.d $(PACKAGE) $(LIBPACKAGE)$(LIBEXT)

install: $(PACKAGE)
	mkdir -p $(PREFIX)/lib
	cp $(LIBPACKAGE)$(LIBEXT) $(PREFIX)/lib
	mkdir -p $(PREFIX)/bin
	cp $(PACKAGE) $(PREFIX)/bin
	export LD_LIBRARY_PATH=/usr/local/lib:$(LD_LIBRARY_PATH)

uninstall:
	rm $(PREFIX)/lib/$(LIBPACKAGE)$(LIBEXT)
	rm $(PREFIX)/bin/$(PACKAGE)

%.o : %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -MD -c $< -o $(patsubst %.cpp, %.o, $<)
