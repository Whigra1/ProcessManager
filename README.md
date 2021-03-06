# ProcessManager

Project manager using corotines

# What are the coroutines?
The coroutines already exist in many programming languages, may it be Python or C#. Coroutines provide one more way to create asynchronous code. 
New c++20 standard defines 3 new keywords: co_yield, co_await, co_return If any of those keywords occur in the function, then it becomes a coroutine.

 Coroutine example:
 
    resumable my_coro() {
      co_yield 1;
    }
To undersand more clearly whats going on under the hood , the compiler generates some additional code for the coroutines. Every coroutine function is transformed by the compiler to the form similar to this:

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


This process manager has 2 class Process and Processor.

Processor is a main class that execute processes from queue and store resumable coroutines objects. In our cases we must implement priority queue with displacement so to make that we can split program execution in 2 parts:
1) We can start execute coroutine function before we find first suspend point
2) After we find that point we return program control to main function that call coroutine and check whether coroutine is can continue execution or not. If not we delete coroutine from queue and delete it from memory stack otherwise we check process queue again and take first process from queue than continue process, if it previos process or start new. 

We can repeat this 2 parts until process queue not empty. 

 Example:
 
    void Execute() {
        std::map <int, struct resumable*> mp;
        while (!process_queue.empty()) {
            Process p = process_queue.top();
            if (mp.find(p.getID()) == mp.end()) {
                auto canContinue = p.StartExecution();
                mp[p.getID()] = &canContinue;
            }
            else {
                auto canContinue = mp[p.getID()]->resume();
                if (!canContinue) {
                    mp.erase(p.getID());
                    process_queue.pop();
                }
            }

            
        }
      
      
To run this program you can simply create Processor class and fill queue with processes

 Example:

    int main() {
      Processor processor = Processor();
      processor.AddToQueue(Process(3, 1));
      processor.AddToQueue(Process(2, 2));
      processor.AddToQueue(Process(1, 3));
      processor.AddToQueue(Process(4, 4));
      processor.AddToQueue(Process(5, 5));
      processor.AddToQueue(Process(6, 6));
      processor.Execute();
      return 0;
  }
 

 Average arival time plot:    
![alt text](https://github.com/Whigra1/ProcessManager/blob/master/photos/plot1.png)

Intencity plot:    
![alt text](https://github.com/Whigra1/ProcessManager/blob/master/photos/plot2.png)

 Priority plot:    
![alt text](https://github.com/Whigra1/ProcessManager/blob/master/photos/plot3.png)
