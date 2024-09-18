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
                    'xcode_settings': {
                        'CC': 'clang++',
                        'MACOSX_DEPLOYMENT_TARGET': '11.1',
                        'CLANG_CXX_LANGUAGE_STANDARD': 'c++17',
                        'GCC_ENABLE_CPP_RTTI': 'YES',
                        'CLANG_CXX_LIBRARY': 'libc++',
                        'ARCHS': ['x86_64'],
                    },
                },
            ],
            [
                'OS=="linux"',
                {
                    'defines': [
                        'LINUX',
                        '__LINUX__',
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
                                    'FloatingPointModel': '2',  # fast
                                },
                            },
                        },
                        'OS=="mac"',
                        {
                            'xcode_settings': {
                                'GCC_OPTIMIZATION_LEVEL': '0',
                                'GCC_FAST_MATH': 'YES',
                                'CLANG_X86_VECTOR_INSTRUCTIONS': 'avx2',
                                'CLANG_CXX_LANGUAGE_STANDARD': 'c++17',
                                'GCC_ENABLE_CPP_RTTI': 'YES',
                                'ARCHS': ['x86_64'],
                            },
                            'cflags': [
                                '-Wno-pragma-pack',
                            ],
                            'cxxflags': [
                                '-Wno-pragma-pack',
                            ]
                        },
                        'OS=="linux"',
                        {
                            'cflags': [
                                '-g',
                                '-ggdb3',
                                ],
                        }
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
                                    'Optimization': '2',  # O2 optimisation
                                    'InlineFunctionExpansion':
                                    2,  # /Ob2, inline anything eligible
                                    'FavorSizeOrSpeed':
                                    1,  # /Ot, favour speed over size
                                    'FloatingPointModel': '2',  # fast
                                    'EnableIntrinsicFunctions': 'true',
                                    'WholeProgramOptimization': 'true',  # No
                                    'EnableEnhancedInstructionSet':
                                    '3',  # enable AVX instructions
                                },
                                'VCLinkerTool': {
                                    'LinkTimeCodeGeneration':
                                    '1',  # Link-time Code Generation
                                },
                                'VCLibrarianTool': {
                                    'AdditionalOptions': [
                                        '/LTCG:INCREMENTAL',  # incremental link-time code generation
                                    ]
                                },
                            },
                        },
                        'OS=="mac"',
                        {
                            'xcode_settings': {
                                'GCC_OPTIMIZATION_LEVEL': 's',
                                'GCC_FAST_MATH': 'YES',
                                'CLANG_X86_VECTOR_INSTRUCTIONS': 'avx2',
                            },
                        },
                        'OS=="linux"',
                        {
                            'cflags':
                            [   
                                '-Ofast', 
                                '-march=native', 
                                '-ffast-math',
                                '-Wnarrowing',
                            ],
                        }
                    ],
                ],
            },
        },
    },
}
