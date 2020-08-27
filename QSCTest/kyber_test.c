#include "kyber_test.h"
#include "nistrng.h"
#include "testutils.h"
#include "../QSC/csp.h"
#include "../QSC/kyber.h"
#include "../QSC/intutils.h"

#define QSCTEST_KYBER_NTESTS 100

/**
* \brief Stress test the key generation, encryption, and decryption functions in a 100 round loop.
* \return Returns one (KYBER_STATUS_SUCCESS) for test success
*/
bool qsctest_kyber_test_operations()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE];
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE];
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE];
	size_t i;
	bool state;

	state = true;

	for (i = 0; i < QSCTEST_KYBER_NTESTS; i++)
	{
		/* alice generates a public key */
		qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

		/* bob derives a secret key and creates a response */
		qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

		/* alice uses Bobs response to get her secret key */
		if (qsc_kyber_decapsulate(ssk1, ct, sk) != true)
		{
			state = false;
			break;
		}

		if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) != true)
		{
			state = false;
			break;
		}
	}

	return state;
}

/**
* \brief Test the validity of a mutated secret key in a 100 round loop.
* \return Returns one (KYBER_STATUS_SUCCESS) for test success
*/
bool qsctest_kyber_test_privatekey()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE];
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE];
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE];
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE];
	size_t i;
	bool state;

	state = true;

	for (i = 0; i < QSCTEST_KYBER_NTESTS; i++)
	{
		/* alice generates a public key */
		qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

		/* bob derives a secret key and creates a response */
		qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

		/* replace secret key with random values */
		if (qsc_csp_generate(sk, QSC_KYBER_PRIVATEKEY_SIZE) != true)
		{
			state = false;
			break;
		}

		/* invalid secret key, should fail */
		if (qsc_kyber_decapsulate(ssk1, ct, sk) == true)
		{
			state = false;
			break;
		}

		/* fail if equal */
		if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) == true)
		{
			state = false;
			break;
		}
	}

	return state;
}

/**
* \brief Test the validity of a mutated cipher-text in a 100 round loop.
* \return Returns one (KYBER_STATUS_SUCCESS) for test success
*/
bool qsctest_kyber_test_ciphertext()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE];
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE];
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE];
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t pbuf[1];
	size_t i;
	size_t pos;
	bool state;

	state = true;

	for (i = 0; i < QSCTEST_KYBER_NTESTS; i++)
	{
		/* get a random offset */
		if (qsc_csp_generate(pbuf, 1) != true)
		{
			state = false;
			break;
		}

		pos = (size_t)pbuf[0];

		/* alice generates a public key */
		qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

		/* bob derives a secret key and creates a response */
		qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

		/* change some byte in the ciphertext (i.e., encapsulated key) */
		ct[pos % QSC_KYBER_CIPHERTEXT_SIZE] ^= 23U;

		/* invalid ciphertext, auth should fail */
		if (qsc_kyber_decapsulate(ssk1, ct, sk) == true)
		{
			state = false;
			break;
		}

		/* fail if equal */
		if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) == true)
		{
			state = false;
			break;
		}
	}

	return state;
}

/**
* \brief Test the public and private keys, ciphertext and shared key
* for correctness against the NIST PQ Round 2 vectors
* \return Returns true for test success
*/
bool qsctest_kyber_kats()
{
	uint8_t ct[QSC_KYBER_CIPHERTEXT_SIZE];
	uint8_t ct2[QSC_KYBER_CIPHERTEXT_SIZE];
	uint8_t pk[QSC_KYBER_PUBLICKEY_SIZE];
	uint8_t pk2[QSC_KYBER_PUBLICKEY_SIZE];
	uint8_t seed[QSCTEST_NIST_RNG_SEED_SIZE];
	uint8_t sk[QSC_KYBER_PRIVATEKEY_SIZE];
	uint8_t sk2[QSC_KYBER_PRIVATEKEY_SIZE];
	uint8_t ssk1[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t ssk2[QSC_KYBER_SHAREDSECRET_SIZE];
	uint8_t ssk3[QSC_KYBER_SHAREDSECRET_SIZE];
	bool ret;

	/* NIST PQ Round 2 Kyber first known answer test vector set */
	hex_to_bin("061550234D158C5EC95595FE04EF7A25767F2E24CC2BC479D09D86DC9ABCFDE7056A8C266F9EF97ED08541DBD2E1FFA1", seed, sizeof(seed));

#if defined(QSC_KYBER_S1Q3329N256)
	hex_to_bin("7605192156310FD02891F471CE8E2AB934F2CAC69108936A85AADB23C27611459656DD601FCF2D5DB7B3C9ABC15835F2874A171598B1CB4A6901089FC63399E2"
		"C6EEE5671CAAA0597FC9D92ED40012D3201602310FFDC1A8ED534521F7A20F949331AD0D452556796869C38966AD88D50FE4F5DB32A48EF1350157BA59222996"
		"D4AE31079E5C988F367D64EE9F11A0051F1F79E12528D594769C7E69CF89A54CC5D3E668FC376B5BC870542F73F6F3AF71A575386A5B98A2D262E510BFC52368"
		"25DFB90989656EB1815209BB161F1CDEA9000B445C019CED8864BCD4B996DA8ADCEFC3EA1FFCDCE9323769B67B01BFB02D87F3F4E1E419DBC8A7E15C4121C2CB"
		"7DA11633A2B8EDF9CE487BC0D4772372E070A23A99B01ACC2ED8120E179B361E3D7C50F2FAF787794588E0509E815D6DF339492FCF3447DD5D1ECEB069F000DD"
		"7DF46CB64B58183035DCA0DAE2D7A41D18956AD503C90A54B40745211CCC85A8BCCB0A3E6F00B37C2D016F8331823442927CAF8CF6BF0C26A437E1CDDCBD1450"
		"208782420B8403596A2F1B8DA8FA611D89410771669578A780870954CDFE2924B1B920E050756D790D7B8AAAD5E043A50073EBA5C6880110321C4F593C753542"
		"F0C12D9987645085FB11E5CC8437591B7F88896C08A00BF0C2CB66B18CB8FB35194EAA3DF66A6CDD6EF10C176AFBBDC9F5D40B81CF675BC5BEF876150C75F0F1"
		"597CB97C8098D47A8E1A30516CD584267DFB663A27168BC4F88CD95F3A3CA1A2A652B5B26843EABD5D1626A9CF09E970F2B7EED8AA6C8FEC586DA2D35D62BF1E"
		"FF138D3CF1291CE33C9B9549DED6F4E309D25A519DFF7C008655ED33E53F8F3266D5809A7F7C6E48BBD21B2FE4D23943A07777D7DF0D3671C1597E17258D1B91"
		"C72A8DCD8893EB8FDC84780A5D998DA461173338C93FB11C61A3F82B6C508421FD107D27690366F492975B6D61D36C617ABA0B9027E4EDCD93C14A698AC82BBB"
		"83D0BD0484C5EB33AA60EFC7A10F16A9A818D038221A4F1037B117409E829912", ct2, QSC_KYBER_CIPHERTEXT_SIZE);
	hex_to_bin("BFC19503C98E88810353F8492E57C71725A25F494F9CACA7E64B1A3361389631691516286E5A39893C9D8A473A79202F303A8B3E9B57A3505F29B907C1DC1EEB"
		"3A3E992685C6E914F7E6CF48D4A0CC5345B1546726CA285545639E5C041F88219C0B504A23486B0B48F6ECCC5654223728C19B88869AA1B81D466A62360FE7CC"
		"349DD290D7AC2663DB0573667A560ABE2C77C71AD92353EBADE4D821C681246B429E6EF27884BA4C18F3049C8032F13B614AF6080DD6517471C2A343AFCFA0BA"
		"498286E1E53F5F2023911A2C8448A3554547B526A8E4D45733FC67DD9255E7E9135B923D19E35682FB83AD7756F7468B78D29B2F592B3F40CDD84A8A551A0FE1"
		"0031D631A3EE710AFF2B84506A6065B60161DC90A8B86E33365C4F73AEACC78F3E41593CE1BB7D332F249B436EE499260B80570B54E6743D90EC0938716F8015"
		"2F064093F2F52ACCC2255FA39E747777296A933F6C75BE72C10D1A671959C2F414736B059F718729AE5A1F341B636B24A1651A6FE0182EBF004F683B1E7BD83D"
		"6ED25D9A3A28C6323149860B90EB703D121F98646BD0C56AADE90B5A39CFF3C688C9DB0723A8B8016A939A366CCF37754CDB95920503D4C4B3C6522FFA29CB8D"
		"BAC31FD0A701C5924DB6651ACBB8CE78244D1093C2A00AC7F364F1BBAB3CE905DBBC6949763783C54FAEA72A957018CFA9B9FCC83CEC2CAD6ECA84502A14F2F9"
		"A530199B48D904C55979995CB44DD5A215D93F374A1AA1B06049F82AFD875A82173CAD905F9F57A66A2ABEAF25788263B86B72B8CA1668C6115E828B5FF198C6"
		"EE891585327DA4969E8F8A4C82F1CF0FEBC1FD271420C51E39184E60F09D52A75B49CA42D5D485CA9422A457ABDA5C00E1101FFF309B87695D7DB33436053F6C"
		"7074BE01A1D5D18686F9712C0C63878A8A57B94A2851160C7565FE747C98D92424580529F1B1C39B816050C197E8343B7C02755C773ACB775085A69F31291689"
		"B28C79634DE277D9EC0A6DE12786F8B5E4CB2EF8E86C525676BBDC72DF35229C908A115525C06312012974BB370C40C89636DC069C2158CF57AB0FA657006BAD"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922", pk2, QSC_KYBER_PUBLICKEY_SIZE);
	hex_to_bin("07638FB69868F3D320E5862BD96933FEB311B362093C9B5D50170BCED43F1B536D9A204BB1F22695950BA1F2A9E8EB828B284488760B3FC84FABA04275D5628E"
		"39C5B2471374283C503299C0AB49B66B8BBB56A4186624F919A2BA59BB08D8551880C2BEFC4F87F25F59AB587A79C327D792D54C974A69262FF8A78938289E9A"
		"87B688B083E0595FE218B6BB1505941CE2E81A5A64C5AAC60417256985349EE47A52420A5F97477B7236AC76BC70E8288729287EE3E34A3DBC3683C0B7B10029"
		"FC203418537E7466BA6385A8FF301EE12708F82AAA1E380FC7A88F8F205AB7E88D7E95952A55BA20D09B79A47141D62BF6EB7DD307B08ECA13A5BC5F6B68581C"
		"6865B27BBCDDAB142F4B2CBFF488C8A22705FAA98A2B9EEA3530C76662335CC7EA3A00777725EBCCCD2A4636B2D9122FF3AB77123CE0883C1911115E50C9E8A9"
		"4194E48DD0D09CFFB3ADCD2C1E92430903D07ADBF00532031575AA7F9E7B5A1F3362DEC936D4043C05F2476C07578BC9CBAF2AB4E382727AD41686A96B254882"
		"0BB03B32F11B2811AD62F489E951632ABA0D1DF89680CC8A8B53B481D92A68D70B4EA1C3A6A561C0692882B5CA8CC942A8D495AFCB06DE89498FB935B775908F"
		"E7A03E324D54CC19D4E1AABD3593B38B19EE1388FE492B43127E5A504253786A0D69AD32601C28E2C88504A5BA599706023A61363E17C6B9BB59BDC697452CD0"
		"59451983D738CA3FD034E3F5988854CA05031DB09611498988197C6B30D258DFE26265541C89A4B31D6864E9389B03CB74F7EC4323FB9421A4B9790A26D17B03"
		"98A26767350909F84D57B6694DF830664CA8B3C3C03ED2AE67B89006868A68527CCD666459AB7F056671000C6164D3A7F266A14D97CBD7004D6C92CACA770B84"
		"4A4FA9B182E7B18CA885082AC5646FCB4A14E1685FEB0C9CE3372AB95365C04FD83084F80A23FF10A05BF15F7FA5ACC6C0CB462C33CA524FA6B8BB359043BA68"
		"609EAA2536E81D08463B19653B5435BA946C9ADDEB202B04B031CC960DCC12E4518D428B32B257A4FC7313D3A7980D80082E934F9D95C32B0A0191A23604384D"
		"BFC19503C98E88810353F8492E57C71725A25F494F9CACA7E64B1A3361389631691516286E5A39893C9D8A473A79202F303A8B3E9B57A3505F29B907C1DC1EEB"
		"3A3E992685C6E914F7E6CF48D4A0CC5345B1546726CA285545639E5C041F88219C0B504A23486B0B48F6ECCC5654223728C19B88869AA1B81D466A62360FE7CC"
		"349DD290D7AC2663DB0573667A560ABE2C77C71AD92353EBADE4D821C681246B429E6EF27884BA4C18F3049C8032F13B614AF6080DD6517471C2A343AFCFA0BA"
		"498286E1E53F5F2023911A2C8448A3554547B526A8E4D45733FC67DD9255E7E9135B923D19E35682FB83AD7756F7468B78D29B2F592B3F40CDD84A8A551A0FE1"
		"0031D631A3EE710AFF2B84506A6065B60161DC90A8B86E33365C4F73AEACC78F3E41593CE1BB7D332F249B436EE499260B80570B54E6743D90EC0938716F8015"
		"2F064093F2F52ACCC2255FA39E747777296A933F6C75BE72C10D1A671959C2F414736B059F718729AE5A1F341B636B24A1651A6FE0182EBF004F683B1E7BD83D"
		"6ED25D9A3A28C6323149860B90EB703D121F98646BD0C56AADE90B5A39CFF3C688C9DB0723A8B8016A939A366CCF37754CDB95920503D4C4B3C6522FFA29CB8D"
		"BAC31FD0A701C5924DB6651ACBB8CE78244D1093C2A00AC7F364F1BBAB3CE905DBBC6949763783C54FAEA72A957018CFA9B9FCC83CEC2CAD6ECA84502A14F2F9"
		"A530199B48D904C55979995CB44DD5A215D93F374A1AA1B06049F82AFD875A82173CAD905F9F57A66A2ABEAF25788263B86B72B8CA1668C6115E828B5FF198C6"
		"EE891585327DA4969E8F8A4C82F1CF0FEBC1FD271420C51E39184E60F09D52A75B49CA42D5D485CA9422A457ABDA5C00E1101FFF309B87695D7DB33436053F6C"
		"7074BE01A1D5D18686F9712C0C63878A8A57B94A2851160C7565FE747C98D92424580529F1B1C39B816050C197E8343B7C02755C773ACB775085A69F31291689"
		"B28C79634DE277D9EC0A6DE12786F8B5E4CB2EF8E86C525676BBDC72DF35229C908A115525C06312012974BB370C40C89636DC069C2158CF57AB0FA657006BAD"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922DC60B903C87188FF7245E30331323D917F24598D3F36C7FA723BBFAA66A4A560"
		"8626ED79D451140800E03B59B956F8210E556067407D13DC90FA9E8B872BFB8F", sk2, QSC_KYBER_PRIVATEKEY_SIZE);
	hex_to_bin("D0FF6083EE6E516C10AECB53DB05426C382A1A75F3E943C9F469A060C634EF4E", ssk3, QSC_KYBER_SHAREDSECRET_SIZE);
#elif defined(QSC_KYBER_S2Q3329N256)
	hex_to_bin("EADD5ADA14DA57F0AEF3505F1CAA6485D4238D999A3EF4B0A59A1CDBE0A27E478547A3A99D2AB09AC7D7C8F5AE3D6432045CBA3FA778345892542BD81C05BEFC"
		"D2E5CC9A579BEFB7C58D02FB94F33392FE17F4EBA2CB510EC74CC9D1D8A87C1066A4869A3983E664BFE9DEA5AE4FDF310C8F59815A678FA325F369AF84FFEBC1"
		"D150431FE3BD2734F636CF658E6C1A6A6E2CBE071F9A7C26119AD105098EDA622CAB8E176762109877D9AE9D6729D44A58E707D6B8AD6E696A33C672DA9D08DA"
		"2A7F9E3BF02218238722A46B31D49DAFF9AF00A6363C3A423B2E873DEFDDBCD969B75A81053D9A97C06DE2BFE3D0CFD3D3C77983B18DBDE23C0728604A71435A"
		"D40DF1579096DDBE02E4612210CAA034DCEFB8B4D7B5E6D2EBA37A79FB61F34B5AF7D9B27B13E4936222411249B7FBB69E73461DAF4AA6F3E2C73944F10CE67C"
		"86FED260BDA7B40DB39B1DE3C7D8F09A77F3C84BC62931D228B24A574AC3F4EB745CFF7E031A3FB2A08595C15370A3C82DB7D9F41BB1D8ECC429CFA3A6583301"
		"6AB6EA60C9390CFA1B65CCEAE550940795386ED24133FBAE8B3017502AF3CFE951D781D36CFEFF85BFDF5AF040BE4065681B3B0A63C2747F0808CF3DA725169D"
		"DED1003DA6CD5DE4CB041942938D0A7F8802D48F2E3C6EEB45CD90AF6FC9F4507E9F8380AC33CACA7751487F65500441D920B94880A497D01C0802BB08D74C5D"
		"4C6BF2D865EE5822B3375C755D1A5E3D3244C320510A1E30357702CD4252072CF86437F7A9DE5561C7E59B94B9584100131AC399F4C1EB19FB4BDF65E62785E9"
		"7C194B8764CCF32FD05D804C2E439DDA2A109274FBFFA81A837C51B26D154F974B882A5B174B308FC48768D222922532B183ABDF6FBB0BC7492766974D321EE6"
		"FB7C5F7B3EEA2378DC6D6BB48019250B8D8D8DEDB522421AEEDB318676982A80E7961EC40E6D7F3339694255BAFF51BE3A7EA7D8793A109BE3AE4423BF082E20"
		"6A573B4F0F93FC16DDE81BD5DC583F528C08A0A9AB8E6CD524E297C9CF0F43C344913830ECB16F91441477BA782EDD4E73E732979D3A664EB99EA5D24B6C84AA"
		"69F377CB0CAD5AE4E641E38B197A0994D58B2387E91760E9B6FEBCB445CF85BBA24A94CDA75E338674428249FE6DE4692601D1EAE0EA021D9BC8077BE8665D07"
		"37748FA30FCF80F7E482584674F633A5006A538267627FD91854E0871268A6B0B05DD51495135DEFB9376E9B841B64E5DBF43CE6C74BCF3AE1FC427E810B7CBF"
		"6957DBF904690E87842543897DE78F13D08D92EBD27FB2CFCC0C765430589057B16B15F207CA1E6F08D52616DD57AD43EFEA6FDDAAEA18D33731FAC7ECAAE950"
		"E1DF3C5A4E6FCB223DF5E86B487FD7092D0822EFFAEC82C4BEC10C600FDB90E77482911B1595277738841409D0F8F113191D47F5E56C115A05DEA759AA6FB1D0"
		"47F9FCA4ED519EA5D21FE3BA5B9434FEA1283DFAD63D01589B0EB61F244351D03341DCD4DF62265AFCAEC6676A877D5CACB359EBB5319610DD447DA97E950B0C",
		ct2, QSC_KYBER_CIPHERTEXT_SIZE);
	hex_to_bin("A0B71F67C6CEC0D35686D513423432E512AC4044557E868A624800109A3355F98F151444E2852E27EA6EDB1992CAD3973C3A6FF79A5A049A259EB5415AA2A262"
		"456EC9495BBB5200D8D3163A5B10226292ECA01021389DA37881E276306550C6EFB6440EC51A2F7348349B851CD4AA0175A0550213C4791D91011220824B2B61"
		"650813ADFD2CB10538BFAB0A726F81129ED2C0F06A16B701090BF048C5A40126D572FCD47AA1218FB01547D150792D2316CB320D5144BA3508A1EBBB5AC1C229"
		"13E8295FAB59BF5837A778CF28227E07E1032DAB7D0E09A15F134148C12009DA536B22CC62474E69CC1554C0814D6CA0B722594383A9D0A2C77FD365A5544295"
		"FBB973F91EA56490D6CA6876497B98B3CB12417C257B6D0F7183DBB69E33075BEB0117B6914C69BA38349422F2F43364822A2570952DD5077B90755F1574115B"
		"8E221427585961913A9BFA0502B5D79AB7811744E6563C5B62C5CC4E93239A0A8CC60FE848F84A95F5902599B54A066293A2021DA196766C17C7E863AF790C27"
		"0B216A25138DDA0C8126E09377879859DB358F9B82B7C8A6792ACEE92A4CBDE3CEDD4500ACBC555C288EFF9795265B9005351C52E2653554ABAAF872DF95CA7F"
		"795903F0B0A182B18AEB0475B29F6E3ABF4C2250FE7B842A73655016A8FC729F390507ACA936825A98B3A32E6B2554CE9528941A3BB8C90996008D74FBCD020A"
		"02E706A6DE7B02AF404C10DB00FAEC02D3EAA6D9561A1565A7B05C6366D09DA7A537F20C7B2859A83E029E13A9BD289157C5B74C84EAA307753D431202A3D9B6"
		"162218BEC5346945BFEF55B624C5C6E373359BB1C479952BBABA4D6555C276573E5152B553901999F69402D150BEF79D74FB2953018FF48666746ACE607814A1"
		"FA33195720F83878D3B575C725744A72070DD044018042DA25714D173090323A51E6C063D203881380912761FC3410839095F26C0E687A00705495E171B57151"
		"ACE0498E30F14CA9B02F6E40831854C2E0AB1ECD0C21D8E4C7E669CD728230B9D11F72C266E34466F9C0159EF424F8F31D95A57BA0E210543C10C6503FB5C63E"
		"D23AA36CD6A6F378261B0B1E79509D8BEB36AA263DC91545E53369DF26837F394C56777C95B648BD1A72921ABF49563F99CB9D98EAB5C66666F6B16F74022481"
		"FA214E617698D3BBD13CB308713FDCC7CFD397B9CA39AFF4C744D5715D58966F2CF9707015C8F3543ED286A3D8D5CBF64ACEDFC02971A91072C69D2EF49829F1"
		"037F050C5B92229856CB12B456CC095282A62687EA38C9778AEA491DFF069711FBBE05E8CD9BF44A8E712619573E12EAA7B23829DC6726BFE33DA136B81E1532"
		"51508F6285BA15B2C1237677FE5B14B4E33F98C326BC58B9D8E075A25B94C8A23233029DCC786B135C56164BA3D160CBCEA854B7971F9CD73A383AAC050A302A"
		"D83B3E3AB90246AD160A321D330ACDEC7CA6643D7EC01F91691F16325BDF396950B88DAFE369C654B852055C970362C61380460757C65890F4E59222E4A4060B"
		"26C0EBC10197590DE3C8F0955D654B371CCB90ACA371B294476C16A4596A1DE8309E2A3612C69B7125310501E0C049B87440D9A6D0ECB999C9A0942AA340F603"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922", pk2, QSC_KYBER_PUBLICKEY_SIZE);
	hex_to_bin("07638FB69868F3D320E5862BD96933FEB311B362093C9B5D50170BCED43F1B536D9A204BB1F22695950BA1F2A9E8EB828B284488760B3FC84FABA04275D5628E"
		"39C5B2471374283C503299C0AB49B66B8BBB56A4186624F919A2BA59BB08D8551880C2BEFC4F87F25F59AB587A79C327D792D54C974A69262FF8A78938289E9A"
		"87B688B083E0595FE218B6BB1505941CE2E81A5A64C5AAC60417256985349EE47A52420A5F97477B7236AC76BC70E8288729287EE3E34A3DBC3683C0B7B10029"
		"FC203418537E7466BA6385A8FF301EE12708F82AAA1E380FC7A88F8F205AB7E88D7E95952A55BA20D09B79A47141D62BF6EB7DD307B08ECA13A5BC5F6B68581C"
		"6865B27BBCDDAB142F4B2CBFF488C8A22705FAA98A2B9EEA3530C76662335CC7EA3A00777725EBCCCD2A4636B2D9122FF3AB77123CE0883C1911115E50C9E8A9"
		"4194E48DD0D09CFFB3ADCD2C1E92430903D07ADBF00532031575AA7F9E7B5A1F3362DEC936D4043C05F2476C07578BC9CBAF2AB4E382727AD41686A96B254882"
		"0BB03B32F11B2811AD62F489E951632ABA0D1DF89680CC8A8B53B481D92A68D70B4EA1C3A6A561C0692882B5CA8CC942A8D495AFCB06DE89498FB935B775908F"
		"E7A03E324D54CC19D4E1AABD3593B38B19EE1388FE492B43127E5A504253786A0D69AD32601C28E2C88504A5BA599706023A61363E17C6B9BB59BDC697452CD0"
		"59451983D738CA3FD034E3F5988854CA05031DB09611498988197C6B30D258DFE26265541C89A4B31D6864E9389B03CB74F7EC4323FB9421A4B9790A26D17B03"
		"98A26767350909F84D57B6694DF830664CA8B3C3C03ED2AE67B89006868A68527CCD666459AB7F056671000C6164D3A7F266A14D97CBD7004D6C92CACA770B84"
		"4A4FA9B182E7B18CA885082AC5646FCB4A14E1685FEB0C9CE3372AB95365C04FD83084F80A23FF10A05BF15F7FA5ACC6C0CB462C33CA524FA6B8BB359043BA68"
		"609EAA2536E81D08463B19653B5435BA946C9ADDEB202B04B031CC960DCC12E4518D428B32B257A4FC7313D3A7980D80082E934F9D95C32B0A0191A23604384D"
		"D9E079BBBAA266D14C3F756B9F2133107433A4E83FA7187282A809203A4FAF841851833D121AC383843A5E55BC2381425E16C7DB4CC9AB5C1B0D91A47E2B8DE0"
		"E582C86B6B0D907BB360B97F40AB5D038F6B75C814B27D9B968D419832BC8C2BEE605EF6E5059D33100D90485D378450014221736C07407CAC260408AA649266"
		"19788B8601C2A752D1A6CBF820D7C7A04716203225B3895B9342D147A8185CFC1BB65BA06B4142339903C0AC4651385B45D98A8B19D28CD6BAB088787F7EE1B1"
		"2461766B43CBCCB96434427D93C065550688F6948ED1B5475A425F1B85209D061C08B56C1CC069F6C0A7C6F29358CAB911087732A649D27C9B98F9A48879387D"
		"9B00C25959A71654D6F6A946164513E47A75D005986C2363C09F6B537ECA78B9303A5FA457608A586A653A347DB04DFCC19175B3A301172536062A658A952775"
		"70C8852CA8973F4AE123A334047DD711C8927A634A03388A527B034BF7A8170FA702C1F7C23EC32D18A2374890BE9C787A9409C82D192C4BB705A2F996CE405D"
		"A0B71F67C6CEC0D35686D513423432E512AC4044557E868A624800109A3355F98F151444E2852E27EA6EDB1992CAD3973C3A6FF79A5A049A259EB5415AA2A262"
		"456EC9495BBB5200D8D3163A5B10226292ECA01021389DA37881E276306550C6EFB6440EC51A2F7348349B851CD4AA0175A0550213C4791D91011220824B2B61"
		"650813ADFD2CB10538BFAB0A726F81129ED2C0F06A16B701090BF048C5A40126D572FCD47AA1218FB01547D150792D2316CB320D5144BA3508A1EBBB5AC1C229"
		"13E8295FAB59BF5837A778CF28227E07E1032DAB7D0E09A15F134148C12009DA536B22CC62474E69CC1554C0814D6CA0B722594383A9D0A2C77FD365A5544295"
		"FBB973F91EA56490D6CA6876497B98B3CB12417C257B6D0F7183DBB69E33075BEB0117B6914C69BA38349422F2F43364822A2570952DD5077B90755F1574115B"
		"8E221427585961913A9BFA0502B5D79AB7811744E6563C5B62C5CC4E93239A0A8CC60FE848F84A95F5902599B54A066293A2021DA196766C17C7E863AF790C27"
		"0B216A25138DDA0C8126E09377879859DB358F9B82B7C8A6792ACEE92A4CBDE3CEDD4500ACBC555C288EFF9795265B9005351C52E2653554ABAAF872DF95CA7F"
		"795903F0B0A182B18AEB0475B29F6E3ABF4C2250FE7B842A73655016A8FC729F390507ACA936825A98B3A32E6B2554CE9528941A3BB8C90996008D74FBCD020A"
		"02E706A6DE7B02AF404C10DB00FAEC02D3EAA6D9561A1565A7B05C6366D09DA7A537F20C7B2859A83E029E13A9BD289157C5B74C84EAA307753D431202A3D9B6"
		"162218BEC5346945BFEF55B624C5C6E373359BB1C479952BBABA4D6555C276573E5152B553901999F69402D150BEF79D74FB2953018FF48666746ACE607814A1"
		"FA33195720F83878D3B575C725744A72070DD044018042DA25714D173090323A51E6C063D203881380912761FC3410839095F26C0E687A00705495E171B57151"
		"ACE0498E30F14CA9B02F6E40831854C2E0AB1ECD0C21D8E4C7E669CD728230B9D11F72C266E34466F9C0159EF424F8F31D95A57BA0E210543C10C6503FB5C63E"
		"D23AA36CD6A6F378261B0B1E79509D8BEB36AA263DC91545E53369DF26837F394C56777C95B648BD1A72921ABF49563F99CB9D98EAB5C66666F6B16F74022481"
		"FA214E617698D3BBD13CB308713FDCC7CFD397B9CA39AFF4C744D5715D58966F2CF9707015C8F3543ED286A3D8D5CBF64ACEDFC02971A91072C69D2EF49829F1"
		"037F050C5B92229856CB12B456CC095282A62687EA38C9778AEA491DFF069711FBBE05E8CD9BF44A8E712619573E12EAA7B23829DC6726BFE33DA136B81E1532"
		"51508F6285BA15B2C1237677FE5B14B4E33F98C326BC58B9D8E075A25B94C8A23233029DCC786B135C56164BA3D160CBCEA854B7971F9CD73A383AAC050A302A"
		"D83B3E3AB90246AD160A321D330ACDEC7CA6643D7EC01F91691F16325BDF396950B88DAFE369C654B852055C970362C61380460757C65890F4E59222E4A4060B"
		"26C0EBC10197590DE3C8F0955D654B371CCB90ACA371B294476C16A4596A1DE8309E2A3612C69B7125310501E0C049B87440D9A6D0ECB999C9A0942AA340F603"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922DEE4ABA000389581717D36F56F39AF7300B31D831A4D8C976128E09DEDE71A5A"
		"8626ED79D451140800E03B59B956F8210E556067407D13DC90FA9E8B872BFB8F", sk2, QSC_KYBER_PRIVATEKEY_SIZE);
	hex_to_bin("ED20140C05D78B15F2E412671A84154217FD77619A2C522D3C3CB688CB34C68B", ssk3, QSC_KYBER_SHAREDSECRET_SIZE);
#elif defined(QSC_KYBER_S3Q3329N256)
	hex_to_bin("C27F01244D4B3FB21D8437F840017CCCB7B7DAD5FB2B47B9B57EAE4F77D0A4555E5092A24969F2273E9702884A08477B568D8017F13875D1F5A6D413BDD228EB"
		"B11260F7F4529CBCEBF9B6862E8A841235F29F60F8E8417434189D579920FE6B98DBE713EC16C3FDDBB81E731D956B06DB4980F49C26F28661FF9CE6E9D861EC"
		"7A09840C19DE0EB6722071F8AA48362D2FF127A4AE46F99337826832ADAC239165F22585BB57A889C9C6AF82367EC7B07237C0535B31B38C1CAC40AC1A0C958A"
		"1887FE34711083FD37AF4BC5B1B4E1E2EE2843693D57DD1E657D4C24ED207EE712AD2A0891458180E9E8BD36FC14D8D633F5B741CEA108D2D4FD751C5A67B05E"
		"30324A67E9DD75C993D4FE0854FB78DF6F3D45A2A9C8E42510F0C3D80203712FB39E36B5DD8B5CCD3D09CEA94203BAF872084571ECF978BDB9548A250EE4907B"
		"4AFC31B21F319AE4BF0AB19CBD11EBE13359D1AAF4FDB83B6502501422A5FE50A8A38EF53DEB603CE23FD9792B04DEB378719AB769AA5897CC65E9B16304CEA5"
		"37E1762BD8C9B109DA14A829E6419F1B9FF8A466E2A6D6B34D74FFE1A59299181759D0D387FCED1D907F5FB5EDB426C05130E6CA5909B276D1A47E713C30D996"
		"DA5E8E57E712C77738F21BE74B42B518432DAD7EF73E6A8C43AA9A626994D71A31812851806E9FBB1F2BD356CEA39D95F2F87CA30DAF6F2733F7BCE79F8DA995"
		"051E49A7FD2264379C0A752E553ED608EB9344C79498F691538564C54F823BB70B12B59E8824B4A4BB1EEAC67C810CCC2E23744783CE958097F7A6BC6E1F1759"
		"7521B8C3D1EE8596A29FFEF14ED91632097C16D5065DB2A963CA7383AC60AD8F4ED0D41BD0BC3BAF198C5125AE911506C926D4C11785FD618229BFF54CB1161A"
		"B8FC7B51DAECCCD9131EDF437D8E528E7581B82C660E8C5E2512D5F6380A528F2AE4AEE263DB9676024BC7AD398BC9CDDAD607968BBAB22329E04D6E771FE647"
		"107AC46667A51AD558A635F026951F4F48C888D701C2AFF4EAB4E34ADB159ABBBFABE59B3F4CF8AAB1DD661E4DD0C5558DC059202EE64625A3B4B92FF4D15697"
		"F16C18D4D2338CFB496E0703526871C9784BAC8EBAE8279CF2713AF3CC2D440E8CD200867B8518AAD3B9E285027DA0ADD9F0229ED4E842D05E226ADAC13A3952"
		"E3835C8FB0A42874C94C661B39DF7B72887D227D583CE6B3BD65F795107BD093389BFEFD1768A5716F685B174ED23E94A5956E29BB2DDB792103E62F68928ACC"
		"603EEC2FF56DB14C08B7CBE4E2B4F2E0EAEE54162E95BB35EF36303EE3E6CC61061373876F7A096A8AF57D782F8C8203DE93423A379122FE7DAD770C3690F978"
		"228460D025CE93B1B336C573E4E55840EA65CFDD6122C672C912F52939D9EA5BE06210F5E7EDB65B66945D7056F559A7D69253F4BDBC579DE964F3E93A86FA38"
		"B6A2C0B54338DCE093F0B4684EE361449F16C279A72B7731E44600A7027768FDD0F643ED10064B98A9DA032F1F5DEAD311E177335094DB4E38514EAE15A8F8EC"
		"F2F2414E378EFBF997B1066B6F69D66909A47E298A7FEC961A83782E0E470FE071DECF4B26ACA6ED688359E1085055FD2B5AE9F4918749897AF133606053D5F6"
		"A8528CCB31AB7F3F2D89A95C5F05B15700E532AD81D59D9DB8A2C29CAC936E3F33DFE24B0B1B71902DC9C30EC8C70BDABA484FCD2B946D735F16EEAD04031CAF"
		"DE9EE01696EC9F0A8D5F36B69C642FFD0AD0D2544F5E7FD89A80498EF68E181617FAD41E0BD59BAAFFEEFE2F99724C719D47A2ECBA721D76F237EBA73DB47D88"
		"B699E3582B073C7EAD2A5B3CF024466396F9F2826CB754F66018E9503F4AD1F9D92121AA9956506051D596FFD467E1AA8D964C1767C925B468BBC9850600C843"
		"490541E8555A3D8BD9F18791EF9EBD3594E74C1FE3D3B80940A8A079F8D2CA8D30134FC66F87008126E43BD06EB6E41C3A70FA4739319BF1A932F02C30645656"
		"0CDA44DDAC43ED6D900445F5BF85BB0CE325947436E0D0685E41B16BC7169518259E5734FDCE080FFE85191B1D8D8DE4DB48143FB564038ACE80104D3A8D0712"
		"45E2AA56C71933F4DCF925EEE844C80FDDF3251F74006A23413318BBFD2ED9E05351B5AAEBCC77CFAC8D5F0364231A50EA8647C72F713E817A2075323029E3B8"
		"8B72442264C597B0F1FC09F9401CE88AC97C5522A56364523C37FEA2D6BD06B2", ct2, QSC_KYBER_CIPHERTEXT_SIZE);
	hex_to_bin("DA18712D316E98DC7AC8C3CA47370EBD770CE32B3BD4B1A0C9529AC6EC8EE028B1CDB2651CB5A6BB3C0C6DF1240A3B914B5656C0DC51C2B91BFCBC37A4660287"
		"D44F81F853C7F49A6D0603D6D723CBEC015FBC434A38241C109C7ED5B1CC461A2CCB9AB7140F19F37A13BB701E142BD54B64EC6B76FEC33B69C2918CB017C414"
		"3423009A3C07B5C181B0C1EB494A62ABC8391397089EA6640967C1204984CD484CCCB00A9A17D087218428EF3BB708783E12827104417375B6956FB50053D048"
		"A4791495824A3480A5B7830256096FDD725C308B3BE84A07F3632E2495C62E96399D80BFA745B9841A1833BC1D27BA45A52168EE59006C3A3A8C4A5A4F5088FC"
		"737181AB5196F7B1B49A2ED813134E112F730B991C54A7196BCF5FC76E134C5843E1698851B2F869AFAFB02787D9C2F136902DC7A7F3D62156D15EC309564092"
		"C61D83B0986C48409981F3C186880A2F63D5860AB601DEAC2B6BA1B428179D73533EB7A3A5113B8561F10B45C3CDE282B6EAD6AB6C604F09C17BFDA08313A326"
		"07675ADF6431CA8718E9C43A73733227E73BC61AC845BA907755CEC68925E5E2BFE912959DB86FBFE2156FD5BBDBF0C9DF8B5302AA8D90A22D12270E006551E4"
		"767E45268ED9692654447811EAB84F0499A8A58CF7C04A595698528045F29897C8FA96D06CCE51E6AFEAC4339589C941C85563D70FACE1928894BAC03619DFF4"
		"BE3F4314A3F7351A09A486B5041E7CB2DA8B96BC6626A49317357C4152A51BA3C28C7E0C9DB41A06A28290F218731107C954D8A66F801D7DE12A037116990B6C"
		"53C129F185C345F27E518B2D5A9125A07076D991B7DAC7CC65A8562EFBCC32A9CA4AD90263B04A4F9036116C7B97480496317575650DCC2152B5BC0E74407E12"
		"FA8E4FFCCCFF76C01A974BD61102E1F5296496C71D0764E13229FFE7846F336E34CAC904CA5670F8CD5052427A79C091A971210C5CFF667AAC249366E10D2B11"
		"376CA39D935204B12CC585E9405403625FB32CB5E5C31B62348160515CCC4FDAF5702D6BAB5C373DB6F350D3E63A5CE3CA5474A0CF1567042CA3258986FF75BD"
		"FCD929E6462F36BCCC3F5A93352A2B36CB162E1874C742870A97B167A050373624EAEB7E5073256B7211B2D94B8406CD6C9533B1536408AB0A29E5B2F0C954EC"
		"E00FBBEB176D724D4CF443CF7020D5FA7094CC1B1BE697BAD33674E4099EC7BB18F4577128CDD97CCD6D4462E5607C512A3E36248E3DDAA2EC089AEFC4CE485C"
		"49D7B009C8D63115FC81FF3A62D15A88441C03EA1C2E72C48839FC6AD73A30744A62B7B21645A6AF7D61B6383B221E21905557CD29D924A60986C511C1EBBC31"
		"6D5630A2414323F53AD55994F6B36539F9C44075A533E481C08445D9CA9E9D382119388BD1D75052217A944CCC7BE909E219711FCC7924769213A192534A5580"
		"08157A396EDFF480CC3B520FF818B38B135C18A80D5105E678836C395C28E92F7A3C4E93C01035F34111CC4902D0C033A63F236A71DA974A6F404F7AA7B5E582"
		"C758142FC1826BBA98B27D57C2E2103E10E30D32797B967714D15661111371A2D9C5399812464622635B442126B09836B081827247D05422972BD0320D8F42BF"
		"57E349461234E4D94F011850BAB5C049B62A59433866FCCE6966495C265C4765A56C06B9FC4276548785F46828CA602DC6D0541F250789C49E8B06559C434460"
		"A843809854E5B46E89389F10F489667491C1935E8AFB9EB4718F86AC458932BCF33C9ABECB2DC0C093A7E81DA0327BB637528105C358EB768D323A37D40A8C19"
		"9B7C4BB1D72B72B690B36DD27A9355191B3C3AA675A7E6F855EB505F579865DBD8910565505E7C0FDD545CBBB871B0B2CC0125B74F6B0C9B79670A6202D1B915"
		"4AC7766B1919C489589086AD8B45BE0D79C976C3C39D6B25213BB0B78A5750E6A8EB34330BB3CFF2B1C0722AC883B048F4B8CD06196DA8AA8B05DA4267552BF8"
		"7ABCA5124DC358A8381040E9B81FB08443150215D6922328000AC0B788B121931A267D80619CE7D29CD76B60CF6583DCE8CEBC490185736C818B25AD260B667B"
		"1FFD46206D0104553AA9FB304554A21C327244CE78AFDBD3B462361BB068A155636409F574C5716572E2A5F2A4B04FB8AAD1236684841787562AAF46C2C0DA46"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922", pk2, QSC_KYBER_PUBLICKEY_SIZE);
	hex_to_bin("07638FB69868F3D320E5862BD96933FEB311B362093C9B5D50170BCED43F1B536D9A204BB1F22695950BA1F2A9E8EB828B284488760B3FC84FABA04275D5628E"
		"39C5B2471374283C503299C0AB49B66B8BBB56A4186624F919A2BA59BB08D8551880C2BEFC4F87F25F59AB587A79C327D792D54C974A69262FF8A78938289E9A"
		"87B688B083E0595FE218B6BB1505941CE2E81A5A64C5AAC60417256985349EE47A52420A5F97477B7236AC76BC70E8288729287EE3E34A3DBC3683C0B7B10029"
		"FC203418537E7466BA6385A8FF301EE12708F82AAA1E380FC7A88F8F205AB7E88D7E95952A55BA20D09B79A47141D62BF6EB7DD307B08ECA13A5BC5F6B68581C"
		"6865B27BBCDDAB142F4B2CBFF488C8A22705FAA98A2B9EEA3530C76662335CC7EA3A00777725EBCCCD2A4636B2D9122FF3AB77123CE0883C1911115E50C9E8A9"
		"4194E48DD0D09CFFB3ADCD2C1E92430903D07ADBF00532031575AA7F9E7B5A1F3362DEC936D4043C05F2476C07578BC9CBAF2AB4E382727AD41686A96B254882"
		"0BB03B32F11B2811AD62F489E951632ABA0D1DF89680CC8A8B53B481D92A68D70B4EA1C3A6A561C0692882B5CA8CC942A8D495AFCB06DE89498FB935B775908F"
		"E7A03E324D54CC19D4E1AABD3593B38B19EE1388FE492B43127E5A504253786A0D69AD32601C28E2C88504A5BA599706023A61363E17C6B9BB59BDC697452CD0"
		"59451983D738CA3FD034E3F5988854CA05031DB09611498988197C6B30D258DFE26265541C89A4B31D6864E9389B03CB74F7EC4323FB9421A4B9790A26D17B03"
		"98A26767350909F84D57B6694DF830664CA8B3C3C03ED2AE67B89006868A68527CCD666459AB7F056671000C6164D3A7F266A14D97CBD7004D6C92CACA770B84"
		"4A4FA9B182E7B18CA885082AC5646FCB4A14E1685FEB0C9CE3372AB95365C04FD83084F80A23FF10A05BF15F7FA5ACC6C0CB462C33CA524FA6B8BB359043BA68"
		"609EAA2536E81D08463B19653B5435BA946C9ADDEB202B04B031CC960DCC12E4518D428B32B257A4FC7313D3A7980D80082E934F9D95C32B0A0191A23604384D"
		"D9E079BBBAA266D14C3F756B9F2133107433A4E83FA7187282A809203A4FAF841851833D121AC383843A5E55BC2381425E16C7DB4CC9AB5C1B0D91A47E2B8DE0"
		"E582C86B6B0D907BB360B97F40AB5D038F6B75C814B27D9B968D419832BC8C2BEE605EF6E5059D33100D90485D378450014221736C07407CAC260408AA649266"
		"19788B8601C2A752D1A6CBF820D7C7A04716203225B3895B9342D147A8185CFC1BB65BA06B4142339903C0AC4651385B45D98A8B19D28CD6BAB088787F7EE1B1"
		"2461766B43CBCCB96434427D93C065550688F6948ED1B5475A425F1B85209D061C08B56C1CC069F6C0A7C6F29358CAB911087732A649D27C9B98F9A48879387D"
		"9B00C25959A71654D6F6A946164513E47A75D005986C2363C09F6B537ECA78B9303A5FA457608A586A653A347DB04DFCC19175B3A301172536062A658A952775"
		"70C8852CA8973F4AE123A334047DD711C8927A634A03388A527B034BF7A8170FA702C1F7C23EC32D18A2374890BE9C787A9409C82D192C4BB705A2F996CE405D"
		"85A4C1A1AB9B6AEB49CCE1C2F8A97C3516C72A00A46263BAA696BF25727719C3216423618FF33380934A6C10545C4C5C5155B12486181FC7A2319873978B6A2A"
		"67490F8256BD2196FE1792A4C00077B812EAE8BED3572499684AB3371876761E450C9F9D2768A36806D7AB2046C91F17599E9AC592990808DCD7B4D0919072F1"
		"4EC361773B7252444C323C308326F4A30F8680D2F748F56A132B82674ED0184620B82AD2CB182C97B481626647491290A011CC73828685A8C367A5B9CF8D621B"
		"0D5C1EFF03172758BD004978C251CD51342228989CAE6332AC486437CB5C57D4307462865253BE217B3515C73DF405B7F28217AD0B8CF60C2FFFAA0A0048B1FB"
		"4ACDCDC38B5250CFEC356A6DE26CFA7A588FDC86F98C854AC64C7BFAA96F5A32CC0610934BAA6A586B9A2054F13BA274174AA0D2B3A81B96A940666F789B5A6B"
		"CDC0A6A0178A0C9A02578A493F6EEA0D2E6C13951C9F249A5E8DD71DD49A742D451F1ABBA19AF8C547855E0AFC728E90ABB499C9BEEB766F4729CDA22263E324"
		"DA18712D316E98DC7AC8C3CA47370EBD770CE32B3BD4B1A0C9529AC6EC8EE028B1CDB2651CB5A6BB3C0C6DF1240A3B914B5656C0DC51C2B91BFCBC37A4660287"
		"D44F81F853C7F49A6D0603D6D723CBEC015FBC434A38241C109C7ED5B1CC461A2CCB9AB7140F19F37A13BB701E142BD54B64EC6B76FEC33B69C2918CB017C414"
		"3423009A3C07B5C181B0C1EB494A62ABC8391397089EA6640967C1204984CD484CCCB00A9A17D087218428EF3BB708783E12827104417375B6956FB50053D048"
		"A4791495824A3480A5B7830256096FDD725C308B3BE84A07F3632E2495C62E96399D80BFA745B9841A1833BC1D27BA45A52168EE59006C3A3A8C4A5A4F5088FC"
		"737181AB5196F7B1B49A2ED813134E112F730B991C54A7196BCF5FC76E134C5843E1698851B2F869AFAFB02787D9C2F136902DC7A7F3D62156D15EC309564092"
		"C61D83B0986C48409981F3C186880A2F63D5860AB601DEAC2B6BA1B428179D73533EB7A3A5113B8561F10B45C3CDE282B6EAD6AB6C604F09C17BFDA08313A326"
		"07675ADF6431CA8718E9C43A73733227E73BC61AC845BA907755CEC68925E5E2BFE912959DB86FBFE2156FD5BBDBF0C9DF8B5302AA8D90A22D12270E006551E4"
		"767E45268ED9692654447811EAB84F0499A8A58CF7C04A595698528045F29897C8FA96D06CCE51E6AFEAC4339589C941C85563D70FACE1928894BAC03619DFF4"
		"BE3F4314A3F7351A09A486B5041E7CB2DA8B96BC6626A49317357C4152A51BA3C28C7E0C9DB41A06A28290F218731107C954D8A66F801D7DE12A037116990B6C"
		"53C129F185C345F27E518B2D5A9125A07076D991B7DAC7CC65A8562EFBCC32A9CA4AD90263B04A4F9036116C7B97480496317575650DCC2152B5BC0E74407E12"
		"FA8E4FFCCCFF76C01A974BD61102E1F5296496C71D0764E13229FFE7846F336E34CAC904CA5670F8CD5052427A79C091A971210C5CFF667AAC249366E10D2B11"
		"376CA39D935204B12CC585E9405403625FB32CB5E5C31B62348160515CCC4FDAF5702D6BAB5C373DB6F350D3E63A5CE3CA5474A0CF1567042CA3258986FF75BD"
		"FCD929E6462F36BCCC3F5A93352A2B36CB162E1874C742870A97B167A050373624EAEB7E5073256B7211B2D94B8406CD6C9533B1536408AB0A29E5B2F0C954EC"
		"E00FBBEB176D724D4CF443CF7020D5FA7094CC1B1BE697BAD33674E4099EC7BB18F4577128CDD97CCD6D4462E5607C512A3E36248E3DDAA2EC089AEFC4CE485C"
		"49D7B009C8D63115FC81FF3A62D15A88441C03EA1C2E72C48839FC6AD73A30744A62B7B21645A6AF7D61B6383B221E21905557CD29D924A60986C511C1EBBC31"
		"6D5630A2414323F53AD55994F6B36539F9C44075A533E481C08445D9CA9E9D382119388BD1D75052217A944CCC7BE909E219711FCC7924769213A192534A5580"
		"08157A396EDFF480CC3B520FF818B38B135C18A80D5105E678836C395C28E92F7A3C4E93C01035F34111CC4902D0C033A63F236A71DA974A6F404F7AA7B5E582"
		"C758142FC1826BBA98B27D57C2E2103E10E30D32797B967714D15661111371A2D9C5399812464622635B442126B09836B081827247D05422972BD0320D8F42BF"
		"57E349461234E4D94F011850BAB5C049B62A59433866FCCE6966495C265C4765A56C06B9FC4276548785F46828CA602DC6D0541F250789C49E8B06559C434460"
		"A843809854E5B46E89389F10F489667491C1935E8AFB9EB4718F86AC458932BCF33C9ABECB2DC0C093A7E81DA0327BB637528105C358EB768D323A37D40A8C19"
		"9B7C4BB1D72B72B690B36DD27A9355191B3C3AA675A7E6F855EB505F579865DBD8910565505E7C0FDD545CBBB871B0B2CC0125B74F6B0C9B79670A6202D1B915"
		"4AC7766B1919C489589086AD8B45BE0D79C976C3C39D6B25213BB0B78A5750E6A8EB34330BB3CFF2B1C0722AC883B048F4B8CD06196DA8AA8B05DA4267552BF8"
		"7ABCA5124DC358A8381040E9B81FB08443150215D6922328000AC0B788B121931A267D80619CE7D29CD76B60CF6583DCE8CEBC490185736C818B25AD260B667B"
		"1FFD46206D0104553AA9FB304554A21C327244CE78AFDBD3B462361BB068A155636409F574C5716572E2A5F2A4B04FB8AAD1236684841787562AAF46C2C0DA46"
		"65EAFD465FC64A0C5F8F3F9003489415899D59A543D8208C54A3166529B53922A5899A0BC465EE5FC2C04155582A40AC709761D2BE61FDC76C593044CEBCC7F2"
		"8626ED79D451140800E03B59B956F8210E556067407D13DC90FA9E8B872BFB8F", sk2, QSC_KYBER_PRIVATEKEY_SIZE);
	hex_to_bin("FBC4EEA691EEF4C1B476A29936453F4C3D4881794EE37BAF0FD72840743E7B7D", ssk3, QSC_KYBER_SHAREDSECRET_SIZE);
#endif

	/*print_hex(ct2, QSC_KYBER_CIPHERTEXT_SIZE, 64);
	printf_s("\n\n");
	print_hex(pk2, QSC_KYBER_PUBLICKEY_SIZE, 64);
	printf_s("\n\n");
	print_hex(sk2, QSC_KYBER_PRIVATEKEY_SIZE, 64);*/

	ret = true;
	qsctest_nistrng_prng_initialize(seed, NULL, 0);

	/* generate public and secret keys */
	qsc_kyber_generate_keypair(pk, sk, qsctest_nistrng_prng_generate);

	/* compare the public key to the expected output */
	if (qsc_intutils_are_equal8(pk, pk2, QSC_KYBER_PUBLICKEY_SIZE) != true)
	{
		printf_s("pk fail!");
		ret = false;
	}

	/* compare the secret key to the expected output */
	if (qsc_intutils_are_equal8(sk, sk2, QSC_KYBER_PRIVATEKEY_SIZE) != true)
	{
		printf_s("sk fail!");
		ret = false;
	}

	/* derive a shared-secret key and creates a response (in: pk | out: ct and ssk2) */
	qsc_kyber_encapsulate(ssk2, ct, pk, qsctest_nistrng_prng_generate);

	/* compare the cipher-text to the expected output */
	if (qsc_intutils_are_equal8(ct, ct2, QSC_KYBER_CIPHERTEXT_SIZE) != true)
	{
		printf_s("ct fail!");
		ret = false;
	}

	/* a uses b's response to get the shared-secret key (in: ct, sk | out: ssk1) */
	if (qsc_kyber_decapsulate(ssk1, ct, sk) != true)
	{
		printf_s("dec fail!");
		ret = false;
	}

	/* compare the two keys for equality */
	if (qsc_intutils_are_equal8(ssk1, ssk2, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		printf_s("ssk2 fail!");
		ret = false;
	}

	/* compare the key to the expected output */
	if (qsc_intutils_are_equal8(ssk1, ssk3, QSC_KYBER_SHAREDSECRET_SIZE) != true)
	{
		printf_s("ssk3 fail!");
		ret = false;
	}

	return ret;
}

/**
* \brief Run the Kyber implementation stress and correctness tests tests
*/
void qsctest_kyber_run()
{
	if (qsctest_kyber_kats() == true)
	{
		printf_s("Success! Passed Kyber known answer test. \n");
	}
	else
	{
		printf_s("Failure! Failed Kyber known answer test. \n \n");
	}

	if (qsctest_kyber_test_operations() == true)
	{
		printf_s("Success! Passed key generation, encryption, and decryption stress test. \n");
	}
	else
	{
		printf_s("Failure! Failed the encryption stress tests. \n \n");
	}

	if (qsctest_kyber_test_privatekey() == true)
	{
		printf_s("Success! Passed secret key tamper test. \n");
	}
	else
	{
		printf_s("Failure! Failed secret key tamper test. \n");
	}

	if (qsctest_kyber_test_ciphertext() == true)
	{
		printf_s("Success! Passed cipher-text tamper test. \n");
	}
	else
	{
		printf_s("Failure! Failed cipher-text tamper test. \n");
	}
}