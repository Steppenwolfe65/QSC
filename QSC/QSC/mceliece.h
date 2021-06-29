/* The AGPL version 3 License (AGPLv3)
*
* Copyright (c) 2021 Digital Freedom Defence Inc.
* This file is part of the QSC Cryptographic library
*
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
*
*
* Implementation Details:
* An implementation of the McEliece asymmetric cipher
* Written by John G. Underhill
* Updated on January 20, 2020
* Contact: support@vtdev.com 
*/

/**
* \file mceliece.h
* \date May 10, 2019
*
* \brief <b>The McEliece api definitions</b> \n
* Contains the primary public api for the Niederreiter dual form of the McEliece asymmetric cipher implementation.
*
* \para <b>Example</b> \n
* \code
* // An example of key-pair creation, encryption, and decryption
* uint8_t ct[QSC_MCELIECE_CIPHERTEXT_SIZE];
* uint8_t pk[QSC_MCELIECE_PUBLICKEY_SIZE];
* uint8_t sk[QSC_MCELIECE_PRIVATEKEY_SIZE];
* uint8_t ssa[QSC_MCELIECE_SHAREDSECRET_SIZE];
* uint8_t ssb[QSC_MCELIECE_SHAREDSECRET_SIZE];
*
* // create the public and secret keys
* qsc_mceliece_generate_keypair(pk, sk);
*
* // output the cipher-text (ct), and the shared key
* qsc_mceliece_encapsulate(ssb, ct, pk);
*
* // decrypt the cipher-text, and output the shared key
* if (qsc_mceliece_decapsulate(ssa, ct, sk) == false)
* {
*     // decapsulation failed, do something..
* }
* \endcode
*
* \remarks
* Classic McEliece is a KEM designed for IND-CCA2 security at a very high security level, even against quantum computers. \n
* The KEM is built conservatively from a PKE designed for OW-CPA security, namely Niederreiter's dual version of McEliece's PKE using binary Goppa codes. \n
* Every level of the construction is designed so that future cryptographic auditors can be confident in the long-term security of post-quantum public-key encryption. \n
* Based on the NIST PQ, SUPERCOP C reference branch of McEliece; including base code, and comments. \n
* McEliece NIST PQ Round 2: <a href="https://classic.mceliece.org/nist/mceliece-20171129.pdf">McEliece</a> conservative code-based cryptography. \n
* Source code: <a href="https://bench.cr.yp.to/supercop.html">SUPERCOP</a> McEliece implementation. \n
* The authors: <a href="https://classic.mceliece.org/">McEliece website</a>. \n
* Authors: Daniel J. Bernstein, Tung Chou, Tanja Lange, and Peter Schwabe. \n
* Updated by John Underhill, June 28 2021.
*/

#ifndef QSC_MCELIECE_H
#define QSC_MCELIECE_H

#include "common.h"

#if defined(QSC_MCELIECE_S3N4608T96)

	/*!
	* \def QSC_MCELIECE_CIPHERTEXT_SIZE
	* \brief The byte size of the cipher-text array
	*/
#	define QSC_MCELIECE_CIPHERTEXT_SIZE 188

	/*!
	* \def QSC_MCELIECE_PRIVATEKEY_SIZE
	* \brief The byte size of the secret private-key array
	*/
#	define QSC_MCELIECE_PRIVATEKEY_SIZE 13608

	/*!
	* \def QSC_MCELIECE_PUBLICKEY_SIZE
	* \brief The byte size of the public-key array
	*/
#	define QSC_MCELIECE_PUBLICKEY_SIZE 524160

#elif defined(QSC_MCELIECE_S5N6688T128)

/*!
* \def QSC_MCELIECE_CIPHERTEXT_SIZE
* \brief The byte size of the cipher-text array
*/
#	define QSC_MCELIECE_CIPHERTEXT_SIZE 240

/*!
* \def QSC_MCELIECE_PRIVATEKEY_SIZE
* \brief The byte size of the secret private-key array
*/
#	define QSC_MCELIECE_PRIVATEKEY_SIZE 13932

/*!
* \def QSC_MCELIECE_PUBLICKEY_SIZE
* \brief The byte size of the public-key array
*/
#	define QSC_MCELIECE_PUBLICKEY_SIZE 1044992

#elif defined(QSC_MCELIECE_S5N6960T119)

/*!
* \def QSC_MCELIECE_CIPHERTEXT_SIZE
* \brief The byte size of the cipher-text array
*/
#	define QSC_MCELIECE_CIPHERTEXT_SIZE 226

/*!
* \def QSC_MCELIECE_PRIVATEKEY_SIZE
* \brief The byte size of the secret private-key array
*/
#	define QSC_MCELIECE_PRIVATEKEY_SIZE 13948

/*!
* \def QSC_MCELIECE_PUBLICKEY_SIZE
* \brief The byte size of the public-key array
*/
#	define QSC_MCELIECE_PUBLICKEY_SIZE 1047319

#elif defined(QSC_MCELIECE_S5N8192T128)

/*!
* \def QSC_MCELIECE_CIPHERTEXT_SIZE
* \brief The byte size of the cipher-text array
*/
#	define QSC_MCELIECE_CIPHERTEXT_SIZE 240 

/*!
* \def QSC_MCELIECE_PRIVATEKEY_SIZE
* \brief The byte size of the secret private-key array
*/
#	define QSC_MCELIECE_PRIVATEKEY_SIZE 14120

/*!
* \def QSC_MCELIECE_PUBLICKEY_SIZE
* \brief The byte size of the public-key array
*/
#	define QSC_MCELIECE_PUBLICKEY_SIZE 1357824

#else
#	error No McEliece implementation is defined, check common.h!
#endif

/*!
* \def QSC_MCELIECE_SHAREDSECRET_SIZE
* \brief The byte size of the shared secret-key array
*/
#define QSC_MCELIECE_SHAREDSECRET_SIZE 32

/*!
* \def QSC_MCELIECE_ALGNAME
* \brief The formal algorithm name
*/
#define QSC_MCELIECE_ALGNAME "MCELIECE"

/**
* \brief Decapsulates the shared secret for a given cipher-text using a private-key
*
* \param secret: Pointer to a shared secret key, an array of QSC_MCELIECE_SHAREDSECRET_SIZE constant size
* \param ciphertext: [const] Pointer to the cipher-text array of QSC_MCELIECE_CIPHERTEXT_SIZE constant size
* \param privatekey: [const] Pointer to the private-key array of QSC_MCELIECE_PRIVATEKEY_SIZE constant size
* \return Returns true for success
*/
QSC_EXPORT_API bool qsc_mceliece_decapsulate(uint8_t* secret, const uint8_t* ciphertext, const uint8_t* privatekey);

/**
* \brief Generates cipher-text and encapsulates a shared secret key using a public-key
*
* \param secret: Pointer to a shared secret, a uint8_t array of QSC_MCELIECE_SHAREDSECRET_SIZE constant size
* \param ciphertext: Pointer to the cipher-text array of QSC_MCELIECE_CIPHERTEXT_SIZE constant size
* \param publickey: [const] Pointer to the public-key array of QSC_MCELIECE_PUBLICKEY_SIZE constant size
* \param rng_generate: Pointer to the random generator
*/
QSC_EXPORT_API void qsc_mceliece_encapsulate(uint8_t* secret, uint8_t* ciphertext, const uint8_t* publickey, void (*rng_generate)(uint8_t*, size_t));

/**
* \brief Generates public and private key for the McEliece key encapsulation mechanism
*
* \param publickey: Pointer to the output public-key array of QSC_MCELIECE_PUBLICKEY_SIZE constant size
* \param privatekey: Pointer to output private-key array of QSC_MCELIECE_PRIVATEKEY_SIZE constant size
* \param rng_generate: Pointer to the random generator function
*/
QSC_EXPORT_API void qsc_mceliece_generate_keypair(uint8_t* publickey, uint8_t* privatekey, void (*rng_generate)(uint8_t*, size_t));

#endif
