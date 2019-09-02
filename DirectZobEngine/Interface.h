#pragma once
#include "Types.h"
#include "DirectZob.h"

namespace Core
{
	class Interface
	{
	public:

		Interface();
		~Interface();
		
		int RunAFrame();
		int GetBufferWidth();
		int GetBufferHeight();
		int* GetBufferData();

		std::vector<std::string>* GetEvents();
		void ClearEvents();

		int Test();
		//const DirectZobType::BufferData* GetBuffer();
	private:
		DirectZob m_directZob;
	};
}

