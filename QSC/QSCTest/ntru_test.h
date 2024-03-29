/**
* \file ntru_test.c
* \brief NTRU test functions \n
* Contains the NTRU implementation KAT and wellness test functions.
*
* \author John Underhill
* \date July 1, 2021
* \updated July 1, 2021
*/

#ifndef QSCTEST_NTRU_TEST_H
#define QSCTEST_NTRU_TEST_H

#include "common.h"

/**
* \brief Test the validity of a mutated cipher-text
* \return Returns true for test success
*/
bool qsctest_ntru_ciphertext_integrity(void);

/**
* \brief Test the public and private keys, cipher-text and shared key
* for correctness against the NIST PQC Round 3 vectors
* \return Returns true for test success
*/
bool qsctest_ntru_kat_test(void);

/**
* \brief Stress test the key generation, encryption, and decryption functions
* \return Returns true for test success
*/
bool qsctest_ntru_operations_test(void);

/**
* \brief Test the validity of an altered secret-key
* \return Returns true for test success
*/
bool qsctest_ntru_privatekey_integrity(void);

/**
* \brief Test the validity of an altered public key
* \return Returns true for test success
*/
bool qsctest_ntru_publickey_integrity(void);

/**
* \brief Run the NTRU implementation stress and correctness tests tests
*/
void qsctest_ntru_run(void);

#endif
