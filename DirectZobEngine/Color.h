#ifndef DZOB_COLOR_H_
#define DZOB_COLOR_H_

#include "Types.h"
#include "./Vector3.h"
namespace directZob 
{
class Color
{
	public:
		Color();
		Color(uint rawValue);
		Color(const Vector3* v);
		Color(int a, int r, int g, int b);
		~Color();

		void Set(uint a, uint r, uint g, uint b);
		void Set(uint rawValue);

		inline const uint GetRawValue() const { return m_value; };
		inline const uint GetRed() const { return m_r; }
		inline const uint GetGreen() const { return m_g; }
		inline const uint GetBlue() const { return m_b; }

		static inline uint GetAlpha(uint c) { return c & 0xFF000000 >> 24; }

	private:

		uint m_value;
		uint m_a;
		uint m_r;
		uint m_g;
		uint m_b;

	public:
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Black;
		static const Color White;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Grey;
};
}
#endif