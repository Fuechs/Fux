#############################################
#        Python Setup for Fux               #
#                                           #
#        Copyright (c) 2022 - Fuechs        #
#############################################

import time, os, sys
clear = lambda: os.system("clear")
init = lambda: os.chdir("../"); os.system("make")
def main(): clear(); time.sleep(0.15); init()
if __name__ == "__main__": main()