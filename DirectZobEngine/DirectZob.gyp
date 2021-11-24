{
    'includes': [
        '../defaults.gypi',
    ],
    'targets': [
        {
            'target_name':
            'DirectZobEngine',
            'type':
            'static_library',
            'dependencies': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
                '../dependencies/nanojpeg/nanojpeg.gyp:nanojpeg',
				'../dependencies/lodepng/lodepng.gyp:lodepng',
				'../dependencies/tga/tga.gyp:tga',
            ],
            'export_dependent_settings': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
                '../dependencies/nanojpeg/nanojpeg.gyp:nanojpeg',
				'../dependencies/lodepng/lodepng.gyp:lodepng',
				'../dependencies/tga/tga.gyp:tga',
            ],
            'include_dirs': [
                '.',
                '../dependencies/reactphysics3d/include',
				'../dependencies/gainput/include',
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    '../dependencies/reactphysics3d/include',
					'../dependencies/gainput/include',
                ],
            },
            'sources': [        
                'DirectZob.cpp',
                'ZobMaterial.cpp',
                'Mesh.cpp',
                'SceneLoader.cpp',
                'DirectZob.h',
                'ZobMaterial.h',            
                'Mesh.h',         
                'SceneLoader.h',
                'Sprite.h',
				'Sprite.cpp',
				'Types.h',
				'Rendering/Color.h',
				'Rendering/Engine.h',
				'Rendering/Engine.cpp',
				'Rendering/Rasterizer.h',
				'Rendering/Rasterizer.cpp',
				'Rendering/Color.cpp',
                'Rendering/Text2D.h',
                'Rendering/Texture.h',
                'Rendering/Triangle.h',
                'Rendering/ZobMatrix2x2.h',
                'Rendering/ZobMatrix4x4.h',
                'Rendering/ZobVector2.h',
                'Rendering/ZobVector3.h',
				'Rendering/Text2D.cpp',
                'Rendering/ZobFont.cpp',
				'Rendering/ZobFont.h',
				'Rendering/Texture.cpp',
                'Rendering/Triangle.cpp',
                'Rendering/ZobMatrix2x2.cpp',
                'Rendering/ZobMatrix4x4.cpp',
                'Rendering/ZobVector2.cpp',
                'Rendering/ZobVector3.cpp',
                'ZobObjects/ZOBGUID.h',
                'ZobObjects/ZOBGUID.cpp',
                'ZobObjects/ZobObject.h',
				'ZobObjects/ZobObject.cpp',
                'ZobObjects/ZobSprite.h',  	                
                'ZobObjects/ZobSprite.cpp',
				'ZobObjects/Light.h',
				'ZobObjects/Light.cpp',
				'ZobObjects/Camera.h',
				'ZobObjects/Camera.cpp',
				'Managers/ZobHUDManager.h',
				'Managers/ZobHUDManager.cpp',
				'Managers/ZobObjectManager.h',
				'Managers/ZobObjectManager.cpp',
				'Managers/MaterialManager.h',
				'Managers/MaterialManager.cpp',
				'Managers/CameraManager.h',
				'Managers/CameraManager.cpp',
				'Managers/LightManager.cpp',
                'Managers/LightManager.h',
				'Managers/MeshManager.h',
				'Managers/MeshManager.cpp',
				'Managers/ZobInputManager.h',
				'Managers/ZobInputManager.cpp',
				'ZobPhysic/ZobPhysicsEngine.h',
				'ZobPhysic/ZobPhysicsEngine.cpp',
				'ZobPhysic/ZobPhysicComponent.h',
				'ZobPhysic/ZobPhysicComponent.cpp',
				'ZobPhysic/ZobPhysicsContactsListener.h',
                'ZobCameraController/ZobCameraController.h',
                'ZobCameraController/ZobCameraController.cpp',
				'ZobCameraController/ZobCameraControllerFollowCar.h',
                'ZobCameraController/ZobCameraControllerFollowCar.cpp',
				'ZobCameraController/ZobCameraControllerOrbital.h',
				'ZobCameraController/ZobCameraControllerOrbital.cpp',
				'ZobCameraController/ZobCameraControllerFPS.h',
				'ZobCameraController/ZobCameraControllerFPS.cpp',
				'Behaviors/ZobBehaviorFactory.h',
				'Behaviors/ZobBehaviorFactory.cpp',
				'Behaviors/ZobBehavior.h',
				'Behaviors/ZobBehavior.cpp',
				'Behaviors/ZobBehaviorCar.h',
				'Behaviors/ZobBehaviorCar.cpp',
				'Behaviors/ZobBehaviorMenu.h',
				'Behaviors/ZobBehaviorMenu.cpp',
				'Behaviors/ZobBehaviorMesh.h',
				'Behaviors/ZobBehaviorMesh.cpp',				
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicShape.h',
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicShape.cpp',	
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicBox.h',
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicBox.cpp',	
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicCapsule.h',
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicCapsule.cpp',	
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicSphere.h',
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicSphere.cpp',	
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicMesh.h',
				'Behaviors/PhysicBehaviors/ZobBehaviorPhysicMesh.cpp',		
				'Misc/ZobXmlHelper.h',		
				'Misc/ZobVariablesExposer.h',
				'Misc/ZobVariablesExposer.cpp',	
				'Misc/ZobUtils.h',					
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'include_dirs': [
                            '../dependencies/fbxsdk/windows/include',
							'../dependencies/optick/windows/include',
                        ],
                        'defines': [
                            'FBXSDK_SHARED',  # required to link with dll
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '../dependencies/fbxsdk/windows/include',
                            ],
                            'defines': [
                                'FBXSDK_SHARED',
                            ],
                        },
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/reactphysics3d/lib/windows/reactphysics3d.lib',
								'../../../dependencies/optick/lib/windows/OptickCore.lib',
								'../../../dependencies/gainput/lib/windows/gainput.lib',
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
                            '../dependencies/fbxsdk/macos/include',
                            '../dependencies/gainput/include',
                            '../dependencies/reactphysics3d/include',
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '../dependencies/fbxsdk/macos/include',
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
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',
                                '../../../dependencies/reactphysics3d/lib/macos/libreactphysics3d.a',
                                '../../../dependencies/gainput/lib/macos/libgainputstatic.a',
                                '../../../dependencies/optick/lib/macos/libOptickCore.dylib',
                            ],
                        },
                    },
                ],
                [
                    'OS=="linux"',
                    {
                        'include_dirs': [
                            '../dependencies/fbxsdk/linux/include',
                            '../dependencies/gainput/include',
                            '../dependencies/reactphysics3d/include',
                        ],
                        'defines': [
                            'LINUX',
                            'linux',
                            '__LINUX__',
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '../dependencies/fbxsdk/linux/include',
                            ],
                        },
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/linux/lib/libfbxsdk.a', 
                                '../../../dependencies/reactphysics3d/lib/linux/libreactphysics3d.a',
                                '../../../dependencies/gainput/lib/linux/libgainputstatic.a',
                                '../../../dependencies/optick/lib/linux/libOptickCore.so',
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
            'target_name':
            'DirectZobExe',
            'type':
            'executable',
            'dependencies': [
                'DirectZobEngine',
            ],
            'sources': [
                'main.cpp',
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'link_settings': {
                            # links fbxsdk as dll
                            'libraries': [
                                # relative to sln
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',
                                '../../../dependencies/gainput/lib/windows/gainput.lib',
                                'user32.lib',
                                'gdi32.lib',
                            ],
                            #		'library_dirs': [
                            #			'/usr/lib',
                            #		],
                        },
                        "copies": [
                            {
                                'destination':
                                '<(PRODUCT_DIR)',
                                'files': [
                                    '../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll',  # copy fbxsdk
									'../dependencies/optick/lib/windows/OptickCore.dll',  # copy OptickCore.dll
                                    '../dependencies/gainput/lib/windows/gainput.dll',
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
                            '../dependencies/fbxsdk/macos/include',
                        ],
                        'link_settings': {
                            'libraries': [
                                # relative to xcode project
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',
				                '../../../dependencies/gainput/lib/macos/libgainputstatic.a',
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
    ],
    'conditions': [
        [
            'OS=="win"',
            {
                'targets': [
                    {
                        'target_name':
                        'DirectZobDInterop',
                        'type':
                        'shared_library',
                        'configurations': {
                            'Debug': {
                                'msvs_settings': {
                                    'VCLinkerTool': {
                                        'AssemblyDebug':
                                        '1',  # add /ASSEMBLYDEBUG to linker options
                                    },
                                },
                            },
                        },
                        'dependencies': [
                            'DirectZobEngine',
                        ],
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'CompileAsManaged': 'true',
                                'ExceptionHandling':
                                '0',  # /clr is incompatible with /EHs
                            },
                        },
                        'sources': [
                            'Wrappers/CameraManagerWrapper.cpp',
                            'Wrappers/LightManagerWrapper.cpp',
                            'Wrappers/DirectZobWrapper.cpp',
                            'Wrappers/EngineWrapper.cpp',
                            'Wrappers/MeshManagerWrapper.cpp',
                            'Wrappers/ZobObjectManagerWrapper.cpp',
                            'Wrappers/ZobObjectWrapper.cpp',
                            'Wrappers/LightManagerWrapper.cpp',
                            'Wrappers/ZobLightWrapper.cpp',
                            'Wrappers/CameraManagerWrapper.cpp',
                            'Wrappers/ZobCameraWrapper.cpp',
                            'Wrappers/ZobSpriteWrapper.cpp',
                            'Wrappers/CameraManagerWrapper.h',
                            'Wrappers/LightManagerWrapper.h',
                            'Wrappers/DirectZobWrapper.h',
							'Wrappers/DirectZobWrapperEvents.h',
                            'Wrappers/EngineWrapper.h',
                            'Wrappers/ManagedVector3.h',
                            'Wrappers/ManagedObject.h',
                            'Wrappers/ManagedRenderOptions.h',
                            'Wrappers/MeshManagerWrapper.h',
                            'Wrappers/ZobObjectManagerWrapper.h',
                            'Wrappers/ZobObjectWrapper.h',
                            'Wrappers/ZobObjectWrapper.h',
                            'Wrappers/LightManagerWrapper.h',
                            'Wrappers/ZobLightWrapper.h',
                            'Wrappers/CameraManagerWrapper.h',
                            'Wrappers/ZobCameraWrapper.h',
                            'Wrappers/ZobSpriteWrapper.h',
							'Wrappers/ZobBehaviorWrapper.h',
							'Wrappers/ZobBehaviorWrapper.cpp',
							'Wrappers/ZobUserControls/ZobUserControls.h',
							'Wrappers/ZobUserControls/ZobUserControls.cpp',
							'Wrappers/Editor/ZobObjectsEditor.h',
							'Wrappers/Editor/ZobObjectsEditor.cpp',
							],
                        'link_settings': {
                            # links fbxsdk as static
                            'libraries': [
                                # relative to sln
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',
                                'user32.lib',
                                'gdi32.lib',
                            ],
                        },
                        "copies": [
                            {
                                'destination':
                                '<(PRODUCT_DIR)',
                                'files': [
                                    #'../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll'  # copy fbxsdk
									#'../dependencies/optick/lib/windows/OptickCore.dll',  # copy OptickCore.dll
									#'../dependencies/gainput/lib/windows/gainput.dll'
                                ],
                            },
                        ],
                    },
                ],
            }
        ],
    ],
}
