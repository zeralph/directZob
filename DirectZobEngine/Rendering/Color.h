#ifndef _COLOR_H_
#define _COLOR_H_

#include "../Types.h"
#include "ZobVector3.h"
class ZobColor
{
	public:
		ZobColor();
		ZobColor(uint rawValue);
		ZobColor(const ZobVector3* v);
		ZobColor(int a, int r, int g, int b);
		~ZobColor();

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
		static const ZobColor Red;
		static const ZobColor Green;
		static const ZobColor Blue;
		static const ZobColor Black;
		static const ZobColor White;
		static const ZobColor Yellow;
		static const ZobColor Magenta;
		static const ZobColor Cyan;
		static const ZobColor Grey;
};

#endif