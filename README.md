pulsar_cpp
==========
**Introduction**

Working in the finance industry there is always a need to write software that can achieve low latency, high performance characteristics. The LMAX disruptor is a Java technology that has gained a lot of interest by developers in this field.

Pulsar is not a port of disruptor, rather it is an attempt to provide a native C++11 library that implements a lot of the concepts used by disruptor.

The goals for pulsar are as follows:

* Better performance than disruptor
* A simpler API than disruptor

**Usage**

Pulsar applications are implemented in terms of publishers and subscribers. A publisher can be subscribed to by 1 or more subscribers. For example

```
    pulsar::publisher< long > p;
    pulsar::subscriber< long >& s0 = p.subscribe();
    pulsar::subscriber< long >& s1 = p.subscribe();
    pulsar::subscriber< long >& s2 = p.subscribe();
```

In the example above there are 3 subscribers - each one will receive the data published by p. All subscribers operate in parallel - there are no guarantees about the order in which each subscriber will see the data. If p publishers item 1, s1 might see that data before s0 or s2.

It is also possible to chain subscribers together to create a pipeline processing arrangement. For example

```
    pulsar::publisher< long > p;
    pulsar::subscriber< long >& s0 = p.subscribe();
    pulsar::subscriber< long >& s1 = s0.subscribe();
    pulsar::subscriber< long >& s2 = s1.subscribe();
```

This example differs from the first as data is guaranteed to be processed in sequence. If p publishes item 1 it is guaranteed to be received by subscribers s0, s1 and s2 in that order.

To start receiving data on the subscriber side you supply a callable object. The subscriber will invoke the supplied callable everytime an item of data arrives. When you are done subscribing the callable should return false. Here is a simple example using a lambda. In this case subscribe would return after the first invocation of the lambda

```
    s.subscribe( []( const long& e ) {
        return false;
    } );
```

The following code demonstrates how we might publish 100 data items.

```
    for( size_t i=0; i<100; ) {
        p.publish( 1, [&]( long& e ){
            e = i++;
        } );
    }
```

The publish() method expects 2 arguments. Argument 1 is the number of data items we want to publish. Argument 2 is a callable object used to populate each data item. If will be called as many times as specified in argument 1.

**Performance**

The following table compares the performance of pulsar against

* LMAX disruptor 3.3.2 (jdk 8u31) https://github.com/LMAX-Exchange/disruptor
* disruptor-- (most popular C++ disruptor implementation on github) https://github.com/fsaintjacques/disruptor--
* A thread safe std queue implementation using condition variables for synchronization

The test case for each is to pass 100 billion items of data (longs) between 2 threads and measure the time taken to calculate the rate at which data is passed (operations per second)

All tests were executed on a Intel i5 4590 3.30GHz CPU

The source code for the pulsar test can be found here - https://github.com/mmcilroy/pulsar_cpp/blob/master/src/one_to_one_performance_test.cpp

tech           | ops
---------------|------------
pulsar         | 291,923,762
disruptor java | 186,572,620
disruptor--    |   7,328,783
std queue      |   8,244,023

**Building**

