#pragma once
#include "UIState.h"
#include "../UIInteractive.h"

namespace Hazel::TmxState
{
	class UIHoverState :public UIState
	{
	public:
		UIHoverState(UIInteractive* owner) : UIState(owner) {}
		~UIHoverState() override = default;

	private:
		void Enter() override;
		Scope<UIState> HandleInput() override;

		friend class UIInteractive;
	};
}


