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
      
		int AudioChannelCount = 16; // Ĭ����Ƶͨ����

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
		Ref<Resource::ResourceManager> m_ResourceManager;   ///< @brief ָ�� ResourceManager �ķ�ӵ��ָ�룬���ڼ��غ͹�����Ƶ��Դ��
		std::string m_CurrentMusic="";         ///< @brief ��ǰ���ڲ��ŵ�����·�������ڱ����ظ�����ͬһ���֡�
		std::string m_CurrentSound="";         ///< @brief ��ǰ���ڲ��ŵ�����·�������ڱ����ظ�����ͬһ���֡�

	};
}
