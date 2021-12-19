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
	m_a = v->GetAlpha();
	m_r = v->GetRed();
	m_g = v->GetGreen();
	m_b = v->GetBlue();
	Set(v->GetAlpha(), v->GetRed(), v->GetGreen(), v->GetBlue());
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
	m_aN = (float)a / 255.0f;
	m_rN = (float)r / 255.0f;
	m_gN = (float)g / 255.0f;
	m_bN = (float)b / 255.0f;
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
const ZobColor ZobColor::Gray	= ZobColor(0xFF, 0x82, 0x82, 0x82);
const ZobColor ZobColor::LightGrey = ZobColor(0xFF, 0xAA, 0xAA, 0xAA);
const ZobColor ZobColor::Smoke = ZobColor(0xFF, 0xBE, 0xBD, 0xB8);
const ZobColor ZobColor::Rhino = ZobColor(0xFF, 0xB9, 0xBB, 0xB6);
const ZobColor ZobColor::Stone = ZobColor(0xFF, 0x87, 0x7F, 0x7D);
const ZobColor ZobColor::Trout = ZobColor(0xFF, 0x97, 0x97, 0x7D);
const ZobColor ZobColor::Seal = ZobColor(0xFF, 0x81, 0x83, 0x80);
const ZobColor ZobColor::Silver = ZobColor(0xFF, 0xC0, 0xC0, 0xC0);