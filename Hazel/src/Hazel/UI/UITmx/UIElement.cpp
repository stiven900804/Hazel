#include "hzpch.h"

#include "UIElement.h"

namespace Hazel
{
	UIElement::UIElement(glm::vec2 position, glm::vec2 size)
		: m_Position(position), m_Size(size)
	{
	}

	bool UIElement::HandleInput()
	{
		// ���Ԫ�ز��ɼ���ֱ�ӷ��� false
		if (!m_Visible) return false;

		// ���������ӽڵ㣬��ɾ��������Ƴ���Ԫ��
		for (auto it = m_Children.begin(); it != m_Children.end();) {
			if (*it && !(*it)->IsNeedRemove()) {
				if ((*it)->HandleInput()) return true;
				++it;
			}
			else {
				it = m_Children.erase(it);
			}
		}
		// �¼�δ�����ģ����ؼ�
		return false;
	}

	void UIElement::Update(float delta_time)
    {
		// �ȸ�������δ������Ƴ����ӽڵ�
		for (auto& child : m_Children)
		{
			if (child && child->IsVisible() && !child->IsNeedRemove())
			child->Update(delta_time);
		}

		// ���Ƴ����б����Ϊ��Ҫ�Ƴ����ӽڵ�
		for (auto it = m_Children.begin(); it != m_Children.end(); )
			{
			if (*it && (*it)->IsNeedRemove())
			{
				(*it)->SetParent(nullptr);
				it = m_Children.erase(it);
			}
			else
			{
				++it;
			}
		}
    }

	void UIElement::Render()
	{
		if (!m_Visible)
			return;

		for (auto& child : m_Children)
		{
			if (child && child->IsVisible())
				child->Render();
		}
	}

	void UIElement::AddChild(Scope<UIElement> child)
	{
		if (child)
		{
			child->SetParent(this);
			m_Children.push_back(std::move(child));
		}
	}

	Scope<UIElement> UIElement::RemoveChild(UIElement* childPtr)
	{
		for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
		{
			if (it->get() == childPtr)
			{
				childPtr->SetParent(nullptr);
				Scope<UIElement> removed = std::move(*it);
				m_Children.erase(it);
				return removed;
			}
		}
		return nullptr;
	}

	void UIElement::RemoveAllChildren()
	{
		for (auto& child : m_Children)
		{
			if (child)
				child->SetParent(nullptr);
		}
		m_Children.clear();
	}

	Math::Rect UIElement::GetBounds() const
	{
		glm::vec2 screenPos = GetScreenPosition();
		return Math::Rect{ screenPos, m_Size };
	}

	glm::vec2 UIElement::GetScreenPosition() const
	{
		glm::vec2 pos = m_Position;
		const UIElement* parent = m_Parent;
      
		while (parent)
		{
			pos += parent->m_Position;
			parent = parent->m_Parent;
		}
		return pos;
	}

    bool UIElement::IsPointInside(const glm::vec2& point) const
    {
		Math::Rect bounds = GetBounds();
		return point.x >= bounds.position.x && point.x <= bounds.position.x + bounds.size.x &&
		point.y >= bounds.position.y && point.y <= bounds.position.y + bounds.size.y;
    }

}
