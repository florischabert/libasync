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

#include <iostream>

using namespace std;

#define assert(cond) \
	do { \
		if (!(cond)) { \
			cout << "assertion failed: " << #cond << endl; \
			failed = true; \
		} \
	} while (0)

bool test_pool() {
	bool failed = false;

	async::pool pool;
	int val = 0;

	pool.push([&]{
		assert(val == 0 || val == 2);
		val = 1;
	});

	pool.push([&]{
		assert(val == 0 || val == 1);
		val = 2;
	});

	pool.push(async::barrier());

	pool.push([&]{
		assert(val == 1 || val == 2);
		val = 3;
	});

	pool.wait();

	assert(val == 3);

	return !failed;
}

bool test_gate() {
	bool failed = false;

	async::pool pool;
	async::gate gate;
	int max_workers = 5;
	int max_loops = 10000;
	int val = 0;

	for (int i = 0; i < max_workers; i++) {
		pool.push([&]{
			for (int j = 0; j < max_loops; j++) {
				gate.push([&]{
					val++;
				});
			}
		});
	}

	assert(val == max_workers * max_loops);

	return !failed;
}

bool test_channel() {
	bool failed = false;

	async::pool pool;
	async::channel<int> channel;

	pool.push([&]{
		int val = channel.pull();
		assert(val == 3);
	});

	channel.push(3);

	pool.wait();

	return !failed;
}

int main(int argc, char const *argv[])
{
	struct {
		bool (*func)();
		string name;
	} tests[] = {
		{ test_pool,     "pool"    },
		{ test_gate,     "gate"    },
		{ test_channel,  "channel" },
	};
	int tests_count = sizeof(tests)/sizeof(tests[0]);
	int failed_count = 0;

	cout << "Running..." << endl;

	for (int i = 0; i < tests_count; i++) {
		cout << "-> " <<  tests[i].name << endl;
		if (!tests[i].func()) {
			failed_count++;
		}
	}

	if (failed_count) {
		cout << failed_count << " tests out of ";
		cout << tests_count << " failed." << endl;
	}
	else {
		cout << "All tests passed." << endl;
	}

	return 0;
}