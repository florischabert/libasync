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

#ifndef __ASYNC_H__
#define __ASYNC_H__

#include <stdexcept>
#include <functional>

namespace async {

struct error : public std::runtime_error {
	error(const char *msg) : std::runtime_error(msg) {}
};

class group {
public:
	group();
	~group();

	group& enter();
	group& leave() throw(error);

	group& wait();

private:
	struct impl; std::unique_ptr<impl> pimpl;
	group(const group&);
	void operator=(const group&);
};

class pool {
public:
	pool();
	~pool();

	pool& async(const std::function<void(void)>&) throw(error);
	pool& sync(const std::function<void(void)>&) throw(error);

	pool& async(group &group, const std::function<void(void)>&) throw(error);
	pool& sync(group &group, const std::function<void(void)>&) throw(error);

	pool& barrier();

	pool& apply(size_t iterations, const std::function<void(size_t idx)>&) throw(error);

	pool& wait();

private:
	struct impl; std::shared_ptr<impl> pimpl;
	pool(const pool&);
	void operator=(const pool&);
};

class queue {
public:
	queue();
	~queue();

	queue& async(const std::function<void(void)>&) throw(error);
	queue& sync(const std::function<void(void)>&) throw(error);

	queue& async(group &group, const std::function<void(void)>&) throw(error);
	queue& sync(group &group, const std::function<void(void)>&) throw(error);

	queue& wait();

private:
	struct impl; std::unique_ptr<impl> pimpl;
	queue(const queue&);
	void operator=(const queue&);
};

}

#endif