#ifndef PRODUCERS
#   define PRODUCERS 10
#endif

#ifndef CONSUMERS
#   define CONSUMERS 5
#endif

#include <csignal>
#include <iostream>
#include <queue>
#include <condition_variable>
#include <sstream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <future>

static const size_t PRODUCER_THREADS = PRODUCERS;
static const size_t CONSUMER_THREADS = CONSUMERS;

namespace
{
std::atomic<bool> interrupted;
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
        std::lock_guard<std::mutex> l(logMtx);
        std::cout << str.str() << std::endl;
    }

    template <class T> log& operator<<(T&& t)
    {
        str << t;
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

    bool operator<(const Task& task) {
        return priority < task.priority;
    }

    void exec() const
    {
        log() << "Task " << id << ' ' << "started";
        if (complexity > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(complexity));
        }
        log() << "Task " << id << ' ' << "completed";
    }
}; // struct Task

class Pool
{
    std::vector<std::thread> threads;
    std::queue<Task> taskQ;
    std::condition_variable productionCv;
    std::condition_variable consumptionCv;
    std::mutex productionMtx;
    std::mutex consumptionMtx;
    std::atomic<bool>& stopped;
    std::unique_ptr<Task> taskPtr;

    void producerLoop(size_t tid)
    {
        log() << "producerLoop " << tid << " started";
        Id id {tid, 0};
        while (!stopped) {
            int32_t delay = std::rand() % 10 + 1;
            std::this_thread::sleep_for(std::chrono::seconds(delay));
            id.uid++;
            std::lock_guard<std::mutex> l(productionMtx);
            taskQ.emplace(id);
            productionCv.notify_one();
        }
        log() << "producerLoop " << tid << " stopped";
    }

    void processorLoop(size_t tid)
    {
        log() << "processorLoop " << tid << " started";
        while(!stopped) {
            std::unique_lock<std::mutex> consLock(consumptionMtx);
            log() << "processorLoop: ul(consumptionMtx)";
            consumptionCv.wait(consLock, [this]() {
                log() << "CHECK";
                return taskPtr || stopped;
            });
            log() << "processorLoop: wait";
            if (taskPtr) taskPtr->promise().set_value();
            if (stopped) break;
            if (!taskPtr) continue;
            auto localTaskPtr = std::move(taskPtr);
            consLock.unlock();
            localTaskPtr->exec();
        }
        log() << "processorLoop " << tid << " stopped";
    }
    
    void wakeUpLoop()
    {
        while (!stopped) {
            std::this_thread::sleep_for(std::chrono::microseconds(50));
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
        for (; prodNum != 0; --prodNum) {
            threads.emplace_back([this, prodNum](){producerLoop(prodNum);});
        }

        for (; consNum != 0; --consNum) {
            threads.emplace_back([this, consNum](){processorLoop(consNum);});
        }

        threads.emplace_back([this](){wakeUpLoop();});
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
            log() << "mainLoop";
            std::unique_lock<std::mutex> prodLock(productionMtx);
            productionCv.wait(prodLock, [this](){
                return !taskQ.empty() || stopped;
            });
            if (stopped) break;
            auto localTaskPtr = std::make_unique<Task>(std::move(taskQ.front()));
            taskQ.pop();
            prodLock.unlock();

            auto future = localTaskPtr->promise().get_future();

            std::unique_lock<std::mutex> consLk(consumptionMtx);
            taskPtr = std::move(localTaskPtr);
            consumptionCv.notify_one();
            log() << "mainLoop: consumptionCv.notify_one()";
            consLk.unlock();
            future.get();
            log() << "mainLoop: consumptionCv.notify_one()";
        }
        log() << "mainLoop interrupted";
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