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
                        },
                    ],
                ],
            },
        },
    },
}
