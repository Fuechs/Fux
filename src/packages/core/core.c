#include "core.h"

char* user_input(char* message) {

  char* input;
  char* input_msg;
  
  strcpy(input_msg, "%s%s", message);
  
  scanf(input_msg, &input);
  
  return input;

}
