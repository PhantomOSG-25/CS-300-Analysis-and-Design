# Test Strategy

The dependency-free test executable validates behavior at the data-structure and ingestion boundaries:

- insertion above and below the root
- duplicate rejection
- in-order sorting
- successful and unsuccessful lookup
- case normalization
- unlimited prerequisite preservation
- duplicate records
- missing titles
- unknown prerequisite references
- self-dependencies and empty prerequisite fields
- atomic preservation of the active catalog after a failed load

CTest runs the executable and reports any failed assertion. GitHub Actions builds and tests the same source with the default C++ toolchains on Windows and Ubuntu.

The test data is synthetic and contains no personal, institutional, or production information.
