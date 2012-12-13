#ifndef __ASYNC_H__
#define __ASYNC_H__

#include <functional>

namespace async {

class mutex {
public:
  mutex();
  ~mutex();

  mutex& lock(const std::function<void(void)>&);

private:
  struct internal *internal_;
  mutex(const mutex&);
  void operator=(const mutex&);
};

class semaphore {
public:
  semaphore(size_t count = 1);
  ~semaphore();
  
  semaphore& up();
  semaphore& down();

private:
  struct internal *internal_;
  semaphore(const semaphore&);
  void operator=(const semaphore&);
};

class barrier {};

extern size_t platform_threads;

class pool {
public:
  pool();
  pool(size_t num_threads);
  ~pool();

  pool& sync(const std::function<void(void)>&);
  pool& push(const std::function<void(void)>&);
  pool& push(const barrier&);

  pool& apply(size_t iterations, const std::function<void(size_t idx)>&);

  pool& wait();

  pool& clear();

private:
  struct internal *internal_;
  pool(const pool&);
  void operator=(const pool&);
};

}

#endif