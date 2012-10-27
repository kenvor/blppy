from distutils.core import setup, Extension

blpapi = Extension('blpapi',
                   include_dirs = ['/usr/local/blpapi/include'],
                   libraries = ['blpapi3_32'],
                   library_dirs = ['/usr/local/blpapi/lib'],
                   sources = ['src/blpapi.c', 
                              'src/Session.cpp', 
                              'src/Service.cpp',
                              'src/Request.cpp',
                              'src/Element.cpp',
                              'src/Event.cpp',
                              'src/Message.cpp'])

setup (name = 'blpapi',
       version = '1.0',
       description = 'Bloomberg BLPAPI',
       py_modules = ['blp'],
       ext_modules = [blpapi])
