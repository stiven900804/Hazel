#include "hzpch.h"
#include "UIHoverState.h"

namespace Hazel::TmxState
{
	void UIHoverState::Enter()
	{
		m_Owner->SetSprite("hover");
		spdlog::debug("�л�����ͣ״̬");
	}


	Scope<UIState> UIHoverState::HandleInput()
	{
		//auto& input_manager = context.getInputManager();
		//auto mouse_pos = input_manager.getLogicalMousePosition();
		//if (!owner_->isPointInside(mouse_pos)) {                // �����겻��UIԪ���ڣ��򷵻�����״̬
		//	return std::make_unique<UINormalState>(owner_);
		//}
		//if (input_manager.isActionPressed("MouseLeftClick")) {  // �����갴�£��򷵻ذ���״̬
		//	return CreateScope<UIPressedState>(owner_);
		//}
		//return nullptr;

		//if (!m_Owner->m_Interactable)
		//	return nullptr;
		//if (!m_Owner->m_IsMouseOver)
		//{
		//	// ����Ƴ����л��� Normal ״̬
		//	return CreateScope<UINormalState>(m_Owner);
		//}
		return nullptr;
	}


}
