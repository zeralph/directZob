#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../Behaviors/ZobBehavior.h"

namespace CLI {
	public ref class ZobBehaviorWrapper : ManagedObject<ZobBehavior>
	{

	public:
		ZobBehaviorWrapper(ZobBehavior* zobBehavior);
	};
}
#endif //_WINDLL 