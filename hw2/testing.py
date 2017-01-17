#!/usr/bin/python

import subprocess
import os
import signal
from time import sleep



def sighandler(signum, frame):
    print "received SIGUSR2"

signal.signal(signal.SIGUSR2, sighandler)

subprocess.call("gcc send_signals.c", shell=True)
proc = subprocess.Popen("./a.out")

pid = proc.pid


sleep(1)
proc.send_signal(signal.SIGUSR1)
sleep(1)
proc.send_signal(signal.SIGUSR1)
sleep(1)
proc.send_signal(signal.SIGUSR1)
sleep(1)
proc.send_signal(signal.SIGINT)
sleep(1)
