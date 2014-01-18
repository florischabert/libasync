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

#include <thread>
#include <vector>

#include <async.h>

namespace async {

struct spawn::impl {
	std::thread thread;
};

spawn::spawn(const std::function<void(void)>& func)
	: pimpl(new impl) {
	pimpl->thread = std::thread(func);
}

spawn::~spawn() {
	pimpl->thread.join();
}

struct apply::impl {
	std::vector<std::thread> threads;
};

apply::apply(size_t iterations, const std::function<void(size_t idx)>& func)
	: pimpl(new impl) {
	for (int i = 0; i < iterations; i++) {
		pimpl->threads.push_back(std::thread(func, i));
	}
}

apply::~apply() {
	for (auto &thread : pimpl->threads) {
		thread.join();
	}
}

}
