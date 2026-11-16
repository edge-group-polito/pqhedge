<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="docs/images/pqhedge-logo-dark.svg">
    <img src="docs/images/pqhedge-logo.svg" alt="pqhedge logo" width="400">
  </picture>
</p>

# pqhedge

**Post-quantum KAT golden model and bare-metal test suite**

Software side of a hardware/software co-design effort on post-quantum cryptography:

- **reference implementations** of the NIST PQC algorithms (portable C, no external libraries);
- **one KAT framework** (Known Answer Tests) around all of them: generate deterministic `.rsp` vectors, replay them byte for byte;
- **a static-test generator** that turns every test into a self-contained, no-file-I/O program, ready for bare-metal targets;
- **a host benchmark** (time, stack, heap, code size).

Software only: nothing here depends on or measures hardware. Everything is driven by the top-level `Makefile`.

> **Status wording.** Only ML-KEM (FIPS 203), ML-DSA (FIPS 204) and SLH-DSA (FIPS 205) are final standards here. Falcon (→ FN-DSA, FIPS 206) and HQC are
> *selected, standard pending*; the NIST additional signatures are *candidates*. Status checked on NIST's CSRC pages on 2026-10-20.

## Quick start

```bash
make test-all                       # generate + replay the 23 standardized variants (100 vectors each)
make test-ml-kem-512                # one variant
make static-test-ml-kem-512         # bare-metal style test into vectors/static/ml-kem-512/
make help                           # all targets
```

Requirements: `gcc`, `make` (and `python3` for benchmark / report scripts). More recipes (on-ramp, stateful, ASCON, static batch runner,
benchmark, cleanup) and the target naming scheme: **[`docs/running.md`](docs/running.md)**.

## What is in it

| Group | Families | Status | In `test-all` |
|---|---|---|:---:|
| [KEM](algorithms/kem/README.md) | ML-KEM (3 variants), HQC (3) | FIPS 203 final · HQC standard pending | yes |
| [Standard signatures](algorithms/signatures/standard/README.md) | ML-DSA (3), SLH-DSA (12), Falcon (2) | FIPS 204 / 205 final · Falcon → FN-DSA pending | yes |
| | FN-DSA experimental (2) | pre-standard guess, **not** FIPS 206 | no |
| [On-ramp signatures](algorithms/signatures/onramp/README.md) | FAEST, MAYO, MQOM, QR-UOV, SDitH, SNOVA, SQIsign, UOV, HAWK, CROSS, LESS (232 variants) | NIST additional signatures: candidates | no |
| [Stateful signatures](algorithms/signatures/stateful/README.md) | XMSS, LMS, HSS | NIST SP 800-208 | no |
| [Symmetric](algorithms/symmetric/README.md) | ASCON v1.2 (7) | not PQC | no |
| [Legacy](algorithms/legacy/README.md) | Kyber, Dilithium, SPHINCS+ (round 3, Haraka) | pre-standard, kept for comparison | no |

Upstream, pinned version and licence of every family: [`docs/coverage.md`](docs/coverage.md) and [`algorithms/README.md`](algorithms/README.md).

## Results at a glance

Host reference C, `-O2`, single run (Intel Core Ultra 7 155H, WSL2). Full tables, method and raw data: **[`results/README.md`](results/README.md)**.

| Variant | keygen (ms) | sign / encaps (ms) | verify / decaps (ms) | pk (B) | ct / sig (B) |
|---|---:|---:|---:|---:|---:|
| ML-KEM-768 | 0.030 | 0.031 | 0.040 | 1184 | 1088 |
| HQC-3 | 3.364 | 10.1 | 11.8 | 4514 | 8978 |
| ML-DSA-65 | 0.098 | 0.277 | 0.097 | 1952 | 3309 |
| SLH-DSA-SHA2-128f | 0.838 | 22.9 | 1.427 | 32 | 17088 |
| Falcon-512 | 10.7 | 3.583 | 0.025 | 897 | 690 |

![Median time per operation and variant, log scale](docs/figures/time_per_operation.png)

## Repository layout

```
pqhedge/
├── Makefile           every build / test / bench target (see `make help`)
├── algorithms/        the implementations, by status: kem/ signatures/{standard,onramp,stateful}/ symmetric/ legacy/
├── harness/           framework shared by all families: .rsp parser, KAT generators (PQCgenKAT_*.c), test runners (main_*.c)
├── tools/             static-test generator + registry, batch runner, benchmark (bench/), table / figure / link-check scripts
├── results/           curated measured data: sizes, benchmark, validation record, tables
├── docs/              documentation, figures/
├── vectors/           generated .rsp and static tests         (git-ignored)
├── build/             compiled binaries                       (git-ignored)
└── bench/             raw benchmark runs                      (git-ignored)
```

## Documentation

| I want to… | Read |
|---|---|
| run tests, static tests, benchmarks | [`docs/running.md`](docs/running.md) |
| know every Makefile flag and aggregate target | [`docs/makefile-flags.md`](docs/makefile-flags.md) |
| understand the generate / test model and the `.rsp` format | [`docs/kat-format.md`](docs/kat-format.md) |
| build static (bare-metal) tests and profile cycle counts | [`docs/static-tests.md`](docs/static-tests.md) |
| add a new algorithm or variant | [`docs/adding-an-algorithm.md`](docs/adding-an-algorithm.md) |
| see what each family is, where it comes from, its licence | [`docs/coverage.md`](docs/coverage.md), [`algorithms/README.md`](algorithms/README.md), each family's `UPSTREAM.md` |
| understand the harness code | [`harness/README.md`](harness/README.md) |
| find the right script | [`tools/README.md`](tools/README.md) |
| see sizes, benchmark numbers, validation record | [`results/README.md`](results/README.md), [`results/validation.md`](results/validation.md) |
| know what changed in the FIPS migration | [`docs/changelog-fips-migration.md`](docs/changelog-fips-migration.md) |

## License

This repository follows the licensing terms of the reference implementations it contains; every family folder carries its upstream licence file or
states it in its `UPSTREAM.md` (CC0, MIT, Apache-2.0 / ISC / MIT, BSD-style, public domain, depending on the family). Two things are **unconfirmed**: the
licence of the early Saarinen FIPS 205 code inside the legacy `*-s-simple` SPHINCS+ folders, and the exact upstream versions of the vendored CROSS, LESS,
HAWK, XMSS, LMS, HSS and ASCON code. Details: [`algorithms/README.md`](algorithms/README.md).

## Authors

- Alessandra Dolmeta — alessandra.dolmeta@polito.it
- Valeria Piscopo — valeria.piscopo@polito.it
