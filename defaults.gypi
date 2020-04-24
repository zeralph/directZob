{
    'target_defaults': {
        'msvs_settings': {
            'VCCLCompilerTool': {
                'MultiProcessorCompilation': 'true',
            },
        },
        'configurations': {
            'Debug': {
                'defines': [
                    'DEBUG',
                ],
                'conditions': [
                    [
                        'OS=="win"',
                        {
                            #'msvs_configuration_platform': 'x64',
                            #'msvs_target_platform': 'x64',
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
                            #'msvs_configuration_platform': 'x64',
                            #'msvs_target_platform': 'x64',
                        },
                    ],
                ],
            },
        },
    },
}
