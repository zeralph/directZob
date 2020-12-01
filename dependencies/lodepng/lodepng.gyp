{
    'includes': [
      '../../defaults.gypi',
    ],
    'targets': [
        {
            'target_name':
            'lodepng',
            'type':
            'static_library',
            'direct_dependent_settings': {
                'include_dirs': [
                    '.',
                ],
            },
            'sources': [
				'lodepng.h',
                'lodepng.cpp',
            ],
        },
    ],
}