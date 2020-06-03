#ifndef DZOB_EVENTS_H
#define DZOB_EVENTS_H

#include <string>
#include <sstream>
#include <vector>
namespace directZob 
{
	class Events
	{
	public:

		enum LogType
		{
			LogInfo = 0,
			LogWarning,
			LogError
		};


		Events();
		~Events();

		void AddEvent(LogType eventType, ::std::string data);

		::std::vector<::std::string>* GetEvents()
		{
			return &m_events;
		}

		void ClearEvents()
		{
			m_events.clear();
		}

	private:

		::std::vector<::std::string> m_events;

		::std::string EscapeJsonString(const ::std::string& input);
	};
}
#endif