#pragma once
#include <string>
#include <sstream>
#include <vector>

class Events
{
public:

	enum LogType
	{
		LogInfo = 0,
		LogWarning,
		LogError
	};

	Events()
	{
		ClearEvents();
	}

	~Events()
	{
		ClearEvents();
	}

	void AddEvent(LogType eventType, std::string data)
	{
		std::string s = "{ \"type\":\"";
		s.append(std::to_string((int)eventType));
		s.append("\", \"data\":\"");
		s.append(escapeJsonString(data));
		s.append("\"}");
		m_events.push_back(s);
	}
	std::vector<std::string>* GetEvents() 
	{ 
		return &m_events; 
	}
	 void ClearEvents() 
	{ 
		m_events.clear(); 
	}
private:

	std::string escapeJsonString(const std::string& input) {
		std::ostringstream ss;
		for (auto iter = input.cbegin(); iter != input.cend(); iter++) {
			//C++98/03:
			//for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
			switch (*iter) {
			case '\\': ss << "\\\\"; break;
			case '"': ss << "\\\""; break;
			case '/': ss << "\\/"; break;
			case '\b': ss << "\\b"; break;
			case '\f': ss << "\\f"; break;
			case '\n': ss << "\\n"; break;
			case '\r': ss << "\\r"; break;
			case '\t': ss << "\\t"; break;
			default: ss << *iter; break;
			}
		}
		return ss.str();
	}

	std::vector<std::string> m_events;
};