# nvidia-ci-demo (v2)

A C++ CI/CD infrastructure demo inspired by how NVIDIA builds and tests its deep learning libraries (cuDNN, TensorRT).

---

## Motivation

Libraries like cuDNN and TensorRT follow this internal structure:

```
C++ source → CMake build → test execution → CI pipeline automation
```

This project implements that exact structure from scratch to understand why each layer exists.

---

## Project Structure

```
nvidia-ci-demo/
├── include/
│   └── math_ops.h          # Function declarations (API interface)
├── src/
│   └── math_ops.cpp        # DL-relevant operation implementations
├── tests/
│   └── test_math_ops.cpp   # Google Test unit tests
├── scripts/
│   └── run_tests.py        # Local build + test automation
├── .github/workflows/
│   └── ci.yml              # GitHub Actions CI pipeline
└── CMakeLists.txt          # CMake build definition
```

---

## Implemented Operations

| Function | What it does | Where it appears in DL |
|---|---|---|
| `matmul` | Matrix multiplication | Attention, FC layers, Conv2D (internally) |
| `relu` | Clamps negatives to zero | Activation function between layers |
| `softmax` | Converts values to probabilities summing to 1 | Final classification layer |

---

## What Each File Does

### `include/math_ops.h` — API interface
Declares what functions exist. No implementation — just the contract.
Other files `#include` this to use the functions.

### `src/math_ops.cpp` — Implementation
Implements `matmul`, `relu`, `softmax`.
Note: `softmax` subtracts the max value before `exp()` to prevent float overflow — the same numerical stability trick used inside PyTorch and cuDNN.

### `tests/test_math_ops.cpp` — Google Test suite
Verifies correctness of each operation:
- `MatmulTest.TwoByTwo` — validates 2×2 matrix multiply result
- `MatmulTest.IncompatibleDimensions` — expects exception on bad dimensions
- `ReluTest.NegativesClamped` — confirms negatives become zero
- `SoftmaxTest.SumsToOne` — output probabilities must sum to 1
- `SoftmaxTest.LargestInputHasHighestProb` — largest input gets highest probability

### `CMakeLists.txt` — Build definition
Defines how to compile and link all files.
Uses `FetchContent` to automatically download Google Test at build time — no manual install needed.

NVIDIA parallel: All CUDA projects use CMake. GPU kernels (`.cu`) are wired in via `find_package(CUDAToolkit)`.

### `scripts/run_tests.py` — Local automation
Wraps cmake → build → run into a single command.
Use this before pushing to catch failures locally before CI runs.

### `.github/workflows/ci.yml` — CI pipeline
On every push or pull request to `main`, GitHub automatically:
1. Spins up an Ubuntu runner
2. Runs in both **Debug** and **Release** build modes (matrix strategy)
3. Caches Google Test so it isn't re-downloaded on every run
4. Configures, builds, and runs tests — reports pass/fail in the Actions tab

NVIDIA parallel: CI runs on A100/H100 GPU clusters and validates hundreds of model tests before any merge is allowed.

---

## Running Locally

```bash
# One command: build + test
python3 scripts/run_tests.py

# Step by step
mkdir build && cd build
cmake ..
make
./test_math_ops
```

Expected output:
```
[==========] Running 5 tests from 3 test suites.
[  PASSED  ] 5 tests.
```

---

## Workflow

```
1. Edit code
2. python3 scripts/run_tests.py   ← verify locally (instant)
3. git push
4. ci.yml triggers automatically  ← final verification on GitHub
5. ✓ green check on the repo
```

---

## Comparison with NVIDIA cuDNN

| This project | NVIDIA cuDNN |
|---|---|
| `math_ops.h` | `cudnn.h` |
| `math_ops.cpp` | CUDA kernel implementations (`.cu`) |
| `libmath_ops.a` | `libcudnn.so` |
| `test_math_ops` | cuDNN accuracy validation binary |
| `CMakeLists.txt` | CMakeLists.txt (same) |
| `run_tests.py` | Internal test runner (local verification) |
| `ci.yml` | Internal CI pipeline (automated gate) |
