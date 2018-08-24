# ss-log-analysis

Basic utlity to parse and analyse log files from "fact/fiction" summer scientist experiment.

Current version to process Summer Scientist 2018 log files: see [https://github.com/paul-baxter/nao-tablet-simple]

To compile (using cmake, from top level):

** $ mkdir build && cd build

** $ cmake ..

** $ make && cd ..

** $ rm -r build


To run (assuming log files are in /data):

** $ ./analysis.sh

** This will create a data file results.dat in top level
