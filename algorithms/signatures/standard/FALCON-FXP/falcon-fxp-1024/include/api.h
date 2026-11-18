/*
 * api.h - Falcon-1024, fixed-point implementation (explicit-randomness API).
 *
 * Maps the pqhedge harness' Falcon-style calls onto the public falcon.h API of
 * the fixed-point prototype (De Almeida Braga, Fouque, Lachguel, Prest).
 * Implemented in wrapper/nist_fxp.c.
 *
 * Signed-message layout (same as the round-3 NIST wrapper):
 *   siglen (2 bytes, big endian) | nonce (40) | message | esig
 *   esig = header byte (0x30 + logn) | compressed signature
 * CRYPTO_BYTES is the maximum overhead of sm over the message
 * (2 + FALCON_SIG_COMPRESSED_MAXSIZE(logn)).
 */
#ifndef FALCON_FXP_API_H
#define FALCON_FXP_API_H

#define FALCON_FXP_LOGN         10

#define CRYPTO_SECRETKEYBYTES   2305
#define CRYPTO_PUBLICKEYBYTES   1793
#define CRYPTO_BYTES            1464
#define CRYPTO_ALGNAME          "Falcon-FXP-1024"

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk, unsigned char *keypair_rnd);

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
	const unsigned char *m, unsigned long long mlen,
	const unsigned char *sk,
	const unsigned char *nonce,
	const unsigned char *sign_seed);

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
	const unsigned char *sm, unsigned long long smlen,
	const unsigned char *pk);

#endif
