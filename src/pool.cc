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

#include <async.h>

namespace async {

class worker {
  std::thread thread;
public:
  void wait() {
  }
};

struct internal {
  std::vector<worker> workers;
  std::vector<std::function<void(void)>> works;
};

pool::pool(): pool(std::thread::hardware_concurrency()) {

}

pool::pool(size_t nthreads) {
  internal_ = new internal;

  for (size_t idx = 0; idx < nthreads; idx++) {
    internal_->workers.push_back(worker());
  }
}

pool::~pool() {
  delete internal_;
}

pool& pool::sync(const std::function<void(void)>& work) {
  internal_->works.insert(internal_->works.begin(), work);
  return *this;
}

pool& pool::push(const std::function<void(void)>& work) {
  internal_->works.insert(internal_->works.begin(), work);
  return *this;
}

pool& pool::apply(size_t iterations, const std::function<void(size_t idx)>&) {
  return *this;
}

pool& pool::push(const class barrier&) {
  return *this;
}

pool& pool::wait() {
  for (worker &w : internal_->workers) {
    w.wait();
  }
  return *this;
}

pool& pool::clear() {
  return *this;
}

}