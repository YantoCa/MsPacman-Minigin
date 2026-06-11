#include "SoundEventQueue.h" 

namespace dae {
    void SoundEventQueue::Push(std::function<void()> event)
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_SoundEvents.push(std::move(event));  
        }
        m_ConditionVariable.notify_one();  
    }

    bool SoundEventQueue::WaitAndPop(std::function<void()>& outEvent)
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
         
        m_ConditionVariable.wait(lock, [this]() { return !m_SoundEvents.empty() || !m_Running; });

        if (!m_Running && m_SoundEvents.empty())
            return false;

        outEvent = std::move(m_SoundEvents.front());
        m_SoundEvents.pop();
        return true;
    }

    void SoundEventQueue::Stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Running = false;
        }
        m_ConditionVariable.notify_all();  
    }
}