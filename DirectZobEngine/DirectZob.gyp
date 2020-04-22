{
    'targets': [
        {
            'target_name':
            'DirectZob',
            'type':
            'executable',
            'dependencies': [
                '../dependencies/minifb/minifb.gyp:minifb',
                '../dependencies/tinyxml/tinyxml.gyp:tinyxml',
            ],
            'sources': [
                'Camera.cpp',
                'CameraManager.cpp',
                'CameraManagerWrapper.cpp',
                'Color.cpp',
                'Cube.cpp',
                'DirectZob.cpp',
                'DirectZobWrapper.cpp',
                'Engine.cpp',
                'EngineWrapper.cpp',
                'Light.cpp',
                'LightManager.cpp',
                'lodepng.cpp',
                'main.cpp',
                'Material.cpp',
                'MaterialManager.cpp',
                'Matrix2x2.cpp',
                'Matrix4x4.cpp',
                'Mesh.cpp',
                'MeshManager.cpp',
                'MeshManagerWrapper.cpp',
                'Rasterizer.cpp',
                'SceneLoader.cpp',
                'Text2D.cpp',
                'Triangle.cpp',
                'Vector2.cpp',
                'Vector3.cpp',
                'ZOBGUID.cpp',
                'ZobObject.cpp',
                'ZobObjectManager.cpp',
                'ZobObjectManagerWrapper.cpp',
                'ZobObjectWrapper.cpp',
            ],
            'conditions': [
                [
                    'OS=="win"',
                    {
                        'include_dirs': [
                            '.',
                            '../dependencies/fbxsdk/windows/include',
                        ],
                        'link_settings': {
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
                    }
                ],
            ],
        },
    ],
}
