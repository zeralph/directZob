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
		ZobColor(const ZobColor* v);
		ZobColor(int a, int r, int g, int b);
		~ZobColor();

		void Set(uint a, uint r, uint g, uint b);
		void Set(uint rawValue);

		inline const uint GetRawValue() const { return m_value; };
		inline const uint GetAlpha() const { return m_a; }
		inline const uint GetRed() const { return m_r; }
		inline const uint GetGreen() const { return m_g; }
		inline const uint GetBlue() const { return m_b; }
		inline const uint GetAlphaNormalized() const { return m_a; }
		inline const float GetRedNormalized() const { return m_r / 255.0f; }
		inline const float GetGreenNormalized() const { return m_g / 255.0f; }
		inline const float GetBlueNormalized() const { return m_b / 255.0f; }
		static inline uint GetAlpha(uint c) { return c & 0xFF000000 >> 24; }

		bool FromString(std::string& s)
		{
			std::size_t del1, del2, del3 = 0;
			del1 = s.find(';');
			if (del1 != std::string::npos)
			{
				del2 = s.find(';', del1 + 1);
				if (del2 != std::string::npos)
				{
					del3 = s.find(';', del2 + 1);
					if (del3 != std::string::npos)
					{
						m_a = atof(s.substr(0, del1).c_str());
						m_r = atof(s.substr(del1 + 1, del2).c_str());
						m_g = atof(s.substr(del2 + 1, del3).c_str());
						m_b = atof(s.substr(del3 + 1, s.size() - 1).c_str());
						return true;
					}
				}
			}
			return false;
		}

		std::string ToString()
		{
			char str[64];
			sprintf(str, "%i;%i;%i;%i", m_a, m_r, m_g, m_b);
			std::string s = std::string(str);
			return s;
			//return std::to_string(x).append(";") + std::to_string(y).append(";") + std::to_string(z); 
		}

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