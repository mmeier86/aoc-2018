/**
 * @file day_03.c
 * @brief Main for AoC Day 03
 */

#include "cloth_cutting.h"
#include "main.h"

#include <stddef.h>

int main(int argc, char** argv){
  return aoc_main(argc, argv, cloth_slicing, find_valid_claim);
}
