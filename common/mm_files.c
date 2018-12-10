/**
 * @file mm_file.c
 * @brief Implementation of file loading function.
 */

#include "mm_files.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

char* mm_file_read(const char* fpath){
  struct stat statbuf;
  if(stat(fpath,&statbuf) == -1){
    return NULL;
  }
  int fd = open(fpath, O_RDONLY);
  if(fd == -1){
    return NULL;
  }
  char* resbuf = malloc(sizeof(char) * statbuf.st_size + 1);
  char* mapres = (char*) mmap(NULL, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if(mapres == MAP_FAILED){
    free(resbuf);
    close(fd);
    return NULL;
  }
  memcpy(resbuf, mapres, statbuf.st_size);
  resbuf[statbuf.st_size] = '\0';
  if(munmap(mapres, statbuf.st_size) == -1){
    free(resbuf);
    close(fd);
    return NULL;
  }
  close(fd);
  return resbuf;
}
