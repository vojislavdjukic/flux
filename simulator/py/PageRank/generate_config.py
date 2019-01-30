#!/usr/bin/python

import subprocess
import threading
import multiprocessing
import os

general_config = '../config/general_config.txt'
common_config = './templates/common.txt'

#remove old configuration files 
#map( os.unlink, (os.path.join('input',f) for f in os.listdir('input')) )

#load common configuration
if os.path.isfile(common_config):
    with open(common_config, 'r') as file:
        common=file.read()

with open(general_config, 'r') as file:
    general=file.read()

for f in os.listdir('templates'):
    if 'common' in f:
        continue
    with open(os.path.join('templates',f), 'r') as file:
        template=file.read()
    
    confFile = "./input/conf_{}".format(f)
    with open(confFile, 'w') as f:
        print confFile
        f.write(general+common+template)

print 'Finished!'
