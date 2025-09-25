#include "hzpch.h"
#include "UIInteractive.h"
#include "Hazel/Resource/ResourceManager.h" // ��������Ƶ������
#include "Hazel/Renderer/Renderer2D.h" // ��������Ƶ������
#include "Hazel/Audio/AudioPlayer.h"

namespace Hazel
{
	UIInteractive::UIInteractive(glm::vec2 position, glm::vec2 size)
		: UIElement(position, size)
	{
		HZ_CORE_TRACE("UIInteractive �������");
		m_CurrentState = Ref<Hazel::UIState>();
	}

	

	void UIInteractive::AddSprite(std::string_view filePath, Ref<Sprite> sprite)
	{
		Resource::ResourceManager* manager = Resource::ResourceManager::Get();
		if (manager)
		{
			auto texture = manager->LoadTexture(std::string(filePath));
			// ��ȡ�ļ�����Ϊ textureName
			std::string textureName = std::filesystem::path(filePath).filename().string();
			if (texture)
			{
				if (m_Size.x == 0.0f && m_Size.y == 0.0f) {
					m_Size.x = texture.get()->GetWidth();
					m_Size.y = texture.get()->GetHeight();
				}
				sprite->SetTexture(texture);
				m_Sprites[textureName] = sprite;
			}
			else
			{
				HZ_CORE_WARN("UIInteractive: Failed to load texture for sprite '{}'", filePath);
			}
		}
		else
		{
			HZ_CORE_ERROR("UIInteractive: ResourceManager instance is null.");
		}
	}

	void UIInteractive::SetSprite(std::string_view name)
	{
		auto it = m_Sprites.find(std::string(name));
		if (it != m_Sprites.end())
		{
			m_CurrentSprite = it->second.get() ? it->second : nullptr;
		}
	}

	void UIInteractive::AddSound(std::string_view name, std::string_view path)
	{
		m_Sounds[std::string(name)] = path;
	}

	void UIInteractive::PlaySound(std::string_view name)
	{
		auto it = m_Sounds.find(std::string(name));
		if (it != m_Sounds.end())
		{
			Resource::ResourceManager* manager = Resource::ResourceManager::Get();
			if (manager)
			{
				AudioPlayer::Get()->PlaySound(it->first);
			}
		}
	}

	void UIInteractive::SetState(Ref<UIState> state)
	{
		m_CurrentState = state;
	}

	bool UIInteractive::HandleInput()
	{
		if (UIElement::HandleInput()) {
			return true;
		}

		if (!m_Interactive || !m_Visible)
			return false;

		if (m_CurrentState)
		{
			auto newState = m_CurrentState->HandleInput();
			if (newState)
				m_CurrentState = std::move(newState);
		}

		// �������Ƿ���ͣ
		//glm::vec2 mousePos = /* ��ȡ���λ�õĴ��� */;
		//m_IsMouseOver = IsPointInside(mousePos);

		//// �����
		//if (m_IsMouseOver && /* ���������¼����� */)
		//{
		//	Clicked();
		//	return true;
		//}

		return false;
	}

	void UIInteractive::Render()
	{
		if (!m_Visible)
			return;

		if (m_CurrentSprite)
		{
			// DrawSpriteʵ�֣�����DrawQuadΪRenderer2D�ľ�̬����
			// ��ȡ�����Դ����
			auto texture = m_CurrentSprite->GetTexture();
			auto sourceRectOpt = m_CurrentSprite->GetSourceRect();
			bool flipped = m_CurrentSprite->IsFlipped();

			if (texture)
			{
				if (sourceRectOpt.has_value())
				{
					const auto& rect = sourceRectOpt.value();
					Renderer2D::DrawQuad(rect.position, rect.size*glm::vec2(1.0, flipped?-1:1), texture);
				}
				else
				{
					Renderer2D::DrawQuad(m_Position, m_Size * glm::vec2(1.0, flipped ? -1 : 1), texture);
				}
			}
		}
	}
}
