#include "hzpch.h"
#include "Hazel/Resource/AudioManager.h"
#include <stdexcept>

namespace Hazel::Resource
{
	// ��̬��Ա��������
	ma_engine AudioManager::m_Engine;

	AudioManager::AudioManager()
	{
		static bool s_EngineInitialized = false;
		if (!s_EngineInitialized) {
			ma_result result = ma_engine_init(nullptr, &m_Engine);
			if (result != MA_SUCCESS) {
				throw std::runtime_error("AudioManager ����: ma_engine_init ʧ��");
			}
			HZ_CORE_TRACE("AudioManager ����ɹ���");
			s_EngineInitialized = true;
		}
	}

	AudioManager::~AudioManager()
	{
	}

	Ref<ma_sound> AudioManager::LoadSound(std::string filePath)
	{
		// ��ȡ�ļ�����Ϊ soundName
		std::string soundName = std::filesystem::path(filePath).filename().string();

		// ����Ѿ�������ֱ�ӷ��ظ���
		auto it = m_Sounds.find(soundName);
		if (it != m_Sounds.end())
			return (it->second);

		auto sound = CreateRef<ma_sound>();
		ma_result result = ma_sound_init_from_file(&m_Engine, std::string(filePath).c_str(), 0, nullptr, nullptr, sound.get());
		if (result != MA_SUCCESS) {
			throw std::runtime_error("AudioManager ����: ������Чʧ��: " + std::string(filePath));
		}

		m_Sounds[soundName] = std::move(sound);
		
		return m_Sounds[soundName];
	}

	Ref<ma_sound> AudioManager::GetSound(std::string soundName)
	{
		auto it = m_Sounds.find(soundName);
		if (it != m_Sounds.end()) {
			return (it->second);
		}
		return nullptr;
	}

	void AudioManager::UnloadSound(std::string soundName)
	{
		auto it = m_Sounds.find(soundName);
		if (it != m_Sounds.end()) {
			ma_sound_uninit(it->second.get());
			m_Sounds.erase(it);
		}
	}

	void AudioManager::ClearSounds()
	{
		for (auto& [_, sound] : m_Sounds) {
			ma_sound_uninit(sound.get());
		}
		m_Sounds.clear();
	}

	Ref<ma_sound> AudioManager::LoadMusic(std::string filePath)
	{
		// ��ȡ�ļ�����Ϊ soundName
		std::string musicName = std::filesystem::path(filePath).filename().string();

		// ����Ѿ�������ֱ�ӷ��ظ���
		auto it = m_Musics.find(musicName);
		if (it != m_Musics.end())
			return it->second;

		auto music = CreateScope<ma_sound>();
		ma_result result = ma_sound_init_from_file(&m_Engine, std::string(filePath).c_str(), 0, nullptr, nullptr, music.get());
		if (result != MA_SUCCESS) {
			throw std::runtime_error("AudioManager ����: ������Чʧ��: " + std::string(filePath));
		}

		m_Musics[musicName] = std::move(music);

		return m_Musics[musicName];
	}

	Ref<ma_sound> AudioManager::GetMusic(std::string musicName)
	{
		auto it = m_Musics.find(std::string(musicName));
		if (it != m_Musics.end()) {
			return (it->second);
		}
		return nullptr;
	}

	void AudioManager::UnloadMusic(std::string musicName)
	{
		auto it = m_Musics.find(std::string(musicName));
		if (it != m_Musics.end()) {
			ma_sound_uninit(it->second.get());
			m_Musics.erase(it);
		}
	}

	void AudioManager::ClearMusic()
	{
		for (auto& [_, music] : m_Musics) {
			ma_sound_uninit(music.get());
		}
		m_Musics.clear();
	}

	void AudioManager::ClearAudio()
	{
		ClearSounds();
		ClearMusic();
	}
}
