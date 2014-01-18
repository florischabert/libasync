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

#include <vector>
#include <async.h>
 
namespace async {
	
template<class type>
struct pipeline<type>::impl {
	struct stage;
	std::vector<stage> stages;
};

template<class type>
struct pipeline<type>::impl::stage {
	stage(const std::function<void(type&)>& func);

	const std::function<void(type&)>& func;
	pool pool;
};

template<class type>
pipeline<type>::impl::stage::stage(const std::function<void(type&)>& _func)
	: func(_func) {
}

template<class type>
pipeline<type>::pipeline() : pimpl(new impl) {
}

template<class type>
pipeline<type>::~pipeline() {
	// wait();
}

template<class type>
pipeline<type>& pipeline<type>::stage(const std::function<void(type&)>& func) {
	// if (func) {
	// 	pimpl->stages.push_back(stage(func));
	// }

	return *this;
}

template<class type>
pipeline<type>& pipeline<type>::push(type val) {
	// pimpl::stage &stage = pimpl->stages.first();
	// stage.pool.push(stage.func);

	return *this;
}

template<class type>
pipeline<type>& pipeline<type>::wait() {
	// for (auto &stage : pimpl->stages) {
	// 	stage.pool.wait();
	// }

	return *this;
}

template class pipeline<int>;

}
