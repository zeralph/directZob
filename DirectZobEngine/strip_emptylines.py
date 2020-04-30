import sys
filePath = sys.argv[1]
file = open(filePath)
content = ''
for line in file:
    if not line.strip(): continue  # skip the empty line
    content = content + line
file = open(filePath, 'w')
file.write(content)
