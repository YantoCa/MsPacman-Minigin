#include "LoggingSoundSystem.h"
#include <iostream>

namespace dae {
    LoggingSoundSystem::LoggingSoundSystem(std::unique_ptr<SoundSystem>&& realSoundSystem)
        : m_pRealSoundSystem(std::move(realSoundSystem)) {
    }

    void LoggingSoundSystem::PlaySound(const soundId id, float volume) {
        std::cout << "[Audio Logs] Intercepted Request -> Play Sound ID: " << id
            << " | Target Volume setting: " << (volume * 100.0f) << "%\n";

        m_pRealSoundSystem->PlaySound(id, volume);
    }

    void LoggingSoundSystem::StopAll() {
        std::cout << "[Audio Logs] Intercepted Request -> Stop All Sound tracks.\n";
        m_pRealSoundSystem->StopAll();
    }
}