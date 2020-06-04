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
            ],
            'export_dependent_settings': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
                '../dependencies/nanojpeg/nanojpeg.gyp:nanojpeg',
            ],
            'sources': [
                'Camera.cpp',
                'CameraManager.cpp',
                'Color.cpp',
                'DirectZob.cpp',
                'Engine.cpp',
                'Light.cpp',
                'LightManager.cpp',
                'lodepng.cpp',
                'Material.cpp',
                'MaterialManager.cpp',
                'Mesh.cpp',
                'MeshManager.cpp',
                'Rasterizer.cpp',
                'SceneLoader.cpp',
				'Sprite.cpp',
                'Text2D.cpp',
				'Texture.cpp',
                'Triangle.cpp',
				'ZobMatrix2x2.cpp',
                'ZobMatrix4x4.cpp',
                'ZobVector2.cpp',
                'ZobVector3.cpp',
                'ZOBGUID.cpp',
                'ZobObject.cpp',
				'ZobSprite.cpp',
                'ZobObjectManager.cpp',
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/windows/include',
							'../dependencies/reactphysics3d/include',
                        ],
                        'defines': [
                            'FBXSDK_SHARED',  # required to link with dll
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '.',
                                '../dependencies/fbxsdk/windows/include',
                            ],
                            'defines': [
                                'FBXSDK_SHARED',
                            ],
                        },
						'link_settings': {
                            'libraries': [
                                '../../../dependencies/reactphysics3d/lib/windows/reactphysics3d.lib',  # relative to xcode project
                            ],
                        },
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'FloatingPointModel': '2', # fast
                            },
                        },
                    },
                ],
                [
                    'OS=="mac"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/macos/include',
							'../dependencies/reactphysics3d/include',
                        ],
                        'defines': [
                            'MACOS',
                            '__APPLE__',
                        ],
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',  # relative to xcode project
                            ],
                        },
                    },
                ],
                [
                    'OS=="linux"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/linux/include',
							'../dependencies/reactphysics3d/include',
                        ],
                        'defines': [
                            'LINUX',
                            'linux',
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '.',
                                '../dependencies/fbxsdk/linux/include',
								'../dependencies/reactphysics3d/include',
                            ],
                        },
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/linux/lib/libfbxsdk.a', 
                                '../../../dependencies/reactphysics3d/lib/linux/libreactphysics3d.a',
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
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',  # relative to sln
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
                                    '../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll'  # copy fbxsdk 
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
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',  # relative to xcode project
                            '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
                            '$(SDKROOT)/System/Library/Frameworks/Metal.framework',
                            '$(SDKROOT)/System/Library/Frameworks/GameKit.framework',
                            '-liconv',
                            '-lz',
                            '-lxml2',
                            '-ObjC',
                            '-v',
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
                            'CameraManagerWrapper.cpp',
							'LightManagerWrapper.cpp',
                            'DirectZobWrapper.cpp',
                            'EngineWrapper.cpp',
                            'MeshManagerWrapper.cpp',
                            'ZobObjectManagerWrapper.cpp',
                            'ZobObjectWrapper.cpp',
							'LightManagerWrapper.cpp',
                            'ZobLightWrapper.cpp',
							'CameraManagerWrapper.cpp',
                            'ZobCameraWrapper.cpp',
							'ZobSpriteWrapper.cpp',
                        ],
                        'link_settings': {
                            # links fbxsdk as static
                            'libraries': [
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.lib',  # relative to sln
                                'user32.lib',
                                'gdi32.lib',
                            ],
                        },
                        "copies": [
                            {
                                'destination':
                                '<(PRODUCT_DIR)',
                                'files': [
                                    '../dependencies/fbxsdk/windows/lib/vs2017/x64/release/libfbxsdk.dll'  # copy fbxsdk 
                                ],
                            },
                        ],
                    },
                ],
            }
        ],
    ],
}
