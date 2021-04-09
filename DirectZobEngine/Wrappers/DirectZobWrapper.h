#ifdef _WINDLL
#pragma once
#include "ManagedObject.h"
#include "../DirectZob.h"
#include "ZobObjectManagerWrapper.h"
#include "CameraManagerWrapper.h"
#include "EngineWrapper.h"
#include <string> 
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

		delegate void					OnEditorUpdate();
		static event OnEditorUpdate^	OnEditorUpdateEvent;

		static void						CallSceneUpdatedCallback();
		static void						CallQueuingCallback();
		static void						CallSceneLoadedCallback();
										DirectZobWrapper(Panel^ objectTreeviewPanel, Panel^ objectPropertiesPanel, PictureBox^ renderWindow);
		void							RegisterObjectCallbacks(engineCallback^ onSelected);
		EngineWrapper^					GetEngineWrapper() { return m_ZobEngineWrapper; }
		int								RunAFrame();
		int								Stop();
		void							StartPhysic();
		void							StopPhysic(bool reset);
		bool							IsPhysicPlaying();
		void							Init(int width, int height);
		void							LoadScene(System::String^ path, System::String^ file, engineCallback^ loaded);
		void							LoadZobObject(System::String^ path, System::String^ file);
		void							NewScene();
		void							Unload();
		bool							CanFastSave();
		void							SaveScene();
		void							RegenerateZobIds();
		void							SaveScene(System::String^ path, System::String^ file);
		static DirectZob*				GetDirectZobInstance() { return m_directZobInstance; }
		void							Lock();
		void							EditorUpdate();
		void							Unlock();
		System::IntPtr					GetBufferDataPointer();
		int								GetBufferDataLength();
		cli::array<System::String^>^	GetEventsAndClear();
		void							Resize(int width, int height);
		System::String^					GetResourcePath();


		ZobObjectManagerWrapper^		GetZobObjectManagerWrapper() { return m_ZobObjectManagerWrapper; }
		CameraManagerWrapper^			GetZobCameraManagerWrapper() { return m_ZobCameraManager; }
		
	private:

		ZobObjectManagerWrapper^		m_ZobObjectManagerWrapper;
		EngineWrapper^					m_ZobEngineWrapper;
		CameraManagerWrapper^			m_ZobCameraManager;
		//array<int>^ m_buffer;
		//void					CallSceneUpdatedCallback();
		delegate void			Del();
		static DirectZob*				m_directZobInstance = NULL;
		volatile bool					m_run;
		static engineCallback^			m_sceneUpdatedCb;
		static engineCallback^			m_queuingCb;
		static engineCallback^			m_sceneLoadedCb;
		static engineCallback^			m_objectSelectedCb;
		cli::array<byte>^				m_bufferData;
		int								m_buffferDataLength;

		Panel^							m_objectTreeviewPanel;
		Panel^							m_objectPropertiesPanel;
		PictureBox^						m_renderWindow;

		ZobObjectWrapper^				m_selectedObjectWrapper;
		float							m_lastFrameTime;
	};
};
#endif //_WINDLL