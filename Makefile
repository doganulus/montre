PACKAGE = libmontre
VERSION = 0.1.0

CXX      ?= g++ 
CXXFLAGS += -std=c++0x -Iinclude -DVERSION=\"$(VERSION)\" -O3
LDFLAGS  += 
prefix   ?= /usr/local

libmontre_hdr    = $(wildcard libmontre/*.h)
libmontre_src    = $(wildcard libmontre/*.cpp)

dist_files = $(libmontre_hdr) $(libmontre_src)

ifeq ($(MSYSTEM), MINGW32)
  EXEEXT    = .exe  
  LIBEXT    = .dll
else
  EXEEXT    =
  LIBEXT    = .so  
endif

.PHONY: all check clean install uninstall dist

all: $(PACKAGE)$(LIBEXT)

$(PACKAGE)$(LIBEXT): $(patsubst %.cpp, %.o, $(lib_src))
	$(CXX) -shared -fPIC $(CXXFLAGS) $(LDFLAGS) $^ $(lib_libs) -Wl,-soname=$(patsubst %$(LIBEXT),lib%.a, $@) -o $@

check: test-$(PACKAGE)$(EXEEXT)	
	./test-$(PACKAGE)$(EXEEXT)

test-$(PACKAGE)$(EXEEXT): $(PACKAGE)$(LIBEXT) $(patsubst %.cpp, %.o, $(test_src))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(test_libs) -o $@

clean: 
	rm -f src/*.o src/*.d test/*.o test/*.d $(PACKAGE)$(LIBEXT) test-$(PACKAGE)$(EXEEXT)	

dist:
	mkdir $(PACKAGE)-$(VERSION)
	cp --parents $(dist_files) $(PACKAGE)-$(VERSION)
	tar -czvf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)

install: $(PACKAGE)$(LIBEXT)
	mkdir -p $(prefix)/include/$(PACKAGE)
	cp $(headers) $(prefix)/include/$(PACKAGE)
	mkdir -p $(prefix)/lib
	cp lib$(PACKAGE).a $(prefix)/lib
	mkdir -p $(prefix)/bin
	cp $(PACKAGE)$(LIBEXT) $(prefix)/bin

uninstall:
	rm -r $(prefix)/include/$(PACKAGE)
	rm $(prefix)/lib/lib$(PACKAGE).a
	rm $(prefix)/bin/$(PACKAGE)$(LIBEXT)

%.o : %.cpp
	$(CXX) -fPIC $(CXXFLAGS) -MD -c $< -o $(patsubst %.cpp, %.o, $<)	

ifneq "$(MAKECMDGOALS)" "clean"
deps  = $(patsubst %.cpp, %.d, $(lib_src))
deps += $(patsubst %.cpp, %.d, $(test_src))
-include $(deps)
endif
