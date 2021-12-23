#!C:\Users\zeral\AppData\Local\Microsoft\WindowsApps\PythonSoftwareFoundation.Python.3.7_qbz5n2kfra8p0\python.exe
# EASY-INSTALL-ENTRY-SCRIPT: 'gyp==0.1','console_scripts','gyp'
__requires__ = 'gyp==0.1'
import re
import sys
from pkg_resources import load_entry_point

if __name__ == '__main__':
    sys.argv[0] = re.sub(r'(-script\.pyw?|\.exe)?$', '', sys.argv[0])
    sys.exit(
        load_entry_point('gyp==0.1', 'console_scripts', 'gyp')()
    )
