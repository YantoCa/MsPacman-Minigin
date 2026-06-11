#pragma once
#include "SoundSystem.h"
#include <memory> 

namespace dae {
	class SDLSoundSystem final : public SoundSystem {
	public:
		SDLSoundSystem(); // could give vector to load sounds in all at once. think about it
		~SDLSoundSystem() override;

		void PlaySound(const soundId id, float volume) override;
		void StopAll() override;

		void LoadSound(const soundId id, const std::string& path);
		void ToggleMute();
	private:
		class SoundImpl; // pimpl
		std::unique_ptr<SoundImpl> m_pImpl;

		bool m_IsMuted{ false };
	};
}