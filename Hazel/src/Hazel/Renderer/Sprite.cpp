#include "hzpch.h"
#include "Sprite.h"
#include "Hazel/Renderer/Texture.h"
#include "Sprite.h"

namespace Hazel
{

	Sprite::Sprite(std::string_view texturePath, const std::optional<Math::Rect>& source_rect, bool is_flipped)
	{
		m_Texture = Texture2D::Create(std::string(texturePath));
	
		m_SourceRect = source_rect;
		m_IsFlipped = is_flipped;
		m_TexturePath = std::string(texturePath);
	}
}
