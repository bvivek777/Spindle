#!/usr/bin/env bash
set -euo pipefail

prompt_for_dependencies() {
    local answer=""
    while true; do
        read -r -p "Install dependencies (y/n)? " answer
        case "${answer,,}" in
            y|yes)
                echo "Installing dependencies (requires sudo)..."
                sudo apt-get update
                sudo apt-get install -y python3 python3-pip cmake
                python3 -m pip install --user --upgrade pip
                python3 -m pip install --user scikit-learn pandas
                break
                ;;
            n|no)
                echo "Skipping dependency installation."
                break
                ;;
            *)
                echo "Please answer with 'y' or 'n'."
                ;;
        esac
    done
}

prompt_for_dependencies

echo "Building library..."
mkdir -p build
cmake -S . -B build
cmake --build build

echo "Preparing ML input sample..."
cp Thread-Library/tests/main.cpp Ai/myfile.txt

echo "Learning started..."
pushd Ai > /dev/null
python3 Project_AI.py learn
echo "Assigning threads to clusters..."
python3 Project_AI.py assign > threadOutput.txt
popd > /dev/null

echo "Running tests..."
mkdir -p build/Thread-Library/tests
mv Ai/threadOutput.txt build/Thread-Library/tests/
pushd build/Thread-Library/tests > /dev/null
./spindleTest < threadOutput.txt
popd > /dev/null