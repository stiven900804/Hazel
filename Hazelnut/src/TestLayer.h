#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Hazel/Renderer/EditorCamera.h"

#include "Hazel/Renderer/TmxCamera.h"

namespace Hazel {

	// TestLayer 继承自 Layer，作为编辑器或测试层的实现
	class TestLayer : public Layer
	{
	public:
		// 构造函数，初始化层
		TestLayer();
		// 析构函数
		virtual ~TestLayer() = default;

		// 层附加时调用
		virtual void OnAttach() override;
		// 层分离时调用
		virtual void OnDetach() override;

		// 每帧更新调用
		void OnUpdate(Timestep ts) override;
		// ImGui 渲染调用
		virtual void OnImGuiRender() override;
		// 事件处理调用
		void OnEvent(Event& e) override;
	private:
		// 键盘按下事件处理
		bool OnKeyPressed(KeyPressedEvent& e);
		// 鼠标按下事件处理
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		// 叠加渲染（如 Gizmo、辅助线等）
		void OnOverlayRender();

		void OpenTmxFile();

		void OpenTmxFile(const std::filesystem::path& path);

		// UI Panels
		void UI_Toolbar();
	private:
		// 场景摄像机控制器
		Hazel::OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		// 编辑器摄像机
		EditorCamera m_EditorCamera;

		TmxCamera m_TmxCamera;

		Ref<Texture2D> m_CheckerboardTexture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};
		SceneState m_SceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconStep, m_IconSimulate, m_IconStop;

	};
}

