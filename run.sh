# Install dependencies required

echo "Do you eant to install all the dependencies (y/n)?"
read value 
if [ $value = "y" ]; then
    sudo apt install python3
    sudo apt install python3-pip
    pip3 install sklearn
    pip3 install pandas
    sudo apt install cmake
fi

# Build Library

echo "building library.."

cd build
cmake ..
make

# copy test file for ML code to parse

cd ..
cp Thread-Library/tests/main.cpp Ai/myfile.txt

# Learn and cluster

echo "Learning started .. "
cd Ai
python3 Project_AI.py learn
echo "Starting assiging thread to cluser .."
python3 Project_AI.py assign > threadOutput.txt

# move result back to library as input and execute code

echo "running tests .."
cd ..
mv Ai/threadOutput.txt build/Thread-Library/tests/
cd build/Thread-Library/tests
./spindleTest < threadOutput.txt