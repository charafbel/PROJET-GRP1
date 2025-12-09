import os
import subprocess
import time

# CONFIGURATION
EXECUTABLE = "./main"
INPUT_DIR = "tsp"
OUTPUT_DIR = "tests"

METHODS = [
    "nn", "rw",
    "gadpx 10000 0.10 100",
    "ga 5000 0.20 50"
]

VALID_EXTENSIONS = (".tsp")

def run_benchmark():
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    files = [f for f in os.listdir(INPUT_DIR) if f.endswith(VALID_EXTENSIONS)]
    files.sort()
    for filename in files:
        input_path = os.path.join(INPUT_DIR, filename)
        base_name = os.path.splitext(filename)[0]

        for method_str in METHODS:
            output_filename = f"{base_name}.txt"
            output_path = os.path.join(OUTPUT_DIR, output_filename)
            cmd = [EXECUTABLE, "-f", input_path, "-m"] + method_str.split() + ["-o", output_path]
            try:
                result = subprocess.run(cmd, capture_output = True, text = True)
                if result.returncode != 0:
                    print(f"[ERREUR] Code: {result.returncode}")
                    print(f"Stderr: {result.stderr}")
            except Exception as e:
                print(f"[EXCEPTION] {e}")

    print("[DONE]")

if __name__ == "__main__":
    run_benchmark()