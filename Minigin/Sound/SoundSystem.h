#pragma once
#include <string>

namespace dae {
	using soundId = unsigned short;

	struct SoundData // look if usefull to keep or not
	{
		const soundId id;
		const std::string path;
	};

	class SoundSystem {
	public:
		virtual ~SoundSystem() = default;

		virtual void PlaySound(const soundId id, float volume) = 0; 
		virtual void StopAll() = 0;
		virtual void ToggleMute() = 0;
	};

	class NullSoundSystem final : public SoundSystem {
	public:
		void PlaySound(const soundId, float) override {} 
		void StopAll() override {}
		void ToggleMute() override {}
	};
}