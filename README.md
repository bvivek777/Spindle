# Spindle
A machine-learning-based thread spawning library built as a project exploration.

## Quick start
1. Make the run script executable: `chmod +x run.sh`
2. Launch the pipeline: `./run.sh`

## Repository layout
- `Ai/` – simple generators and experiments that feed the main library.
- `Thread-Library/` – core C++ thread primitives and tests.
- `run.sh` – convenience entry point that wires the pieces together.

## Notes
- Replace the placeholder routines in `Ai/` with your own models.
- Update `Thread-Library/` if you need different synchronization semantics.
