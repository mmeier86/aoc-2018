/**
 * @file aoc_err.h
 * @brief Error handling for AoC
 */

#pragma once


/**
 * @brief Helper function to set AoC error message
 *
 * Sets the @e aoc_err_msg string.
 * If errcode is 0, @e aoc_err_msg is set to errmsg. Otherwise, @e aoc_err_msg
 * is set to @e errmsg\nstrerror(errcode). Note the explicit lack of a
 * trailing newline.
 *
 * NOTE: Errors in this function are explicitly not handled - when an
 * error occurs in your error handling function, you're pretty well screwed
 * and the program will just bail out.
 *
 * @param errmsg The error message to show to the user
 * @param errcode The syscall error code which occured. 
 */
void set_aoc_err_msg(const char* errmsg, int errcode);

/**
 * @brief Returns the latest unretrieved AoC error message
 *
 * The latest, not yet retrieved error message set with set_aoc_err_msg
 * is returned. Subsequent calls to this function without intervening calls
 * to set_aoc_err_msg will return NULL.
 *
 * The returned string is null terminated and dynamically allocated. The
 * caller is responsible for cleaning it up.
 */
char* get_latest_aoc_err_msg();
