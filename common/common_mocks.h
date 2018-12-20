/**
 * @file common_mocks.h
 * @brief Mocks for common library modules
 */

#pragma once

#include "mm_files.h"
#include "tokenizer.h"

struct mock_mm_file_read_t{
  char* retval;
  const char* param_1;
  int callcount;
  int set_errno_to;
};

extern struct mock_mm_file_read_t* mock_mm_file_read;

struct mock_get_tokenizer_t{
  tok_t* retval;
  char* param_1;
  char* param_2;
  int callcount;
};

extern struct mock_get_tokenizer_t* mock_get_tokenizer;

struct mock_n_tok_t{
  char* retval;
  tok_t* param_1;
  int callcount;
};

extern struct mock_n_tok_t* mock_n_tok;

struct mock_free_tok_t{
  tok_t* param_1;
  int callcount;
};

extern struct mock_free_tok_t* mock_free_tok;
