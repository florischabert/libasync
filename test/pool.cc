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

test pool_async("pool: async", []{
	const int max_loops = 100;
	const int max_val = 100;
	async::pool pool;
	async::queue lock_queue;

	for (int i = 0; i < max_loops; i++) {
		int val = 0;

		for (int i = 0; i < max_val; i++) {
			pool.async([&]{
				lock_queue.sync([&]{
					val++;
				});
			});
		}

		pool.barrier();

		pool.async([&]{
			assert(val == max_val);
			val = 42;
		});

		pool.wait();
		assert(val == 42);
	}
});

test pool_sync("pool: sync", []{
	const int max_loops = 100;
	async::pool pool;

	for (int i = 0; i < max_loops; i++) {
		int val = 0;
		
		pool.sync([&]{
			assert(val == 0);
			val = 42;
		});
		assert(val == 42);
	}
});

test pool_apply("pool: apply", []{
	const int max_idx = 100;
	async::pool pool;
	int val = 0;

	pool.apply(max_idx, [&](size_t idx){
			val += idx;
	});

	int canary = 0;
	for (int i = 0; i < max_idx; i++) {
		canary += i;
	}
	assert(canary == val);
});	
