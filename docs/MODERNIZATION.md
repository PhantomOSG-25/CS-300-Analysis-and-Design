# Modernization and Provenance

The historical final project established the core approach: parse course records, insert them into a binary search tree, traverse in order, and search by identifier. This portfolio reconstruction retains that reasoning while correcting reliability and presentation issues.

| Historical implementation | Maintained reconstruction |
| --- | --- |
| Windows-only header | Portable standard C++20 |
| Raw owning pointers | `std::unique_ptr` and RAII |
| Public root node | Encapsulated tree state |
| Duplicate IDs inserted silently | Explicit duplicate rejection |
| Maximum of two prerequisites | Variable-length prerequisite vector |
| Missing file treated like an empty tree | Explicit file-open failure |
| Partial insertion before later parse errors | Two-phase atomic load |
| Limited malformed-data handling | Line-numbered diagnostics and relationship checks |
| Fragile numeric input | Stream recovery and EOF handling |
| Missing assignment data | Clearly labeled synthetic sample |
| No build or tests | CMake, CTest, and two-platform CI |

The current branch removes raw course documents, repeated sample datasets, binaries, and supplied parser scaffolding from the reviewer-facing tree. Git history continues to preserve those materials and their original licenses. No third-party code is incorporated into the maintained implementation.
