{
    'target_defaults': {
        'conditions': [
            [
                'OS=="win"',
                {
                    'msvs_settings': {
                        'VCCLCompilerTool': {
                            'MultiProcessorCompilation': 'true',
                        },
                    },
                    'defines': [
                        'WINDOWS',
                    ],
                },
            ],
            [
                'OS=="mac"',
                {
                    'defines': [
                        'MACOS',
                    ],
                },
            ],
            [
                'OS=="linux"',
                {
                    'defines': [
                        'LINUX',
                    ],
                },
            ],
        ],
        'configurations': {
            'Debug': {
                'defines': [
                    'DEBUG',
                ],
                'conditions': [
                    [
                        'OS=="win"',
                        {
                            'msvs_configuration_platform': 'x64',
                            'msvs_target_platform': 'x64',
							'msvs_settings': {
                                'VCCLCompilerTool': {
                                    'Optimization': '0',  # Od optimisation
									'FloatingPointModel': '2', # fast
                                },
                            },
                        },
                    ],
                ],
            },
            'Release': {
                'defines': [
                    'RELEASE',
                ],
                'conditions': [
                    [
                        'OS=="win"',
                        {
                            'msvs_configuration_platform': 'x64',
                            'msvs_target_platform': 'x64',
                            'msvs_settings': {
                                'VCCLCompilerTool': {
                                    'Optimization':'2',  # O2 optimisation
									'InlineFunctionExpansion': 2, # /Ob2, inline anything eligible
									'FavorSizeOrSpeed': 1, # /Ot, favour speed over size
									'FloatingPointModel': '2', # fast
									'EnableIntrinsicFunctions': 'true',
									'WholeProgramOptimization': 'true', # No
                                },
                            },
                        },
                    ],
                ],
            },
        },
    },
}
