#pragma once
#include <string>
#include "Hazel/Renderer/Texture.h"

namespace Hazel::Resource
{
	class TextureManager
	{
	public:
		Ref<Texture2D> LoadTexture(const std::string& filePath);
		Ref<Texture2D> GetTexture(std::string textureName);       ///< @brief ���Ի�ȡ�Ѽ��������ָ�룬���δ�������Լ���
		glm::vec2 GetTextureSize(std::string textureName);      ///< @brief ��ȡָ������ĳߴ�
		void UnloadTexture(const std::string& textureName);
		void ClearTextures();

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Textures;
	};
}

