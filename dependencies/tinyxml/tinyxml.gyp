{
    'targets': [
      {
        'target_name': 'tinyxml',
        'type': 'static_library',
		'direct_dependent_settings': {
          'include_dirs': [
			'.'
          ],
        },
        'sources': [
          'tinystr.cpp',
		  'tinyxml.cpp',
		  'tinyxmlerror.cpp',	
		  'tinyxmlparser.cpp',
        ],
      },
    ],
}