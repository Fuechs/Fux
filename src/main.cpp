#include <iostream>

#include "include/std.h"

using namespace std;

int name(int argc, char** argv) {
   
   if (argc < 2) {
      print_help();
      return 1;
   }
   
   else if (argc == 2 && argv[1] != ("version" || "help")) {
        
      cout << "Missing argument: source file" << endl;
      return 1;
   
   }
   
   cout << "SoonTM" << endl;
   return 0;
   
}

void print_help() {
   
   cout << "Fux | Help" << endl;
   cout << "v" << version_x << "." << version_y << "." << version_z << version_c << endl;
   cout << "Usage: fux [command] [source file]" << endl;
   cout << "Commands:" << endl;
   cout << "compile           compiles the source file in the following argument" << endl;
   cout << "run               runs the compiled binary file in the following argument" << endl;
   cout << "execute           compiles and runs the source file in the following argument" << endl;
   cout << "version           shows current version of fux" << endl;
   cout << "help              shows this help" << endl;
   cout << "[source file]:    path to an file" << endl;

}
