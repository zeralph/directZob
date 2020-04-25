{
    'includes': [
      '../../defaults.gypi',
    ],
    'targets': [
        {
            'target_name':
            'nanojpeg',
            'type':
            'static_library',
            'direct_dependent_settings': {
                'include_dirs': [
                    '.',
                ],
            },
            'sources': [
                'nanojpeg.cpp',
            ],
        },
    ],
}