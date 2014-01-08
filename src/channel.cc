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

struct internal {
  std::mutex mutex;
  std::condition_variable condition;
  unsigned long count;
};

semaphore::semaphore(size_t count) {
  internal_ = new internal;
  internal_->count = count;
}

semaphore::~semaphore() {
  delete internal_;
}

semaphore& semaphore::up() {
  std::unique_lock<std::mutex> lock(internal_->mutex);
  
  ++internal_->count;
  internal_->condition.notify_one();

  return *this;
}

semaphore& semaphore::down() {
  std::unique_lock<std::mutex> lock(internal_->mutex);

  while (!internal_->count) {
    internal_->condition.wait(lock);
  }
  --internal_->count;
  
  return *this;
}

}
