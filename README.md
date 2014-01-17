libasync
========

What is it?
-----------
libasync is a C++11 library for concurrency and multithreading.

libasync allows you to do concurrent work on pools of threads. It provides APIs to create data-driven pipelines composed of different compute steps. You also get barriers, channels, gates and all the tools write complex concurrent algorithms.

libasync only uses APIs defined in the C++11 standard. This means that it's fully portable as it doesn't depend on platform specific libraries - provided your C++ compiler correctly implements the standard.

How to use it?
--------------

*Simple threads*

	async::spawn([]{
		// running on its own thread
	});

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

	// wait for our computations to be done
	pool.wait();

*Gates*

	async::gate gate();
	
	gate.push([]{
		// critical section
	});

*Channels*

	async::channel<std::string> channel();
	
	async::spawn([&]{
		std::cout << channel.pop() << std::endl;
	});

	async::spawn([&]{
		channel.push("hey");
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

