# <picture><source media="(prefers-color-scheme: dark)" srcset="../docs/images/pqhedge-icon.svg"><img src="../docs/images/pqhedge-icon-light.svg" alt="pqhedge" height="32" align="absmiddle"></picture> `results/` — measured data, tables and figures

Everything the top-level README summarizes comes from here. Tables between `<!-- BEGIN … -->` / `<!-- END … -->` markers are
**generated** (`make results-update`); do not edit them by hand.

- [Sizes](#sizes) — pk / sk / ciphertext-or-signature bytes
- [Host benchmark](#host-benchmark) — time, stack, heap, code size
- [Files in this folder](#files-in-this-folder)

## Sizes

From the compiled `api.h` constants of every variant (`tools/gen_size_table.py`, [`sizes.csv`](sizes.csv)); bytes. The last column is the
ciphertext (KEM) or `CRYPTO_BYTES` (signature; for Falcon the maximum).

<!-- BEGIN sizes:root -->
| Variant | pk (B) | sk (B) | ct / sig (B) |
|---|---:|---:|---:|
| ml-kem-512 | 800 | 1632 | 768 |
| ml-kem-768 | 1184 | 2400 | 1088 |
| ml-kem-1024 | 1568 | 3168 | 1568 |
| ML-DSA-44 | 1312 | 2560 | 2420 |
| ML-DSA-65 | 1952 | 4032 | 3309 |
| ML-DSA-87 | 2592 | 4896 | 4627 |
| falcon-512 | 897 | 1281 | 690 |
| falcon-1024 | 1793 | 2305 | 1330 |
| SLH-DSA-SHA2-128s | 32 | 64 | 7856 |
| SLH-DSA-SHA2-128f | 32 | 64 | 17088 |
| SLH-DSA-SHA2-192s | 48 | 96 | 16224 |
| SLH-DSA-SHA2-192f | 48 | 96 | 35664 |
| SLH-DSA-SHA2-256s | 64 | 128 | 29792 |
| SLH-DSA-SHA2-256f | 64 | 128 | 49856 |
| SLH-DSA-SHAKE-128s | 32 | 64 | 7856 |
| SLH-DSA-SHAKE-128f | 32 | 64 | 17088 |
| SLH-DSA-SHAKE-192s | 48 | 96 | 16224 |
| SLH-DSA-SHAKE-192f | 48 | 96 | 35664 |
| SLH-DSA-SHAKE-256s | 64 | 128 | 29792 |
| SLH-DSA-SHAKE-256f | 64 | 128 | 49856 |
| HQC2025_1 | 2241 | 2321 | 4433 |
| HQC2025_3 | 4514 | 4602 | 8978 |
| HQC2025_5 | 7237 | 7333 | 14421 |
| fndsa-exp-512 | 897 | 1345 | 666 |
| fndsa-exp-1024 | 1793 | 2369 | 1280 |
<!-- END sizes:root -->

![Public key, secret key and ciphertext / signature sizes per variant](../docs/figures/sizes.png)

## Host benchmark

Portable reference C (no SIMD, no assembly): Intel Core Ultra 7 155H under WSL2, gcc 13.3 `-O2`, one CPU pinned, 2026-10-20, machine about 94 % idle,
median of up to 1000 calls within 3 s per operation, **single run** (run-to-run variation not quantified for these families). Method and raw data:
[`benchmark.csv`](benchmark.csv), [`benchmark_meta.json`](benchmark_meta.json), [`../tools/bench/`](../tools/bench); every command that was run is in
[`validation.md`](validation.md). These numbers are for the reference code and are not comparable with optimized (AVX2) packages.

![Median time per operation and variant, log scale](../docs/figures/time_per_operation.png)

<!-- BEGIN results:root -->
| Variant | NIST cat. | keygen (ms) | sign / encaps (ms) | verify / decaps (ms) | peak stack (B) | peak heap (B) | code (B) |
|---|---:|---:|---:|---:|---:|---:|---:|
| ML-KEM-512 | 1 | 0.019 | 0.021 | 0.027 | 12288 | 0 | 22921 |
| ML-KEM-768 | 3 | 0.030 | 0.031 | 0.040 | 16384 | 0 | 22665 |
| ML-KEM-1024 | 5 | 0.047 | 0.044 | 0.055 | 24576 | 0 | 24017 |
| HQC-1 | 1 | 1.232 | 2.647 | 3.625 | 65536 | 0 | 30854 |
| HQC-3 | 3 | 3.364 | 10.1 | 11.8 | 131072 | 0 | 32945 |
| HQC-5 | 5 | 8.678 | 19.0 | 26.4 | 208896 | 0 | 39274 |
| ML-DSA-44 | 2 | 0.050 | 0.166 | 0.057 | 49152 | 0 | 32601 |
| ML-DSA-65 | 3 | 0.098 | 0.277 | 0.097 | 73728 | 0 | 33041 |
| ML-DSA-87 | 5 | 0.146 | 0.377 | 0.146 | 110592 | 0 | 33505 |
| SLH-DSA-SHA2-128f | 1 | 0.838 | 22.9 | 1.427 | 4096 | 0 | 35448 |
| SLH-DSA-SHA2-128s | 1 | 57.7 | 461.0 | 0.441 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-128f | 1 | 2.608 | 34.6 | 1.828 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-128s | 1 | 105.5 | 702.1 | 0.791 | 4096 | 0 | 35448 |
| SLH-DSA-SHA2-192f | 3 | 1.249 | 42.0 | 2.049 | 4096 | 0 | 35448 |
| SLH-DSA-SHA2-192s | 3 | 105.1 | 976.3 | 0.628 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-192f | 3 | 2.037 | 71.2 | 3.232 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-192s | 3 | 180.8 | 1585.6 | 0.923 | 4096 | 0 | 35448 |
| SLH-DSA-SHA2-256f | 5 | 3.778 | 76.3 | 1.763 | 4096 | 0 | 35448 |
| SLH-DSA-SHA2-256s | 5 | 55.2 | 989.5 | 0.922 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-256f | 5 | 6.401 | 132.3 | 3.192 | 4096 | 0 | 35448 |
| SLH-DSA-SHAKE-256s | 5 | 91.8 | 1160.9 | 2.544 | 4096 | 0 | 35448 |
| Falcon-512 | 1 | 10.7 | 3.583 | 0.025 | 45056 | 0 | 90938 |
| Falcon-1024 | 5 | 40.7 | 9.257 | 0.062 | 86016 | 0 | 90946 |
| FN-DSA-exp-512 | 1 | 3.212 | 3.060 | 0.029 | 32768 | 0 | 107405 |
| FN-DSA-exp-1024 | 5 | 14.8 | 5.523 | 0.048 | 61440 | 0 | 107405 |
<!-- END results:root -->

Peak stack is measured in 4 KiB pages (so 4096 means "at most 4 KiB"); heap is peak `malloc` use (0 means none of these implementations allocates).
Code size is the algorithm objects only. Regenerate with `make bench-fips` (idle machine) and `make results-update`.

The earlier level-1 benchmark of the Round-3 on-ramp families (39 variants, median of 3 runs, 2026-10-14, not rerun after the migration) is in
[`../algorithms/signatures/onramp/README.md`](../algorithms/signatures/onramp/README.md#benchmark-of-the-round-3-families-level-1) and [`benchmark_onramp_l1.csv`](benchmark_onramp_l1.csv).

## Files in this folder

| File | Content | Produced by |
|---|---|---|
| [`sizes.csv`](sizes.csv) | pk / sk / ciphertext-or-signature bytes of every registry variant (272), from each variant's compiled `api.h` | `tools/gen_size_table.py` (`make results-update`) |
| [`benchmark.csv`](benchmark.csv) | host benchmark, one row per variant and operation (time, heap, stack, resident-set growth, sizes, code size) for ML-KEM, ML-DSA, SLH-DSA, Falcon, HQC and experimental FN-DSA | `tools/collect_results.py` from `make bench-fips` |
| [`benchmark_meta.json`](benchmark_meta.json) | host, compiler, flags and date of that run | copied from the run's `meta.json` |
| [`benchmark_onramp_l1.csv`](benchmark_onramp_l1.csv), [`benchmark_onramp_l1_spread.csv`](benchmark_onramp_l1_spread.csv) | the earlier level-1 benchmark of the Round-3 on-ramp families (39 variants, per-metric median of 3 runs, 2026-10-14, before the migration, not rerun) and its per-metric spread | `tools/collect_results.py --onramp-l1` |
| [`static_kat_status_onramp.csv`](static_kat_status_onramp.csv) | static-test outcome of all 232 on-ramp variants after the restructure (default stack and the rerun of the 4 SNOVA level-5 variants with a bigger stack) | scratch script, see `validation.md` |
| [`static_kat_status.csv`](static_kat_status.csv) | PASS / FAIL of the static (no-file-I/O) KAT tests of the validated variants | `tools/collect_results.py` from `tools/run_all_static_kat.sh` logs |
| [`validation.md`](validation.md) | every validation command run during the FIPS migration and its outcome | written by hand from the logs |

Figures built from these files are in [`../docs/figures/`](../docs/figures). The raw benchmark JSON, the run logs and the older level-1 on-ramp benchmark live in `bench/`
(git-ignored); regenerate with `make bench-fips` / `make bench-l1` on an otherwise idle machine.

The raw level-1 on-ramp signature runs behind `benchmark_onramp_l1.csv` (39 variants: `bench/results`, `results_run2`, `results_run3`, `results_median3`, the loaded-machine runs in `old_loaded_run/` and the v2-sized FAEST runs in `old_v2_faest_runs/`, each with `results.md`, charts, `meta.json` and `raw/<variant>.json`) are not in the working tree; they are kept in git history at commit `cf92c80` (restore with `git checkout cf92c80 -- bench`).

What is **not** here: a *new* benchmark of the on-ramp families (the earlier level-1 one, from before the migration and not rerun, is `benchmark_onramp_l1.csv`).
