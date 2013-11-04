CXX=clang++
CPPFLAGS=-std=c++11 -Iinclude
CXXFLAGS=-Wall -Wextra -Werror -Wno-shift-count-overflow

test: test.o
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) -MMD $(CXXFLAGS) $(CPPFLAGS) $< -c

clean:
	-rm test *.o *.d

-include test.d
