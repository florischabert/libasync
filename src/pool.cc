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
