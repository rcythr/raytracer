#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace raytracer {

template<typename T>
class ThreadPool {

    typedef void (*THandler)(void* ctx, T);

    std::atomic<bool> shutdown_flag;
    std::vector<std::thread> threads;
    std::queue<std::pair<void*, T>> queue;
    THandler handler;

    std::mutex mutex;
    std::condition_variable cv;

  public:
    ThreadPool(size_t num_threads, THandler handler) 
        : shutdown_flag(false) 
        , handler(handler)
    {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.push_back(std::thread([=]() {
                while (true) {
                    // Retrieve an element from the queue
                    std::pair<void*, T> event;
                    {
                        std::unique_lock<std::mutex> ul(mutex);
                        while (queue.empty()) {
                            if (shutdown_flag.load()) {
                                return;
                            }
                            cv.wait(ul);
                        }

                        event = std::move(queue.front());
                        queue.pop();
                    }

                    handler(event.first, event.second);
                }
            }));
        }
    }

    void enqueue(void* ctx, T val) {
        {
            std::lock_guard<std::mutex> lg(mutex);
            queue.push(std::make_pair(ctx, val));
        }
        cv.notify_one();
    }

    void stop() {
        shutdown_flag.store(true);
        cv.notify_all();
    }

    void join() {
        size_t num_threads = threads.size();
        for (size_t i = 0; i < num_threads; ++i) {
            threads[i].join();
        }
    }
};
}
