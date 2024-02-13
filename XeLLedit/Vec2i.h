#pragma once
#include <stdint.h>

namespace JC
{
	class Vec2i
	{
	public:
		Vec2i(int32_t x, int32_t y);
		Vec2i(const Vec2i& v);
		Vec2i();
		void setX(int32_t x);
		void setY(int32_t y);

		// Sets both coordinates
		void set(int32_t x, int32_t y);

		int32_t getX() const;
		int32_t getY() const;


		int32_t& operator[](int8_t index);

	private:
		int32_t m_Data[2];
	};
}

