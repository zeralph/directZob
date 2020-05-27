{
    'includes': [
      '../../defaults.gypi',
    ],
    'variables' : 
    {
                'minifb_macos_flags%': "DO_NOT_USE_METAL_API", # fake text, but we need something
            },
    'targets': [
        {
            'target_name':
            'minifb',
            'type':
            'static_library',
            'direct_dependent_settings': {
                'include_dirs': ['include'],
            },
            'include_dirs': ['include', 'src'],
            'sources': [
                'src/MiniFB_common.c',
                'src/MiniFB_cpp.cpp',
                'src/MiniFB_internal.c',
                'src/MiniFB_timer.c',
            ],
            'conditions': [
                ['OS=="mac"', {
                    'sources': [
                        'src/macosx/MacMiniFB.m',
                        'src/macosx/OSXWindow.m',
                        'src/macosx/OSXView.m',
                        'src/macosx/OSXViewDelegate.m',
                    ],
                    'defines': [
                        '<(minifb_macos_flags)',
                    ],
                }],
                ['OS=="linux"', {
                    'sources': [
                        'src/MiniFB_linux.c',
                        'src/x11/X11MiniFB.c',
                    ]
                }],
                ['OS=="win"', {
                    'sources': [
                        'src/Windows/WinMiniFB.c',
                    ]
                }],
            ],
        },
    ],
}
