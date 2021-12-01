#include "Color.h"
#include "MiniFB.h"

ZobColor::ZobColor() :
	m_r(0),
	m_g(0),
	m_b(0),
	m_a(0),
	m_value(0)
{

}

ZobColor::ZobColor(const ZobVector3* v)
{
	Set(255, (int)(v->x * 255.0f), (int)(v->y * 255.0f), (int)(v->z * 255.0f));
}

ZobColor::ZobColor(const ZobColor* v)
{
	m_a = 255;
	m_r = v->GetRed();
	m_g = v->GetGreen();
	m_b = v->GetBlue();
}

ZobColor::ZobColor(uint rawValue):
	m_value(rawValue)
{
	Set(rawValue);
}

ZobColor::ZobColor(int a, int r, int g, int b):
	m_a(a),
	m_r(r),
	m_g(g),
	m_b(b)
{
	Set(a, r, g, b);
}

ZobColor::~ZobColor()
{

}

void ZobColor::Set(uint a, uint r, uint g, uint b)
{
	m_a = a;
	m_r = r;
	m_b = b;
	m_g = g;
	m_value = MFB_RGB(r, g, b);
}

void ZobColor::Set(uint rawValue)
{
	m_value = rawValue;
	m_r = (m_value & 0xFF0000) >> 16;
	m_g = (m_value & 0x00FF00) >> 8;
	m_b = (m_value & 0x0000FF);
}


const ZobColor ZobColor::Red = ZobColor(0xFF, 0xFF, 0x00, 0x00);
const ZobColor ZobColor::Green = ZobColor(0xFF, 0x00, 0xFF, 0x00);
const ZobColor ZobColor::Blue = ZobColor(0xFF, 0x00, 0x00, 0xFF);
const ZobColor ZobColor::Black = ZobColor(0xFF, 0x00, 0x00, 0x00);
const ZobColor ZobColor::White = ZobColor(0xFF, 0xFF, 0xFF, 0xFF);
const ZobColor ZobColor::Yellow = ZobColor(0xFF, 0xFF, 0xFF, 0x00);
const ZobColor ZobColor::Magenta = ZobColor(0xFF, 0xFF, 0x00, 0xFF);
const ZobColor ZobColor::Cyan = ZobColor(0xFF, 0x00, 0xFF, 0xFF);
const ZobColor ZobColor::Grey	= ZobColor(0xFF, 0x10, 0x10, 0x10);