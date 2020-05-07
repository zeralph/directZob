import sys
import os.path
import platform

version = platform.python_version()
majorVersion = version[0]
if majorVersion is '2':
    print('running in pyhton 2')
    UNIX_NEWLINE = '\n'
    WINDOWS_NEWLINE = '\r\n'
    filePath = sys.argv[1]
    if not os.path.exists(filePath):
        print("cannot open " + filePath)
    else:
        file = open(filePath)
        content = ''
        for line in file: 
            lines = line.replace(WINDOWS_NEWLINE, UNIX_NEWLINE) 
            content = content + lines
        file.close()
        file = open(filePath, 'w')
        file.write(content)
        file.close()
else:
    print('running in pyhton 3')
    filePath = sys.argv[1]
    file = open(filePath)
    content = ''
    for line in file:
        if not line.strip(): continue  # skip the empty line
        content = content + line
    file = open(filePath, 'w')
    file.write(content)
