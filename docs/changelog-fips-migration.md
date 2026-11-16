# Changelog — FIPS migration (what changed against the tag `pre-fips-migration`, and why)

A review pointed out that the repository labelled round-3 Kyber, Dilithium and SPHINCS+ code as ML-KEM, ML-DSA and SLH-DSA. Those are **not** byte-compatible with FIPS 203 / 204 / 205
(different hashing and domain separation, no context string), so their KATs were not FIPS KATs. What was done:

- **Replaced** the pre-standard code by FIPS-final upstream implementations, each pinned to a tag or commit and documented in an `UPSTREAM.md`: ML-KEM (mlkem-native), ML-DSA (mldsa-native, pure
  ML-DSA with an empty context), SLH-DSA (slhdsa-c, the 12 FIPS 205 sets). Cross-checked against independent references: upstream's published KAT hashes (ML-KEM, ML-DSA) and NIST ACVP vectors
  (SLH-DSA). SPHINCS+ Haraka / Haraka-AES, which are **not** in FIPS 205, left `test-all`; the old Kyber, Dilithium and SPHINCS+ code is kept in [`algorithms/legacy/`](../algorithms/legacy/README.md).
- **Relabelled** Falcon ("round 3, basis of the upcoming FN-DSA"; now also tested against the package's official KAT files, 100/100 records per size) and HQC ("selected, standard pending", pinned to the released v5.0.0 and checked against its official KATs); corrected the status of
  the on-ramp schemes (round reached, HAWK withdrawn, CROSS / LESS round 2 only) and of ASCON (the code is Ascon v1.2, not the SP 800-232 functions). Added the experimental pre-standard
  FN-DSA (c-fn-dsa, integer-only build), outside `test-all`.
- **Restructured** the repository (`original/` → `algorithms/`, `common/` + `kat/gen` → `harness/`, `kat/rsp` + `kat/static` → `vectors/`), with all old `make` target names still working. The generated
  `.rsp` files are byte-identical before and after the move (66 of 66 compared).
- **Added** the generated size tables, the host benchmark of the new families and the figures above, `results/`, and these documents.
- `make test-all` went from 35 to 23 variants (the dropped ones were the pre-standard trees). All of it passes; the exact commands and outcomes, including what was **not** run (notably the default 100-vector on-ramp run and the stateful sweeps), are in [`results/validation.md`](../results/validation.md).
