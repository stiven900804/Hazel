#pragma once

#include "Hazel/Renderer/Texture.h"
#include "miniaudio.h"

namespace Hazel::Resource
{
	class  TextureManager;
	class  AudioManager;
	class  FontManager;
	class  Font;

	class ResourceManager final
	{
	private:
		 ResourceManager();
	public:
		~ResourceManager();

		void Clear();

		// --- 统一资源访问接口 ---
		Ref<Texture2D> LoadTexture(std::string file_path);
		Ref<Texture2D> GetTexture(std::string fileName);
		void UnloadTexture(std::string fileName);
		glm::vec2 GetTextureSize(std::string fileName);
		void ClearTextures();

		// -- Sound Effects (Chunks) --
		Ref<ma_sound> LoadSound(std::string filePath);
		Ref<ma_sound> GetSound(std::string soundName);
		void UnloadSound(std::string soundName);
		void ClearSounds();

		// -- Music --
		Ref<ma_sound> LoadMusic(std::string filePath);
		Ref<ma_sound> GetMusic(std::string musicName);
		void UnloadMusic(std::string musicName);
		void ClearMusic();

		// -- Fonts --
		Ref<Font> LoadFont(std::string fontPath);
		Ref<Font> GetFont(std::string fontName);
		void UnloadFont(std::string fontName);
		void ClearFonts();

		static ResourceManager* Get();
	private:
		static ResourceManager* sResourceManager; // 单例实例
       
	private:
		Scope<TextureManager> m_TextureManager;
		Scope<AudioManager> m_AudioManager;
		Scope<FontManager> m_FontManager;
	};
}
