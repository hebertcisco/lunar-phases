# Contributing

Thanks for helping improve Lunar Phases.

## Development setup

Requirements:

- A C11 compiler
- CMake 3.16 or newer
- `make` is optional; it is only a convenience wrapper around CMake

Run the normal checks before submitting a change:

```sh
make test
make sanitize
```

## Changes and pull requests

- Keep public APIs small and document behavior in the corresponding header.
- Prefer focused changes over broad refactors.
- Add or update deterministic tests for behavior changes.
- Keep source, identifiers, comments, and user-facing output in English.
- Do not commit generated build directories or IDE files.
- Explain any change to astronomical constants or algorithms, including its source.

Please include the compiler and platform used when reporting portability issues.

