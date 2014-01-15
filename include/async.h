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

#include <functional>

namespace async {

class barrier {};

class pool {
public:
	pool();
	pool(size_t num_threads);
	~pool();

	pool& push(const std::function<void(void)>&);
	pool& push(const barrier&);

	pool& apply(size_t iterations, const std::function<void(size_t idx)>&);

	pool& wait();

	pool& clear();

private:
	class impl; std::shared_ptr<impl> pimpl;
	pool(const pool&);
	void operator=(const pool&);
};

class gate {
public:
	gate();
	~gate();

	gate& push(const std::function<void(void)>&);

private:
	class impl; std::unique_ptr<impl> pimpl;
	gate(const gate&);
	void operator=(const gate&);
};

}

#endif