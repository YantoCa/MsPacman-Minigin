#include "SDLSoundSystem.h"

#include "SoundEventQueue.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <map>
#include <iostream> 

#include <thread>
#include <vector>

namespace dae {
    class SDLSoundSystem::SoundImpl {
    public:
        SoundImpl() {
            MIX_Init();
            m_Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
            if (!m_Mixer) {
                std::cerr << "MIX_CreateMixerDevice failed: " << SDL_GetError() << "\n";
                return;
            }
            m_SoundThread = std::thread(&SoundImpl::ThreadWorkerLoop, this);
        }

        ~SoundImpl() {
            m_Queue.Stop();
            if (m_SoundThread.joinable()) {
                m_SoundThread.join();
            }

            for (auto& [id, entry] : m_Sounds) {
                for (MIX_Track* track : entry.second) {
                    MIX_StopTrack(track, 0);
                    MIX_DestroyTrack(track);
                }
                MIX_DestroyAudio(entry.first);
            }

            if (m_Mixer) {
                MIX_DestroyMixer(m_Mixer);
            }
            MIX_Quit();
        }

        void Load(const soundId id, const std::string& path) { 
            MIX_Audio* clip = MIX_LoadAudio(m_Mixer, path.c_str(), true);
            if (clip) {
                m_Sounds.emplace(id, std::pair{ clip, std::vector<MIX_Track*>{ } });
            }
            else {
                std::cerr << "Failed to load audio file: " << path << " (" << SDL_GetError() << ")\n";
            }
        }

        void SubmitPlay(soundId id, float volume) {
            m_Queue.Push([this, id, volume]() {
                auto it = m_Sounds.find(id);
                if (it == m_Sounds.end()) return;

                MIX_Audio* audio = it->second.first;
                auto& tracks = it->second.second;

                MIX_Track* track = nullptr;
                for (MIX_Track* t : tracks) {
                    if (!MIX_TrackPlaying(t)) {
                        track = t;
                        break;
                    }
                }

                if (!track) {
                    track = MIX_CreateTrack(m_Mixer);
                    tracks.push_back(track);
                }

                MIX_SetTrackAudio(track, audio);
                MIX_SetTrackGain(track, volume);
                MIX_PlayTrack(track, 0);
                });
        }

        void SubmitStopAll() {
            m_Queue.Push([this]() {
                for (auto& [id, entry] : m_Sounds) {
                    for (MIX_Track* track : entry.second) {
                        MIX_StopTrack(track, 0);
                    }
                }
                });
        }

    private:
        void ThreadWorkerLoop() {
            std::function<void()> task;
            while (m_Queue.WaitAndPop(task)) {
                task();
            }
        }

        std::map<soundId, std::pair<MIX_Audio*, std::vector<MIX_Track*>>> m_Sounds;
        MIX_Mixer* m_Mixer{ nullptr };
        SoundEventQueue m_Queue{};
        std::thread m_SoundThread;
    };

    // Wrapper
    SDLSoundSystem::SDLSoundSystem() : m_pImpl{ std::make_unique<SoundImpl>() } {}
    SDLSoundSystem::~SDLSoundSystem() = default;
     
    void SDLSoundSystem::PlaySound(const soundId id, float volume) {
        if (!m_IsMuted) m_pImpl->SubmitPlay(id, volume);
    }

    void SDLSoundSystem::StopAll() {
        m_pImpl->SubmitStopAll();
    }

    void SDLSoundSystem::LoadSound(const soundId id, const std::string& path) {
        m_pImpl->Load(id, path);
    }

    void SDLSoundSystem::ToggleMute() {
        m_IsMuted = !m_IsMuted;
    }
}