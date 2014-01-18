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

#include <mutex>
#include <condition_variable>
#include <queue>
#include <async.h>

namespace async {

template<class type>
struct channel<type>::impl {
	std::queue<type> queue;
	std::mutex mutex;
	std::condition_variable ready;
};

template<class type>
channel<type>::channel() : pimpl(new impl) {
}

template<class type>
channel<type>::~channel() {
}

template<class type>
channel<type>& channel<type>::push(type val) {
	{
		std::lock_guard<std::mutex> lock(pimpl->mutex);
		pimpl->queue.push(val);
		pimpl->ready.notify_one();
	}

	return *this;
}

template<class type>
type channel<type>::pop() {
	type val;

	{
		std::unique_lock<std::mutex> lock(pimpl->mutex);

		while (pimpl->queue.empty()) {
			pimpl->ready.wait(lock);
		}

		val = pimpl->queue.front();
		pimpl->queue.pop();
	}
	
	return val;
}

template class channel<int>;

}
