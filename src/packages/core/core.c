#include <stdio.h>

int output(char* message) {
  if (!message) return 1;
  print("%s", message);
  return 0;
}

char* input() {
  char* read[2048];
  fread("%s", &read);
  if (!read) return NULL;
  return read;
}



int main(char** argv) {
  return execute_command(argv[2]);
}
