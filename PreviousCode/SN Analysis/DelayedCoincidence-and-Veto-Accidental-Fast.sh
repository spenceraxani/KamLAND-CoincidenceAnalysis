#! /bin/sh

source /home/yukiko/.bashrc

cd /data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat

RunMin=$1
RunMax=$2


#(./DelayedCoincidence-and-Veto-Accidental-Fast.pl) > $1 2>&1

#root="./result/DelayedCoincidence-and-Veto-Accidental_Rall_"${RunMin}"-"${RunMax}".root"
#log="./log/DelayedCoincidence-and-Veto-Accidental-Rall2_"${RunMin}"-"${RunMax}".log"
log="./log/DelayedCoincidence-and-Veto-Accidental-v3-"${RunMin}"-"${RunMax}".log"
#log="./log/test-"${RunMin}"-"${RunMax}".log"

# (./DelayedCoincidence-and-Veto-Accidental-Fast.Linux $root $RunMin $RunMax ) > $log 2>&1
 (./DelayedCoincidence-and-Veto-Accidental-Fast.Linux $RunMin $RunMax ) > $log 2>&1

echo " " 

#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_000001-000500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_000501-001000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_001001-001500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_001501-002000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_002001-002500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_002501-003000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_003001-003500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_003501-004000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_004001-004500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_004501-005000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_005001-005500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_005501-006000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_006001-006500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_006501-007000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_007001-007500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_007501-008000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_008001-008500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_008501-009000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_009001-009500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_009501-010000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_010001-010500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_010501-011000.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_011001-011500.root) > $1 2>&1
#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/DelayedCoincidence-and-Veto-Accidental_Rall_011501-011648.root) > $1 2>&1


#(./DelayedCoincidence-and-Veto-Accidental-Fast.Linux ./result/test5030-5035.root) > $1 2>&1