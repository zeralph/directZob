#include "Color.h"
#include "MiniFB.h"
using namespace directZob;
Color::Color() :
	m_r(0),
	m_g(0),
	m_b(0),
	m_a(0),
	m_value(0)
{

}

Color::Color(const Vector3* v)
{
	Set(255, (int)(v->x * 255.0f), (int)(v->y * 255.0f), (int)(v->z * 255.0f));
}

Color::Color(uint rawValue):
	m_value(rawValue)
{
	Set(rawValue);
}

Color::Color(int a, int r, int g, int b):
	m_a(a),
	m_r(r),
	m_g(g),
	m_b(b)
{
	Set(a, r, g, b);
}

Color::~Color()
{

}

void Color::Set(uint a, uint r, uint g, uint b)
{
	m_a = a;
	m_r = r;
	m_b = b;
	m_g = g;
	m_value = MFB_RGB(r, g, b);
}

void Color::Set(uint rawValue)
{
	m_value = rawValue;
	m_r = (m_value & 0xFF0000) >> 16;
	m_g = (m_value & 0x00FF00) >> 8;
	m_b = (m_value & 0x0000FF);
}


const Color Color::Red =	 Color(0xFF, 0xFF, 0x00, 0x00);
const Color Color::Green =	 Color(0xFF, 0x00, 0xFF, 0x00);
const Color Color::Blue =	 Color(0xFF, 0x00, 0x00, 0xFF);
const Color Color::Black =	 Color(0xFF, 0x00, 0x00, 0x00);
const Color Color::White =	 Color(0xFF, 0xFF, 0xFF, 0xFF);

const Color Color::Yellow =	 Color(0xFF, 0xFF, 0xFF, 0x00);
const Color Color::Magenta = Color(0xFF, 0xFF, 0x00, 0xFF); 
const Color Color::Cyan =	 Color(0xFF, 0x00, 0xFF, 0xFF);
const Color Color::Grey	= Color(0xFF, 0x10, 0x10, 0x10);