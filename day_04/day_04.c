/**
 * @file day_04.c
 * @brief Main for AoC Day 04
 */

#include "repose_record.h"
#include "main.h"

#include <stddef.h>

int main(int argc, char** argv){
  return aoc_main(argc, argv, most_asleep_guard, NULL);
}
