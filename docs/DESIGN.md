# Design and Complexity

## Requirements mapping

| Requirement | Design response |
| --- | --- |
| Load a course file | Stream parser with line-numbered diagnostics |
| Print courses alphanumerically | Binary-search-tree in-order traversal |
| Find one course | Iterative tree lookup by normalized ID |
| Show prerequisites | Variable-length prerequisite collection |
| Reject unreliable input | Validation before atomic catalog replacement |

## Ownership and invariants

Each node exclusively owns its children through `std::unique_ptr`; destruction therefore releases the complete tree without manual `delete` calls. The root and node type are private. Course identifiers are unique and normalized before insertion.

Loading occurs in two phases. The parser first validates records, duplicates, and relationships in temporary storage. Only a fully valid dataset is moved into the destination tree. An unsuccessful load leaves the previous catalog unchanged.

## Complexity

For `n` courses and tree height `h`:

- Insert: `O(h)`, average `O(log n)`, worst `O(n)`
- Lookup: `O(h)`, average `O(log n)`, worst `O(n)`
- Ordered traversal: `O(n)` time and `O(h)` call-stack space
- Relationship validation: expected `O(n + p)`, where `p` is the number of prerequisite references, using a hash set

The unbalanced tree is retained as the central learning artifact. A balanced tree would guarantee logarithmic insertion and lookup; a hash table would provide expected constant-time lookup but would require a separate sorting step for ordered output.
