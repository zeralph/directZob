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
                        #'USE_METAL_API',
                    ],
                    'xcode_settings': {
                        'CC': 'clang++',
                        'MACOSX_DEPLOYMENT_TARGET': '10.14',
                        'CLANG_CXX_LANGUAGE_STANDARD': 'c++11',
                        'CLANG_CXX_LIBRARY': 'libc++',
                        'ARCHS': ['x86_64'],
                    },
                    'link_settings': {
                        'libraries': [
                            '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
                            '$(SDKROOT)/System/Library/Frameworks/Metal.framework',
                            '$(SDKROOT)/System/Library/Frameworks/GameKit.framework',
                            '-liconv',
                            '-lz',
                            '-lxml2',
                            '-ObjC',
                            '-v',
                        ],
                    },
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
                            'cflags': ['-g'],
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
                            ['-Ofast', '-march=haswell', '-ffast-math'],
                        }
                    ],
                ],
            },
        },
    },
}
