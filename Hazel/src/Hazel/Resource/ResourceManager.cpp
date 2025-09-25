#include "hzpch.h"
#include "Hazel/Resource/ResourceManager.h"
#include "Hazel/Resource/TextureManager.h"
#include "Hazel/Resource/AudioManager.h"
#include "Hazel/Resource/FontManager.h"
#include "Hazel/Renderer/Font.h"

#include "miniaudio.h"

namespace Hazel::Resource
{
	ResourceManager* ResourceManager::sResourceManager = nullptr; // 单例实例
    // 构造函数
    ResourceManager::ResourceManager()
        : m_TextureManager(CreateScope<TextureManager>()),
          m_AudioManager(CreateScope<AudioManager>()),
          m_FontManager(CreateScope<FontManager>())
    {
    }

    // 析构函数
    ResourceManager::~ResourceManager()
    {
        Clear();
    }

	ResourceManager* ResourceManager::Get()
	{
		if (sResourceManager)
		{
			return sResourceManager;
		}
		else
		{
			sResourceManager = new ResourceManager();
			return sResourceManager;
		}
	}

    void ResourceManager::Clear()
    {
        ClearTextures();
        ClearSounds();
        ClearMusic();
        ClearFonts();
    }

    // --- 统一资源访问接口 ---
    Ref<Texture2D> ResourceManager::LoadTexture(std::string file_path)
    {
        return m_TextureManager->LoadTexture(file_path);
    }

    Ref<Texture2D> ResourceManager::GetTexture(std::string fileName)
    {
        return m_TextureManager->GetTexture(fileName);
    }

    void ResourceManager::UnloadTexture(std::string fileName)
    {
        m_TextureManager->UnloadTexture(fileName);
    }

    glm::vec2 ResourceManager::GetTextureSize(std::string fileName)
    {
        return m_TextureManager->GetTextureSize(fileName);
    }

    void ResourceManager::ClearTextures()
    {
        m_TextureManager->ClearTextures();
    }

    // -- Sound Effects (Chunks) --
    Ref<ma_sound> ResourceManager::LoadSound(std::string filePath)
    {
        return m_AudioManager->LoadSound(filePath);
    }

    Ref<ma_sound> ResourceManager::GetSound(std::string soundName)
    {
        return m_AudioManager->GetSound(soundName);
    }

    void ResourceManager::UnloadSound(std::string soundName)
    {
        m_AudioManager->UnloadSound(soundName);
    }

    void ResourceManager::ClearSounds()
    {
        m_AudioManager->ClearSounds();
    }

    // -- Music --
    Ref<ma_sound> ResourceManager::LoadMusic(std::string filePath)
    {
        return m_AudioManager->LoadMusic(filePath);
    }

    Ref<ma_sound> ResourceManager::GetMusic(std::string musicName)
    {
        return m_AudioManager->GetMusic(musicName);
    }

    void ResourceManager::UnloadMusic(std::string musicName)
    {
        m_AudioManager->UnloadMusic(musicName);
    }

    void ResourceManager::ClearMusic()
    {
        m_AudioManager->ClearMusic();
    }

    // -- Fonts --
    Ref<Font> ResourceManager::LoadFont(std::string fontPath)
    {
        return m_FontManager->LoadFont(fontPath);
    }

    Ref<Font> ResourceManager::GetFont(std::string fontName)
    {
        return m_FontManager->GetFont(fontName);
    }

    void ResourceManager::UnloadFont(std::string fontName)
    {
        m_FontManager->UnloadFont(fontName);
    }

    void ResourceManager::ClearFonts()
    {
        m_FontManager->ClearFonts();
    }
}
        
