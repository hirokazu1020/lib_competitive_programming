CXX := g++
CXXFLAGS := -g -std=c++20 -O3 -Wall -march=native

TESTSRC := main.cc $(wildcard */*.cc)
OBJS := $(TESTSRC:%.cc=build/%.o)

DETECTOR := valgrind --leak-check=full

test: build/test
	$(DETECTOR) ./build/test

clean:
	rm -rf build

build/test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -lgtest -lgtest_main -o $@ 

build/%.o: %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I../ $*.cc -MMD -c -o $@

-include $(OBJS:%.o=%.d)
