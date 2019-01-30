#!/usr/bin/python

import subprocess
import threading
import multiprocessing
import os

number_of_threads = 6
template = '../../simulator 1 input/{0} > trace/result_{1}'

#generate config
os.system('python generate_config.py')

#run make
bashCommand = "make --directory ../../"
import subprocess
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()

def run_exp(rw, semaphore):
    semaphore.acquire()
    print template.format(*rw)
    subprocess.call(template.format(*rw), shell=True)
    semaphore.release()

threads = []
semaphore = threading.Semaphore(number_of_threads)

#remove old traces
map( os.unlink, (os.path.join('trace',f) for f in os.listdir('trace')) )

for f in os.listdir('input'):
    threads.append(threading.Thread(target=run_exp, args=((f, f), semaphore)))

print '\n'
[t.start() for t in threads]
[t.join() for t in threads]
print 'finished', len(threads), 'experiments'

#run logs
os.system('python draw_fct.py > logs/log.txt')
#draw stats
os.system('python draw_stats.py')
