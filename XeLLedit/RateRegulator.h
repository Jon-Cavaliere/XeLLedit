#pragma once
#include <stdint.h>
#include <SDL.h>

namespace JC
{
	// Singleton class for managing deltaTime and frame cap
	class RateRegulator
	{
	public:
		static RateRegulator* getInstance();
		void begin();
		void end();

		// Returns deltatime
		float dt();
	private:
		RateRegulator();
		// deleting copy constructor
		RateRegulator(const RateRegulator& obj) = delete;
		uint8_t m_FrameCap;
		bool m_CapFPS;
		float m_DeltaTime;
		uint32_t m_CurrentTime;
		uint32_t m_LastTime;
		static RateRegulator* p_Instance;
	};
}

