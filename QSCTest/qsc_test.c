/* The GPL version 3 License (GPLv3)
*
* Copyright (c) 2020 Digital Freedom Defence Inc. Inc.
* This file is part of the QSC Cryptographic library
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Written by John G. Underhill
* Contact: develop@vtdev.com */

/* 
  Version 1.0
- system wide renaming	- done
- add mceliece			- done
- add kyber				- done
- add sphincs+			- done
- add dilithium			- done
- add drbgs				- done
- add providers			- done
- add prng				- done
- add rcs new			- done
- add csx-512			- done
- documentation			- done
- error handling		- done
- integrate memutils	- done
- review				- done
  Version 1.1
- add ecdh				- done
- add ecdsa				- done
- prototype tls 1.3		-
- publish 1.1 eta October 2020
*/

/* Test Format

Signature Schemes:
qsctest_xxx_kat_test
qsctest_xxx_operations_test
qsctest_xxx_privatekey_integrity
qsctest_xxx_publickey_integrity
qsctest_xxx_signature_integrity
qsctest_xxx_stress_test

Asymmetric Ciphers:
qsctest_xxx_ciphertext_integrity
qsctest_xxx_kat_test
qsctest_xxx_operations_test
qsctest_xxx_privatekey_integrity
qsctest_xxx_publickey_integrity
qsctest_xxx_stress_test
*/

#include "../QSC/common.h"
#include "../QSC/cpuid.h"
#include "../QSC/secrand.h"
#include "chacha_test.h"
#include "common.h"
#include "csx_test.h"
#include "dilithium_test.h"
#include "ecdh_test.h"
#include "ecdsa_test.h"
#include "kyber_test.h"
#include "mceliece_test.h"
#include "poly1305_test.h"
#include "rcs_test.h"
#include "secrand_test.h"
#include "sha2_test.h"
#include "sha3_test.h"
#include "sphincsplus_test.h"
#include "testutils.h"

void print_title()
{
	print_safe("*************************************************** \n");
	print_safe("* QSC: Quantum Secure Cryptographic library in C  * \n");
	print_safe("*                                                 * \n");
	print_safe("* Release:   v1.0.0.0 (A1)                        * \n");
	print_safe("* License:   GPLv3                                * \n");
	print_safe("* Date:      September 24, 2020                   * \n");
	print_safe("* Contact:   develop@vtdev.com                    * \n");
	print_safe("*************************************************** \n");
	print_safe("\n");
}

/**
* \wait for input
*/
void qsctest_wait()
{
	wint_t ret;

	ret = getwchar();
}

int32_t main()
{
	bool valid;
	qsc_cpu_features features;

	valid = qsc_runtime_features(&features);

	if (valid == false)
	{
		print_safe("The CPU type was not recognized on this system! \n");
		print_safe("Some features may be disabled. \n\n");
	}

	print_title();

	if (features.has_aesni == true)
	{
		print_safe("AES-NI is available on this system. \n");
		print_safe("The QSC_SYSTEM_AESNI_ENABLED flag has been detected, AES-NI intrinsics are enabled. \n\n");
	}
	else
	{
		print_safe("AES-NI was not detected on this system. \n\n");
	}

	print_safe("*** Test the ChaCha stream cipher with known answer tests *** \n");
	qsctest_chacha_run();
	print_safe("\n");

	print_safe("*** Test the CSX-512 stream cipher using stress tests and KAT vectors *** \n");
	qsctest_csx_run();
	print_safe("\n");

	print_safe("*** Test the RCS stream cipher using stress tests and KAT vectors *** \n");
	qsctest_rcs_run();
	print_safe("\n");

	print_safe("*** Test the Poly1305 MAC generator with known answer tests *** \n");
	qsctest_poly1305_run();
	print_safe("\n");

	print_safe("*** Test HKDF, HMAC, and SHA2 implementations using the official KAT vectors *** \n");
	qsctest_sha2_run();
	print_safe("\n");

	print_safe("*** Test SHAKE, cSHAKE, KMAC, and SHA3 implementations using the official KAT vectors *** \n");
	qsctest_sha3_run();
	print_safe("\n");

	print_safe("*** Test the Secure Random provider and entropy provider implementations *** \n");
	qsctest_secrand_run();
	print_safe("\n");

	print_safe("*** Test the McEliece implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_mceliece_run();
	print_safe("\n");

	print_safe("*** Test the Kyber implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_kyber_run();
	print_safe("\n");

	print_safe("*** Test the ECDH implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_ecdh_run();
	print_safe("\n");

	print_safe("*** Test the Dilithium implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_dilithium_run();
	print_safe("\n");

	print_safe("*** Test the SphincsPlus implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_sphincsplus_run();
	print_safe("\n");

	print_safe("*** Test the ECDSA implementation using stress, validity checks, and known answer tests *** \n");
	qsctest_ecdsa_run();
	print_safe("\n");

	print_safe("Completed! Press any key to close..");
	qsctest_wait();

    return 0;
}
