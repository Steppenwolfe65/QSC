#include "rcs_test.h"
#include "../QSC/intutils.h"
#include "../QSC/rcs.h"
#include "../QSC/csp.h"
#include "testutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool qsctest_rcs256_kat_test()
{
	uint8_t ad[20];
	uint8_t dec[32] = { 0 };
	uint8_t enc1[32 + QSC_RCS256_MAC_LENGTH] = { 0 };
	uint8_t enc2[32 + QSC_RCS256_MAC_LENGTH] = { 0 };
	uint8_t exp1[32 + QSC_RCS256_MAC_LENGTH] = { 0 };
	uint8_t exp2[32 + QSC_RCS256_MAC_LENGTH] = { 0 };
	uint8_t key[QSC_RCS256_KEY_SIZE] = { 0 };
	uint8_t msg[32] = { 0 };
	uint8_t nce[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t ncpy[QSC_RCS_NONCE_SIZE] = { 0 };
	bool status;
	qsc_rcs_state state;

	/* vectors from CEX */
	/* rcsc256k256 */
	hex_to_bin("7940917E9219A31248946F71647B15421535941574F84F79F6110C1F2F776D03EBC24989F5DC4F8598BF155E24944745E52B7DC27161CA3D9DB7951647F41DB8", exp1, sizeof(exp1));
	hex_to_bin("ABF3574126DAA563B423B0EEEE9970FD0C8F060F65CB00CDC05BB0DC047DB2ADA45F363A919EE677C9C1C7478A63E78E0C66AB17078AEC4E30C6B9063BB20B68", exp2, sizeof(exp2));
	hex_to_bin("000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F", key, sizeof(key));
	hex_to_bin("000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F", msg, sizeof(msg));
	hex_to_bin("FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0DFDEDDDCDBDAD9D8D7D6D5D4D3D2D1D0", nce, sizeof(nce));

	memset(ad, 0x01, sizeof(ad));

	/* copy the nonce */
	memcpy(ncpy, nce, QSC_RCS_NONCE_SIZE);

	/* initialize the key parameters struct, info is optional */
	qsc_rcs_keyparams kp = { key, QSC_RCS256_KEY_SIZE, nce };

	status = true;

	/* initialize the state and create the round-keys */
	qsc_rcs_initialize(&state, &kp, true, RCS256);

	/* set associted data */
	qsc_rcs_set_associated(&state, ad, sizeof(ad));

	/* test encryption */
	qsc_rcs_transform(&state, enc1, msg, sizeof(msg));

	if (qsc_intutils_are_equal8(enc1, exp1, sizeof(exp1)) == false)
	{
		status = false;
	}

	/* test encryption and mac chaining */
	qsc_rcs_transform(&state, enc2, msg, sizeof(msg));

	if (qsc_intutils_are_equal8(enc2, exp2, sizeof(exp2)) == false)
	{
		status = false;
	}

	/* reset the nonce */
	kp.nonce = ncpy;

	/* initialize the state */
	qsc_rcs_initialize(&state, &kp, false, RCS256);

	/* set associted data */
	qsc_rcs_set_associated(&state, ad, sizeof(ad));

	/* test decryption */
	if (qsc_rcs_transform(&state, dec, enc1, sizeof(dec)) == false)
	{
		status = false;
	}

	if (qsc_intutils_are_equal8(dec, msg, sizeof(dec)) == false)
	{
		status = false;
	}

	/* erase the round-key array and reset the state */
	qsc_rcs_dispose(&state);

	return status;
}

bool qsctest_rcs512_kat_test()
{
	uint8_t ad[20];
	uint8_t dec[64] = { 0 };
	uint8_t enc1[64 + QSC_RCS512_MAC_LENGTH] = { 0 };
	uint8_t enc2[64 + QSC_RCS512_MAC_LENGTH] = { 0 };
	uint8_t exp1[64 + QSC_RCS512_MAC_LENGTH] = { 0 };
	uint8_t exp2[64 + QSC_RCS512_MAC_LENGTH] = { 0 };
	uint8_t key[QSC_RCS512_KEY_SIZE] = { 0 };
	uint8_t msg[64] = { 0 };
	uint8_t nce[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t ncpy[QSC_RCS_NONCE_SIZE] = { 0 };
	bool status;
	qsc_rcs_state state;

	/* vectors from CEX */
	/* rcsc512k512 */
	hex_to_bin("21E97A126E35BE731EF204E48248A2EEB01B692992F73786602F21031FBFB7C8A1CF250F2EC948D5985B92667349B72EFA751048AF0B919AE9E16F177F5C97F2"
		"CA4C3AF6D1BF2FA8694483FC1F429A7ECC7C9B5F6FCB8504265DE0385B4D012A8E11035F172C98090549B38FA4B0525ED747EE9670B240C1EC3C2E03070E3E11", exp1, sizeof(exp1));
	hex_to_bin("388270BF8DF03483BB287FFA527D81403F0362210FD525657C8541250DFFE3BAD1285FAB37A6821DA524F3F7FF7EFCB39C5B59E3897B177E45D6AA7F4BB5BE77"
		"F8A3206DD873100E1CC7AD430EDB01A4D464EAE2DB23BF310E53C65A1AABDC92D0F1F64D9A427447296475CA9429F715967863ED209715453FA48030E43C7C35", exp2, sizeof(exp2));
	hex_to_bin("000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F", key, sizeof(key));
	hex_to_bin("000102030405060708090A0B0C0D0E0F000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F202122232425262728292A2B2C2D2E2F", msg, sizeof(msg));
	hex_to_bin("FFFEFDFCFBFAF9F8F7F6F5F4F3F2F1F0DFDEDDDCDBDAD9D8D7D6D5D4D3D2D1D0", nce, sizeof(nce));

	memset(ad, 0x01, sizeof(ad));

	/* copy the nonce */
	memcpy(ncpy, nce, sizeof(nce));

	/* initialize the key parameters struct, info is optional */
	qsc_rcs_keyparams kp = { key, QSC_RCS512_KEY_SIZE, nce };

	status = true;

	/* initialize the state and create the round-keys */
	qsc_rcs_initialize(&state, &kp, true, RCS512);

	/* set associted data */
	qsc_rcs_set_associated(&state, ad, sizeof(ad));

	/* test encryption */
	qsc_rcs_transform(&state, enc1, msg, sizeof(msg));

	if (qsc_intutils_are_equal8(enc1, exp1, sizeof(exp1)) == false)
	{
		status = false;
	}

	/* test encryption and mac chaining */
	qsc_rcs_transform(&state, enc2, msg, sizeof(msg));

	if (qsc_intutils_are_equal8(enc2, exp2, sizeof(exp2)) == false)
	{
		status = false;
	}

	/* reset the nonce */
	kp.nonce = ncpy;

	/* initialize the state */
	qsc_rcs_initialize(&state, &kp, false, RCS512);

	/* set associted data */
	qsc_rcs_set_associated(&state, ad, sizeof(ad));

	/* test decryption */
	if (qsc_rcs_transform(&state, dec, enc1, sizeof(dec)) == false)
	{
		status = false;
	}

	if (qsc_intutils_are_equal8(dec, msg, sizeof(dec)) == false)
	{
		status = false;
	}

	/* erase the round-key array and reset the state */
	qsc_rcs_dispose(&state);

	return status;
}

bool qsctest_rcs256_stress_test()
{
	uint8_t aad[20] = { 0 };
	uint8_t* dec;
	uint8_t* enc;
	uint8_t key[QSC_RCS256_KEY_SIZE] = { 0 };
	uint8_t* msg;
	uint8_t ncopy[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t nonce[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t pmcnt[sizeof(uint16_t)] = { 0 };
	uint16_t mlen;
	size_t tctr;
	bool status;
	qsc_rcs_state state;

	tctr = 0;
	status = true;

	while (tctr < QSCTEST_RCS_TEST_CYCLES)
	{
		mlen = 0;

		while (mlen == 0)
		{
			/* unlikely but this could return zero */
			qsc_csp_generate(pmcnt, sizeof(pmcnt));
			memcpy(&mlen, pmcnt, sizeof(uint16_t));
		}

		dec = (uint8_t*)malloc(mlen);
		enc = (uint8_t*)malloc(mlen + QSC_RCS256_MAC_LENGTH);
		msg = (uint8_t*)malloc(mlen);

		if (dec != NULL && enc != NULL && msg != NULL)
		{
			qsc_intutils_clear8(dec, mlen);
			qsc_intutils_clear8(enc, mlen + QSC_RCS256_MAC_LENGTH);
			qsc_intutils_clear8(msg, mlen);
			memcpy(nonce, ncopy, QSC_RCS_NONCE_SIZE);

			/* use a random sized message 1-65535 */
			qsc_csp_generate(msg, mlen);

			qsc_rcs_keyparams kp1 = { key, sizeof(key), nonce, NULL, 0 };

			/* encrypt the message */
			qsc_rcs_initialize(&state, &kp1, true, RCS256);
			qsc_rcs_set_associated(&state, aad, sizeof(aad));

			if (qsc_rcs_transform(&state, enc, msg, mlen) == false)
			{
				status = false;
			}

			/* reset the nonce */
			memcpy(kp1.nonce, ncopy, QSC_RCS_NONCE_SIZE);

			/* decrypt the message */
			qsc_rcs_initialize(&state, &kp1, false, RCS256);
			qsc_rcs_set_associated(&state, aad, sizeof(aad));

			if (qsc_rcs_transform(&state, dec, enc, mlen) == false)
			{
				status = false;
			}

			/* compare decryption output to message */
			if (qsc_intutils_are_equal8(dec, msg, mlen) == false)
			{
				status = false;
			}

			free(dec);
			free(enc);
			free(msg);

			++tctr;
		}
		else
		{
			status = false;
			break;
		}
	}

	return status;
}

bool qsctest_rcs512_stress_test()
{
	uint8_t aad[20] = { 0 };
	uint8_t* dec;
	uint8_t* enc;
	uint8_t key[QSC_RCS512_KEY_SIZE] = { 0 };
	uint8_t* msg;
	uint8_t ncopy[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t nonce[QSC_RCS_NONCE_SIZE] = { 0 };
	uint8_t pmcnt[sizeof(uint16_t)] = { 0 };
	uint16_t mlen;
	size_t tctr;
	bool status;
	qsc_rcs_state state;

	tctr = 0;
	status = true;

	while (tctr < QSCTEST_RCS_TEST_CYCLES)
	{
		mlen = 0;

		while (mlen == 0)
		{
			/* unlikely but this could return zero */
			qsc_csp_generate(pmcnt, sizeof(pmcnt));
			memcpy(&mlen, pmcnt, sizeof(uint16_t));
		}

		dec = (uint8_t*)malloc(mlen);
		enc = (uint8_t*)malloc(mlen + QSC_RCS512_MAC_LENGTH);
		msg = (uint8_t*)malloc(mlen);

		if (dec != NULL && enc != NULL && msg != NULL)
		{
			qsc_intutils_clear8(dec, mlen);
			qsc_intutils_clear8(enc, mlen + QSC_RCS512_MAC_LENGTH);
			qsc_intutils_clear8(msg, mlen);
			memcpy(nonce, ncopy, QSC_RCS_NONCE_SIZE);

			/* use a random sized message 1-65535 */
			qsc_csp_generate(msg, mlen);

			qsc_rcs_keyparams kp1 = { key, sizeof(key), nonce, NULL, 0 };

			/* encrypt the message */
			qsc_rcs_initialize(&state, &kp1, true, RCS512);
			qsc_rcs_set_associated(&state, aad, sizeof(aad));

			if (qsc_rcs_transform(&state, enc, msg, mlen) == false)
			{
				status = false;
			}

			/* reset the nonce */
			memcpy(kp1.nonce, ncopy, QSC_RCS_NONCE_SIZE);

			/* decrypt the message */
			qsc_rcs_initialize(&state, &kp1, false, RCS512);
			qsc_rcs_set_associated(&state, aad, sizeof(aad));

			if (qsc_rcs_transform(&state, dec, enc, mlen) == false)
			{
				status = false;
			}

			/* compare decryption output to message */
			if (qsc_intutils_are_equal8(dec, msg, mlen) == false)
			{
				status = false;
			}

			free(dec);
			free(enc);
			free(msg);

			++tctr;
		}
		else
		{
			status = false;
			break;
		}
	}

	return status;
}

void qsctest_rcs_run()
{
	if (qsctest_rcs256_kat_test() == true)
	{
		printf_s("Success! Passed the RCS-256 known answer tests. \n");
	}
	else
	{
		printf_s("Failure! Failed the RCS-256 known answer tests. \n");
	}

	if (qsctest_rcs512_kat_test() == true)
	{
		printf_s("Success! Passed the RCS-512 known answer tests. \n");
	}
	else
	{
		printf_s("Failure! Failed the RCS-512 known answer tests. \n");
	}

	if (qsctest_rcs256_stress_test() == true)
	{
		printf_s("Success! Passed the RCS-256 stress test. \n");
	}
	else
	{
		printf_s("Failure! Failed the RCS-256 stress test. \n");
	}

	if (qsctest_rcs512_stress_test() == true)
	{
		printf_s("Success! Passed the RCS-512 stress test. \n");
	}
	else
	{
		printf_s("Failure! Failed the RCS-512 stress test. \n");
	}
}