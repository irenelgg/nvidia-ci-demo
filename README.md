# nvidia-ci-demo (v1 — Initial)

A minimal C++ CI/CD infrastructure demo inspired by how NVIDIA builds and tests its deep learning libraries (cuDNN, TensorRT).

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
│   └── math_ops.cpp        # Function implementations
├── tests/
│   └── test_math_ops.cpp   # Unit tests for correctness
├── scripts/
│   └── run_tests.py        # Automates build + test in one command
├── .github/workflows/
│   └── ci.yml              # GitHub Actions CI pipeline
└── CMakeLists.txt          # CMake build definition
```

---

## What Each File Does

### `CMakeLists.txt` — Build definition
Defines how to compile and link C++ files.
- `math_ops.cpp` → `libmath_ops.a` (static library)
- `test_math_ops.cpp` + library → `test_math_ops` (executable)

NVIDIA parallel: All CUDA projects (cuDNN, TensorRT) use CMake. GPU kernels (`.cu`) are wired in via `find_package(CUDAToolkit)`.

### `scripts/run_tests.py` — Test automation
Wraps the cmake → make → run sequence into a single command.
CI calls this script, so local and CI behavior stay identical.

### `.github/workflows/ci.yml` — CI pipeline
On every push or pull request to `main`, GitHub automatically:
1. Spins up an Ubuntu runner
2. Installs CMake
3. Runs `run_tests.py` and reports pass/fail in the Actions tab

NVIDIA parallel: Internally, CI runs on A100/H100 GPU clusters and validates hundreds of model tests before any merge is allowed.

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

---

## Build Flow

```
CMakeLists.txt
     ↓  cmake ..
  Makefile (auto-generated)
     ↓  make
  libmath_ops.a  +  test_math_ops   (inside build/)
     ↓  ./test_math_ops
  test results
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
| `run_tests.py` | Internal test runner |
| `ci.yml` | Internal CI pipeline |
