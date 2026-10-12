# Database Systems Labs

SQL exercises, database models, and database engine implementation sources.

- `Lab1`: Sakila SQL exercises, supplied public sample schema/data, and self-check scripts.
- `Lab2`: views, stored routines, permissions, and audit-trigger exercises.
- `Lab3`: campus trading database schema snapshots and reviewed ER/model diagrams.
- `Lab4/rucbase-lab`: Rucbase database engine source, tests, documentation, and original licenses.
- `Lab4/source-snapshot`: additional supplied source snapshot.
- `compiler-workbench`: supplied grammar samples, license notice, and workbench/demo sources recovered from archives.

The personal student identifier is replaced consistently by `0000000000`, including the SQL test email. Lab1 submissions are named `0000000000_student_lab1.sql`, so the supplied self-check can infer the same anonymous identifier from the filename. For example, run `python lab1_selfcheck.py --sql 0000000000_student_lab1.sql` from `Lab1/test` after configuring MySQL. The local database password is cleared in `lab1_config.json`; supply your own password locally or with the script's command-line option. The SQL user-creation example uses the placeholder `CHANGE_ME`.

Use a compatible MySQL environment for the SQL exercises and the original Rucbase instructions for its CMake/C++ build. Private home-directory paths in documentation and generated lexer line directives are generalized. Proprietary model-state files, installers, binaries, build caches, and archive files are excluded. The older compiler workbench uses Windows/MFC tooling; obtain any missing development dependencies separately. Original upstream copyright notices and licenses are preserved.

## Publication scope

This repository is a reviewed export of local coursework. Word/PDF reports, archive files, personal records, credentials, local IDE settings, generated binaries, and original Git history are excluded. Original source bytes and asset files are retained except for the documented database privacy substitutions. The root `.gitignore` uses an explicit upload allowlist; review new files before adding them.
