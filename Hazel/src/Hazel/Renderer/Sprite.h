#pragma once
#include <optional>
#include "Hazel/Math/Math.h"
#include "Hazel/Renderer/Texture.h"

namespace Hazel {

	class Sprite final
	{
	public:
		/*
		* @brief 默认构造函数（创建一个空的/无效的精灵）
		*/
		Sprite() = default;

		Sprite(std::string_view texturePath, const std::optional<Math::Rect>& source_rect = std::nullopt, bool is_flipped = false);
		// --- Getters and Setters ---
		std::string_view GetTexturePath() const { return m_TexturePath; }                                     ///< @brief 获取纹理路径
		const std::optional<Math::Rect>& GetSourceRect() const { return m_SourceRect; }                       ///< @brief 获取源矩形 (如果使用整个纹理则为 std::nullopt)
		bool IsFlipped() const { return m_IsFlipped; }                                                        ///< @brief 获取是否水平翻转

		void SetTexturePath(std::string_view texturePath) { m_TexturePath = std::string(texturePath); }       ///< @brief 设置纹理路径
		void SetSourceRect(const std::optional<Math::Rect>& sourceRect) { m_SourceRect = sourceRect; }        ///< @brief 设置源矩形 (如果使用整个纹理则为 std::nullopt)
		void SetFlipped(bool flipped) { m_IsFlipped = flipped; }                                              ///< @brief 设置是否水平翻转
    
		Ref<Texture2D> GetTexture() const { return m_Texture; }
		void SetTexture(const Ref<Texture2D>& texture) { m_Texture = texture; }

		private:
			std::optional<Math::Rect> m_SourceRect;        ///< @brief 可选：要绘制的纹理部分
			bool m_IsFlipped = false;                     ///< @brief 是否水平翻转
			std::string m_TexturePath;
			Ref<Texture2D> m_Texture;
	};
}



