#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"


namespace Hazel
{
	class Layer
	{
	public:
		template <typename Str = std::string>
		Layer(Str&& debugName = "Layer")
			: m_DebugName(std::forward<Str>(debugName))
		{}

		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

