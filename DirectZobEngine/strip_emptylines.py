import sys
import os.path

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

