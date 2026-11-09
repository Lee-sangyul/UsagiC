# Changelog

## v0.1.1-alpha — 2026-11-09

### Fixed
- Japanese sample's inverted preprocessor condition no longer triggers an unintended #error.
- UTF-8 BOM-prefixed source files are accepted.
- Existing output files are protected when -o is specified; --force provides explicit overwrite consent.
- Auto-generated C files are regenerated only when a UsagiC-generated marker exists; source file protection remains active.
- Added native MSVC C11 argument handling, improved Windows same-path detection, and UTF-8 compiler flags.
- Updated make clean for all included samples.

### Tests
- GCC and Clang each run complete native compilation and execution tests.
- CMake/CTest registers Japanese sample compilation, execution, and Unix integration tests.
- Added Windows/MSVC CI and coverage for BOM, overwritten-file safety, Korean/Japanese mixing, and C comments.

## v0.1.0-alpha — 2026-11-09

Initial native C11 transpiler, UsagiC keyword syntax, GCC/Clang integration, examples, bilingual documentation and tests.
