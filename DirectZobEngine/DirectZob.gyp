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
                'Cube.cpp',
                'DirectZob.cpp',
                'Engine.cpp',
                'Light.cpp',
                'LightManager.cpp',
                'lodepng.cpp',
                'Material.cpp',
                'MaterialManager.cpp',
                'Matrix2x2.cpp',
                'Matrix4x4.cpp',
                'Mesh.cpp',
                'MeshManager.cpp',
                'Rasterizer.cpp',
                'SceneLoader.cpp',
                'Text2D.cpp',
                'Triangle.cpp',
                'Vector2.cpp',
                'Vector3.cpp',
                'ZOBGUID.cpp',
                'ZobObject.cpp',
                'ZobObjectManager.cpp',
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/windows/include',
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
                        ],
                        'defines': [
                            'MACOS',
                        ],
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',  # relative to xcode project
                            ],
                        },
						'xcode_settings':{
							'CC':'clang',
							'MACOSX_DEPLOYMENT_TARGET':'10.14',
							'CLANG_CXX_LANGUAGE_STANDARD':'c++11',
							'CLANG_CXX_LIBRARY':'libc++',
						},
                    },
                ],
                [
                    'OS=="linux"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/linux/include',
                        ],
                        'defines': [
                            'LINUX',
                        ],
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '.',
                                '../dependencies/fbxsdk/linux/include',
                            ],
                        },
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/linux/lib/libfbxsdk.a',  # relative to xcode project
                                '/usr/lib/x86_64-linux-gnu/libpthread.so',
                                '/usr/lib/x86_64-linux-gnu/libxml2.so',
                                '/usr/lib/x86_64-linux-gnu/libX11.so',
                                '/usr/lib/x86_64-linux-gnu/libdl.so',
                                '/usr/lib/x86_64-linux-gnu/libz.so',
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
                            ],
                        },
						'xcode_settings':{
							'CC':'clang',
							'MACOSX_DEPLOYMENT_TARGET':'10.14',
							'CLANG_CXX_LANGUAGE_STANDARD':'c++11',
							'CLANG_CXX_LIBRARY':'libc++',
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
                            'DirectZobWrapper.cpp',
                            'EngineWrapper.cpp',
                            'MeshManagerWrapper.cpp',
                            'ZobObjectManagerWrapper.cpp',
                            'ZobObjectWrapper.cpp',
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
