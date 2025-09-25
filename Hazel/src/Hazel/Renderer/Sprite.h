#pragma once
#include <optional>
#include "Hazel/Math/Math.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class Sprite final
	{
	public:
		/*
		* @brief Ĭ�Ϲ��캯��������һ���յ�/��Ч�ľ��飩
		*/
		Sprite() = default;

		Sprite(std::string_view texturePath, const std::optional<Math::Rect>& source_rect = std::nullopt, bool is_flipped = false);
		// --- Getters and Setters ---
		std::string_view GetTexturePath() const { return m_TexturePath; }                                     ///< @brief ��ȡ����·��
		const std::optional<Math::Rect>& GetSourceRect() const { return m_SourceRect; }                       ///< @brief ��ȡԴ���� (���ʹ������������Ϊ std::nullopt)
		bool IsFlipped() const { return m_IsFlipped; }                                                        ///< @brief ��ȡ�Ƿ�ˮƽ��ת

		void SetTexturePath(std::string_view texturePath) { m_TexturePath = std::string(texturePath); }       ///< @brief ��������·��
		void SetSourceRect(const std::optional<Math::Rect>& sourceRect) { m_SourceRect = sourceRect; }        ///< @brief ����Դ���� (���ʹ������������Ϊ std::nullopt)
		void SetFlipped(bool flipped) { m_IsFlipped = flipped; }                                              ///< @brief �����Ƿ�ˮƽ��ת
    
		Ref<Texture2D> GetTexture() const { return m_Texture; }
		void SetTexture(const Ref<Texture2D>& texture) { m_Texture = texture; }

		private:
			std::optional<Math::Rect> m_SourceRect;        ///< @brief ��ѡ��Ҫ���Ƶ�������
			bool m_IsFlipped = false;                     ///< @brief �Ƿ�ˮƽ��ת
			std::string m_TexturePath;
			Ref<Texture2D> m_Texture;
	};
}



