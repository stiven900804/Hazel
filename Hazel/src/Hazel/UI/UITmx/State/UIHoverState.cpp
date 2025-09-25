#include "hzpch.h"
#include "UIHoverState.h"
#include "UINormalState.h"
#include "UIPressedState.h"
#include "Hazel/Core/Input.h"

namespace Hazel::TmxState
{
	void UIHoverState::Enter()
	{
		m_Owner->SetSprite(UI_HOVER_STATE);
		HZ_CORE_INFO("ÇÐ»»µ½ÐüÍ£×´Ì¬");
	}

	Scope<UIState> UIHoverState::HandleInput()
	{
		auto mouse_pos = Input::GetMousePosition();
		if (!m_Owner->IsPointInside(mouse_pos))
		{
			return CreateScope<UINormalState>(m_Owner);
		}
		if (Input::IsMouseButtonPressed(Hazel::Mouse::ButtonLeft)) {
			return CreateScope<UIPressedState>(m_Owner);
		}

		if (!m_Owner->IsInteractive())
		{
			return nullptr;
		}
		//if (!m_Owner->m_IsMouseOver)
		//{
		//	// Êó±êÒÆ³ö£¬ÇÐ»»µ½ Normal ×´Ì¬
		//	return CreateScope<UINormalState>(m_Owner);
		//}
		return nullptr;
	}


}
