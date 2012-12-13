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
