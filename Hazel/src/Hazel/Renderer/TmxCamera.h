#pragma once

#include "Hazel/Renderer/Camera.h" 
#include "Hazel/Math/Math.h" 
#include "Hazel/Scene/Components.h" 
#include "Hazel/Core/Timestep.h" 
#include <optional>

namespace Hazel
{
	class TmxCamera : public Camera
	{
	public:
		TmxCamera();
		TmxCamera(glm::vec2 viewportSize, glm::vec2 position=glm::vec2(0.0), std::optional<Math::Rect> limitBounds = std::nullopt);

		// ���ÿ������ƶ�����
		//TmxCamera(const TmxCamera&) = delete;
		//TmxCamera& operator=(const TmxCamera&) = delete;
		//TmxCamera(TmxCamera&&) = delete;
		//TmxCamera& operator=(TmxCamera&&) = delete;

		void SetViewportSize(float w,float h);
		void Move(const glm::vec2& offset);                                     ///< @brief �ƶ����

		glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;              ///< @brief ��������ת��Ļ����
		glm::vec2 WorldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scrollFactor) const; ///< @brief ��������ת��Ļ���꣬�����Ӳ����
		glm::vec2 ScreenToWorld(const glm::vec2& screenpos) const;             ///< @brief ��Ļ����ת��������

		void OnUpdate(Timestep ts);

		void Update(float delta_time);

		void SetPosition(glm::vec2 position) { m_Position = position; clampPosition();}                                   ///< @brief �������λ��
		void SetLimitBounds(std::optional<Math::Rect> limit_bounds) { m_LimitBounds = limit_bounds; clampPosition();}  ///< @brief ��������������ƶ���Χ
		void SetTarget(TransformComponent* target) { m_Target = target; }          ///< @brief ���ø���Ŀ��任���

		const glm::vec2& GetPosition() const { return m_Position; };                                   ///< @brief ��ȡ���λ��
		std::optional<Math::Rect> GetLimitBounds() const { return m_LimitBounds; };              ///< @brief ��ȡ����������ƶ���Χ
		glm::vec2 GetViewportSize() const { return m_ViewportSize; };                                     ///< @brief ��ȡ�ӿڴ�С
		TransformComponent* GetTarget() { return m_Target; };                 ///< @brief ��ȡ����Ŀ��任���

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void clampPosition();
		void RecalculateViewMatrix();
	
	private:
		glm::vec2 m_ViewportSize;                                                ///< @brief �ӿڴ�С����Ļ��С��
		glm::vec2 m_Position;                                                     ///< @brief ������Ͻǵ���������
		std::optional<Math::Rect> m_LimitBounds;								///< @brief ����������ƶ���Χ����ֵ��ʾ������
		float m_SmoothSpeed = 5.0f;                                              ///< @brief ����ƶ���ƽ���ٶ�
		TransformComponent* m_Target = nullptr;									///< @brief ����Ŀ��任�������ֵ��ʾ������

		float m_Rotation = 0.0f;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	};

}
