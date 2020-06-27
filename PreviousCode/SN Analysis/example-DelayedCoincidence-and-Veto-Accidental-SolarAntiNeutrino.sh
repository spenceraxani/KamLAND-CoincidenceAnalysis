#! /bin/sh

. /home/yukiko/.bashrc


cd /data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat

( ./example-DelayedCoincidence-and-Veto-Accidental-SolarAntiNeutrino.pl ) >  $argv[1] 2>&1
