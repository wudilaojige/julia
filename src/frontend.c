// This file is a part of Julia. License is MIT: https://julialang.org/license

#include "julia.h"
#include "julia_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Parsing

// Pointer to the current parser
static jl_parse_func_t jl_current_parser = NULL;

JL_DLLEXPORT void jl_set_parser(jl_parse_func_t parser)
{
    jl_current_parser = parser;
}

JL_DLLEXPORT jl_value_t *jl_parse(const char* text, size_t text_len,
                                  const char* filename, size_t filename_len,
                                  size_t offset, int rule)
{
    return (*jl_current_parser)(text, text_len, filename, filename_len, offset, rule);
}

// C API
// parse an entire string as a file, reading multiple expressions
JL_DLLEXPORT jl_value_t *jl_parse_all(const char *text, size_t text_len,
                                      const char *filename, size_t filename_len)
{
    jl_value_t *p = jl_parse(text, text_len, filename, filename_len,
                             0, JL_PARSE_ALL);
    return jl_svecref(p, 0);
}

// this is for parsing one expression out of a string, keeping track of
// the current position.
// FIXME: Add filename?
JL_DLLEXPORT jl_value_t *jl_parse_string(const char *text, size_t text_len,
                                         int pos0, int greedy)
{
    return jl_parse(text, text_len, "none", 4,
                    pos0, greedy ? JL_PARSE_STATEMENT : JL_PARSE_ATOM);
}

// deprecated
JL_DLLEXPORT jl_value_t *jl_parse_input_line(const char *str, size_t len,
                                             const char *filename, size_t filename_len)
{
    return jl_parse_all(str, len, filename, filename_len);
}

#ifdef __cplusplus
}
#endif
