CXX?=g++
STANDARD?=c++14
CXXFLAGS=-std=$(STANDARD) -Wall -pedantic -g -I../../googletest/googletest/include -I.. -I../uint128_t
LDFLAGS=-L../../googletest/build/install/lib -lgtest -lpthread
TARGET=test

TESTCASES  =
TESTCASES += testcases/constructor.o
TESTCASES += testcases/assignment.o
TESTCASES += testcases/typecast.o
TESTCASES += testcases/accessors.o
TESTCASES += testcases/and.o
TESTCASES += testcases/or.o
TESTCASES += testcases/xor.o
TESTCASES += testcases/invert.o
TESTCASES += testcases/leftshift.o
TESTCASES += testcases/rightshift.o
TESTCASES += testcases/logical.o
TESTCASES += testcases/gt.o
TESTCASES += testcases/gte.o
TESTCASES += testcases/lt.o
TESTCASES += testcases/lte.o
TESTCASES += testcases/equals.o
TESTCASES += testcases/notequals.o
TESTCASES += testcases/add.o
TESTCASES += testcases/sub.o
TESTCASES += testcases/mult.o
TESTCASES += testcases/div.o
TESTCASES += testcases/mod.o
TESTCASES += testcases/fix.o
TESTCASES += testcases/unary.o
TESTCASES += testcases/functions.o
TESTCASES += testcases/type_traits.o

all: $(TARGET)

.PHONY: clean clean-all

$(TESTCASES): %.o : %.cpp ../uint256_t.h ../uint128_t/uint128_t.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

../uint128_t/uint128_t.o: ../uint128_t/uint128_t.h ../uint128_t/uint128_t.cpp ../uint128_t/uint128_t.include
	$(CXX) $(CXXFLAGS) -c ../uint128_t/uint128_t.cpp -o $@

../uint256_t.o: ../uint256_t.h ../uint256_t.cpp ../uint256_t.include
	$(CXX) $(CXXFLAGS) -c ../uint256_t.cpp -o $@

$(TARGET): test.cpp ../uint256_t.o ../uint128_t/uint128_t.o $(TESTCASES)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)
	$(MAKE) -C ../uint128_t/tests run

clean:
	rm -f $(TARGET)
	$(MAKE) -C ../uint128_t/tests clean

clean-all:
	rm -f ../uint256_t.o $(TESTCASES)
	$(MAKE) -C ../uint128_t/tests clean-all
