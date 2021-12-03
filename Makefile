SUBDIRS := $(wildcard day*)
CXXFLAGS = -std=c++20 -O3 -g -march=native -pedantic -Wall -Wextra

all: $(SUBDIRS)

$(SUBDIRS):
	$(CXX) $@/main.cpp -o $@/out $(CXXFLAGS)

clean:
	rm day*/day*

.PHONY: all $(SUBDIRS) clean
