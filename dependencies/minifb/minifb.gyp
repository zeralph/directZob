{
    'targets': [
      {
        'target_name': 'minifb',
        'type': 'static_library',
		'direct_dependent_settings': {
          'include_dirs': [
			'.'
          ],
        },
       'include_dirs': [
			'.'
        ],
        'sources': [
          'MiniFB_common.c',
		  'MiniFB_cpp.cpp',
		  'MiniFB_internal.c',
		  'Windows/WinMiniFB.c',
        ],
		'conditions': [
			['OS!="win"', {'sources!': ['Windows/WinMiniFB.c']}],
		],
      },
    ],
}