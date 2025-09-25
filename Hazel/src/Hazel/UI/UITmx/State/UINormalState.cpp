#include "hzpch.h"
#include "UIHoverState.h"
#include "UINormalState.h"
#include "UIPressedState.h"
#include "Hazel/Core/Input.h"

namespace Hazel::TmxState
{
	void UINormalState::Enter()
	{
		m_Owner->SetSprite(UI_NORMAL_STATE);
		HZ_CORE_INFO("ÇÐ»»µ½Õý³£×´Ì¬");
	}

	Scope<UIState> UINormalState::HandleInput()
	{
		auto mouse_pos = Input::GetMousePosition();
		if (!m_Owner->IsPointInside(mouse_pos))
		{
			m_Owner->PlaySound(UI_HOVER_STATE);
			return CreateScope<UIHoverState>(m_Owner);
		}

		return nullptr;
	}
}
