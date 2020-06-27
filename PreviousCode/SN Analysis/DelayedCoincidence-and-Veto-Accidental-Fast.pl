#! /usr/bin/perl

#$BinDir  = "/data/work/hiroko/ReactorAnalysis/vector-file-A2";
#$BinDir  = "/data/work/yukiko/8B/vector-file-Kat";
$BinDir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat";
#$fileDir = "/data/work/hiroko/ReactorAnalysis/vector-file-A2";
#$fileDir = "/data/work/yukiko/8B/vector-file-Kat";
$fileDir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat";

$Dir     = "dcvf-Kat-accidental";
$lockDir = "lock-dcvf-Kat-accidental";

$physics0="/data/datavf/v2-v1.04-solar-phase";

$ex = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat/DelayedCoincidence-and-Veto-Accidental-Fast.Linux";

open(IN0,"find ${physics0}/ -name '*run*' |");
@Filename=<IN0>;
close(IN0);

#@Filename=(@Filename0);

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

  ### skip new run
  next if(6820<$RunNumber&&$RunNumber<6890); # high dark rate period
  next if(7930<$RunNumber&&$RunNumber<8070); # high dark rate period
  next if(10675<$RunNumber&&$RunNumber<11000); # high dark rate period

  # 11.4.22 vf update
#  next if($RunNumber < 9627);
#  next if($RunNumber > 10320);


  # 11.4.22 vf update
#  next if($RunNumber < 10000);
#  next if($RunNumber > 10051);


  # 11.8.4 vf update
#  next if($RunNumber < 10051);
#  next if($RunNumber > 10625);

  # 12.12.18 vf update
#  next if($RunNumber < 10626);
#  next if($RunNumber > 10675);

  # reprocess runs (HV10 dead period)
  #next if($RunNumber < 9792);
  #next if($RunNumber > 10016);

# 13.6.07 vf update
  next if($RunNumber < 179);
  next if($RunNumber > 11789);


  next if( -e "${fileDir}/${lockDir}/${lockfile}" );
  print "start run $RunNumber\n";

  open(OUT,"> ${fileDir}/${lockDir}/${lockfile}");
  $status=flock(OUT,6);
  next if($status==0);

  open(IN, "${ex} ${fileDir}/${Dir}/${dcf} ${physics0}/${gvf} |");
  print OUT while(<IN>);
  close(OUT);
  close(IN);

  print "finish run $RunNumber\n\n";

}
exit;


