libasync
========

What is it?
-----------
libasync is a C++11 library for concurrency and multithreading.

libasync allows you to do concurrent work on multithreaded pools and serial queues. There's barriers and work groups for synchronization. Widely inspired by the great dispatch library.
It also provides APIs to create data-driven pipelines composed of different compute steps.

libasync only uses APIs defined in the C++11 standard. This means that it's fully portable as it doesn't depend on platform specific libraries - provided your C++ compiler correctly implements the standard.

How to use it?
--------------

*Pools*

	 // multithreaded pool
	async::pool pool;

	pool.async([]{
		work();
	});

	pool.async([]{
		work();
	});

	pool.barrier();

	pool.async([]{
		work_post_barrier();
	};

	// wait for the tasks to be done
	pool.wait();

	pool.apply(n, [](size_t idx){
		work_chunk(idx);
	});

	// run and wait for completion
	pool.sync([]{
		task();
	};

*Queues*

	// serial queue
	async::queue queue;

	queue.async([]{
		work();
	};

	// run and wait for completion
	queue.sync([]{
		task();
	};

	// using a queue as a lock
	async::queue lock_queue;
	async::pool pool;

	pool.async([&]{
		work();

		lock_queue.sync([]{
			critical();
		};
	};

	pool.async([&]{
		work();

		lock_queue.sync([]{
			critical();
		};
	};

*Groups*

	async::group group;
	async::pool pool;

	pool.async(group, []{
		work();
	});

	pool.async(group, []{
		work();
	});

	pool.async(group, []{
		unrelated_work();
	});

	group.enter();
	work();
	group.leave();

	group.wait();

