{
    'includes': [
      '../../defaults.gypi',
    ],
    'targets': [
        {
            'target_name':
            'minifb',
            'type':
            'static_library',
            'direct_dependent_settings': {
                'include_dirs': ['.'],
            },
            'include_dirs': ['.'],
            'sources': [
                'MiniFB_common.c',
                'MiniFB_cpp.cpp',
                'MiniFB_internal.c',
            ],
            'conditions': [
                [
                    'OS=="mac"', {
                        'sources': [
                            'macosx/MacMiniFB.m',
                            'macosx/OSXWindow.m',
                            'macosx/OSXWindowFrameView.m',
                        ],
						'xcode_settings':{
							'CC':'clang',
							'MACOSX_DEPLOYMENT_TARGET':'10.7',
							'CLANG_CXX_LANGUAGE_STANDARD':'c++11',
							'CLANG_CXX_LIBRARY':'libc++',
						},
                    }
                ],
                ['OS=="linux"', {
                    'sources': [
                        'x11/X11MiniFB.c',
                    ]
                }],
                ['OS=="win"', {
                    'sources': [
                        'Windows/WinMiniFB.c',
                    ]
                }],
            ],
        },
    ],
}
