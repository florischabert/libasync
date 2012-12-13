#include <async.h>

#include <iostream>

using namespace std;

void compute() {
  cout << "compute" << endl;
}

int main(int argc, char const *argv[])
{
  async::pool pool;
  async::mutex mutex;

  for (int i = 0; i < 10; i++) {
    pool.push([&]{
      mutex.lock([&]{
        compute();
      });

      compute();
    });
  }

  pool.push(async::barrier());

  pool.push([&]{
    cout << "after barrier" << endl;
  });

  pool.wait();

  return 0;
}