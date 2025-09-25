#include "hzpch.h"
#include "TextureManager.h"
#include <unordered_map>
#include <glm/glm.hpp>

namespace Hazel::Resource
{
	Ref<Texture2D> TextureManager::LoadTexture(const std::string& filePath)
    {
		// 提取文件名作为 textureName
		std::string textureName = std::filesystem::path(filePath).filename().string();

		// 如果已经加载则不重复加载
		if (m_Textures.find(textureName) != m_Textures.end())
			return Ref<Texture2D>();

		// 创建并加载纹理
		Ref<Texture2D> texture = Texture2D::Create(filePath);
		if (texture)
		{
			m_Textures[textureName] = texture;
		}
		return texture;	 
    }

	Ref<Texture2D> TextureManager::GetTexture(std::string textureName)
	{
		auto it = m_Textures.find(textureName);
		if (it != m_Textures.end())
			return it->second;


		HZ_CORE_WARN("TextureManager: Failed to load texture '{}'", textureName);
		return nullptr;

	}

	glm::vec2 TextureManager::GetTextureSize(std::string textureName)
	{
		auto it = m_Textures.find(textureName);
		if (it != m_Textures.end() && it->second)
		{
			auto& texture = it->second;
			return glm::vec2(static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()));
		}
		
		// 未找到返回(0,0)
		return glm::vec2(0.0f, 0.0f);
	}

	void TextureManager::UnloadTexture(const std::string& textureName)
	{
		auto it = m_Textures.find(textureName);
		if (it != m_Textures.end())
		{
			it->second.reset();
			m_Textures.erase(it);
		}
	}

	void TextureManager::ClearTextures()
	{
		for (auto& pair : m_Textures)
		{
			pair.second.reset();
		}
		m_Textures.clear();
	}
}
