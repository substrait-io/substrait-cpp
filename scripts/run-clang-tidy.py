#!/usr/bin/env python
# SPDX-License-Identifier: Apache-2.0

# Run clang-tidy recursively and parallel on directory
# Usage: run-clang-tidy sourcedir builddir excludedirs extensions
# extensions and excludedirs are specified as comma-separated
# string without dot, e.g. 'c,cpp'
# e.g. run-clang-tidy . build test,other c,cpp file

import os, sys, subprocess, multiprocessing
manager = multiprocessing.Manager()
failedfiles = manager.list()

print("Arguments: " + str(sys.argv))
# Get absolute source dir after removing leading and trailing seperators from input.
sourcedir = sys.argv[1].rstrip(os.sep)
print("Source directory: " + sourcedir)
builddir = sourcedir + os.sep + sys.argv[2].rstrip(os.sep)
print("Build directory: " + builddir)
# Split exclude dirs into a tuple
excludedirs = tuple([(sourcedir + os.sep + s).rstrip(os.sep) for s in sys.argv[3].split(',')])
# If the build directory is not the same as the source directory, exclude it
if not sourcedir == builddir:
    excludedirs = excludedirs + (builddir,)
print("Exclude directories: " + str(excludedirs))
# Split extensions into a tuple
extensions = tuple([("." + s) for s in sys.argv[4].split(',')])
print("Extensions: " + str(extensions))

clang_tidy_options = sys.argv[5]
print("clang-tidy options: " + str(clang_tidy_options))

def runclangtidy(filepath):
    proc = subprocess.Popen("clang-tidy " + clang_tidy_options + "  -p=" + builddir + " " + filepath, shell=True)
    if proc.wait() != 0:
        print("Error file: " + filepath)
        failedfiles.append(filepath)

def collectfiles(dir, exclude, exts):
    collectedfiles = []
    for root, dirs, files in os.walk(dir):
        for file in files:
            filepath = root + os.sep + file
            if (len(exclude) == 0 or not filepath.startswith(exclude)) and filepath.endswith(exts):
                collectedfiles.append(filepath)
    return collectedfiles

# Define the pool AFTER the global variables and subprocess function because multiprocessing
# has stricter requirements on member ordering
# See: https://stackoverflow.com/questions/41385708/multiprocessing-example-giving-attributeerror
pool = multiprocessing.Pool()
pool.map(runclangtidy, collectfiles(sourcedir, excludedirs, extensions))
pool.close()
pool.join()
if len(failedfiles) > 0:
    print("Errors in " + str(len(failedfiles)) + " files")
    sys.exit(1)
print("No errors found")
sys.exit(0)
