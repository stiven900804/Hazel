#pragma once

#define UI_NORMAL_STATE		"normal"
#define UI_HOVER_STATE		"hover"
#define UI_PRESSED_STATE	"pressed"

namespace Hazel
{
	class UIInteractive; // ǰ������
	class UIState
	{
	public:
		UIState(UIInteractive* owner) : m_Owner(owner) {}
		virtual ~UIState() = default;

		// ɾ���������ƶ����캯��/��ֵ�����
		UIState(const UIState&) = delete;
		UIState& operator=(const UIState&) = delete;
		UIState(UIState&&) = delete;
		UIState& operator=(UIState&&) = delete;
	protected:
		// --- ���ķ��� --- 
		virtual void Enter() {}
		virtual Scope<UIState> HandleInput() = 0;
	protected:
		UIInteractive* m_Owner = nullptr;    ///< @brief ָ�򸸽ڵ�

		friend class UIInteractive;
	};
}
