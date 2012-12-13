libasync
========

- pool
- barrier, sem
- gated

- network, files, non-blocking, async

What is it?
-----------
libasync is a C++11 library which makes concurrency dead easy.

libasync allows you to do concurrent work on pools of threads without breaking a sweat. We handle the complexity for you.
It also provides tools to create data-driven pipelines composed of different compute steps. You also get barriers, semaphores and everything you need to write complex concurrent algrithms.

libasync only uses APIs defined in the C++11 standard. This means that it's fully portable as it doesn't depend on platform specific libraries - provided your C++ compiler correctly implements the standard.

How to use it?
--------------
This can't get easier:

*Threads Pool*

    async::pool pool(); // create a thread pool
    pool << []{
	    // some concurrent computation
    };
    pool << []{
	    // some more concurrent computation
    };

    pool << async::barrier;

    pool << []{
       // to be computed after the barrier
    };

    // let's wait for our computations to be done
    pool.wait();

*Pipeline and Steps*