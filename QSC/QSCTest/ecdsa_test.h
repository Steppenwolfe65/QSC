/**
* \file ecdsa_test.c
* \brief ECDSA test functions \n
* Contains the ECDSA implementation KAT and wellness test functions.
*
* \author John Underhill
* \date September 20, 2020
*/

#ifndef QSCTEST_ECDSA_TEST_H
#define QSCTEST_ECDSA_TEST_H

#include "common.h"

#ifdef _DEBUG
#	define QSCTEST_ECDSA_ITERATIONS 10
#else
#	define QSCTEST_ECDSA_ITERATIONS 100
#endif

/**
* \brief Test the ECDSA known answer test vectors
*/
bool qsctest_ecdsa_kat_test(void);

/**
* \brief Test the validity of a mutated secret key
* \return Returns true for test success
*/
bool qsctest_ecdsa_privatekey_integrity(void);

/**
* \brief Test the validity of a mutated public key
* \return Returns true for test success
*/
bool qsctest_ecdsa_publickey_integrity(void);

/**
* \brief Test the validity of a mutated signature
* \return Returns true for test success
*/
bool qsctest_ecdsa_signature_integrity(void);

/**
* \brief Stress test the key generation, encryption, and decryption functions in a looping test.
* \return Returns true for test success
*/
bool qsctest_ecdsa_stress_test(void);

/**
* \brief Run the ECDSA implementation stress and correctness tests tests
*/
void qsctest_ecdsa_run(void);

#endif
