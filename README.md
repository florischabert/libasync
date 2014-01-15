libasync
========

What is it?
-----------
libasync is a C++11 library for concurrency, multithreading and asynchronous IOs.

libasync allows you to do concurrent work on pools of threads. It provides tools to create data-driven pipelines composed of different compute steps. You also get barriers, channels, gates and everything you need to write complex concurrent algrithms.
The library also supports asynchronous IOs to use with files descriptors, network sockets, etc.

libasync only uses APIs defined in the C++11 standard. This means that it's fully portable as it doesn't depend on platform specific libraries - provided your C++ compiler correctly implements the standard.

How to use it?
--------------

*Threading pools*

	async::pool pool(); // create a thread pool

	pool.push([]{
		// some concurrent computation
	});

	pool.push([]{
		// some more concurrent computation
	});

	pool.push(async::barrier);

	pool.push([]{
	   // to be computed after the barrier
	};

	// let's wait for our computations to be done
	pool.wait();

*Gates*

	async::gate gate();
	
	gate.push([]{
		// critical section
	});

*Pipelining*
	
	async::pipeline<mytype> pipeline();

	pipeline.stage([](mytype &val){
		// 1st stage, work on val
	});

	pipeline.stage([](mytype &val){
		// 2nd stage, work on val
	});

	mytype val;
	pipeline.push(val);

	pipeline.wait();

*Async IOs*

	async::descriptor descriptor();

	descriptor.on_ready([]{
		// handle new descriptor state
	});

	async::socket socket();

	socket.on_receive([]{
		// handle received data
	});
