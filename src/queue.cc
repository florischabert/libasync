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

struct queue::impl {
	std::mutex mutex;
	std::condition_variable event;
	std::condition_variable ready;
	std::thread worker;
	std::queue<std::function<void(void)>> jobs;
	bool killed;

	impl();
	void worker_thread(void);
};

queue::impl::impl() {
	killed = false;
	worker = std::thread(&impl::worker_thread, this);
}

void queue::impl::worker_thread(void) {
	std::unique_lock<std::mutex> lock(mutex);

	while (!killed) {
		std::function<void(void)> job = nullptr;

		while (!killed && jobs.empty()) {
			ready.notify_all();
			event.wait(lock);
		}

		if (!jobs.empty()) {
			job = jobs.front();
			jobs.pop();

			lock.unlock();
			job();
			lock.lock();
		}
	}
}

queue::queue() : pimpl(new impl) {
}

queue::~queue() {
	wait();

	pimpl->killed = true;
	pimpl->event.notify_all();
	pimpl->worker.join();
}

queue& queue::async(const std::function<void(void)>& job) throw(error) {
	if (!job) {
		throw error("Bad job.");
	}

	std::lock_guard<std::mutex> lock(pimpl->mutex);
	pimpl->jobs.push(job);

	pimpl->event.notify_one();

	return *this;
}

queue& queue::async(group &group, const std::function<void(void)>& job) throw(error) {
	if (!job) {
		throw error("Bad job.");
	}

	group.enter();

	async([&] {
		job();
		group.leave();
	});

	return *this;
}

queue& queue::sync(const std::function<void(void)>& job) throw(error) {
	bool done = false;
	std::mutex sync_mutex;
	std::condition_variable sync_event;

	if (!job) {
		throw error("Bad job.");
	}

	async([&]{
		job();

		std::lock_guard<std::mutex> sync_lock(sync_mutex);
		done = true;
		sync_event.notify_all();
	});

	std::unique_lock<std::mutex> sync_lock(sync_mutex);
	while (!done) {
		sync_event.wait(sync_lock);
	}

	return *this;
}

queue& queue::sync(group &group, const std::function<void(void)>& job) throw(error) {
	if (!job) {
		throw error("Bad job.");
	}

	group.enter();

	sync([&] {
		job();
		group.leave();
	});

	return *this;
}

queue& queue::wait() {
	std::unique_lock<std::mutex> lock(pimpl->mutex);

	while (!pimpl->jobs.empty()) {
		pimpl->ready.wait(lock);
	}

	return *this;
}

}
