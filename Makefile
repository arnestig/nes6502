PROGNAME=nes6502
NESVIEW=nesparser

CXX = g++
INSTALL = install -o root -g root -m 755
INSTALL_DIR = install -p -d -o root -g root -m 755
INSTALL_DATA = install -p -o root -g root -m 644
CFLAGS += 
CPPFLAGS +=
CXXFLAGS += -g -Wall 
LDFLAGS += -lgtest -pthread

ifneq (,$(filter noopt,$(DEB_BUILD_OPTIONS)))
	CXXFLAGS += -O0
else
	CXXFLAGS += -O2
endif
ifeq (,$(filter nostrip,$(DEB_BUILD_OPTIONS)))
	INSTALL += -s
endif
ifneq (,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
	NUMJOBS = $(patsubst parallel=%,%,$(filter parallel=%,$(DEB_BUILD_OPTIONS)))
	MAKEFLAGS += -j$(NUMJOBS)
endif

OBJFILES := $(patsubst src/%.cpp,obj/%.o,$(wildcard src/*.cpp))
OBJFILES_UNIT := $(patsubst src/unittest/%.cpp,obj/unittest/%.o,$(wildcard src/unittest/*.cpp))
OBJFILES_NESVIEW := $(patsubst src/nesparser/%.cpp,obj/nesparser/%.o,$(wildcard src/nesparser/*.cpp))

all: $(PROGNAME) $(NESVIEW)

$(NESVIEW): $(OBJFILES_NESVIEW)
	$(CXX) -o $(NESVIEW) $(INCLUDE_DIR) $(OBJFILES_NESVIEW) $(LDFLAGS)

$(PROGNAME): $(OBJFILES) $(OBJFILES_UNIT)
	$(CXX) -o $(PROGNAME) $(INCLUDE_DIR) $(OBJFILES) $(OBJFILES_UNIT) $(LDFLAGS)

obj/nesparser/%.o: src/nesparser/%.cpp
	@mkdir -p obj/nesparser
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

obj/unittest/%.o: src/unittest/%.cpp
	@mkdir -p obj/unittest
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

obj/%.o: src/%.cpp 
	@mkdir -p obj
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f $(OBJFILES) $(OBJFILES_UNIT) $(PROGNAME)

rebuild: clean all

install:
	$(INSTALL_DIR) $(DESTDIR)/usr/bin
	$(INSTALL) $(PROGNAME) $(DESTDIR)/usr/bin

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(PROGNAME)

.PHONY: install uninstall

