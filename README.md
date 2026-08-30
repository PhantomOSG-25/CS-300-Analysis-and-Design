# C++ Course Planner

A portable C++20 console application that loads a course catalog, validates prerequisite relationships, stores courses in a binary search tree, prints the catalog in alphanumeric order, and retrieves individual course details.

This is a portfolio reconstruction of my CS 300 final project. The implementation preserves the original learning objective—selecting and applying an appropriate data structure—while adding safe ownership, reproducible data, automated tests, cross-platform builds, and explicit provenance.

## What this demonstrates

- Binary-search-tree insertion, lookup, and in-order traversal
- CSV-style file parsing with line-numbered validation errors
- Atomic catalog replacement: invalid input never corrupts the active catalog
- Duplicate detection and prerequisite relationship validation
- RAII and `std::unique_ptr` ownership
- Portable C++20 and CMake
- Automated tests on Windows and Linux

## Architecture

```text
CSV input -> validator/normalizer -> binary search tree -> console operations
                 |                         |                  |
          relationship checks       ordered traversal    lookup by ID
```

The tree is intentionally unbalanced because the course requirement focused on implementing and evaluating a binary search tree. Average insertion and lookup are `O(log n)` when reasonably balanced, but degrade to `O(n)` for sorted or adversarial input. In-order traversal is `O(n)` and naturally produces the required sorted course list. A production catalog with uncontrolled input would use a balanced tree or a hash index plus an ordered view.

## Build and test

Requirements: a C++20 compiler and CMake 3.20 or later.

```powershell
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Run on Windows:

```powershell
.\build\Release\course_planner.exe .\data\sample_courses.csv
```

On single-configuration generators such as Unix Makefiles, the executable is normally `./build/course_planner`.

## Data format

Each nonblank line contains a course ID, title, and zero or more prerequisite IDs:

```text
CS300,Analysis and Design,CS200,CS210
```

IDs are trimmed and normalized to uppercase. The loader rejects missing identifiers or titles, duplicates, empty prerequisites, self-dependencies, and prerequisites that do not appear in the same file. Quoted CSV fields and commas inside titles are intentionally outside this small demonstration format.

The included [sample catalog](data/sample_courses.csv) is synthetic portfolio data created for reproducible review. It is not the unavailable original assignment input.

## Repository guide

- `include/course_planner` — public model, tree, and loader interfaces
- `src` — implementation and console application
- `tests` — dependency-free automated verification
- `data` — synthetic sample input
- `docs` — design, test, and modernization notes

## Provenance

The historical repository contained course documents, repeated bid datasets, supplied CSV-parser scaffolding, and several data-structure exercises. Those artifacts remain available in Git history but are intentionally absent from the current branch. This maintained implementation is derived from my final-project logic and the original assignment requirements; it does not claim that the reconstructed code is the untouched historical submission.

See [Modernization](docs/MODERNIZATION.md) for the precise relationship to the original work.

## Limitations

- The binary search tree does not self-balance.
- The parser supports a deliberately small comma-delimited format, not full RFC 4180 CSV.
- The application validates referenced prerequisites but does not currently detect multi-course prerequisite cycles.
- Catalog data is held in memory for the lifetime of the process.
- No license is granted unless a license file is added later.
