#include "hzpch.h"
#include "UIHoverState.h"
#include "UINormalState.h"
#include "UIPressedState.h"
#include "Hazel/Core/Input.h"

namespace Hazel::TmxState
{
	void UIPressedState::Enter()
	{
		m_Owner->SetSprite(UI_PRESSED_STATE);
		m_Owner->PlaySound(UI_PRESSED_STATE);
		HZ_CORE_INFO("ÇÐ»»µ½°´ÏÂ×´Ì¬");
	}

	Scope<UIState> UIPressedState::HandleInput()
	{
		auto mouse_pos = Input::GetMousePosition();
		
		if (Input::IsMouseButtonReleased(Hazel::Mouse::ButtonLeft)) 
		{
			if (!m_Owner->IsPointInside(mouse_pos))
			{
				return CreateScope<UINormalState>(m_Owner);
			}
			else
			{
				m_Owner->Clicked();
				return CreateScope<UIHoverState>(m_Owner);
			}
			
		}
		return nullptr;
	}
}
