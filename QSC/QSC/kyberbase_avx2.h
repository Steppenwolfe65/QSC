/*
* Copyright (c) 2023 Quantum Secure Cryptographic Solutions QSCS Corp. (QSCS.ca).
* This file is part of the QSC Cryptographic library.
* The QSC library was written as a prototyping library for post-quantum primitives,
* in the hopes that it would be useful for educational purposes only.
* Any use of the QSC library in a commercial context, or reproduction of original material
* contained in this library is strictly forbidden unless prior written consent is obtained
* from the QSCS Corporation.
*
* The AGPL version 3 License (AGPLv3)
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QSC_KYBERBASE_AVX2_H
#define QSC_KYBERBASE_AVX2_H

#include "common.h"

/* \cond DOXYGEN_IGNORE */

/**
* \file kyberbase_avx2.h
* \brief The Kyber AVX2	functions
*/

#if defined(QSC_SYSTEM_HAS_AVX2)

 /*!
 \def QSC_KYBER_K
 * Read Only: The k level
 */
#if defined(QSC_KYBER_S3K2400)
#	define QSC_KYBER_K 3
#elif defined(QSC_KYBER_S5K3168)
#	define QSC_KYBER_K 4
#elif defined(QSC_KYBER_S6K3936)
#	define QSC_KYBER_K 5
#else
#	error "The Kyber parameter set is invalid!"
#endif

 /*!
 \def QSC_KYBER_N
 * Read Only: The polynomial dimension N
 */
#define QSC_KYBER_N 256

 /*!
 \def QSC_KYBER_Q
 * Read Only: The modulus prime factor Q
 */
#define QSC_KYBER_Q 3329

 /*!
 \def QSC_KYBER_ETA
 * Read Only: The binomial distribution factor
 */
#define QSC_KYBER_ETA 2

 /*!
 \def QSC_KYBER_MSGBYTES
 * Read Only: The size in bytes of the shared secret
 */
#define QSC_KYBER_MSGBYTES 32

 /*!
 \def QSC_KYBER_SYMBYTES
 * Read Only: The size in bytes of hashes, and seeds
 */
#define QSC_KYBER_SYMBYTES 32

 /*!
 \def QSC_KYBER_POLYBYTES
 * Read Only: The secret key base multiplier
 */
#define QSC_KYBER_POLYBYTES 384

 /*!
 \def QSC_KYBER_POLYVEC_BYTES
 * Read Only: The base size of the compressed public key polynomial
 */
#if (QSC_KYBER_K == 3)
#	define QSC_KYBER_POLYVECBASE_BYTES 320
#elif (QSC_KYBER_K == 4 || QSC_KYBER_K == 5)
#	define QSC_KYBER_POLYVECBASE_BYTES 352
#endif

 /*!
 \def QSC_KYBER_POLYCOMPRESSED_BYTES
 * Read Only: The cipher-text compressed byte size
 */
#if (QSC_KYBER_K == 3)
#	define QSC_KYBER_POLYCOMPRESSED_BYTES 128
#elif (QSC_KYBER_K == 4 || QSC_KYBER_K == 5)
#	define QSC_KYBER_POLYCOMPRESSED_BYTES 160
#endif

 /*!
 \def QSC_KYBER_POLYVEC_COMPRESSED_BYTES
 * Read Only: The base size of the public key
 */
#define QSC_KYBER_POLYVEC_COMPRESSED_BYTES (QSC_KYBER_K * QSC_KYBER_POLYVECBASE_BYTES)

 /*!
 \def QSC_KYBER_POLYVEC_BYTES
 * Read Only: The base size of the secret key
 */
#define QSC_KYBER_POLYVEC_BYTES (QSC_KYBER_K * QSC_KYBER_POLYBYTES)

 /*!
 \def QSC_KYBER_INDCPA_PUBLICKEY_BYTES
 * Read Only: The base INDCPA formatted public key size in bytes
 */
#define QSC_KYBER_INDCPA_PUBLICKEY_BYTES (QSC_KYBER_POLYVEC_BYTES + QSC_KYBER_SYMBYTES)

 /*!
 \def QSC_KYBER_INDCPA_SECRETKEY_BYTES
 * Read Only: The base INDCPA formatted private key size in bytes
 */
#define QSC_KYBER_INDCPA_SECRETKEY_BYTES (QSC_KYBER_POLYVEC_BYTES)

 /*!
 \def QSC_KYBER_INDCPA_BYTES
 * Read Only: The size of the INDCPA formatted output cipher-text
 */
#define QSC_KYBER_INDCPA_BYTES (QSC_KYBER_POLYVEC_COMPRESSED_BYTES + QSC_KYBER_POLYCOMPRESSED_BYTES)

 /*!
 \def QSC_KYBER_PUBLICKEY_BYTES
 * Read Only: The byte size of the public-key array
 */
#define QSC_KYBER_PUBLICKEY_BYTES  (QSC_KYBER_INDCPA_PUBLICKEY_BYTES)

 /*!
 \def QSC_KYBER_SECRETKEY_BYTES
 * Read Only: The byte size of the secret private-key array
 */
#define QSC_KYBER_SECRETKEY_BYTES  (QSC_KYBER_INDCPA_SECRETKEY_BYTES + QSC_KYBER_INDCPA_PUBLICKEY_BYTES + 2 * QSC_KYBER_SYMBYTES)

 /*!
 \def QSC_KYBER_CIPHERTEXT_BYTES
 * Read Only: The byte size of the cipher-text array
 */
#define QSC_KYBER_CIPHERTEXT_BYTES (QSC_KYBER_INDCPA_BYTES)

#if defined(QSC_GCC_ASM_ENABLED)
	void ntt_avx(__m256i *r, const __m256i *qdata);
	void invntt_avx(__m256i *r, const __m256i *qdata);
	void nttpack_avx(__m256i *r, const __m256i *qdata);
	void nttunpack_avx(__m256i *r, const __m256i *qdata);
	void basemul_avx(__m256i *r, const __m256i *a, const __m256i *b, const __m256i *qdata);
	void ntttobytes_avx(uint8_t *r, const __m256i *a, const __m256i *qdata);
	void nttfrombytes_avx(__m256i *r, const uint8_t *a, const __m256i *qdata);
	int16_t reduce_avx(int16_t* r, const int16_t* qdata);
	int16_t csubq_avx(int16_t* r, const int16_t* qdata);
	int16_t tomont_avx(int16_t* r, const int16_t* qdata);

	/*void ntt_avx(int16_t r[QSC_KYBER_N]);
	void invntt_avx(int16_t r[QSC_KYBER_N]);
	extern const int16_t qdata[];
	void nttpack_avx(int16_t* r, const int16_t* qdata);;
	void nttunpack_avx(int16_t* r, const int16_t* qdata);
	void ntttobytes_avx(uint8_t* r, const int16_t* a, const int16_t* qdata);
	void nttfrombytes_avx(int16_t* r, const uint8_t* a, const int16_t* qdata);
	int16_t reduce_avx(int16_t* r, const int16_t* qdata);
	int16_t csubq_avx(int16_t* r, const int16_t* qdata);
	int16_t tomont_avx(int16_t* r, const int16_t* qdata);*/
#endif

/* kem.h */

/**
* \brief Generates shared secret for given cipher text and private key
*
* \param ss: Pointer to output shared secret (an already allocated array of KYBER_SECRET_BYTES bytes)
* \param ct: [const] Pointer to input cipher text (an already allocated array of KYBER_CIPHERTEXT_SIZE bytes)
* \param sk: [const] Pointer to input private key (an already allocated array of KYBER_SECRETKEY_SIZE bytes)
* \return Returns true for success
*/
bool qsc_kyber_avx2_decapsulate(uint8_t ss[QSC_KYBER_MSGBYTES], const uint8_t ct[QSC_KYBER_CIPHERTEXT_BYTES],
	const uint8_t sk[QSC_KYBER_SECRETKEY_BYTES]);

/**
* \brief Generates cipher text and shared secret for given public key
*
* \param ct: Pointer to output cipher text (an already allocated array of KYBER_CIPHERTEXT_SIZE bytes)
* \param ss: Pointer to output shared secret (an already allocated array of KYBER_BYTES bytes)
* \param pk: [const] Pointer to input public key (an already allocated array of KYBER_PUBLICKEY_SIZE bytes)
* \param rng_generate: Pointer to the random generator function
*/
void qsc_kyber_avx2_encapsulate(uint8_t ct[QSC_KYBER_CIPHERTEXT_BYTES], uint8_t ss[QSC_KYBER_MSGBYTES],
	const uint8_t pk[QSC_KYBER_PUBLICKEY_BYTES], bool (*rng_generate)(uint8_t*, size_t));

/**
* \brief Generates public and private key for the CCA-Secure Kyber key encapsulation mechanism
*
* \param pk: Pointer to output public key (an already allocated array of KYBER_PUBLICKEY_SIZE bytes)
* \param sk: Pointer to output private key (an already allocated array of KYBER_SECRETKEY_SIZE bytes)
* \param rng_generate: Pointer to the random generator function
*/
void qsc_kyber_avx2_generate_keypair(uint8_t pk[QSC_KYBER_PUBLICKEY_BYTES], uint8_t sk[QSC_KYBER_SECRETKEY_BYTES],
	bool (*rng_generate)(uint8_t*, size_t));

#endif

/* \endcond DOXYGEN_IGNORE */

#endif
