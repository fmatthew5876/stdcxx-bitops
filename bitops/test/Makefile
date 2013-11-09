CXX=clang++
CXXFLAGS+=-std=c++11 -Iinclude
CXXFLAGS+=-g
CXXFLAGS+=-Wall -Wextra -Werror -Wno-shift-count-overflow
CPPFLAGS+=-I../include -I$(GTEST_INC)
LDFLAGS+=-pthread

TESTS:=shift.test cc.test satmath.test

all: $(TESTS)

run: $(patsubst %.test, run%.test, $(TESTS))

run%.test: %.test
	./$<

%.test: %.o libgtest.a libgtest_main.a
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc
	$(CXX) -MMD $(CXXFLAGS) $(CPPFLAGS) $< -c

clean:
	-rm *.test *.o *.d *.a

distclean: clean
	-rm -rf $(GTEST_DIR) $(GTEST_ZIP)

libgtest.a: libgtest.o
	$(AR) rcs $@ $^

libgtest_main.a: libgtest_main.o
	$(AR) rcs $@ $^

GTEST_VERSION=1.7.0
GTEST_DIR=gtest-$(GTEST_VERSION)
GTEST_INC=$(GTEST_DIR)/include
GTEST_ZIP=$(GTEST_DIR).zip
GTEST_CXXFLAGS=$(CXXFLAGS) -I$(GTEST_DIR) -I$(GTEST_INC) -Wno-error


libgtest_main.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -o $@ -c $(GTEST_DIR)/src/gtest_main.cc $(GTEST_CXXFLAGS)

libgtest.o: $(GTEST_DIR)/src/gtest-all.cc
	$(CXX) -o $@ -c $^ $(GTEST_CXXFLAGS)

$(GTEST_DIR)/src/gtest-all.cc: $(GTEST_ZIP)
	unzip $^
	cd $(GTEST_DIR); patch -p1 < ../gtest.patch
	touch $@

$(GTEST_ZIP):
	wget http://googletest.googlecode.com/files/$(GTEST_ZIP)

