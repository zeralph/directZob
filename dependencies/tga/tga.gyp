{
    'includes': [
      '../../defaults.gypi',
    ],
    'targets': [
        {
            'target_name':
            'tga',
            'type':
            'static_library',
            'direct_dependent_settings': {
                'include_dirs': [
                    '.',
                ],
            },
            'sources': [
				'tga.h',
				'tga.cpp',
            ],
        },
    ],
}