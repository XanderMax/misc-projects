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
    Id id;
    int32_t priority = 0;
    int32_t complexity = 0;
public:
    Task(const Id&);

    Task(const Task&) = default;
    Task(Task&&) = default;

    Task& operator=(const Task&) = default;

    bool operator<(const Task& task) const
    {
        return priority < task.priority;
    }

    void exec() const;
}; // struct Task

class PackagedTask
{
    Task task;
    std::promise<void> p;
public:
    template <class T> PackagedTask(T&& task) : task(std::forward<T>(task)) {}

    std::promise<void>& promise()
    {
        return p;
    }

    void exec()
    {
        task.exec();
    }
}; // class PackagedTask

class Pool
{
    using TaskPtr = std::unique_ptr<PackagedTask>;
    std::vector<std::thread> threads;
    std::vector<Task> tasks;
    std::condition_variable productionCv;
    std::condition_variable consumptionCv;
    std::mutex productionMtx;
    std::mutex consumptionMtx;
    std::atomic<bool>& stopped;
    TaskPtr currentTask;

    TaskPtr popMax();
    void producerLoop(size_t);
    void processorLoop();
    void wakeUpLoop();

public:
    Pool(size_t prodNum, size_t consNum, std::atomic<bool>& stopped);
    Pool(Pool&) = delete;
    Pool& operator=(Pool&) = delete;
    ~Pool();

    void mainLoop();
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

Task::Task(const Id& id)
    : id(id)
    , priority(std::rand() % 10 + 1)
    , complexity(std::rand() % 5 + 1)
{
}

void Task::exec() const
{
    log() << "Task " << id << '\t' << "started";
    if (complexity > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(complexity));
    }
    log() << "Task " << id << '\t' << "completed";
}

Pool::Pool(size_t prodNum, size_t consNum, std::atomic<bool>& stopped)
    : stopped(stopped)
{
    threads.emplace_back([this](){wakeUpLoop();});

    for (; prodNum != 0; --prodNum) {
        threads.emplace_back([this, prodNum](){producerLoop(prodNum);});
    }

    for (; consNum != 0; --consNum) {
        threads.emplace_back([this](){processorLoop();});
    }
}

Pool::~Pool()
{
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

void Pool::mainLoop()
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
        currentTask = std::move(localTaskPtr);
        consumptionCv.notify_one();
        consLk.unlock();
        future.get();
    }
}

Pool::TaskPtr Pool::popMax()
{
    auto max = std::max_element(tasks.begin(), tasks.end());
    if (max == tasks.end()) return nullptr;
    auto ptr = std::make_unique<PackagedTask>(std::move(*max));
    tasks.erase(max);
    return ptr;
}

void Pool::producerLoop(size_t tid)
{
    Id id {tid, 0};
    while (!stopped) {
        int32_t delay = std::rand() % 10 + 1;
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        id.uid++;
        std::lock_guard<std::mutex> l(productionMtx);
        tasks.emplace_back(id);
        productionCv.notify_one();
    }
}

void Pool::processorLoop()
{
    while(!stopped) {
        std::unique_lock<std::mutex> consLock(consumptionMtx);
        consumptionCv.wait(consLock, [this]() {
            return currentTask || stopped;
        });
        if (currentTask) currentTask->promise().set_value();
        if (stopped) break;
        auto localTaskPtr = std::move(currentTask);
        consLock.unlock();
        localTaskPtr->exec();
    }
}

void Pool::wakeUpLoop()
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
        if (currentTask) currentTask->promise().set_value();
        currentTask = nullptr;
        consumptionCv.notify_all();
    }
}
