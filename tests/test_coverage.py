import os
import sys
import subprocess


folder_build = os.path.join(os.path.dirname(__file__), "..", "build", "tests")
folder_results = os.path.join(folder_build, "coverage_reports")
path_test_executable = os.path.join(folder_build, "RDB_Diplomaterv_Monitor_Unit_Tests")
path_coverage_file = os.path.join(folder_results, "coverage.info")


def main():
    # Changing the wokring folder to this script's folder
    os.chdir(sys.path[0])

    # Running the tests so that the .gcno and .gcov files get generated
    subprocess.run([path_test_executable])

    # Creating the results folder if it not yet exists
    if not os.path.isdir(folder_results):
        print("Creating the results folder (" + folder_results + ")...")
        os.mkdir(folder_results)

    # Generate the coverage file from the .gcno and .gcov files
    subprocess.run(["lcov", "-d", folder_build, "-c", "-o", path_coverage_file])

    # Filtering the coverage file: Remove some paths/files we don't want to calculate the code coverage
    subprocess.run(["lcov", "--remove", path_coverage_file, "/usr/include/*", "*/googlemock/*", "*/googletest/*", "*/QtCore/*", "-o", path_coverage_file])

    # Generate the HTML files from the coverage file
    subprocess.run(["genhtml", "-o", folder_results, path_coverage_file])
 
    # Reset our counts
    subprocess.run(["lcov", "-d", folder_results, "-z"])


if "__main__" == __name__:
    main()
