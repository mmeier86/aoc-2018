/**
 * @file main.h
 * @brief Runner for AoC day modules
 */

#pragma once

#include "tokenizer.h"

int aoc_main(int argc, char** argv, char* (*dayfunc)(tok_t*));
