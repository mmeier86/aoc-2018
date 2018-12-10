/**
 * @file mm_files.h
 * @brief Functions for memory mapping text files.
 */

/**
 * @brief Read text file into a string.
 *
 * Reads an entire text file from @e fpath into a string
 * and returns that string.
 *
 * @e fpath cannot be NULL and must provide the path
 * to an existing file.
 *
 * The returned string is placed in heap memory. Freeing
 * that memory is the duty of the caller.
 *
 * The function returns NULL upon error.
 *
 * @param fpath null terminated path to the file to load
 * @returns Pointer to a string containing the file content or NULL on
 *          error.
 */
char* mm_file_read(const char* fpath);
