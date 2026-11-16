# Running things

All commands run from the repository root. Requirements: `gcc`, `make`, `python3` (benchmark and report scripts only). Nothing else.

## How it works in one minute

Each algorithm is tested in two phases (details: [`kat-format.md`](kat-format.md)):

1. **generate** — a generator (`harness/gen/PQCgenKAT_*.c`) runs the algorithm with a deterministic DRBG and writes a `.rsp` file to `vectors/rsp/`;
2. **test** — a runner (`harness/gen/main_*.c`) reads the `.rsp` back, replays every operation and compares byte for byte.

`make test-<x>` does both. A **static test** is the same test with the vectors baked into C arrays: one self-contained program with no file I/O,
meant for bare-metal targets ([`static-tests.md`](static-tests.md)).

## Target names

| Pattern | Meaning | Example |
|---|---|---|
| `test-all` | the 23 standardized / standard-pending variants | `make test-all` |
| `test-<aggregate>` | a group of families | `test-onramp-ds`, `test-stateful-ds`, `test-ascon`, `test-fndsa-exp` |
| `test-<family>` | all variants of one family | `test-mlkem`, `test-mldsa`, `test-slhdsa`, `test-hqc`, `test-falcon` |
| `test-<variant>` | one parameter set | `test-ml-kem-512`, `test-ML-DSA-44`, `test-SLH-DSA-SHA2-128s`, `test-HQC2025_1` |
| `gen-…` | generate only (same names) | `gen-mlkem`, `gen-all` |
| `static-test-…` | static test (same names) | `static-test-ml-kem-512`, `static-test-all` |

The exact variant names of each family are listed in the category READMEs under [`../algorithms/`](../algorithms/README.md); `make help` prints a summary.

## Recipes

### Check that everything standardized still passes

```bash
make test-all                      # 100 vectors per variant
make test-all KATNUM=5             # quick smoke run
```

### One algorithm

```bash
make test-mlkem KATNUM=10          # a family
make test-ml-kem-512               # one variant
make test-ML-DSA-44 DEBUG=1        # print inputs / outputs on failure
make test-hawk-256 RUN_KEYGEN=0    # skip a phase (uses the keys stored in the .rsp)
```

`KATNUM` must be the same for generation and test. All flags: [`makefile-flags.md`](makefile-flags.md).

### Families outside `test-all`

```bash
make test-onramp-ds KATNUM=10      # on-ramp candidates: very long at the default 100 (an earlier run was stopped after 90 min)
make test-stateful-ds              # XMSS, LMS, HSS (default variant each)
make test-ascon                    # ASCON v1.2 (not PQC)
make test-fndsa-exp                # experimental FN-DSA
```

### Static (no file I/O) tests

```bash
make static-test-ml-kem-512                       # generate one into vectors/static/ml-kem-512/
make static-test-all STATIC_KATNUM=5              # all standardized variants
make static-test-ml-kem-512 PROFILING=cva6        # with RISC-V cycle counters (cva6, xheep, cw305)

tools/run_all_static_kat.sh                        # generate + compile + run + PASS/FAIL summary (standardized set)
tools/run_all_static_kat.sh --benchmark-all        # + one representative of every other family
tools/run_all_static_kat.sh --targets all --benchmark-all --kat-num 3
```

### Benchmark, tables and figures

```bash
make bench-fips                    # host benchmark, idle machine; writes bench/results_fips_<date>/
make results-update                # rebuild results/*.csv, the tables in results/README.md and docs/figures/
python3 -I tools/check_links.py    # after editing any Markdown file
```

### Clean up

```bash
make clean            # build/
make clean-rsp        # vectors/rsp/
make clean-static     # vectors/static/
make distclean        # all three
```

## Where outputs go

| Path | Content | Tracked in git |
|---|---|---|
| `build/` | compiled binaries | no |
| `vectors/rsp/` | generated `.rsp` KAT files | no |
| `vectors/static/` | generated static tests | no |
| `bench/` | raw benchmark runs | no |
| `results/`, `docs/figures/` | curated tables, CSVs and figures | yes |
