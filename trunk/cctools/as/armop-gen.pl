#!/usr/bin/perl
use strict;
use warnings;
use Data::Dumper qw(Dumper);

sub parse_encoding
{
    my ($str) = @_;
    my $result = 0;

    foreach my $chunk (split /,/, $str) {
        $chunk =~ /^\s*(\d+)\s*\((\d+)\)\s*$/ or die
            "Parse error in encoding specification";
        $result |= (oct "0b$1") << $2;
    }

    return $result;
}

sub build_ops_from_spec
{
    my ($spec, $type, $enc, $ops, $defs) = @_;

    my @substs = $spec =~ /\{([^}]+)\}/g;
    if (!@substs) {
        $ops->{$spec} = { enc => $enc, type => $type };
        return;
    }

    foreach my $subst (@substs) {
        my $def = $defs->{$subst};
        die "Substitution '$subst' not defined anywhere" if not defined $def;

        foreach my $repl (keys %$def) {
            my $result = $spec;
            $result =~ s/\{\Q$subst\E\}/$repl/g;
            build_ops_from_spec($result, $type, $enc | ($def->{$repl}{enc}),
                $ops, $defs);
        }
    }
}

open F, 'armop.in' or die $!;

my $defs = {};

# Look for definitions.
while (my $line = <F>) {
    next unless $line =~ /^\s*\.define\s*(.+)$/;
    my $def_name = $1;
    my $def = {};
    while ($line = <F>) {
        last if $line =~ /^\s*\.enddefine/;
        next unless $line =~ /^(\S+)\s+e:(.*)$/;
        my ($str, $enc) = ($1, $2);
        $str = '' if $str eq '*';
        $def->{$str} = { enc => parse_encoding($enc) };
    }
    $defs->{$def_name} = $def;
}

seek F, 0, 0;

my $ops = {};
# Now build up the list of ops.
while (my $line = <F>) {
    chomp $line;
    $line =~ s/^\s*//;
    if ($line =~ /^\.define/) {
        $line = <F> until $line =~ /^\.enddefine/;
        next;
    }
    next if $line eq '' or $line =~ /^#/;   # skip blank lines and comments

    $line =~ /^(\S+)\s*t:\s*(\w+)\s*e:(.*)$/ or die
        "Parse error in op declaration";
    my ($spec, $type, $enc_spec) = ($1, $2, $3);
    my $enc = parse_encoding($enc_spec);

    build_ops_from_spec($spec, $type, $enc, $ops, $defs);
}

close F;

open OUT, '>', 'armop.c';
print OUT "/* Automatically generated by $0 from armop.in.\n";
print OUT " * Do not edit by hand! */\n\n";
print OUT qq/#include "arm.h"\n\n/;

my $count = scalar keys %$ops;
print OUT "int arm_op_count = $count;\n";

print OUT "struct arm_op_info[] = {\n";

my $n = 0;
foreach my $op (sort keys %$ops) {
    printf OUT '    { "%s", %s, 0x%08x }%s%s', $op, $ops->{$op}{type},
        $ops->{$op}{enc}, (++$n == $count ? '' : ','), $/;
}

print OUT "};\n\n"; 

close OUT;

