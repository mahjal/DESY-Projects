#!/usr/bin/env python3

import os
import sys
import time
import glob
import json
import random

work_dir = f"/afs/desy.de/user/s/school03/www/myProject/Run"
run_dir = f"{work_dir}/HTCondor_run"
output_dir = f"/afs/desy.de/user/s/school03/www/myProject"

############################################################################################
def get_shell_script(project_path, analysis_name, dataset_name, file_list, run_dir, job_id):
############################################################################################
	
	file_list_string = ''
	for hepmc_files in file_list:
		file_list_string += f'{hepmc_files} '
		
	script=''
	script+=f'''#!/bin/bash

### Job configuration ###
echo "Processing job number {job_id} ... "
CWD=`pwd -P`
mkdir -p /tmp/job_{job_id}
cd /tmp/job_{job_id}

### Rivet setup ###
source /afs/desy.de/user/s/school78/www/setup.sh

export RIVET_ANALYSIS_PATH={project_path}/build
export RIVET_REF_PATH={project_path}/analyses
export RIVET_DATA_PATH={project_path}/analyses

### Running Rivet ###
[ ! -d {project_path}/{analysis_name}_YODA ] && mkdir -p {project_path}/{analysis_name}_YODA
rivet -a {analysis_name} -o {project_path}/{analysis_name}_YODA/{dataset_name}.yoda {file_list_string}

### Cleaning ###
cd $CWD
rm -rf /tmp/job_{job_id}

echo "Shell script has finished."
'''
	
	return script

####################################################################################
def get_condor_submit_file(run_dir, nJobs):
####################################################################################
	
	script_name = run_dir + "/rivet_job"
	
	file=''
	file+=f'+RequestRuntime       = 10500\n'
	file+=f'RequestMemory         = 2000\n'
	file+=f'universe              = vanilla\n'
	file+=f'executable            = {script_name}_$(ProcId).sh\n'
	file+=f'output                = {script_name}_$(ProcId).out\n'
	file+=f'error                 = {script_name}_$(ProcId).err\n'
	file+=f'log                   = {script_name}_$(ProcId).log\n'
	file+=f'transfer_executable   = True\n'
	file+=f'queue {nJobs}\n'
	
	return file

####################################################################################
def main():
####################################################################################
	
	if(not os.path.exists(run_dir)):
		os.system(f"mkdir {run_dir}")
	else:
		os.system(f"rm -rf {run_dir}/*")
	

	with open(f"{work_dir}/datasets.json") as file_in:
		datasets = json.load(file_in)
	
	job_id=0
	
	analysis_dict = {}
	analysis_dict['CMS_2021_I1949191'] = ["WZTo3LNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8","WZTo3LNu_TuneCP5_13TeV-amcatnloFXFX-pythia8","WZTo3LNu_13TeV-powheg-pythia8"]
#	analysis_dict['CMS_2021_I1992937'] = ["DY01234jets_13TeV-sherpa","DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8","DYJetsToLL_0J_TuneCP5_13TeV-amcatnloFXFX-pythia8",\
#										  "DYJetsToLL_1J_TuneCP5_13TeV-amcatnloFXFX-pythia8","DYJetsToLL_2J_TuneCP5_13TeV-amcatnloFXFX-pythia8",\
#										  "DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8"]
	
	for analysis in analysis_dict.keys():
		
		for sample in analysis_dict[analysis]:
		
			with open(f'{run_dir}/rivet_job_{str(job_id)}.sh','w') as bash_file:
				bash_file.write(get_shell_script(output_dir, analysis, sample, datasets[sample], run_dir, job_id))
			
			job_id+=1
	
	
	with open(f'{run_dir}/rivet_jobs.submit','w') as file_out:
		file_out.write(get_condor_submit_file(run_dir, job_id))

	os.system(f'chmod u+x {run_dir}/*.sh')

	print(f'\nRivet run is ready to be submitted comprising {len(list(analysis_dict.keys()))} analys(e/i)s for a total number of jobs of {job_id} ...\n')

if __name__ == "__main__":
	main()

