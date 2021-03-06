#! /usr/bin/perl -w

# ----------------------------------------- #
#$target_dir  = "/data/work/yuhei/analysis/GammaRayBurst/dcvf";
#$target_dir  = "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat/dcvf-Kat";
#$target_dir  = "/userdata/work/koji/GW150914/vector-file-Kat/dcvf-Kat-GW";
$target_dir  = "/userdata/work/koji/GW150914/vector-file-Kat/dcvf-Kat-GW/accidental";

#$output_file = "./analysis_list/dcvf-Kat.list-2013-9626";
#$output_file = "./analysis_list/dcvf-Kat-XePhase.list-2013-11648";
#$output_file = "./analysis_list/dcvf-Kat.list-2013-10675";
#$output_file = "./analysis_list/dcvf-Kat.list-2013-all";
#$output_file = "./analysis_list/dcvf-Kat.list-2013-Period2";
#$output_file = "./analysis_list/dcvf-Kat.list-2013-179-11947";
#$output_file = "./analysis_list/dcvf-Kat.list-GW";
$output_file = "./analysis_list/dcvf-Kat.list-GW-accidental";

#$RUN_MIN = 179; # fix
#$RUN_MAX = 9626;
#$RUN_MAX = 10675;
#$RUN_MAX = 11648;
#$RUN_MAX = 11947; #130829 update


#period1
#$RUN_MIN = 179; # fix
#$RUN_MAX = 6820;

#period2
#$RUN_MIN = 6890; # fix
#$RUN_MAX = 10675;


#XePhase
$RUN_MIN = 13000; # fix
$RUN_MAX = 14000;

#GW analysis
#$RUN_MIN = 13415; # fix
#$RUN_MAX = 13430;

# ----------------------------------------- #

open (OUT, "> $output_file");

for ($Run = $RUN_MIN ; $Run <= $RUN_MAX ; $Run++ ) {
   $target_file = sprintf "$target_dir/run%06d.dcvf", $Run;

    if ($Run <= 6820 || ($Run >= 6890 && $Run <= 7930) || ($Run >= 8070 && $Run <= 10675) || $Run >= 11000) {

	if (-e $target_file) {
	    print OUT "$Run $target_file\n";
	}
    }
}


system "wc -l $output_file";

exit;
