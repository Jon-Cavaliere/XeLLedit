#pragma once
#include <stdint.h>
#include "Vec2i.h"
namespace JC
{
	class Vec2f
	{
	public:
		Vec2f(float x, float y);
		Vec2f(const Vec2f& v);
		Vec2f(const Vec2i& v);
		Vec2f(int32_t x, int32_t y);
		Vec2f();
		void setX(float x);
		void setY(float y);

		float getX() const;
		float getY() const;

		void normalize();

		float& operator[](int8_t index);

		void operator*=(float other);

	private:
		float m_Data[2];
	};
}
