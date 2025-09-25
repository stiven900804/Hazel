#include "hzpch.h"
#include "FontManager.h"
namespace Hazel::Resource
{
	Ref<Font> FontManager::LoadFont(const std::string& fontPath)
	{
		// 提取文件名作为 fontName
		std::string fontName = std::filesystem::path(fontPath).filename().string();
		if (m_Fonts.find(fontName) != m_Fonts.end())
		{
			HZ_CORE_WARN("Font '{}' with size {} is already loaded.", fontName);
			return m_Fonts[fontName];
		}
		try {
			Ref<Font> font = CreateRef<Font>(fontPath);
			m_Fonts[fontName] = font;
			HZ_CORE_INFO("Loaded font '{}' with size {}.", fontPath);
			return font;
		}
		catch (const std::exception& e) {
			HZ_CORE_ERROR("Failed to load font '{}': {}", fontPath, e.what());
			return nullptr;
		}
	}
	void FontManager::UnloadFont(const std::string& fontName)
	{
		auto it = m_Fonts.find(fontName);
		if (it != m_Fonts.end()) {
			m_Fonts.erase(it);
			HZ_CORE_INFO("Unloaded font '{}'.", fontName);
		}
		else {
			HZ_CORE_WARN("Font '{}' not found for unloading.", fontName);
		}
	}

	Ref<Font> FontManager::GetFont(std::string fontName)
	{
		auto it = m_Fonts.find(fontName);
		if (it != m_Fonts.end()) {
			return it->second;
		}
		HZ_CORE_WARN("Font '{}' not found.", fontName);
		return nullptr;
	}

	void FontManager::ClearFonts()
	{
		m_Fonts.clear();
		HZ_CORE_INFO("Cleared all loaded fonts.");
	}
}
