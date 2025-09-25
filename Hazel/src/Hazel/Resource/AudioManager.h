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

		// 当前设计中，我们只需要一个AudioManager，所有权不变，所以不需要拷贝、移动相关构造及赋值运算符
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		AudioManager(AudioManager&&) = delete;
		AudioManager& operator=(AudioManager&&) = delete;

		// 首字母大写版本
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
		// 音效存储 (文件路径 -> miniaudio sound对象)
		std::unordered_map<std::string, Ref<ma_sound>> m_Sounds;
		
		// 音乐存储 (文件路径 -> miniaudio music对象)
		std::unordered_map<std::string, Ref<ma_sound>> m_Musics;

		static ma_engine m_Engine;
	};
}

