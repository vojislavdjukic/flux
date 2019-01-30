import os
import subprocess

local_dir = subprocess.check_output(['pwd'])

local_dir = local_dir[:-1]

def open_and_run(open_dir, run_command):
    command = 'cd %s/%s && python %s'%(local_dir, open_dir, run_command)
    print command
    os.system(command)


experiments = [
    'KMeans',
    'PageRank',
    'SGD',
    'web_server',
    'tensorflow'
]

for experiment in experiments:
    open_and_run(experiment,'run_experiments.py')
