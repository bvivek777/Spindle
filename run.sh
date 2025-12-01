#!/usr/bin/env bash
set -euo pipefail

# Resolve repository root regardless of the invocation directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"
AI_DIR="${SCRIPT_DIR}/Ai"
TEST_SRC="${SCRIPT_DIR}/Thread-Library/tests/main.cpp"
ASSIGNMENT_FILE="${AI_DIR}/threadOutput.txt"
DATA_FILE="${AI_DIR}/myfile.txt"

prompt_for_deps() {
    read -r -p "Do you want to install all the dependencies (y/n)? " response || response="n"
    case "${response,,}" in
        y|yes)
            echo "Installing dependencies..."
            sudo apt-get update -y
            sudo apt-get install -y python3 python3-pip cmake build-essential
            python3 -m pip install --user --upgrade pip
            python3 -m pip install --user --upgrade scikit-learn pandas
            ;;
        *)
            echo "Skipping dependency installation."
            ;;
    esac
}

build_project() {
    echo "Building library..."
    cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}"
    cmake --build "${BUILD_DIR}"
}

prepare_ml_input() {
    if [[ ! -f "${TEST_SRC}" ]]; then
        echo "Test source file not found at ${TEST_SRC}" >&2
        exit 1
    fi
    cp "${TEST_SRC}" "${DATA_FILE}"
}

run_learning_pipeline() {
    echo "Learning started..."
    python3 "${AI_DIR}/Project_AI.py" learn
    echo "Assigning threads to clusters..."
    python3 "${AI_DIR}/Project_AI.py" assign > "${ASSIGNMENT_FILE}"
}

run_tests() {
    local test_dir="${BUILD_DIR}/Thread-Library/tests"
    mkdir -p "${test_dir}"
    cp "${ASSIGNMENT_FILE}" "${test_dir}/threadOutput.txt"
    echo "Running tests..."
    pushd "${test_dir}" >/dev/null
    ./spindleTest < threadOutput.txt
    popd >/dev/null
}

prompt_for_deps
build_project
prepare_ml_input
run_learning_pipeline
run_tests
