#include "hzpch.h"
#include "TmxCamera.h"
#include "OrthographicCamera.h"

namespace Hazel
{
	TmxCamera::TmxCamera()
	{
		TmxCamera(glm::vec2(1920,1080));
	}

	TmxCamera::TmxCamera(glm::vec2 viewportSize, glm::vec2 position, std::optional<Math::Rect> limitBounds)
		: m_ViewportSize(viewportSize), m_Position(position), m_LimitBounds(limitBounds)
	{
		float left = position.x;
		float right = position.x + viewportSize.x;
		float bottom = position.y + viewportSize.y;
		float top = position.y;
		if (limitBounds.has_value()) {
			left = std::max(left, limitBounds->position.x);
			right = std::min(right, limitBounds->position.x + limitBounds->size.x);
			bottom = std::min(bottom, limitBounds->position.y + limitBounds->size.y);
			top = std::max(top, limitBounds->position.y);
		}
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top,  -10.0f, 10.0f);
		m_ViewMatrix = glm::mat4(1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void TmxCamera::SetViewportSize(float w, float h)
	{
		glm::vec2 viewportSize = glm::vec2(w,h);
		float left = m_Position.x;
		float right = m_Position.x + viewportSize.x;
		float bottom = m_Position.y + viewportSize.y;
		float top = m_Position.y;

		if (m_LimitBounds.has_value()) {
			left = std::max(left, m_LimitBounds->position.x);
			right = std::min(right, m_LimitBounds->position.x + m_LimitBounds->size.x);
			bottom = std::min(bottom, m_LimitBounds->position.y + m_LimitBounds->size.y);
			top = std::max(top, m_LimitBounds->position.y);
		}
		m_ProjectionMatrix = glm::ortho(left, right,bottom, top, -10.0f, 10.0f);
		m_ViewMatrix = glm::mat4(1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void TmxCamera::Move(const glm::vec2& offset)
	{
		m_Position += offset;
		clampPosition();
	}

	glm::vec2 TmxCamera::WorldToScreen(const glm::vec2& worldPos) const
	{
		// 世界坐标减去相机左上角坐标，得到屏幕坐标
		return worldPos - m_Position;
	}

	glm::vec2 TmxCamera::WorldToScreenWithParallax(const glm::vec2& world_pos, const glm::vec2& scrollFactor) const
	{
		// 视差滚动：世界坐标减去相机左上角坐标乘以视差因子
		return world_pos - m_Position * scrollFactor;
	}

	glm::vec2 TmxCamera::ScreenToWorld(const glm::vec2& screenpos) const
	{
		// 屏幕坐标加上相机左上角坐标，得到世界坐标
		return screenpos + m_Position;
	}

	void TmxCamera::OnUpdate(Timestep ts)
	{
		Update(ts);
	}

	void TmxCamera::Update(float delta_time)
	{
		if (m_Target)
		{
			glm::vec2 targetPos = glm::vec2(m_Target->Translation.x, m_Target->Translation.y);
			glm::vec2 desiredPosition = targetPos - m_ViewportSize * 0.5f;

			// 计算当前位置与目标位置的距离
			auto distance = glm::distance(m_Position, desiredPosition);
			constexpr float SNAP_THRESHOLD = 1.0f; // 设置一个距离阈值  (constexpr: 编译时常量，避免每次调用都计算)

			if (distance < SNAP_THRESHOLD) {
				// 如果距离小于阈值，直接吸附到目标位置
				m_Position = desiredPosition;
			}
			else {
				// 否则，使用线性插值平滑移动   glm::mix(a,b,t): 在向量 a 和 b 之间进行插值，t 是插值因子，范围在0到1之间。
										 // 公式: (b-a)*t + a;   t = 0 时结果为 a，t = 1 时结果为 b
				m_Position = glm::mix(m_Position, desiredPosition, m_SmoothSpeed * delta_time);
				m_Position = glm::vec2(glm::round(m_Position.x), glm::round(m_Position.y));    // 四舍五入到整数,省略的话偶尔会出现画面割裂
			}

			//m_Position += (desiredPosition - m_Position) * glm::min(m_SmoothSpeed * delta_time, 1.0f);
			clampPosition();
		}
		// 更新视图矩阵
		m_ViewMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0, 1.0, 1.0f))*glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0, -5.0f));
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void TmxCamera::clampPosition()
	{
		if (!m_LimitBounds.has_value())
			return;

		const Math::Rect& bounds = m_LimitBounds.value();
		glm::vec2 minPos = bounds.position;
		glm::vec2 maxPos = bounds.position + bounds.size - m_ViewportSize;

		// 确保 max_cam_pos 不小于 min_cam_pos (视口可能比世界还大)
		maxPos.x = std::max(minPos.x, maxPos.x);
		maxPos.y = std::max(minPos.y, maxPos.y);


		m_Position.x = glm::clamp(m_Position.x, minPos.x, maxPos.x);
		m_Position.y = glm::clamp(m_Position.y, minPos.y, maxPos.y);
	}

	void TmxCamera::RecalculateViewMatrix()
	{
		
	}

}


