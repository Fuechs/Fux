#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

char* main(char** argv) {

  if (strcmp("user_input", argv[1]) == 0) {
  
    return user_input(argv[2]);
    
  }
  
  if (strcmp("lower_string", argv[1]) == 0 {
  
    return NULL;
    
  }
  
  return NULL;
  
}

char* user_input(char* message) {

  char* input;
  char* input_msg;
  
  strcpy(input_msg, "%s%s", message);
  
  scanf(input_msg, &input);
  
  return input;

}
      
char* lower_string(char* string) {

  return NULL;

}
      
char* upper_string(char* string) {

  return NULL;
  
}
