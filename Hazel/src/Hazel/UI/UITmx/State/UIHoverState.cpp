#include "hzpch.h"
#include "UIHoverState.h"

namespace Hazel::TmxState
{
	void UIHoverState::Enter()
	{
		m_Owner->SetSprite("hover");
		spdlog::debug("切换到悬停状态");
	}


	Scope<UIState> UIHoverState::HandleInput()
	{
		//auto& input_manager = context.getInputManager();
		//auto mouse_pos = input_manager.getLogicalMousePosition();
		//if (!owner_->isPointInside(mouse_pos)) {                // 如果鼠标不在UI元素内，则返回正常状态
		//	return std::make_unique<UINormalState>(owner_);
		//}
		//if (input_manager.isActionPressed("MouseLeftClick")) {  // 如果鼠标按下，则返回按下状态
		//	return CreateScope<UIPressedState>(owner_);
		//}
		//return nullptr;

		//if (!m_Owner->m_Interactable)
		//	return nullptr;
		//if (!m_Owner->m_IsMouseOver)
		//{
		//	// 鼠标移出，切换到 Normal 状态
		//	return CreateScope<UINormalState>(m_Owner);
		//}
		return nullptr;
	}


}
