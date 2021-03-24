#ifdef _WINDLL
#include "ZobBehaviorWrapper.h"


namespace CLI
{
	ZobBehaviorWrapper::ZobBehaviorWrapper(ZobBehavior* zobBehavior):ManagedObject(zobBehavior, false)
	{
	}

}
#endif //_WINDLL