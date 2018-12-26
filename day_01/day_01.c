/**
 * @file day_01.c
 * @brief Main for AoC Day 01
 */

#include "chronal_calibration.h"
#include "main.h"

int main(int argc, char** argv){
  return aoc_main(argc, argv, compute_freq, get_first_repetition);
}
