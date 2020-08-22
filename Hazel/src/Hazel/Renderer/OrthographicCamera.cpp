#include "hzpch.h"
#include "OrthographicCamera.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace Hazel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		HZ_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	glm::vec3 OrthographicCamera::GetMouseWorldPosition(glm::vec2 mousepos)
	{
		return glm::unProject(
			{ mousepos.x, (720.f - mousepos.y), 0 },
			m_ViewMatrix, 
			m_ProjectionMatrix, 
			RenderCommand::GetViewport() 
		);
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix= m_ProjectionMatrix * m_ViewMatrix;
	}
}
