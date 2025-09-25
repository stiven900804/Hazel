#pragma once
#include <string>
#include "Hazel/Renderer/Texture.h"

namespace Hazel::Resource
{
	class TextureManager
	{
	public:
		Ref<Texture2D> LoadTexture(const std::string& filePath);
		Ref<Texture2D> GetTexture(std::string textureName);       ///< @brief 尝试获取已加载纹理的指针，如果未加载则尝试加载
		glm::vec2 GetTextureSize(std::string textureName);      ///< @brief 获取指定纹理的尺寸
		void UnloadTexture(const std::string& textureName);
		void ClearTextures();

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
	};
}

