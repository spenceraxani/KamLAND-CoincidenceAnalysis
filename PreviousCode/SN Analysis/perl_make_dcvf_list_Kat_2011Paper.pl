#! /usr/bin/perl -w

# ----------------------------------------- #
$target_dir  = "/data/work/shimizu/tmp/dcvf-Kat";
$output_file = "./analysis_list/dcvf-Kat.list-2011Paper";


$RUN_MIN = 179; # fix
$RUN_MAX = 10625;
# ----------------------------------------- #

open (OUT, "> $output_file");

for ($Run = $RUN_MIN ; $Run <= $RUN_MAX ; $Run++ ) {
   $target_file = sprintf "$target_dir/run%06d.dcvf", $Run;

    if ($Run <= 6820 || ($Run >= 6890 && $Run <= 7930) || $Run >= 8070) {

	if (-e $target_file) {
	    print OUT "$Run $target_file\n";
	}
    }
}

system "wc -l $output_file";

exit;
