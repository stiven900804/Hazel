#pragma once

#include "Hazel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Hazel/Renderer/EditorCamera.h"

#include "Hazel/Renderer/TmxCamera.h"

namespace Hazel {

	// TestLayer �̳��� Layer����Ϊ�༭������Բ��ʵ��
	class TestLayer : public Layer
	{
	public:
		// ���캯������ʼ����
		TestLayer();
		// ��������
		virtual ~TestLayer() = default;

		// �㸽��ʱ����
		virtual void OnAttach() override;
		// �����ʱ����
		virtual void OnDetach() override;

		// ÿ֡���µ���
		void OnUpdate(Timestep ts) override;
		// ImGui ��Ⱦ����
		virtual void OnImGuiRender() override;
		// �¼��������
		void OnEvent(Event& e) override;
	private:
		// ���̰����¼�����
		bool OnKeyPressed(KeyPressedEvent& e);
		// ��갴���¼�����
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		// ������Ⱦ���� Gizmo�������ߵȣ�
		void OnOverlayRender();

		void OpenTmxFile();

		void OpenTmxFile(const std::filesystem::path& path);

		// UI Panels
		void UI_Toolbar();
	private:
		// ���������������
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

		// �༭�������
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

