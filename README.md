# ProcessManager

Project manager using corotines

Has 2 class Process and Processor.

Processor main class that execute process queue and store resumable objects

Process class mostly created for ordering in processor queue

struct resumable is a special resumable object that store coroutine in struct promise_type so we can create coroutine method that returns resumable struct

 Code:
 
    resumable my_coro() {
      co_yield 1;
    }
Compiler automaticaly refactor code to something like this.

 Code:
 
    Promise promise;
    co_await promise.initial_suspend();
    try {
      // co-routine body
    }
    catch(...) {
      promise.unhandled_exception(); 
    }
    final_suspend:
      co_await promise.final_suspend();
     
![alt text](https://github.com/Whigra1/ProcessManager/tree/master/photos/plot1.png?raw=true)
