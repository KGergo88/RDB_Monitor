import os
import sys
import subprocess
import argparse


def main(args):
    # Running the tests so that the .gcno and .gcov files get generated
    subprocess.run([args.test_executable])

    # Setting the results folder
    results_folder = os.getcwd()
    if args.output_directory is not None:
        results_folder = args.output_directory

    # Creating the results folder if it does not exists
    if not os.path.isdir(results_folder):
        print("Creating the results folder (" + results_folder + ")")
        os.mkdir(results_folder)

    # The path to the file containing the coverage info
    coverage_file = os.path.join(results_folder, "coverage.info")

    # Generate the coverage file from the .gcno and .gcov files
    subprocess.run(["lcov", "-d", os.path.dirname(args.test_executable), "-c", "-o", coverage_file])

    # Filtering the coverage file: Remove some paths/files we don't want to calculate the code coverage
    subprocess.run(["lcov", "--remove", coverage_file, "/usr/include/*", "*/googlemock/*", "*/googletest/*", "*/QtCore/*", "-o", coverage_file])

    # Generate the HTML files from the coverage file
    subprocess.run(["genhtml", "-o", results_folder, coverage_file])
 
    # Reset our counts
    subprocess.run(["lcov", "-d", results_folder, "-z"])


if "__main__" == __name__:
    parser = argparse.ArgumentParser(
        prog="test_coverage.py",
        description="Script to run the unit tests and generate the coverage reports."
    )
    parser.add_argument("-t", "--test_executable", type=str, required=True, help="Path of the unit test executable.")
    parser.add_argument("-o", "--output_directory", type=str, required=False, help="Path of the folder where the results shall be put. Defaults to the working directory if left empty.")
    args = parser.parse_args()
    main(args)
