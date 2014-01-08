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


bool test_channel() {
		

}

int main(int argc, char const *argv[])
{
	// pools, gates

	async::pool pool();
	async::gate state_gate();
	int state = 0;

	pool.push([]{
		assert(state == 2);

		sleep(2);

		async::gated(state_gate, []{
			state = 1;
		});
	});

	pool.push([]{
		assert(state == 3);

		sleep(1);

		async::gated(state_gate, []{
			state = 2;
		});
	});

	pool.push(async::barrier());

	pool.push([]{
		assert(state == 0);

		async::gated(state_gate, []{
			state = 3;
		});
	});

	pool.wait();

	// channels

	async::channel chan();

	async::run([]{
		std::cout << chan.receive() << std::endl;
	});

	chan.send("hey");

	return 0;
}