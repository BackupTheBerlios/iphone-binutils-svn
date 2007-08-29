#!/usr/bin/perl
use strict;
use warnings;

if (@ARGV < 2) {
    print STDERR "usage: $0 input-file output-file\n";
    exit 1;
}

open F, $ARGV[0] or die $!;
open OUT, '>', $ARGV[1] or die $!;

my %macro_library;
my ($cur_macro, $macro_args);
$macro_args = [];

while (<F>) {
    chomp;
    if (/\.macro\s*(\w+)\s*(.*)/i) {
        die "nested macro" if defined $cur_macro;
        $cur_macro = $1;

        $macro_args = [];
        @$macro_args = split /[, \t\n]+/, $2;

        my @defaults;
        for (my $i = 0; $i < @$macro_args; $i++) {
            if ($macro_args->[$i] =~ /^(\w+)=(.*)$/) {
                $macro_args->[$i] = $1;
                $defaults[$i] = $2;
            }
        }

        $macro_library{$cur_macro} =
            {
                args => $macro_args,
                defaults => \@defaults
            };

        print OUT "\.macro $cur_macro\n";
    } elsif (/\.endm/) {
        undef $cur_macro;
        $macro_args = [];

        print OUT "\.endmacro\n";
    } else {
        my $line = $_;

        for (my $i = 0; $i < @$macro_args; $i++) {
            $line =~ s/\\$macro_args->[$i]/\$$i/g;
        }

        print OUT "$line\n";
    }
}

close OUT;
close F; 

