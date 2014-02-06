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

#include <async.h>

namespace async {

struct group::impl {
	size_t size;
	std::mutex mutex;
	std::condition_variable event;
};

group::group() : pimpl(new impl) {
	pimpl->size = 0;
}

group::~group() {
	wait();
}

group& group::enter() {
	std::lock_guard<std::mutex> lock(pimpl->mutex);
	pimpl->size++;

	return *this;
}

group& group::leave() throw(error) {
	std::lock_guard<std::mutex> lock(pimpl->mutex);

	if (pimpl->size == 0) {
		throw error("Group is empty.");
	}	

	pimpl->size--;
	pimpl->event.notify_all();

	return *this;
}

group& group::wait() {
	std::unique_lock<std::mutex> lock(pimpl->mutex);

	while (pimpl->size > 0) {
		pimpl->event.wait(lock);
	}

	return *this;
}

}
