{
	'includes': [
		'defaults.gypi',
	],
	'targets': [
		{
			'target_name':
			'DirectZobEngine',
			'type':
			'static_library',
			'dependencies': [
				'dependencies/minifb/minifb.gyp:minifb',
				'dependencies/tinyxml/tinyxml.gyp:tinyxml',
				'dependencies/nanojpeg/nanojpeg.gyp:nanojpeg',
				'dependencies/lodepng/lodepng.gyp:lodepng',
				'dependencies/tga/tga.gyp:tga',
			],
			'export_dependent_settings': [
				'dependencies/minifb/minifb.gyp:minifb',
				'dependencies/tinyxml/tinyxml.gyp:tinyxml',
				'dependencies/nanojpeg/nanojpeg.gyp:nanojpeg',
				'dependencies/lodepng/lodepng.gyp:lodepng',
				'dependencies/tga/tga.gyp:tga',
			],
			'include_dirs': [
				'.',
				'dependencies/reactphysics3d/include',
				'dependencies/gainput/include',
				'dependencies/physfs/include',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'dependencies/reactphysics3d/include',
					'dependencies/gainput/include',
				],
			},
			'sources': [		
				'DirectZobEngine/DirectZob.cpp',
				'DirectZobEngine/ZobMaterial.cpp',
				'DirectZobEngine/Mesh.cpp',
				'DirectZobEngine/SceneLoader.cpp',
				'DirectZobEngine/DirectZob.h',
				'DirectZobEngine/ZobMaterial.h',			
				'DirectZobEngine/Mesh.h',		 
				'DirectZobEngine/SceneLoader.h',
				'DirectZobEngine/Sprite.h',
				'DirectZobEngine/Sprite.cpp',
				'DirectZobEngine/Types.h',
				'DirectZobEngine/Rendering/Color.h',
				'DirectZobEngine/Rendering/Engine.h',
				'DirectZobEngine/Rendering/Engine.cpp',
				'DirectZobEngine/Rendering/Rasterizer.h',
				'DirectZobEngine/Rendering/Rasterizer.cpp',
				'DirectZobEngine/Rendering/Color.cpp',
				'DirectZobEngine/Rendering/Text2D.h',
				'DirectZobEngine/Rendering/Texture.h',
				'DirectZobEngine/Rendering/Triangle.h',
				'DirectZobEngine/Rendering/ZobMatrix2x2.h',
				'DirectZobEngine/Rendering/ZobMatrix4x4.h',
				'DirectZobEngine/Rendering/ZobVector2.h',
				'DirectZobEngine/Rendering/ZobVector3.h',
				'DirectZobEngine/Rendering/Text2D.cpp',
				'DirectZobEngine/Rendering/ZobFont.cpp',
				'DirectZobEngine/Rendering/ZobFont.h',
				'DirectZobEngine/Rendering/Texture.cpp',
				'DirectZobEngine/Rendering/Triangle.cpp',
				'DirectZobEngine/Rendering/ZobMatrix2x2.cpp',
				'DirectZobEngine/Rendering/ZobMatrix4x4.cpp',
				'DirectZobEngine/Rendering/ZobVector2.cpp',
				'DirectZobEngine/Rendering/ZobVector3.cpp',
				'DirectZobEngine/ZobObjects/ZobEntity.h',
				'DirectZobEngine/ZobObjects/ZobEntity.cpp',
				'DirectZobEngine/ZobObjects/ZobObject.h',
				'DirectZobEngine/ZobObjects/ZobObject.cpp',
				'DirectZobEngine/ZobObjects/Light.h',
				'DirectZobEngine/ZobObjects/Light.cpp',
				'DirectZobEngine/ZobObjects/Camera.h',
				'DirectZobEngine/ZobObjects/Camera.cpp',
				'DirectZobEngine/Managers/ZobHUDManager.h',
				'DirectZobEngine/Managers/ZobHUDManager.cpp',
				'DirectZobEngine/Managers/ZobObjectManager.h',
				'DirectZobEngine/Managers/ZobObjectManager.cpp',
				'DirectZobEngine/Managers/MaterialManager.h',
				'DirectZobEngine/Managers/MaterialManager.cpp',
				'DirectZobEngine/Managers/CameraManager.h',
				'DirectZobEngine/Managers/CameraManager.cpp',
				'DirectZobEngine/Managers/LightManager.cpp',
				'DirectZobEngine/Managers/LightManager.h',
				'DirectZobEngine/Managers/MeshManager.h',
				'DirectZobEngine/Managers/MeshManager.cpp',
				'DirectZobEngine/Managers/ZobInputManager.h',
				'DirectZobEngine/Managers/ZobInputManager.cpp',
				'DirectZobEngine/ZobPhysic/ZobPhysicsEngine.h',
				'DirectZobEngine/ZobPhysic/ZobPhysicsEngine.cpp',
				'DirectZobEngine/ZobPhysic/ZobPhysicComponent.h',
				'DirectZobEngine/ZobPhysic/ZobPhysicComponent.cpp',
				'DirectZobEngine/ZobPhysic/ZobPhysicsContactsListener.h',
				'DirectZobEngine/ZobCameraController/ZobCameraController.h',
				'DirectZobEngine/ZobCameraController/ZobCameraController.cpp',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerFollowCar.h',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerFollowCar.cpp',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerOrbital.h',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerOrbital.cpp',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerFPS.h',
				'DirectZobEngine/ZobCameraController/ZobCameraControllerFPS.cpp',
				'DirectZobEngine/Components/ZobComponentFactory.h',
				'DirectZobEngine/Components/ZobComponentFactory.cpp',
				'DirectZobEngine/Components/ZobComponent.h',
				'DirectZobEngine/Components/ZobComponent.cpp',
				'DirectZobEngine/Components/ZobComponentCar.h',
				'DirectZobEngine/Components/ZobComponentCar.cpp',	
				'DirectZobEngine/Components/ZobComponentLight.h',
				'DirectZobEngine/Components/ZobComponentLight.cpp',
				'DirectZobEngine/Components/ZobComponentRotator.h',
				'DirectZobEngine/Components/ZobComponentRotator.cpp',				
				'DirectZobEngine/Components/GraphicComponents/ZobComponentMesh.h',
				'DirectZobEngine/Components/GraphicComponents/ZobComponentMesh.cpp',
				'DirectZobEngine/Components/GraphicComponents/ZobComponentSprite.h',
				'DirectZobEngine/Components/GraphicComponents/ZobComponentSprite.cpp',
				'DirectZobEngine/Components/GraphicComponents/ZobComponentSkybox.h',
				'DirectZobEngine/Components/GraphicComponents/ZobComponentSkybox.cpp',				
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicShape.h',
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicShape.cpp',	
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicBox.h',
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicBox.cpp',	
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicCapsule.h',
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicCapsule.cpp',	
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicSphere.h',
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicSphere.cpp',	
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicMesh.h',
				'DirectZobEngine/Components/PhysicComponents/ZobComponentPhysicMesh.cpp',		
				'DirectZobEngine/Components/HudComponents/ZobComponentText.h',
				'DirectZobEngine/Components/HudComponents/ZobComponentText.cpp',
				'DirectZobEngine/Components/HudComponents/ZobComponentImage.h',
				'DirectZobEngine/Components/HudComponents/ZobComponentImage.cpp',
				'DirectZobEngine/Misc/ZobXmlHelper.h',		
				'DirectZobEngine/Misc/ZobVariablesExposer.h',
				'DirectZobEngine/Misc/ZobVariablesExposer.cpp',	
				'DirectZobEngine/Misc/ZobUtils.h',	
				'DirectZobEngine/Misc/ZobFilePath.h',
				'DirectZobEngine/Misc/ZobFilePath.cpp',				
				'DirectZobEngine/Misc/ZobGeometryHelper.h',
				'DirectZobEngine/Misc/ZobGeometryHelper.cpp',	
				'DirectZobEngine/Embed/DefaultFontData.h',
				'DirectZobEngine/Embed/DefaultFontImage.h',				
			],
			'conditions': [
				[
					'OS=="win"',
					{
						'include_dirs': [
							'dependencies/fbxsdk/windows/include',
							'dependencies/optick/windows/include',
						],
						'defines': [
							'FBXSDK_SHARED',  # required to link with dll
						],
						'direct_dependent_settings': {
							'include_dirs': [
								'dependencies/fbxsdk/windows/include',
							],
							'defines': [
								'FBXSDK_SHARED',
							],
						},
						'link_settings': {
							'libraries': [
								'../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',
								'../dependencies/reactphysics3d/lib/windows/Release/reactphysics3d.lib',
								'../dependencies/optick/lib/windows/OptickCore.lib',
								'../dependencies/gainput/lib/windows/gainput.lib',
								'../dependencies/physfs/lib/windows/physfs.lib',
								'user32.lib',
								'gdi32.lib',
							],
						},
						'msvs_settings': {
							'VCCLCompilerTool': {
								'FloatingPointModel': '2',  # fast
							},
						},
					},
				],
				[
					'OS=="mac"',
					{
						'include_dirs': [
							'dependencies/fbxsdk/macos/include',
							'dependencies/gainput/include',
							'dependencies/reactphysics3d/include',
							'dependencies/physfs/include',
						],
						'direct_dependent_settings': {
							'include_dirs': [
								'dependencies/fbxsdk/macos/include',
							],
							'defines': [
								'FBXSDK_SHARED',
							],
						},

						'defines': [
							'MACOS',
							'__APPLE__',
						],
						'link_settings': {
							'libraries': [
								# relative to xcode project
								'../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',
                                '../dependencies/reactphysics3d/lib/macos/arm64/libreactphysics3d.a',
								#'../dependencies/reactphysics3d/lib/macos/x86_64/libreactphysics3d.a',
								'../dependencies/gainput/lib/macos/arm64/libgainputstatic.a',
								'../dependencies/optick/lib/macos/arm64/libOptickCore.a',
								'../dependencies/physfs/lib/macos/arm64/libphysfs.a',
							],
						},
					},
				],
				[
					'OS=="linux"',
					{
						'include_dirs': [
							'dependencies/fbxsdk/linux/include',
							'dependencies/gainput/include',
							'dependencies/reactphysics3d/include',
							'dependencies/physfs/include',
						],
						'defines': [
							'LINUX',
							'linux',
							'__LINUX__',
						],
						'direct_dependent_settings': {
							'include_dirs': [
								'dependencies/fbxsdk/linux/include',
							],
						},
						'link_settings': {
							'libraries': [
								'../dependencies/fbxsdk/linux/lib/libfbxsdk.a', 
								'../dependencies/reactphysics3d/lib/linux/libreactphysics3d.a',
								'../dependencies/gainput/lib/linux/libgainputstatic.a',
								'../dependencies/optick/lib/linux/libOptickCore.a',
								'../dependencies/physfs/lib/linux/libphysfs.a',
								'-lpthread',
								'-lxml2',
								'-lX11',
								'-ldl',
								'-lz',
							],
							'library_dirs': [
								'/usr/lib/x86_64-linux-gnu',
							],
						},
					},
				],
			],
		},
		{
			'target_name':'DirectZobExe',
			'type':'executable',
			'dependencies': [
				'DirectZobEngine',
			],
			'sources': [
				'DirectZobExe/main.cpp',
			],
			'conditions': [
				[
					'OS=="win"',
					{
						'link_settings': {
							'libraries': [
								# relative to sln
								#'../dependencies/gainput/lib/windows/gainput.lib',
								#'../dependencies/physfs/lib/windows/physfs.lib',
								#'user32.lib',
								#'gdi32.lib',
							],
						},
						'copies': [
							{
								'destination':
								'<(PRODUCT_DIR)',
								'files': [
									'dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll',  # copy fbxsdk
									'dependencies/optick/lib/windows/OptickCore.dll',  # copy OptickCore.dll
									'dependencies/gainput/lib/windows/gainput.dll',
									'dependencies/physfs/lib/windows/physfs.dll',
									
								],
							},
						],
					},
					'OS=="linux"',
					{
					},
					'OS=="mac"',
					{
						'include_dirs': [
							'.',
							'dependencies/fbxsdk/macos/include',
						],
						'link_settings': {
							'libraries': [
								# relative to xcode project
								'../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',
								'../dependencies/gainput/lib/macos/arm64/libgainputstatic.a',
								'../dependencies/physfs/lib/macos/arm64/libphysfs.a',
								'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
								'$(SDKROOT)/System/Library/Frameworks/Metal.framework',
								'$(SDKROOT)/System/Library/Frameworks/GameKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/WebKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/StoreKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/MetalKit.framework',
								'-liconv',
								'-lz',
								'-lxml2',
								'-ObjC',
								'-v',
								#'-lmingw32',
								'-lstdc++',
								#'-enable-auto-import',
							],
						},
					},
				],
			],
		},
		{
			'target_name':
			'DirectZobEditor',
			'type':
			'executable',
			'dependencies': [
				'DirectZobEngine',
			],
			'sources': [
				'DirectZobEditorWx/directZobEditor.cpp',
				'DirectZobEditorWx/MainWindow.h',
				'DirectZobEditorWx/MainWindow.cpp',
				'DirectZobEditorWx/MainWindowInterface.h',
				'DirectZobEditorWx/MainWindowInterface.cpp',
				'DirectZobEditorWx/ZobEditorManager.h',
				'DirectZobEditorWx/ZobEditorManager.cpp',
                'DirectZobEditorWx/Inspector.h',
				'DirectZobEditorWx/Inspector.cpp',
                'DirectZobEditorWx/ZobControls.h',
				'DirectZobEditorWx/ZobControls.cpp',
			],
			'conditions': [
				[
					'OS=="win"',
					{
						'include_dirs': [
							#'.',
							'dependencies/wxwidgets/include',
						],
						'defines': [
						],
						'link_settings': {
							# links fbxsdk as dll
							'libraries': [
								# relative to sln
								'../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',
								'../dependencies/gainput/lib/windows/gainput.lib',
								'../dependencies/physfs/lib/windows/physfs.lib',
								'user32.lib',
								'gdi32.lib',
							],
							#		'library_dirs': [
							#			'/usr/lib',
							#		],
						},
						'copies': [
							{
								'destination':
								'<(PRODUCT_DIR)',
								'files': [
									'dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll',  # copy fbxsdk
									'dependencies/optick/lib/windows/OptickCore.dll',  # copy OptickCore.dll
									'dependencies/gainput/lib/windows/gainput.dll',
									'dependencies/physfs/lib/windows/physfs.dll',
									
								],
							},
						],
					},
					'OS=="linux"',
					{
						'include_dirs': [
							'.',
							'dependencies/fbxsdk/linux/include',
                            '../../../wxWidgets-3.2.6/include',
						],
                        'cxxflags': [
                        	'`../../../wxWidgets-3.2.6/build/wx-config --cxxflags`'
                        ],
                        "cflags_c": [
                            '`../../../wxWidgets-3.2.6/build/wx-config --cxxflags`'
                    	],
						'link_settings': {
							'libraries': [
								'../dependencies/fbxsdk/linux/lib/libfbxsdk.a', 
								'../dependencies/reactphysics3d/lib/linux/libreactphysics3d.a',
								'../dependencies/gainput/lib/linux/libgainputstatic.a',
								'../dependencies/optick/lib/linux/libOptickCore.a',
								'../dependencies/physfs/lib/linux/libphysfs.a',
								'-lpthread',
								'-lxml2',
								'-lX11',
								'-ldl',
								'-lz',
								'`/home/zeralph/Documents/wxWidgets-3.2.6/build/wx-config --libs`',
							],
							'library_dirs': [
								'/usr/lib/x86_64-linux-gnu',
							],
						},
					},
					'OS=="mac"',
					{
						'include_dirs': [
							'.',
							'dependencies/fbxsdk/macos/include',
                            '../../wxWidgets-3.2.6/include',
						],
                        'cxxflags': [
                        	'`wx-config --cxxflags`',
                        ],
                        "cflags_c": [
                             "`/home/zeralph/Documents/wxWidgets-3.2.6/build/wx-config --cxxflags`"
                    	],
						'link_settings': {
							'libraries': [
								# relative to xcode project
								'../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',
								'../dependencies/gainput/lib/macos/arm64/libgainputstatic.a',
								'../dependencies/physfs/lib/macos/arm64/libphysfs.a',
								'$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
								'$(SDKROOT)/System/Library/Frameworks/Metal.framework',
								'$(SDKROOT)/System/Library/Frameworks/GameKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/WebKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/IOKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/StoreKit.framework',
								'$(SDKROOT)/System/Library/Frameworks/MetalKit.framework',
								'-liconv',
								'-lz',
								'-lxml2',
								'-ObjC',
								'-v',
								#'-lmingw32',
								'-lstdc++',
								#'-enable-auto-import',
                                '`/Users/lucasrichalland/Documents/wxWidgets-3.2.6/build/wx-config --libs`',
							],
						},
					},
				],
			],
		},		
	],
}
