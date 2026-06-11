#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <condition_variable>

//std::function<void() = general purpose wrapper for functions that return void (nothing)
namespace dae {
    class SoundEventQueue final
    {
    public:
        SoundEventQueue() = default;
        ~SoundEventQueue() = default;
         
        SoundEventQueue(const SoundEventQueue&) = delete;
        SoundEventQueue& operator=(const SoundEventQueue&) = delete;
        SoundEventQueue(SoundEventQueue&&) = delete;
        SoundEventQueue& operator=(SoundEventQueue&&) = delete;

        void Push(std::function<void()> event);
        bool WaitAndPop(std::function<void()>& outEvent);
        void Stop();
    private:
        std::queue<std::function<void()>>m_SoundEvents{}; 
        std::mutex m_Mutex{};
        std::condition_variable m_ConditionVariable{};

        std::atomic<bool> m_Running{ true };
    };
}