# Advent of Code 2018 #

My solutions to the [Advent of Code 2018](https://adventofcode.com/2018).

Please note that this code has been written on a Linux system, using
the Linux API. I've tried to catch all non-standard functions and check
for them in the build, but I might have missed some.
Thus, please make sure you're using a Linux system and a compiler with the
GNU extensions enabled if you get missing header/symbol errors during build.

## Common Module ##

The `common/` module contains some usefull utilities for working with
Advent of Code puzzles.

### Tokenizer ###

The tokenizer utility (`tokenizer.{c,h}`) provides facilities to split
a string among a common delimiter (`\n` in AoC inputs). It uses `strtok_r`
under the hood to facilitate the actual splitting of the string.

The functions are, in short:

  * `get_tokenizer`: This function initializes a tokenizer for the string `s`
  with delimiter `delim`. This is the only way to instantiate a tokenizer.
  The input string is not changed.
  * `n_tok` Returns the next token or `NULL` if there are no more tokens. Note
  that the tokens returned by this function are only valid until `free_tok` or
  `reset_tok` are called on the producing tokenizer.
  * `free_tok` Free all internal memory and finally the tokenizer itself.
  Invalidates all produced tokens!
  * `reset_tok` Resets the tokenizer, so that the next token produced by
  `n_tok` will be the original input string's first token again
  * `tok_count` Returns the currently remaining number of tokens. Before the
  first call to `n_tok`, this retuns the total number of tokens in the input
  string. Is reset by `reset_tok`.
  
### File Reading ###

The mm_files utility (`mm_files.{c,h}`) provides a facility to read a complete
file into memory via the `mmap` function. That function is used to read the
entire file. The content is then memcopied to a buffer and the file is
unmapped again. The buffer is then returned to the caller.

### UT supporting streams ###

The `aoc_streams.h` header provides `STDOUT_STREAM` and `STDERR_STREAM` defines for
use in programs. If the `UNITTESTS_ENABLED` define is defined at compile time,
those two defines will be set to `exten FILE*` variables named `stdout_ut` and 
`stderr` respectively. Those can then be defined in a test file and checked in UTs
to make sure the program outputs what it needs to output.

Of course, for this to work the user needs to use `fprintf` and friends with `STDERR_STREAM/STDOUT_STREAM` as their file parameters instead of plain `printf`.

### Error Handling ###

The `aoc_err.{c,h}` module provides facilities for the AoC day functions to 
communicate errors to the `aoc_main` function.

To this end, the `set_aoc_err_msg` function can be used to store an error message,
which can then be retrieved with the `get_latest_aoc_err_msg` function.

Note that the `get_latest_aoc_err_msg` only every retrieves the latest message,
as the buffer only holds a single message.

### AoC Main ###

The `aoc_main` function (`main.{c,h}`) is the core of the AoC common code. It
parses command line parameters and calls the day's AoC functions on them.

`aoc_main` accepts the `argc` and `argv` parameters from `main`, plus two
functions taking a tokenizer as input and returning a dynamically allocated
string containing the result.

It is called in the form `aoc_main(argc, argv, func_part1, func_part2)`. Then it does the following:
  * Checks whether the command line parameters contain a "1" or "2" to indicate
  which function to call
  * Uses the *mm_files*  module to load the AoC input file given in `argv[2]`
  * Initializes a tokenizer with the full file content an `\n` as a delimiter
  * Hands that tokenizer over to the `func_part1` or `func_part2` depending
  on whether `argv[1]` is "1" or "2"
  * Prints the function's result to stdout
  * Cleans up after itself

## Days ##

### Day 01 ###

  * Directory: `day_01/`
  * Task: https://adventofcode.com/2018/day/1
  * Input: `day_01/input.txt`
  * Build: `cd day_01 && mkdir build && cmake .. && make day_01`
  * UT: `cd day_01 && mkdir build && cmake -DUNITTESTS_ENABLED=ON .. && make check`

