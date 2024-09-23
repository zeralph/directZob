#ifdef _WINDLL
#pragma once

namespace DirectZobInterface
{
	public ref class DirectZobWrapperEvents
	{
	public:
		DirectZobWrapperEvents() {}
		~DirectZobWrapperEvents() {}
		void FireOnNewSceneEvent()
		{
			this->OnNewSceneEvent();
		}
		void FireOnEditorUpdateEvent()
		{
			this->OnEditorUpdateEvent();
		}
		delegate void					OnNewScene();
		static event OnNewScene^ OnNewSceneEvent;
		delegate void					OnEditorUpdate();
		static event OnEditorUpdate^ OnEditorUpdateEvent;
	};
}
#endif