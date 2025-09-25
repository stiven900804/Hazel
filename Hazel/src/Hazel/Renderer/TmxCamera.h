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

		// 禁用拷贝和移动语义
		//TmxCamera(const TmxCamera&) = delete;
		//TmxCamera& operator=(const TmxCamera&) = delete;
		//TmxCamera(TmxCamera&&) = delete;
		//TmxCamera& operator=(TmxCamera&&) = delete;

		void SetViewportSize(float w,float h);
		void Move(const glm::vec2& offset);                                     ///< @brief 移动相机

		glm::vec2 WorldToScreen(const glm::vec2& worldPos) const;              ///< @brief 世界坐标转屏幕坐标
		glm::vec2 WorldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scrollFactor) const; ///< @brief 世界坐标转屏幕坐标，考虑视差滚动
		glm::vec2 ScreenToWorld(const glm::vec2& screenpos) const;             ///< @brief 屏幕坐标转世界坐标

		void OnUpdate(Timestep ts);

		void Update(float delta_time);

		void SetPosition(glm::vec2 position) { m_Position = position; clampPosition();}                                   ///< @brief 设置相机位置
		void SetLimitBounds(std::optional<Math::Rect> limit_bounds) { m_LimitBounds = limit_bounds; clampPosition();}  ///< @brief 设置限制相机的移动范围
		void SetTarget(TransformComponent* target) { m_Target = target; }          ///< @brief 设置跟随目标变换组件

		const glm::vec2& GetPosition() const { return m_Position; };                                   ///< @brief 获取相机位置
		std::optional<Math::Rect> GetLimitBounds() const { return m_LimitBounds; };              ///< @brief 获取限制相机的移动范围
		glm::vec2 GetViewportSize() const { return m_ViewportSize; };                                     ///< @brief 获取视口大小
		TransformComponent* GetTarget() { return m_Target; };                 ///< @brief 获取跟随目标变换组件

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void clampPosition();
		void RecalculateViewMatrix();
	
	private:
		glm::vec2 m_ViewportSize;                                                ///< @brief 视口大小（屏幕大小）
		glm::vec2 m_Position;                                                     ///< @brief 相机左上角的世界坐标
		std::optional<Math::Rect> m_LimitBounds;								///< @brief 限制相机的移动范围，空值表示不限制
		float m_SmoothSpeed = 5.0f;                                              ///< @brief 相机移动的平滑速度
		TransformComponent* m_Target = nullptr;									///< @brief 跟随目标变换组件，空值表示不跟随

		float m_Rotation = 0.0f;

		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
	};

}
