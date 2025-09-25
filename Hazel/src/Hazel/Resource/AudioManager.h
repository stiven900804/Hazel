#pragma once

#include "miniaudio.h"
#include "Hazel/Audio/AudioPlayer.h"
#include "Hazel/Core/Base.h"
#include <unordered_map>

namespace Hazel::Resource
{
	class AudioManager final
	{
	public:
		AudioManager();

		~AudioManager();

		// ��ǰ����У�����ֻ��Ҫһ��AudioManager������Ȩ���䣬���Բ���Ҫ�������ƶ���ع��켰��ֵ�����
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		AudioManager(AudioManager&&) = delete;
		AudioManager& operator=(AudioManager&&) = delete;

		// ����ĸ��д�汾
		Ref<ma_sound> LoadSound(std::string filePath);
		Ref<ma_sound> GetSound(std::string soundName);
		void UnloadSound(std::string soundName);
		void ClearSounds();

		Ref<ma_sound> LoadMusic(std::string filePath);
		Ref<ma_sound> GetMusic(std::string musicName);
		void UnloadMusic(std::string musicName);

		void ClearMusic();

		void ClearAudio();

	private:
		// ��Ч�洢 (�ļ�·�� -> miniaudio sound����)
		std::unordered_map<std::string, Ref<ma_sound>> m_Sounds;
		
		// ���ִ洢 (�ļ�·�� -> miniaudio music����)
		std::unordered_map<std::string, Ref<ma_sound>> m_Musics;

		static ma_engine m_Engine;
	};
}

