#pragma once
#include "UIState.h"
#include "../UIInteractive.h"

namespace Hazel::TmxState
{
	class UIPressedState :public UIState
	{
	public:
		UIPressedState(UIInteractive* owner) : UIState(owner) {}
		~UIPressedState() override = default;

	private:
		void Enter() override;
		Scope<UIState> HandleInput() override;

		friend class UIInteractive;
	};
}
