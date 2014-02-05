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
#include <string>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <async.h>

namespace async {

struct pool::impl {
	std::mutex mutex;
	std::condition_variable event;
	std::condition_variable ready;
	std::vector<std::thread> workers;
	std::queue<std::function<void(void)>> jobs;
	size_t nready;
	bool barrier;
	bool killed;

	impl();
	void worker_thread(void);
};

pool::impl::impl(void) {
	killed = false;
	barrier = false;
	nready = 0;
}

void pool::impl::worker_thread(void) {
	std::unique_lock<std::mutex> lock(mutex);

	while (!killed) {
		std::function<void(void)> job = nullptr;

		while (!killed && (jobs.empty() || barrier)) {
			nready++;
			ready.notify_all();
			event.wait(lock);
			nready--;
		}

		if (!jobs.empty()) {
			job = jobs.front();
			jobs.pop();

			if (job) {
				lock.unlock();
				job();
				lock.lock();
			}
			else {
				barrier = true;
				while (nready != workers.size() - 1) {
					ready.wait(lock);
				}

				barrier = false;
				event.notify_all();
			}
		}
	}
}


	queue& async(group &group, const std::function<void(void)>&);
	queue& sync(group &group, const std::function<void(void)>&);

	pool& apply(size_t iterations, const std::function<void(size_t idx)>&);

	pool& wait();

pool::pool() : pimpl(new impl) {
	const int nthreads = std::thread::hardware_concurrency();

	for (size_t idx = 0; idx < nthreads; idx++) {
		pimpl->workers.push_back(std::thread(&impl::worker_thread, pimpl));
	}
}

pool::~pool() {
	wait();

	pimpl->killed = true;
	pimpl->event.notify_all();

	for (auto &worker : pimpl->workers) {
		worker.join();
	}
}

pool& pool::async(const std::function<void(void)>& job) {
	if (job) {
		std::lock_guard<std::mutex> lock(pimpl->mutex);
		pimpl->jobs.push(job);

		pimpl->event.notify_one();
	}

	return *this;
}

pool& pool::async(group &group, const std::function<void(void)>& job) {
	return *this;
}

pool& pool::sync(const std::function<void(void)>& job) {
	return *this;
}

pool& pool::sync(group &group, const std::function<void(void)>& job) {
	return *this;
}

pool& pool::barrier() {
	{
		std::lock_guard<std::mutex> lock(pimpl->mutex);
		pimpl->jobs.push(nullptr);
	}
	
	pimpl->event.notify_one();

	return *this;
}

pool& pool::wait() {
	std::unique_lock<std::mutex> lock(pimpl->mutex);

	while (pimpl->nready != pimpl->workers.size() || !pimpl->jobs.empty()) {
		pimpl->ready.wait(lock);
	}

	return *this;
}

pool& pool::apply(size_t iterations, const std::function<void(size_t idx)>& job) {
	for (int idx = 0; idx < iterations; idx++) {
		async([&]{
			job(idx);
		});
	}

	return *this;
}

}
