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
		// 如果元素不可见，直接返回 false
		if (!m_Visible) return false;

		// 遍历所有子节点，并删除标记了移除的元素
		for (auto it = m_Children.begin(); it != m_Children.end();) {
			if (*it && !(*it)->IsNeedRemove()) {
				if ((*it)->HandleInput()) return true;
				++it;
			}
			else {
				it = m_Children.erase(it);
			}
		}
		// 事件未被消耗，返回假
		return false;
	}

	void UIElement::Update(float delta_time)
    {
		// 先更新所有未被标记移除的子节点
		for (auto& child : m_Children)
		{
			if (child && child->IsVisible() && !child->IsNeedRemove())
			child->Update(delta_time);
		}

		// 再移除所有被标记为需要移除的子节点
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
