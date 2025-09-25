#include "hzpch.h"
#include "AudioPlayer.h"

#include "Hazel/Resource/ResourceManager.h"
#include <string>
#include <string_view>
#include <unordered_map>
#include <mutex>

#include "miniaudio.h"

namespace Hazel
{
	// 保留原有 Ref<Resource::ResourceManager> 构造函数
	AudioPlayer::AudioPlayer(Ref<Resource::ResourceManager> resourceManager)
	{
		m_ResourceManager = resourceManager;
		m_CurrentMusic = "";
		m_CurrentSound = "";
		if (!m_ResourceManager) {
			throw std::runtime_error("AudioPlayer 构造失败: 提供的 ResourceManager 指针为空。");
		}
	}

	AudioPlayer* AudioPlayer::Get()
	{
		if (sAudioPlayer)
		{
			return sAudioPlayer;
		}
		else
		{
			Resource::ResourceManager* manager = Resource::ResourceManager::Get();
			if (manager)
			{
				sAudioPlayer = new AudioPlayer(Ref<Resource::ResourceManager>(manager, [](Resource::ResourceManager*) {})); // 现在可以匹配 ResourceManager* 构造函数
				return sAudioPlayer;
			}
			else
			{
				return nullptr;
			}
		}
	}
  
	int AudioPlayer::PlaySound(std::string_view soundname, int channel)
	{
		auto sound = m_ResourceManager->GetSound(std::string(soundname));
		if (!sound)
			return -1;
		ma_sound_start(sound.get());
		m_CurrentSound = soundname;
		return 0;
	}

    bool AudioPlayer::PlayMusic(std::string_view musicname, int loops, int fadeInMs)
    {
		auto music = m_ResourceManager->GetMusic(std::string(musicname));
		if (!music)
		return false;

		// 如果当前有音乐在播放，先淡出
		if (!m_CurrentMusic.empty())
		{
			auto currentMusic = m_ResourceManager->GetMusic(m_CurrentMusic);
			if (currentMusic && fadeInMs > 0) 
			{
				float currentVolume = ma_sound_get_volume(currentMusic.get());
				const int steps = 20;
				float stepVolume = currentVolume / steps;
				int stepDelay = fadeInMs / steps;
				for (int i = 0; i < steps; ++i) {
								ma_sound_set_volume(currentMusic.get(), currentVolume - stepVolume * (i + 1));
								std::this_thread::sleep_for(std::chrono::milliseconds(stepDelay));
				}
				ma_sound_stop(currentMusic.get());
				ma_sound_set_volume(currentMusic.get(), currentVolume); // 恢复音量，避免下次播放音量为0
			} 
			else if (currentMusic) 
			{
				ma_sound_stop(currentMusic.get());
			}
		}

		m_CurrentMusic = std::string(musicname);
		ma_sound_set_looping(music.get(), loops != 0);
		ma_sound_start(music.get());
		// fadeInMs 可根据 miniaudio 支持实现淡入
		return true;
    }

	void AudioPlayer::StopMusic(int fadeOutMs)
	{
		if (m_CurrentMusic.empty())
			return;
		auto music = m_ResourceManager->GetMusic(m_CurrentMusic);
		if (music)
		{
			if (fadeOutMs > 0) 
			{
				float currentVolume = ma_sound_get_volume(music.get());
				const int steps = 20;
				float stepVolume = currentVolume / steps;
				int stepDelay = fadeOutMs / steps;
				for (int i = 0; i < steps; ++i) 
				{
					ma_sound_set_volume(music.get(), currentVolume - stepVolume * (i + 1));
					std::this_thread::sleep_for(std::chrono::milliseconds(stepDelay));
				}
			}
			ma_sound_stop(music.get());
		}
		m_CurrentMusic.clear();
	}

	void AudioPlayer::PauseMusic()
	{
		if (m_CurrentMusic.empty())
			return;
		auto music = m_ResourceManager->GetMusic(m_CurrentMusic);
		if (music)
			ma_sound_stop(music.get());
	}

	void AudioPlayer::ResumeMusic()
	{
		if (m_CurrentMusic.empty())
			return;
		auto music = m_ResourceManager->GetMusic(m_CurrentMusic);
		if (music)
			ma_sound_start(music.get());
	}



	void AudioPlayer::SetMusicVolume(float volume)
	{
		if (m_CurrentMusic.empty())
			return;
		auto music = m_ResourceManager->GetMusic(m_CurrentMusic);
		if (music)
			ma_sound_set_volume(music.get(), volume);
	}

	float AudioPlayer::GetMusicVolume()
	{
		if (m_CurrentMusic.empty())
			return 0.0f;
		auto music = m_ResourceManager->GetMusic(m_CurrentMusic);
		if (music)
			return ma_sound_get_volume(music.get());
		return 0.0f;
	}

	void AudioPlayer::SetSoundVolume(float volume, int channel)
	{
		// 假设 channel=-1 表示设置当前播放的音效
		if (m_CurrentSound.empty())
			return;
		auto sound = m_ResourceManager->GetSound(m_CurrentSound);
		if (sound)
			ma_sound_set_volume(sound.get(), volume);
	}

	float AudioPlayer::GetSoundVolume(int channel)
	{
		if (m_CurrentSound.empty())
			return 0.0f;
		auto music = m_ResourceManager->GetSound(m_CurrentSound);
		if (music)
			return ma_sound_get_volume(music.get());
		return 1.0f;
	}
}

