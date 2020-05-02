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
                    'xcode_settings':{
                        'CC':'clang++',
                        'MACOSX_DEPLOYMENT_TARGET':'10.17',
                        'CLANG_CXX_LANGUAGE_STANDARD':'c++11',
                        'CLANG_CXX_LIBRARY':'libc++',
                        'ARCHS': ['x86_64']
                    },
                    'link_settings': {
                        'libraries': [
                            '$(SDKROOT)/System/Library/Frameworks/Cocoa.framework',
                            '$(SDKROOT)/System/Library/Frameworks/Metal.framework',
                            #'$(SDKROOT)/System/Library/Frameworks/StoreKit.framework',
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
									'FloatingPointModel': '2', # fast
                                },
                            },
                        },
                        'OS=="linux"',
                        {
                            'cflags': 
                            [
                                '-g'
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
                                    'Optimization':'2',  # O2 optimisation
									'InlineFunctionExpansion': 2, # /Ob2, inline anything eligible
									'FavorSizeOrSpeed': 1, # /Ot, favour speed over size
									'FloatingPointModel': '2', # fast
									'EnableIntrinsicFunctions': 'true',
									'WholeProgramOptimization': 'true', # No
                                    'EnableEnhancedInstructionSet' : '5', # enable AVX2 instructions
                                },
                            },
                        },
                        'OS=="linux"',
                        {
                            'cflags': 
                            [
                                '-Ofast',
								'-march=haswell',
                                '-ffast-math'
                            ],
                        }
                    ],
                ],
            },
        },
    },
}
