#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae {
	// Decorator
	class LoggingSoundSystem final : public SoundSystem {
	public:
		LoggingSoundSystem(std::unique_ptr<SoundSystem>&& realSoundSystem);
		~LoggingSoundSystem() override = default;

		void PlaySound(const soundId id, float volume) override;
		void StopAll() override;

	private:
		std::unique_ptr<SoundSystem> m_pRealSoundSystem;
	};
}