#pragma once
#include "Hazel/Renderer/Font.h"
#include <string>
namespace Hazel::Resource
{
	class FontManager
	{
	public:
		Ref<Font> LoadFont(const std::string& fontPath);
		void UnloadFont(const std::string& fontName);
		Ref<Font> GetFont(std::string fontName);
		void ClearFonts();

	private:
		std::unordered_map<std::string, Ref<Font>> m_Fonts;
	};

}
