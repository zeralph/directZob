#ifdef _WINDLL
#include "ZobBehaviorWrapper.h"


namespace CLI
{
	List<String^>^ ZobBehaviorWrapper::GetBehaviors()
	{
		List<String^>^ list = gcnew List<String^>();
		for (int i = 0; i < ZobBehavior::__eBehavior_MAX__; i++)
		{
			std::string f = std::string(ZobBehaviorFactory::eBehaviorTypeStr[i]);
			String^ s = gcnew String(f.c_str());
			list->Add(s);
		}
		return list;
	}

	void ZobBehaviorWrapper::CreateBehavior(ZobObjectWrapper^ z, String^ s)
	{
		ZobObject* zo = z->GetInstance();
		{
			if (zo)
			{
				std::string so;
				MarshalString(s, so);
				ZobBehaviorFactory::CreateBehavior(zo, so.c_str());
			}
		}
	}
}
#endif //_WINDLL