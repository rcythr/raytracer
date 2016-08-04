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

    std::atomic<bool> shutdown_flag;
    std::vector<std::thread> threads;
    std::queue<T> queue;

    std::mutex mutex;
    std::condition_variable cv;

  public:
    ThreadPool(size_t num_threads) 
        : shutdown_flag(false)
    {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.push_back(std::thread([=]() {
                while (true) {
                    // Retrieve an element from the queue
                    T event;
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

                    event();
                }
            }));
        }
    }

    void enqueue(T val) {
        {
            std::lock_guard<std::mutex> lg(mutex);
            queue.push(val);
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
