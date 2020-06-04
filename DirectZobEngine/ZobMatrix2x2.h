#pragma once
#include "Types.h"
#include "ZobVector2.h"

class ZobMatrix2x2
{
public:
	ZobMatrix2x2();
	ZobMatrix2x2(const ZobMatrix2x2* m);
	~ZobMatrix2x2();

	void Identity();
	void CopyFrom(const ZobMatrix2x2* m);
	void Mul(const ZobMatrix2x2* m);
	void Mul(ZobVector2 * v) const;
	void SetScale(const float x, const float y);
	void SetRotation(const float degrees);
	void SetShear(const float x, const float y);
	void SetTranslation(const float x, const float y);
	ZobVector2 GetScale() const { return ZobVector2(m_data[0][0], m_data[1][1]); }
	ZobVector2 GetTranslation() const { return ZobVector2{ m_data[0][2], m_data[1][2] }; }
	inline float 	GetValue(const int i, const int j) const { return m_data[i][j]; }

private:
	float 			m_data[3][3];
};

