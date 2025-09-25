#pragma once
#include "UIState.h"
#include "../UIInteractive.h"

namespace Hazel::TmxState
{
	class UINormalState :public UIState
	{
	public:
		UINormalState(UIInteractive* owner) : UIState(owner) {}
		~UINormalState() override = default;

	private:
		void Enter() override;
		Scope<UIState> HandleInput() override;

		friend class UIInteractive;
	};
}
