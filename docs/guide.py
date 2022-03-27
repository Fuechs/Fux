import os, sys, time

platforms = ["linux", "linux2", "darwin"]

if sys.platform in platforms:
    clear = lambda: os.system("clear")
elif sys.platform == "win32":
    clear = lambda: os.system("cls")

def print_title(width: int = 28, width2: int = 5, edge: int = 0):

    print('|'+                               '-'*width+     '|')
    print('|'+"     ______ _   ___   __"    +' '*(width2-1)+    '|')
    print('|'+"    |  ___| | | \ \ / /"     +' '*width2+    '|')
    print('|'+"    | |_  | | | |\ V / "     +' '*width2+    '|')        
    print('|'+"    |  _| | | | |/   \ "     +' '*width2+    '|')
    print('|'+"    | |   | |_| / /^\ \ "    +' '*(width2-1)+'|')
    print('|'+"    \_|    \___/\/   \/"     +' '*width2+    '|')
    print('|'+                               ' '*width+     '|')
    print('|'+                               '-'*width+     '|') 


clear()
print_title()