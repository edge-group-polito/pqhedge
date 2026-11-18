/*
 * nist_fxp.c - harness (NIST-style, explicit randomness) wrapper around the
 * public API of the Falcon fixed-point prototype (fixed-point-c/falcon.h).
 *
 * Not upstream code. The degree is selected by FALCON_FXP_LOGN (api.h).
 *   keypair_rnd (48 B) -> SHAKE256 PRNG for falcon_keygen_make
 *   nonce       (40 B) -> signature nonce
 *   sign_seed   (48 B) -> SHAKE256 PRNG for the Gaussian sampler
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "api.h"
#include "falcon.h"

#define LOGN      FALCON_FXP_LOGN
#define NONCELEN  40
#define SIGMAX    FALCON_SIG_COMPRESSED_MAXSIZE(LOGN)

/* The temporary buffers are static: not reentrant, test code only. */
#define TMP_MAX_A  FALCON_TMPSIZE_KEYGEN(LOGN)
#define TMP_MAX_B  FALCON_TMPSIZE_SIGNDYN(LOGN)
#define TMP_MAX_C  FALCON_TMPSIZE_VERIFY(LOGN)
#define TMP_MAX_AB (TMP_MAX_A > TMP_MAX_B ? TMP_MAX_A : TMP_MAX_B)
#define TMP_SIZE   (TMP_MAX_AB > TMP_MAX_C ? TMP_MAX_AB : TMP_MAX_C)

static union {
	uint8_t b[TMP_SIZE];
	uint64_t dummy_u64;
} tmp;

int crypto_sign_keypair(unsigned char *pk, unsigned char *sk, unsigned char *keypair_rnd)
{
	shake256_context rng;

	shake256_init_prng_from_seed(&rng, keypair_rnd, 48);
	if (falcon_keygen_make(&rng, LOGN,
		sk, CRYPTO_SECRETKEYBYTES, pk, CRYPTO_PUBLICKEYBYTES,
		tmp.b, sizeof tmp.b) != 0) {
		return -1;
	}
	return 0;
}

int crypto_sign(unsigned char *sm, unsigned long long *smlen,
	const unsigned char *m, unsigned long long mlen,
	const unsigned char *sk,
	const unsigned char *nonce,
	const unsigned char *sign_seed)
{
	shake256_context rng, hd;
	uint8_t sig[SIGMAX];
	size_t sig_len = sizeof sig, esig_len;

	shake256_init_prng_from_seed(&rng, sign_seed, 48);
	shake256_init(&hd);
	shake256_inject(&hd, nonce, NONCELEN);
	shake256_inject(&hd, m, (size_t)mlen);
	if (falcon_sign_dyn_finish(&rng, sig, &sig_len, FALCON_SIG_COMPRESSED,
		sk, CRYPTO_SECRETKEYBYTES, &hd, nonce,
		tmp.b, sizeof tmp.b) != 0) {
		return -1;
	}

	/* sig = header | nonce | compressed; esig = header | compressed */
	esig_len = sig_len - NONCELEN;
	memmove(sm + 2 + NONCELEN, m, (size_t)mlen);
	sm[0] = (unsigned char)(esig_len >> 8);
	sm[1] = (unsigned char)esig_len;
	memcpy(sm + 2, nonce, NONCELEN);
	sm[2 + NONCELEN + mlen] = sig[0];
	memcpy(sm + 2 + NONCELEN + mlen + 1, sig + 1 + NONCELEN,
		esig_len - 1);
	*smlen = 2 + NONCELEN + mlen + esig_len;
	return 0;
}

int crypto_sign_open(unsigned char *m, unsigned long long *mlen,
	const unsigned char *sm, unsigned long long smlen,
	const unsigned char *pk)
{
	uint8_t sig[SIGMAX];
	size_t esig_len, msg_len;
	const unsigned char *esig;

	if (smlen < 2 + NONCELEN) {
		return -1;
	}
	esig_len = ((size_t)sm[0] << 8) | (size_t)sm[1];
	if (esig_len < 1 || esig_len > (size_t)(smlen - 2 - NONCELEN)
		|| NONCELEN + esig_len > sizeof sig) {
		return -1;
	}
	msg_len = (size_t)smlen - 2 - NONCELEN - esig_len;
	esig = sm + 2 + NONCELEN + msg_len;

	/* rebuild sig = header | nonce | compressed */
	sig[0] = esig[0];
	memcpy(sig + 1, sm + 2, NONCELEN);
	memcpy(sig + 1 + NONCELEN, esig + 1, esig_len - 1);

	if (falcon_verify(sig, NONCELEN + esig_len, FALCON_SIG_COMPRESSED,
		pk, CRYPTO_PUBLICKEYBYTES,
		sm + 2 + NONCELEN, msg_len,
		tmp.b, sizeof tmp.b) != 0) {
		return -1;
	}
	memmove(m, sm + 2 + NONCELEN, msg_len);
	*mlen = msg_len;
	return 0;
}
