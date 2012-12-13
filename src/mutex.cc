#include <mutex>

#include <async.h>

namespace async {

struct internal {
  std::mutex mutex;
};

mutex::mutex() {
  internal_ = new internal;
}

mutex::~mutex() {
  delete internal_;
}

mutex& mutex::lock(const std::function<void(void)>& func) {
  std::unique_lock<std::mutex> lock(internal_->mutex);
  func();

  return *this;
}

}
