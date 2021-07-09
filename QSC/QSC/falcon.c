#include "falcon.h"
#include "falconbase.h"

void qsc_falcon_generate_keypair(uint8_t* publickey, uint8_t* privatekey, void (*rng_generate)(uint8_t*, size_t))
{
	assert(publickey != NULL);
	assert(privatekey != NULL);
	assert(rng_generate != NULL);

	qsc_falcon_ref_generate_keypair(publickey, privatekey, rng_generate);
}

void qsc_falcon_sign(uint8_t* signedmsg, size_t* smsglen, const uint8_t* message, size_t msglen, const uint8_t* privatekey, void (*rng_generate)(uint8_t*, size_t))
{
	assert(signedmsg != NULL);
	assert(smsglen != NULL);
	assert(message != NULL);
	assert(privatekey != NULL);
	assert(rng_generate != NULL);

	qsc_falcon_ref_sign(signedmsg, smsglen, message, msglen, privatekey, rng_generate);
}

bool qsc_falcon_verify(uint8_t* message, size_t* msglen, const uint8_t* signedmsg, size_t smsglen, const uint8_t* publickey)
{
	assert(message != NULL);
	assert(msglen != NULL);
	assert(signedmsg != NULL);
	assert(publickey != NULL);

	bool res;

	res = qsc_falcon_ref_open(message, msglen, signedmsg, smsglen, publickey);

	return res;
}
