#!/bin/bash

# Function to build the project
build() {
  echo "Building the project..."
  mkdir -p build
  cd build
  cmake ..
  cmake --build .
}

# Function to run the project
run() {
  echo "Running the project..."
  # Check if we are already in the 'build' directory, if not, cd into it
  if [ ! -d "build" ]; then
    echo "Build directory does not exist. Run the build first."
    exit 1
  fi
  cd build
  ./bin/main
}

# Function to clean the build and bin directories
clean() {
  echo "Cleaning build and bin directories..."
  rm -rf build bin
}

# Parse command line arguments
while getopts "brc" flag; do
  case "${flag}" in
    b) # Build the project
      build
      ;;
    r) # Run the project
      run
      ;;
    c) # Clean the build and bin directories
      clean
      ;;
    *) # Invalid flag
      echo "Usage: $0 [-b] to build, [-r] to run, or [-c] to clean"
      exit 1
      ;;
  esac
done

# Default behavior if no flags are passed
if [ $OPTIND -eq 1 ]; then
  echo "Usage: $0 [-b] to build, [-r] to run, or [-c] to clean"
  exit 1
fi
