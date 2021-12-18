#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../DirectZob.h"
#include "ZobObjectManagerWrapper.h"
#include "ZobGlobalsWrapper.h"
#include "EngineWrapper.h"
#include "DirectZobWrapperEvents.h"
#include <string> 
#include "Editor/ZobObjectsEditor.h"
#using "System.Windows.Forms.dll"
#using "System.dll"
#using "System.Drawing.dll"

using namespace System;
using namespace System::Windows;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Drawing;

namespace CLI
{
	// delegate void onFrameEndCallback();
	public delegate void engineCallback();
	public ref class DirectZobWrapper : public ManagedObject<DirectZob>
	{
	public:

		static DirectZobWrapper^		GetWrapper() { return m_singleton; }
		//delegate void					OnNewScene();
		//static event OnNewScene^		OnNewSceneEvent;
		//delegate void					OnEditorUpdate();
		//static event OnEditorUpdate^	OnEditorUpdateEvent;

		static void						CallSceneUpdatedCallback();
		static void						CallQueuingCallback();
		static void						CallSceneLoadedCallback();
										DirectZobWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel, Panel^ globalPropertiesPanel, PictureBox^ renderWindow);
		void							RegisterObjectCallbacks(engineCallback^ onSelected);
		EngineWrapper^					GetEngineWrapper() { return m_ZobEngineWrapper; }
		int								RunAFrame();
		int								Stop();
		void							CreateLight(int type);
		void							CreateEditorCamera();
		void							CreateCamera(System::String^ sTSype);
		void							StartPhysic();
		void							StopPhysic(bool reset);
		bool							IsPhysicPlaying();
		void							Init(int width, int height);
		void							LoadScene(System::String^ path, System::String^ file, engineCallback^ loaded);
		void							LoadZobObject(System::String^ path, System::String^ file);
		void							NewScene(System::String^ workspace);
		void							Unload();
		bool							CanFastSave();
		void							SaveScene();
		void							RegenerateZobIds();
		void							SaveScene(System::String^ path, System::String^ file);
		void							Lock();
		void							EditorUpdate();
		void							Unlock();
		System::IntPtr					GetBufferDataPointer();
		int								GetBufferDataLength();
		cli::array<System::String^>^	GetEventsAndClear();
		void							Resize(int width, int height);
		System::String^					GetResourcePath();
		cli::array<System::String^>^	GetCameraList();
		System::String^					GetCurrentCameraName();
		void 							SetCurrentCamera(System::String^ name);

		ZobObjectManagerWrapper^		GetZobObjectManagerWrapper() { return m_ZobObjectManagerWrapper; }
		ZobGlobalsWrapper^				GetZobGlobalsWrapper() { return m_ZobGlobalsWrapper; }
		
	private:
		
		DirectZobWrapperEvents^			m_events;
		ZobObjectManagerWrapper^		m_ZobObjectManagerWrapper;
		ZobGlobalsWrapper^				m_ZobGlobalsWrapper;
		EngineWrapper^					m_ZobEngineWrapper;
		//array<int>^ m_buffer;
		//void					CallSceneUpdatedCallback();
		delegate void			Del();

		volatile bool					m_run;
		static engineCallback^			m_sceneUpdatedCb;
		static engineCallback^			m_queuingCb;
		static engineCallback^			m_sceneLoadedCb;
		static engineCallback^			m_objectSelectedCb;
		cli::array<byte>^				m_bufferData;
		int								m_buffferDataLength;

		Panel^							m_objectTreeviewPanel;
		Panel^							m_objectPropertiesPanel;
		Panel^							m_globalPropertiesPanel;
		PictureBox^						m_renderWindow;

		float							m_lastFrameTime;

		static DirectZobWrapper^		m_singleton;
	};
};
#endif //_WINDLL