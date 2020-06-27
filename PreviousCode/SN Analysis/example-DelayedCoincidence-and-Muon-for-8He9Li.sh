#! /bin/sh

. /home/yukiko/.bashrc


cd /data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat

( ./example-DelayedCoincidence-and-Muon-for-8He9Li.pl ) >  $argv[1] 2>&1
