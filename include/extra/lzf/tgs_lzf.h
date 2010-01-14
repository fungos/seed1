#ifndef SEED_LZF_H
#define SEED_LZF_H

#include "Defines.h"
#include "extra/LZF/lzf.h"

/*
 * Decompress data compressed with some version of the lzf_compress
 * function and stored at location in_data and length in_len. The result
 * will be stored at out_data up to a maximum of out_len characters.
 *
 * If the output buffer is not large enough to hold the decompressed
 * data, a 0 is returned and errno is set to E2BIG. Otherwise the number
 * of decompressed bytes (i.e. the original length of the data) is
 * returned.
 *
 * If an error in the compressed data is detected, a zero is returned and
 * errno is set to EINVAL.
 *
 * This function is very fast, about as fast as a copying loop.
 */


u32 tgs_lzf_getuncompressedsize(const void *const in_data);

BOOL tgs_lzf_iscompressed(const void *const in_data);

unsigned int tgs_lzf_decompress(const void *const in_data,  unsigned int in_len, void *out_data,  unsigned int out_len);

#endif // SEED_LZF_H
