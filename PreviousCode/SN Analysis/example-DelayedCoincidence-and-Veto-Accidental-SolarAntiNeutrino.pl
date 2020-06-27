#! /usr/bin/perl

$ex = "./example-DelayedCoincidence-and-Veto-Accidental-SolarAntiNeutrino.Linux";
#$BinDir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat";
#$OutDir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat/accidental-Kat2";
$BinDir  = "/userdata/work/koji/GW150914/vector-file-Kat";
$OutDir = "/userdata/work/koji/GW150914/vector-file-Kat/accidental-Kat2";


$VFList = "./v2-v1.04-solar-phase.list";

$ok = -e "$ex";
if(!$ok){
    print "No file $ex\n";
    exit;
}

$ok = -e "$OutDir";
if(!$ok){
    print "No file $OutDir\n";
    exit;
}

$N=0;
open(VF,"${VFList}");
while(<VF>){
    chomp;

    $GVF = $_;

    $run = $GVF;
    $run =~ s/.*(run0*[0-9]*)\.gvf/\1/g;

    $run_number = $run;
    $run_number =~ s/run0*([0-9]*)/\1/g;

    #next if($run_number>9626);
    #next if($run_number>9087);
    #next if($run_number>6801);
    #next if($run_number<6802);
    #next if($run_number<6940);
    next if($run_number<11020);

    next if($run_number>11947);


    $file = "${OutDir}/${run}.root";
    $log =  "${OutDir}/${run}.log";

    next if( -e "${log}");

    open(OUT,"> ${log}");
    $status=flock(OUT,6);
    next if($status==0);
    open(IN, "${ex} ${file} $GVF 2>&1 |");
    print OUT while(<IN>);
    close(OUT);
    close(IN);

    #check status
    if($?>0){
        print "ERROR: error status -1\n";
        print "  -->  stop perl process\n";
        exit;
    }
}
exit;
