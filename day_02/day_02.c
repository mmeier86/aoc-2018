/**
 * @file day_02.c
 * @brief Main for AoC Day 02
 */

#include "inventory_mgmt.h"
#include "main.h"

#include <stddef.h>

int main(int argc, char** argv){
  return aoc_main(argc, argv, box_checksum, similar_id);
}
