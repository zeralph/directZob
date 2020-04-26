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
                        ]
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