SUBDIRS := $(wildcard day*)
BINARIES := $(wildcard day*/out)
CXXFLAGS = -std=c++20 -O3 -g -march=native -pedantic -Wall -Wextra

all: $(SUBDIRS)

runall: $(BINARIES)

$(SUBDIRS):
	$(CXX) $@/main.cpp -o $@/out $(CXXFLAGS)

$(BINARIES):
	$@

clean:
	rm day*/out

.PHONY: all runall $(SUBDIRS) $(BINARIES) clean
