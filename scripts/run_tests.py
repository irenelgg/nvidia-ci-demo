import subprocess
import sys
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD_DIR = os.path.join(ROOT, "build")

def run(cmd, cwd=None):
    result = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"FAILED: {' '.join(cmd)}")
        print(result.stderr)
        sys.exit(1)
    return result.stdout

print("=== [1/3] Configuring with CMake ===")
os.makedirs(BUILD_DIR, exist_ok=True)
run(["cmake", ".."], cwd=BUILD_DIR)

print("=== [2/3] Building ===")
run(["make"], cwd=BUILD_DIR)

print("=== [3/3] Running tests ===")
output = run([os.path.join(BUILD_DIR, "test_math_ops")])
print("ALL TESTS PASSED")
