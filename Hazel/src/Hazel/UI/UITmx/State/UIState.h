#pragma once

#define UI_NORMAL_STATE		"normal"
#define UI_HOVER_STATE		"hover"
#define UI_PRESSED_STATE	"pressed"

namespace Hazel
{
	class UIInteractive; // 前置声明
	class UIState
	{
	public:
		UIState(UIInteractive* owner) : m_Owner(owner) {}
		virtual ~UIState() = default;

		// 删除拷贝和移动构造函数/赋值运算符
		UIState(const UIState&) = delete;
		UIState& operator=(const UIState&) = delete;
		UIState(UIState&&) = delete;
		UIState& operator=(UIState&&) = delete;
	protected:
		// --- 核心方法 --- 
		virtual void Enter() {}
		virtual Scope<UIState> HandleInput() = 0;
	protected:
		UIInteractive* m_Owner = nullptr;    ///< @brief 指向父节点

		friend class UIInteractive;
	};
}
