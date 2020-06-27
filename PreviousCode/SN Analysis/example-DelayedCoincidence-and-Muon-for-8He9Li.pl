#! /usr/bin/perl

$BinDir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat";
$Dir     = "./muon-dcvf-A2";
$lockDir = "./lock-muon-A2";
#$lockDir = "./lock-muon-A2-10000";

$physics0 = "/data/datavf/v2-v1.04-solar-phase";

$ex = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat/example-DelayedCoincidence-and-Muon-for-8He9Li.Linux";

open(IN0,"find ${physics0}/ -name '*run*' |");
@Filename=<IN0>;
close(IN0);

foreach $f (@Filename){
  chomp $f;
  $f =~ s/.*\///;
  $gvf = $f;
  $lockfile = $f;
  $dcf = $f;

  $lockfile =~ s/\.gvf/\.lock/;
  $dcf =~ s/\.gvf/\.dcvf/;  

  $RunNumber = $f;
  $RunNumber =~ s/.*run0*([0-9]*)/\1/g;
  $RunNumber = $1;

#  ### skip new run
#  next if(6820 < $RunNumber && $RunNumber < 6890); # high dark rate period
#  next if(7930 < $RunNumber && $RunNumber < 8070); # high dark rate period
  next if(10675<$RunNumber&&$RunNumber<11000); # high dark rate period

# vf update 11.4.23
#  next if($RunNumber < 9627);
#  next if($RunNumber > 10320);

# update for runNo > 10000
#  next if($RunNumber < 10000);
#  next if($RunNumber > 10051);


#  next if($RunNumber < 10052);
#  next if($RunNumber > 10625);

  # 12.12.18 vf update
#  next if($RunNumber < 10626);
#  next if($RunNumber > 10675);

  # reprocess runs (HV10 dead period)
#  next if($RunNumber < 9792);
#  next if($RunNumber > 10016);

# vf update 13.8.27
  next if($RunNumber < 11000);
  next if($RunNumber > 11947);



  next if( -e "${BinDir}/${lockDir}/${lockfile}" );
  print "start run $RunNumber\n";

  open(OUT,"> ${BinDir}/${lockDir}/${lockfile}");
  $status=flock(OUT,6);
  next if($status==0);

  open(IN, "${ex} ${BinDir}/${Dir}/${dcf} ${physics0}/${gvf} |");
  print OUT while(<IN>);
  close(OUT);
  close(IN);

  print "finish run $RunNumber\n\n";

}
exit;


