#!/usr/bin/env python

import sys, os, re, subprocess
from optparse import OptionParser


def make_filelist(input_dir):

    proc = subprocess.Popen( [ '/afs/cern.ch/project/eos/installation/cms/bin/eos.select', 'ls -l', input_dir ], stdout = subprocess.PIPE, stderr = subprocess.STDOUT )
    output = proc.communicate()[0]
    if proc.returncode != 0:
        print output
        sys.exit(1)

    return output.splitlines()


def process_input_dir(input_dir, joblist):

    input_dir = input_dir.rstrip('/')+'/'
    filelist = make_filelist(input_dir)

    for line in filelist:
        size = int(line.split()[4])
        filename = line.split()[8]
        m1 = re.search('_\d+_\d+_\w+.root', filename)
        job = int(filename[m1.start():].lstrip('_').split('_')[0])
        submission = int(filename[m1.start():].lstrip('_').split('_')[1])
        if job in joblist.keys():
          joblist[job][0].append(filename)
          joblist[job][1].append(submission)
          joblist[job][2].append(size)
        else:
          joblist[job] = [[filename],[submission],[size]]

    return


# usage description
usage = """Usage: ./findDuplicateFiles.py [options]\n
Example: ./findDuplicateFiles.py -d datasetList.txt\n
For more help: ./findDuplicateFiles.py --help
"""

def main():
  # input parameters
  parser = OptionParser(usage=usage)

  parser.add_option("-d", "--dataset_list", dest="dataset_list", action='store', help="Text file containing a list of datasets to be processed", metavar="DATASET_LIST")

  (options, args) = parser.parse_args()

  # make sure all necessary input parameters are provided
  if not options.dataset_list:
    print usage
    sys.exit()

  dataset_list = options.dataset_list

  # open and read the dataset_list file
  dataset_list_file = open(dataset_list,"r")
  dataset_list_lines = dataset_list_file.readlines()

  # loop over datasets
  for line in dataset_list_lines:
    line_elements = line.split()
    if (len(line_elements)==0 or line_elements[0][0]=='#'): continue

    print 'Duplicate files for dataset %s:'%line_elements[0]

    joblist = {}
    process_input_dir(line_elements[2], joblist)

    jobs = joblist.keys()
    jobs.sort()
    for j in jobs:
      lngth = len(joblist[j][0])
      # keep the file from the latest submission or the largest file from the same submission
      if lngth>1:
        #print joblist[j]
        max_sub = max(joblist[j][1])
        sizes = []
        for i in range(lngth):
          if joblist[j][1][i]==max_sub:
            sizes.append(joblist[j][2][i])
        max_size = max(sizes)
        for i in range(lngth):
          if joblist[j][1][i]==max_sub and joblist[j][2][i]==max_size:
            continue
          print os.path.join(line_elements[2],joblist[j][0][i])

  # close all open files
  dataset_list_file.close()


if __name__ == "__main__":
  main()
