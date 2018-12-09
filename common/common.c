/**
 * @file common.c
 * @brief Implementation of common functions for the AoC 2018
 */

#include "common.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>

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
