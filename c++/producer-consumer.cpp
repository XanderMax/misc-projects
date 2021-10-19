#ifndef PRODUCERS
#   define PRODUCERS 10
#endif
#ifndef CONSUMERS
#   define CONSUMERS 5
#endif
#ifndef INTERRUPT_AFTER
#   define INTERRUPT_AFTER 0
#endif

#include <csignal>
#include <iostream>
#include <algorithm>
#include <condition_variable>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <future>
#include <chrono>
#include <iomanip>

namespace
{
const size_t FORCED_INT_TIMEOUT  = INTERRUPT_AFTER;
const size_t PRODUCER_THREADS    = PRODUCERS;
const size_t CONSUMER_THREADS    = CONSUMERS;

std::atomic<bool> interrupted;
const auto start = std::chrono::system_clock::now();
std::mutex _log_mtx;
} // namespace

class log
{
    std::stringstream str;
    std::mutex& logMtx;
    
public:

    log() : logMtx(_log_mtx) {}

    log(log&) = delete;
    log& operator=(log&) = delete;

    ~log()
    {
        auto now = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = now-start;
        std::lock_guard<std::mutex> l(logMtx);
        std::cout << std::setw(10) << elapsed_seconds.count() << ": " << str.str() << std::endl;
    }

    template <class T> log& operator<<(T&& t)
    {
        str << std::forward<T>(t);
        return *this;
    }
}; // class log

struct Id
{
    size_t gid = 0;
    size_t uid = 0;
}; //struct Id

class Task
{
    const Id id;
    const int32_t priority = 0;
    const int32_t complexity = 0;
    std::promise<void> p;
public:
    Task (const Id& id)
        : id(id)
        , priority(std::rand() % 10 + 1)
        , complexity(std::rand() % 5 + 1)
    {
    }

    Task(Task&) = delete;

    Task(Task&& other)
        : id(other.id)
        , priority(other.priority)
        , complexity(other.complexity)
        , p(std::move(other.p))
    {
    }

    Task& operator=(Task&) = delete;

    std::promise<void>& promise()
    {
        return p;
    }

    bool operator<(const Task& task) const {
        return priority < task.priority;
    }

    void exec() const
    {
        constexpr size_t w = 6;
        log() << "Task " << id << '\t' << "started";
        if (complexity > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(complexity));
        }
        log() << "Task " << id << '\t' << "completed";
    }
}; // struct Task

class Pool
{
    using TaskPtr = std::unique_ptr<Task>;
    std::vector<std::thread> threads;
    std::vector<TaskPtr> tasks;
    std::condition_variable productionCv;
    std::condition_variable consumptionCv;
    std::mutex productionMtx;
    std::mutex consumptionMtx;
    std::atomic<bool>& stopped;
    std::unique_ptr<Task> taskPtr;

    std::unique_ptr<Task> popMax()
    {
        auto max = tasks.begin();
        for (auto it = tasks.begin(), end = tasks.end(); it != end; ++it) {
            if (*(*max) < *(*it)) max = it;
        }
        if (max == tasks.end()) return nullptr;
        auto ptr = std::move(*max);
        tasks.erase(max);
        return ptr;
    }

    void producerLoop(size_t tid)
    {
        Id id {tid, 0};
        while (!stopped) {
            int32_t delay = std::rand() % 10 + 1;
            std::this_thread::sleep_for(std::chrono::seconds(delay));
            id.uid++;
            std::lock_guard<std::mutex> l(productionMtx);
            tasks.emplace_back(std::make_unique<Task>(id));
            productionCv.notify_one();
        }
    }

    void processorLoop(size_t tid)
    {
        while(!stopped) {
            std::unique_lock<std::mutex> consLock(consumptionMtx);
            consumptionCv.wait(consLock, [this]() {
                return taskPtr || stopped;
            });
            if (taskPtr) taskPtr->promise().set_value();
            if (stopped) break;
            auto localTaskPtr = std::move(taskPtr);
            consLock.unlock();
            localTaskPtr->exec();
        }
    }
    
    void wakeUpLoop()
    {
        size_t i = FORCED_INT_TIMEOUT;
        while (!stopped) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            if (FORCED_INT_TIMEOUT != 0) if (--i == 0) {
                stopped = true;
            }
        }
        log() << "INTERRUPTED";
        {
            std::lock_guard<std::mutex> l(productionMtx);
            productionCv.notify_all();
        }
        {
            std::lock_guard<std::mutex> l(consumptionMtx);
            if (taskPtr) taskPtr->promise().set_value();
            taskPtr = nullptr;
            consumptionCv.notify_all();
        }
    }

public:
    Pool(size_t prodNum, size_t consNum, std::atomic<bool>& stopped)
        : stopped(stopped)
    {
        threads.emplace_back([this](){wakeUpLoop();});

        for (; prodNum != 0; --prodNum) {
            threads.emplace_back([this, prodNum](){producerLoop(prodNum);});
        }

        for (; consNum != 0; --consNum) {
            threads.emplace_back([this, consNum](){processorLoop(consNum);});
        }
    }

    Pool(Pool&) = delete;
    Pool& operator=(Pool&) = delete;

    ~Pool()
    {
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }

    void mainLoop()
    {
        while(!stopped) {
            std::unique_lock<std::mutex> prodLock(productionMtx);
            productionCv.wait(prodLock, [this](){
                return !tasks.empty() || stopped;
            });
            if (stopped) break;
            auto localTaskPtr = popMax();
            prodLock.unlock();

            auto future = localTaskPtr->promise().get_future();

            std::unique_lock<std::mutex> consLk(consumptionMtx);
            taskPtr = std::move(localTaskPtr);
            consumptionCv.notify_one();
            consLk.unlock();
            future.get();
        }
    }
}; // class ProducerPool

std::ostream& operator<<(std::ostream& out, const Id& id)
{
    out << '\'' << id.gid << '-' << id.uid << '\'';
    return out;
}

void signal_handler(int signal)
{
    interrupted = true;
}

int main()
{
    interrupted = false;
    
    std::signal(SIGINT, signal_handler);

    Pool pp(PRODUCER_THREADS, CONSUMER_THREADS, interrupted);

    try {
        pp.mainLoop();
    } catch (const std::exception& e) {
        log() << "exception occurred: " << e.what();
        interrupted = true;
    } catch (...) {
        log() << "something has been thrown";
        interrupted = true;
    }
    
}