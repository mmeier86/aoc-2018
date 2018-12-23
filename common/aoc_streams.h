/**
 * @file aoc_streams.h
 * @brief Stream redirections for AoC
 *
 * The stdout/stderr streams get redirected to memstreams when
 * UNITTESTS_ENABLED is defined at compile time, to make output
 * testing easier.
 */

#pragma once

#include <stdio.h>

#ifdef UNITTESTS_ENABLED
extern FILE* stdout_ut;
extern FILE* stderr_ut;
#define STDOUT_STREAM stdout_ut
#define STDERR_STREAM stderr_ut
#else
#define STDOUT_STREAM stdout
#define STDERR_STREAM stderr
#endif
