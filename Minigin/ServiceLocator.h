#pragma once
#include "Sound/SoundSystem.h"
#include <memory>

namespace dae
{
    class ServiceLocator final
    {
    public:
        static SoundSystem& GetSoundSystem() { return *m_SoundSystemInstance; }

        static void RegisterSoundSystem(std::unique_ptr<SoundSystem>&& ss)
        {
            m_SoundSystemInstance = ss ? std::move(ss) : std::make_unique<NullSoundSystem>(); // if no soundSystem was need. pass along a null one (empty)
        }

    private:
        inline static std::unique_ptr<SoundSystem> m_SoundSystemInstance = std::make_unique<NullSoundSystem>();; // tell compiler, each header is talking about this variable (no cpp needed now)
    };
}

