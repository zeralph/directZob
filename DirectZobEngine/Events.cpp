#include "Events.h"

using namespace directZob;


Events::Events()
{
	ClearEvents();
}

Events::~Events()
{
	ClearEvents();
}

::std::string Events::EscapeJsonString(const ::std::string& input)
{
	::std::string ss;
	for (::std::string::const_iterator iter = input.cbegin(); iter != input.cend(); iter++)
	{
		switch (*iter)
		{
			case '\\': ss.append(::std::string("\\\\")); break;
			case '"': ss.append(::std::string("\\\"")); break;
			case '/': ss.append(::std::string("\\/")); break;
			case '\b': ss.append(::std::string("\\b")); break;
			case '\f': ss.append(::std::string("\\f")); break;
			case '\n': ss.append(::std::string("\\n")); break;
			case '\r': ss.append(::std::string("\\r")); break;
			case '\t': ss.append(::std::string("\\t")); break;
			default:
			{
				const char c = (*iter);
				ss.append(&c); 
				break;
			}
		}
	}
	return ss;
};

void Events::AddEvent(LogType eventType, ::std::string data)
{
	::std::string s = "{ \"type\":\"";
	s.append(::std::to_string((int)eventType));
	s.append("\", \"data\":\"");
	s.append(EscapeJsonString(data));
	s.append("\"}");
	m_events.push_back(s);
}