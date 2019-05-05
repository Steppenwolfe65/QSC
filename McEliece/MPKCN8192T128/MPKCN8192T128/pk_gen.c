/*
  This file is for public-key generation
*/

#include "pk_gen.h"
#include "params.h"
#include "benes.h"
#include "root.h"
#include "util.h"
#include <string.h>
#ifndef MQC_COMPILER_GCC
#include <stdlib.h>
#endif

/* input: secret key sk */
/* output: public key pk */
int32_t pk_gen(uint8_t* pk, const uint8_t* sk)
{
	gf g[SYS_T + 1];
	gf L[SYS_N];
	gf inv[SYS_N];
	int32_t ret;
	size_t c;
	size_t i;
	size_t j;
	size_t k;
	size_t row;
	uint8_t b;
	uint8_t mask;

	ret = 1;

#ifdef MQC_COMPILER_GCC
	uint8_t mat[GFBITS * SYS_T][SYS_N / 8];
#else
	uint8_t** mat = malloc(GFBITS * SYS_T * sizeof(uint8_t*));

	for (i = 0; i < GFBITS * SYS_T; ++i)
	{
		mat[i] = malloc(SYS_N / 8);
		memset(mat[i], 0, SYS_N / 8);
	}
#endif

	if (mat != NULL)
	{
		g[SYS_T] = 1;

		for (i = 0; i < SYS_T; i++)
		{
			g[i] = load2(sk);
			g[i] &= GFMASK;
			sk += 2;
		}

		support_gen(L, sk);
		root(inv, g, L);

		for (i = 0; i < SYS_N; i++)
		{
			inv[i] = gf_inv(inv[i]);
		}

		for (i = 0; i < PK_NROWS; i++)
		{
			for (j = 0; j < SYS_N / 8; j++)
			{
				mat[i][j] = 0;
			}
		}

		for (i = 0; i < SYS_T; i++)
		{
			for (j = 0; j < SYS_N; j += 8)
			{
				for (k = 0; k < GFBITS; k++)
				{
					b = (inv[j + 7] >> k) & 1; b <<= 1;
					b |= (inv[j + 6] >> k) & 1; b <<= 1;
					b |= (inv[j + 5] >> k) & 1; b <<= 1;
					b |= (inv[j + 4] >> k) & 1; b <<= 1;
					b |= (inv[j + 3] >> k) & 1; b <<= 1;
					b |= (inv[j + 2] >> k) & 1; b <<= 1;
					b |= (inv[j + 1] >> k) & 1; b <<= 1;
					b |= (inv[j + 0] >> k) & 1;

					mat[i * GFBITS + k][j / 8] = b;
				}
			}

			for (j = 0; j < SYS_N; j++)
			{
				inv[j] = gf_mul(inv[j], L[j]);
			}
		}

		for (i = 0; i < (GFBITS * SYS_T + 7) / 8; i++)
		{
			for (j = 0; j < 8; j++)
			{
				row = i * 8 + j;

				if (row >= GFBITS * SYS_T)
				{
					break;
				}

				for (k = row + 1; k < GFBITS * SYS_T; k++)
				{
					mask = mat[row][i] ^ mat[k][i];
					mask >>= j;
					mask &= 1;
					mask = ~mask + 1;

					for (c = 0; c < SYS_N / 8; c++)
					{
						mat[row][c] ^= mat[k][c] & mask;
					}
				}

				// return if not systematic
				if (((mat[row][i] >> j) & 1) == 0)
				{
					return -1;
				}

				for (k = 0; k < GFBITS * SYS_T; k++)
				{
					if (k != row)
					{
						mask = mat[k][i] >> j;
						mask &= 1;
						mask = ~mask + 1;

						for (c = 0; c < SYS_N / 8; c++)
						{
							mat[k][c] ^= mat[row][c] & mask;
						}
					}
				}
			}
		}

		for (i = 0; i < PK_NROWS; i++)
		{
			memcpy(pk + i * PK_ROW_BYTES, mat[i] + PK_NROWS / 8, PK_ROW_BYTES);
		}

#ifndef MQC_COMPILER_GCC
		if (mat != NULL)
		{
			for (i = 0; i < GFBITS * SYS_T; ++i)
			{
				free(mat[i]);
			}

			free(mat);
		}
#endif

		ret = 0;
	}

	return ret;
}

