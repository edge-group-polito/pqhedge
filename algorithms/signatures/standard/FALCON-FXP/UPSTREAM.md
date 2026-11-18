# <picture><source media="(prefers-color-scheme: dark)" srcset="../../../../docs/images/pqhedge-icon.svg"><img src="../../../../docs/images/pqhedge-icon-light.svg" alt="pqhedge" height="32" align="absmiddle"></picture> Falcon fixed-point (FALCON-FXP) — upstream provenance

**EXPERIMENTAL. Not in `test-all`.** Fixed-point ("type 1") C prototype of Falcon from the paper *Toward a Secure Fixed-Point
Implementation of the Falcon Signature Scheme* (De Almeida Braga, Fouque, Lachguel, Prest). It is derived from the Falcon round-3
reference code and from Thomas Pornin's fixed-point `ntrugen`, with the paper's optimizations and bound checks. It is a proof of
concept, not FN-DSA (FIPS 206) and **not wire-compatible with the round-3 Falcon** in `../FALCON`: the key generation rejects more
keys and uses another RNG stream, so its keys and signatures differ from the official Falcon KATs (stated in `fixed-point-c/inner.h`).

| Item | Value |
|---|---|
| Source | local copy of the artifact folder `/home/aledolme/home/pqrv/fixed-point-c`, imported 2026-11-18 |
| Upstream URL / commit | **not recorded** in the local copy (no version control); to be filled in from the paper's artifact page |
| License | MIT for the Falcon code (`LICENSE`, text taken from the preserved upstream README); `ntrugen` is public domain (`fixed-point-c/ntrugen/LICENSE`) |
| Variants | `falcon-fxp-512` (logn 9), `falcon-fxp-1024` (logn 10) |
| Arithmetic | `FALCON_FPR_MODE`: **2 = fixed point (default here)**, 0 = native `double` (needs `-lm`; the Makefile adds it). Mode 1 (emulated) is not maintained upstream |
| Portability | uses `unsigned __int128` (gcc / clang), as the upstream README says; not ISO C |

## Layout

```
FALCON-FXP/
├── fixed-point-c/          upstream sources, byte-identical to the source folder (37 files, checked with cmp)
│   └── ntrugen/src/        fixed-point key generation (ng_*.c, sha3.c)
├── wrapper/nist_fxp.c      NOT upstream: harness API on top of the public falcon.h
└── falcon-fxp-{512,1024}/include/api.h    NOT upstream: sizes and prototypes per variant
```

## What was left out of the import

`test_falcon.c`, `speed.c`, `find_issue.c`, `test_timecop.c`, `timecop.c`, `timecop/`, `bench.sh`, the upstream `Makefile`s, and
`keygen_ref/` (reference floating-point NTRUGEN, only reached with `-DTEST_REFERENCE`, which is not used here). They stay in the
source folder. The signing path used is the one `falcon_sign_dyn_finish` calls in `fixed-point-c/falcon.c`.

## Local changes

None to upstream files. New code:

- `wrapper/nist_fxp.c`: `crypto_sign_keypair(pk, sk, keypair_rnd)` seeds a SHAKE256 PRNG with the 48-byte `keypair_rnd` and calls
  `falcon_keygen_make`; `crypto_sign(..., nonce, sign_seed)` hashes the 40-byte `nonce` and the message, seeds the sampler PRNG with the
  48-byte `sign_seed` and calls `falcon_sign_dyn_finish` (compressed format); `crypto_sign_open` rebuilds the signature and calls
  `falcon_verify`. Signed-message layout as in the round-3 wrapper: `siglen(2, BE) | nonce(40) | message | esig`, with
  `esig = header (0x30 + logn) | compressed signature`.
- `include/api.h`: `CRYPTO_BYTES` is the maximum overhead of `sm` over the message, `2 + FALCON_SIG_COMPRESSED_MAXSIZE(logn)` (754 / 1464).
- The Makefile builds this family with `-UDEBUG`: upstream treats any defined `DEBUG` (the harness passes `-DDEBUG=0`) as a debug build
  and prints rejection statistics on every signature. As a consequence the harness' `DEBUG=1` hex dumps are not available here.

## Verification

Run on 2026-11-18, gcc, `-O2 -std=c11 -D_GNU_SOURCE -DFALCON_FPR_MODE=2`:

- `make test-falcon-fxp`: 100 KAT vectors per variant generated and replayed, keygen / sign / verify **100 pass, 0 fail** for both sizes.
- `tools/run_all_static_kat.sh -t falcon-fxp -k 5`: both static folders compile with plain `gcc` and pass 5 vectors.
- A scratch round trip with `-DFALCON_FPR_MODE=0 -lm` (keygen, sign, open, tampered signature rejected) works for Falcon-512.

**No independent reference exists**: the keys differ from the official Falcon KATs by design, so these KATs only show that the
harness replays this implementation deterministically and that sign / verify agree. They are not cross-checked against another implementation.
