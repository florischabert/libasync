# Copyright (c) 2014, Floris Chabert. All rights reserved.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

LIB=libasync.a
SRC=$(wildcard src/*.cc)
TESTS=$(wildcard test/*.cc)
CXXFLAGS += -std=c++11 -stdlib=libc++ -Iinclude -MD -MP -Wall -g

all: $(LIB)

%.o: %.cc
	@echo CXX $@
	@$(CXX) $(CXXFLAGS) -MD -MF $(@:.o=.d) -o $@ -c $<

$(LIB): $(SRC:.cc=.o)
	@echo AR $@
	$(AR) rcs $@ $^

test: $(TESTS:.cc=.o) $(LIB)
	@echo LD $@
	$(CXX) $(LDFLAGS) -o $@ $^

-include $(SRC:.cc=.d) $(TESTS:.cc=.d)

clean:
	@$(RM) $(LIB) $(SRC:.cc=.o) $(SRC:.cc=.d) test $(TESTS:.cc=.o) $(TESTS:.cc=.d)

.PHONY: clean


