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
	// ����ԭ�� Ref<Resource::ResourceManager> ���캯��
	AudioPlayer::AudioPlayer(Ref<Resource::ResourceManager> resourceManager)
	{
		m_ResourceManager = resourceManager;
		m_CurrentMusic = "";
		m_CurrentSound = "";
		if (!m_ResourceManager) {
			throw std::runtime_error("AudioPlayer ����ʧ��: �ṩ�� ResourceManager ָ��Ϊ�ա�");
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
				sAudioPlayer = new AudioPlayer(Ref<Resource::ResourceManager>(manager, [](Resource::ResourceManager*) {})); // ���ڿ���ƥ�� ResourceManager* ���캯��
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

		// �����ǰ�������ڲ��ţ��ȵ���
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
				ma_sound_set_volume(currentMusic.get(), currentVolume); // �ָ������������´β�������Ϊ0
			} 
			else if (currentMusic) 
			{
				ma_sound_stop(currentMusic.get());
			}
		}

		m_CurrentMusic = std::string(musicname);
		ma_sound_set_looping(music.get(), loops != 0);
		ma_sound_start(music.get());
		// fadeInMs �ɸ��� miniaudio ֧��ʵ�ֵ���
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
		// ���� channel=-1 ��ʾ���õ�ǰ���ŵ���Ч
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

