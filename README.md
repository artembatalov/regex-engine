# regex-engine

A regex-to-DFA compiler implemented in C++17.

## Pipeline

```
Pattern string
    │
    ▼
Parser  (recursive descent)
    │
    ▼
AST
    │
    ▼
Thompson's NFA construction  (ε-NFA, states as integers)
    │
    ▼
Powerset (subset) construction  (ε-closure + move)
    │
    ▼
DFA  →  matches / rejects strings
```

### Supported grammar

```
<regex>  ::= <term> ( '|' <term> )*
<term>   ::= <factor>+
<factor> ::= <atom> ( '*' | '+' | '?' )?
<atom>   ::= char | '(' <regex> ')'
```

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

## Examples

```bash
./build/example_matching
./build/example_counting
```

`example_matching` tests `(a|b)*abb` and `0*(10*10*)*` against sample inputs.

`example_counting` enumerates and counts all words of length ≤ 6 accepted by `ab*c|d` over Σ = {a, b, c, d}.
