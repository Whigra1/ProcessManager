#include <iostream>
#include <queue>
#include <map>
#include <list>
#include <experimental/coroutine>
struct resumable {
    struct promise_type {
        int current_value;
        
        using coro_handle = std::experimental::coroutine_handle<promise_type>;

        auto get_return_object() {
            return coro_handle::from_promise(*this);
        }

        auto initial_suspend() {
            std::cout << "Start execute new process" << std::endl;
            return std::experimental::suspend_always();
        }

        auto final_suspend() noexcept {
            std::cout << "Done execute process process" << std::endl;
            return std::experimental::suspend_always();
        }


        void return_void() {}
        auto yield_value(int value) { // поддержка co_yield
            current_value = value;
            return std::experimental::suspend_always{};
        }
        void unhandled_exception() { std::terminate(); }
    };

    using coro_handle = std::experimental::coroutine_handle<promise_type>;
    resumable(coro_handle handle) : handle_(handle) {}
    
    int current_value() { return handle_.promise().current_value; }

    bool resume() {
        if (!handle_.done()) handle_.resume();
        return !handle_.done();
    }
    ~resumable() {
        
    }
private:
    coro_handle handle_;
};


class Process {
public:
    int priority;
    int tick = 0;
    Process(int priority, int ID) {
        this->priority = priority;
        this->ID = ID;
    }

    resumable StartExecution() {
        int n = this->priority;
        co_yield 0;
        std::cout << "Init process with priority " << priority;
        for (int i = tick; i < priority; i++)
        {
            std::cout << "Executing process with priority " << priority << " And tick " << n << std::endl;
            co_yield i;
        }
    }
public:
    int getID() {
        return ID;
    }
private:
    int ID;
};


class Processor {
public:
    Processor() {}
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
    }
    void AddToQueue(Process req) { process_queue.push(req);  }
private:
    struct ComparePriority {
        bool operator()(Process const& p1, Process const& p2)
        {
            return p1.priority > p2.priority;
        }
    };
    std::list<resumable> resumables;
    std::priority_queue<Process, std::vector<Process>, ComparePriority> process_queue;

};

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
