/**
* \file kem.h
* \date February 16, 2018
*
* \brief <b>The NewHope KEM definitions</b> \n
* Contains the primary public api for the NewHope CCA-secure Key Encapsulation Mechanism implementation.
*
* \para <b>Example</b> \n
* \code
* // An example of key-pair creation, encryption, and decryption
* uint8_t pk[NEWHOPE_PUBLICKEY_SIZE];
* uint8_t sk[NEWHOPE_SECRETKEY_SIZE];
* uint8_t key_a[NEWHOPE_SECRETKEY_SIZE];
* uint8_t key_b[NEWHOPE_SECRETKEY_SIZE];
* uint8_t sendb[NEWHOPE_CIPHERTEXT_SIZE];
*
* // create the public and secret keys
* crypto_kem_keypair(pk, sk);
* // output the cipher-text (sendb), and bobs shared key
* crypto_kem_enc(sendb, key_b, pk);
* // decrypt the cipher-text, and output alices shared key
* if (crypto_kem_dec(key_a, sendb, sk) == false)
* {
*     // authentication failed, do something..
* }
* \endcode
*
* \remarks Based entirely on the C reference branch of the NIST Post Quantum Round 2 reference branch of NewHope; including base code, comments, and api. \n
* The NewHope: <a href="https://eprint.iacr.org/2015/1092">key exchange</a> from the ring learning with errors problem. \n
* A Simple, Provably <a href="http://eprint.iacr.org/2012/688.pdf">Secure Key Exchange</a> Scheme Based on the Learning with Errors Problem.
* Github source code <a href="https://github.com/tpoeppelmann/newhope">NewHope</a> repository.
*/

#ifndef NEWHOPE_KEM_H
#define NEWHOPE_KEM_H

#include "params.h"
#include <cstdbool>
#include <stdint.h>

/* bogus integral type warnings */
/*lint -e970 */
/*lint -e731 */

/**
* \brief Generates public and private key for the CCA-Secure NewHope key encapsulation mechanism
*
* \param pk Pointer to output public key (an already allocated array of NEWHOPE_PUBLICKEY_SIZE bytes)
* \param sk Pointer to output private key (an already allocated array of NEWHOPE_SECRETKEY_SIZE bytes)
* \return Returns true for success
*/
bool crypto_kem_keypair(uint8_t* pk, uint8_t* sk);

/**
* \brief Generates cipher text and shared secret for given public key
*
* \param ct Pointer to output cipher text (an already allocated array of NEWHOPE_CIPHERTEXT_SIZE bytes)
* \param ss Pointer to output shared secret (an already allocated array of NEWHOPE_NEWHOPE_SHAREDKEY_BYTES bytes)
* \param pk Pointer to input public key (an already allocated array of NEWHOPE_PUBLICKEY_SIZE bytes)
* \return Returns true for success
*/
bool crypto_kem_enc(uint8_t* ct, uint8_t* ss, const uint8_t* pk);

/**
* \brief Generates shared secret for given cipher text and private key
*
* \param ss pointer to output shared secret (an already allocated array of NEWHOPE_SECRET_BYTES bytes)
* \param ct pointer to input cipher text (an already allocated array of NEWHOPE_CIPHERTEXT_SIZE bytes)
* \param sk pointer to input private key (an already allocated array of NEWHOPE_SECRETKEY_SIZE bytes)
* \return Returns true for success
*/
bool crypto_kem_dec(uint8_t* ss, const uint8_t* ct, const uint8_t* sk);

#endif
