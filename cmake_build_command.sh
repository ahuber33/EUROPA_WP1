#! /bin/bash

# Run this command in your build directory re-build the simulation of needed
# The second argument passed to cmake should obviously be updated to match
# the location of your source directory containing your CMakeLists.txt

 cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DGeant4_DIR=$G4COMP ../