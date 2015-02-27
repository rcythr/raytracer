#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace raytracer {

template <typename TaskTy> class ThreadPool {
    struct EventConcept {
        EventConcept(int val) : event_type(val) {}

        virtual ~EventConcept() {}

        int event_type;
    };

    struct StopEvent : EventConcept {
        StopEvent() : EventConcept(0) {}
    };

    struct RunEvent : EventConcept {
        RunEvent(TaskTy&& task) : EventConcept(1), task(task) {}

        TaskTy task;
    };

    std::vector<std::thread> threads;
    std::queue<std::unique_ptr<EventConcept> > queue;
    std::mutex mutex;
    std::condition_variable cv;

  public:
    typedef std::function<void(TaskTy&)> TaskHandler;

    ThreadPool(size_t num_threads, TaskHandler handler) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads.push_back(std::thread([=]() {
                while (true) {
                    // Retrieve an element from the queue
                    std::unique_ptr<EventConcept> event;
                    {
                        std::unique_lock<std::mutex> ul(mutex);
                        while (queue.empty()) {
                            cv.wait(ul);
                        }

                        event = std::move(queue.front());
                        queue.pop();
                    }

                    if (event->event_type == 0)
                        break;

                    handler(static_cast<RunEvent*>(event.get())->task);
                }
            }));
        }
    }

    void enqueue(TaskTy&& task) {
        {
            std::lock_guard<std::mutex> lg(mutex);
            queue.push(std::unique_ptr<RunEvent>(
                new RunEvent(std::forward<TaskTy>(task))));
        }
        cv.notify_one();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lg(mutex);
            for (size_t i = threads.size(); i > 0; --i) {
                queue.push(std::unique_ptr<StopEvent>(new StopEvent()));
            }
        }
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
