{
    'targets': [
        {
            'target_name':
            'DirectZobLib',
            'type':
            'static_library',
            'dependencies': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
            ],
            'export_dependent_settings': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
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
                        'direct_dependent_settings': {
                            'include_dirs': [
                                '.',
                                '../dependencies/fbxsdk/windows/include',
                            ],
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
                        'link_settings': {
                            'libraries': [
                                '../../../dependencies/fbxsdk/macos/lib/clang/release/libfbxsdk.a',  # relative to xcode project
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
                'DirectZobLib',
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
                    '../../../dependencies/fbxsdk/windows/lib/vs2017/x86/release/libfbxsdk.lib',  # relative to sln
                    'kernel32.lib',
                    'user32.lib',
                    'gdi32.lib',
                    'winspool.lib',
                    'comdlg32.lib',
                    'advapi32.lib',
                    'shell32.lib',
                    'ole32.lib',
                    'oleaut32.lib',
                    'uuid.lib',
                    'odbc32.lib',
                    'odbccp32.lib',
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
                        '../dependencies/fbxsdk/windows/lib/vs2017/x86/release/libfbxsdk.dll'  # copy fbxsdk 
                    ],
                },
            ],
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
                        'DirectZobDll',
                        'type':
                        'shared_library',
                        'dependencies': [
                            'DirectZobLib',
                        ],
                        'msvs_settings': {
                            'VCCLCompilerTool': {
                                'CompileAsManaged': 'true',
                                'ExceptionHandling':
                                '0'  # /clr is incompatible with /EHs
                            }
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
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x86/release/libfbxsdk-md.lib',  # relative to sln
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x86/release/libxml2-md.lib',
                                '../../../dependencies/fbxsdk/windows/lib/vs2017/x86/release/zlib-md.lib',
                                'kernel32.lib',
                                'user32.lib',
                                'gdi32.lib',
                                'winspool.lib',
                                'comdlg32.lib',
                                'advapi32.lib',
                                'shell32.lib',
                                'ole32.lib',
                                'oleaut32.lib',
                                'uuid.lib',
                                'odbc32.lib',
                                'odbccp32.lib',
                            ],
                        },
                    },
                ],
            }
        ],
    ],
}
