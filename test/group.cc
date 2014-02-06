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

#include <async.h> 
#include "test.h"

test group_enter("group: enter", []{
	const int max_loops = 100;
	bool did_except = false;
	async::group group;

	try {
		for (int i = 0; i < max_loops; i++) {
			group.enter();
		}

		for (int i = 0; i < max_loops; i++) {
			group.leave();
		}

		group.wait();
	}
	catch (async::error& e) {
		did_except = true;
	}
	assert(!did_except);

	try {
		group.leave();
	}
	catch (async::error& e) {
		did_except = true;
	}
	assert(did_except);
});

test group_async("group: async", []{
	const int max_loops = 100;
	async::group group;
	async::pool pool;

	for (int i = 0; i < max_loops; i++) {
		int val = 42;

		group.enter();

		pool.async(group, [&]{
			assert(val == 42 || val == 43);
			val = 43;
		});

		pool.async([&]{
			group.enter();

			assert(val == 42 || val == 43);
			val = 43;
			
			group.leave();
		});

		pool.barrier();

		pool.async([&]{
			group.wait();

			assert(val == 44);
			val = 45;
		});

		pool.async(group, [&]{
			assert(val == 43);
			val = 44;

			group.leave();
		});

		pool.wait();
		assert(val == 45);
	}
});
