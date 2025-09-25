#pragma once

#include "Hazel/Core/Base.h"
#include <string>
#include <string_view>

namespace Hazel::Resource {
	class ResourceManager;
}

namespace Hazel
{
	class AudioPlayer
	{
	public:
		AudioPlayer(Ref<Resource::ResourceManager> resourceManager);

		~AudioPlayer() = default;

		int PlaySound(std::string_view soundname, int channel = -1);
      
		int AudioChannelCount = 16; // 默认音频通道数

		bool PlayMusic(std::string_view musicname, int loops = -1, int fadeInMs = 0);
		void StopMusic(int fadeOutMs = 0);
		void PauseMusic();
		void ResumeMusic();
		void SetSoundVolume(float volume, int channel = -1);
		void SetMusicVolume(float volume);
		float GetMusicVolume();
		float GetSoundVolume(int channel = -1);

		static AudioPlayer* Get();
		static AudioPlayer* sAudioPlayer;
	private:
		Ref<Resource::ResourceManager> m_ResourceManager;   ///< @brief 指向 ResourceManager 的非拥有指针，用于加载和管理音频资源。
		std::string m_CurrentMusic="";         ///< @brief 当前正在播放的音乐路径，用于避免重复播放同一音乐。
		std::string m_CurrentSound="";         ///< @brief 当前正在播放的音乐路径，用于避免重复播放同一音乐。

	};
}
