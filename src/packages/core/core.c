#include <stdio.h>


int main() {
  return 0;
}

int output(char* message) {
  if (!message) return 1;
  print("%s", message);
  return 0;
}
