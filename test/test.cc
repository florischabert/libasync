/**
 * @section LICENSE
 * Copyright (c) 2014, Floris Chabert. All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "test.h"

std::vector<test> &get_tests() {
	static std::vector<test> tests;
	return tests;
}

test::test(std::string _name, const std::function<void()> _func)
	: name(_name), func(_func) {
	get_tests().push_back(std::move(*this));
}

int main(int argc, char const *argv[])
{
	int failed_count = 0;

	std::cout << "Testing..." << std::endl;
	for (auto &t : get_tests()) {
		std::cout << "-> " <<  t.name << std::endl;
		try {
			t.func();
		}
		catch (const std::runtime_error& e) {
			std::cerr << e.what() << std::endl;
			failed_count++;
		}
	}

	if (failed_count) {
		std::cout << failed_count << " tests failed." << std::endl;
	}
	else {
		std::cout << "All tests passed." << std::endl;
	}

	return failed_count;
}
