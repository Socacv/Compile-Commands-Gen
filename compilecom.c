#include <linux/limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
enum IS_HEAP_FLAG{
  STR1_HEAP_F = 0x01,
  STR2_HEAP_F = 0x02,
  WITH_SPACE  = 0x04,
  WITH_SLASH  = 0x08
};
void append(char **str1,char **str2, uint8_t isHeap){
  if(!str1 || !str2){
    fprintf(stderr,"Invalid arguments for append()");
    return;
  }
  uint32_t str1len = strlen(*str1);
  uint32_t str2len = strlen(*str2);
  uint32_t newstrlen = str1len + str2len;
  char* newstr;
  if(isHeap & WITH_SPACE){
    newstr = calloc(newstrlen+2, sizeof(char));
    memset(newstr, 0, newstrlen+1); 
    memcpy(newstr, *str1, str1len);
    *(newstr+str1len) = ' '; 
    memcpy(newstr+str1len+1,*str2,str2len);
  }
  else if(isHeap & WITH_SLASH){
    newstr = calloc(newstrlen+2, sizeof(char));
    memset(newstr, 0, newstrlen+1); 
    memcpy(newstr, *str1, str1len);
    *(newstr+str1len) = '/'; 
    memcpy(newstr+str1len+1,*str2,str2len);
  }else{
    newstr = calloc(newstrlen+1, sizeof(char));
    memset(newstr, 0, newstrlen+1); 
    memcpy(newstr, *str1, str1len);
    memcpy(newstr+str1len,*str2,str2len);
  }
  if(isHeap & STR1_HEAP_F){
    free(*str1);
  }
  *str1 = newstr;
  if(isHeap & STR2_HEAP_F){
    free(*str2);
    *str2 = NULL;
  }
}
void usage(){
  fprintf(stdout, "Usage: gencc  <source.cpp> <cflags>\n"
                   "Cflags can be generated with pkg-config\n");
}
int main(int argc, char *argv[])
{
  if(argc < 3){
    usage();
    exit(1);
  }
  if(argv[1][0] == '-'){
    fprintf(stderr, "First file must be the file name\n");
    usage();
    exit(1);
  }
  FILE* fd = fopen("compile_commands.json", "w+");
  char pwd[PATH_MAX];
  char* fpwd = pwd;
  getcwd(pwd, sizeof(pwd));

  append(&fpwd, &argv[1], WITH_SLASH);
  
  char* startcommand = calloc(14,sizeof(char));
  memset(startcommand, 0, 14);
  memcpy(startcommand, "/usr/bin/g++", 13);
  for(int i = 2; i < argc; i++){
    append(&startcommand, &argv[i], STR1_HEAP_F | WITH_SPACE);
  }
  fprintf(fd, "[{\"directory\": \"%s\",\"command\": \"%s\",\"file\": \"%s\"}]",pwd, startcommand,fpwd);
  free(fpwd);
  free(startcommand);
  fclose(fd);
  return EXIT_SUCCESS;
}
