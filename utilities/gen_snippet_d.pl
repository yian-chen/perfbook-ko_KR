#!/usr/bin/perl
# SPDX-License-Identifier: GPL-2.0-or-later
#
# Generate CodeSamples/snippets.d
#
# Note: Output file name is specified in gen_snippet_d.sh
#
# Copyright (C) Akira Yokosawa, 2018
#
# Authors: Akira Yokosawa <akiyks@gmail.com>

use strict;
use warnings;

my @fcvsources;
my $snippet_key;
my $source;
my $src_under_sub;

$snippet_key = '\\begin\{snippet\}' ;
@fcvsources = `grep -l -r -F $snippet_key CodeSamples` ;
chomp @fcvsources ;

print "# Do not edit!\n" ;
print "# Generated by utilities/gen_snippet_d.pl.\n\n" ;
print "FCVSNIPPETS = " ;
foreach $source (@fcvsources) {
    my @snippet_commands1 ;
    my $subdir ;
    my $snippet ;
    @snippet_commands1 = `grep -F $snippet_key $source` ;
    chomp @snippet_commands1 ;
    $source =~ m!.*/([^/]+)/[^/]+! ;
    $subdir = $1 ;
    foreach $snippet (@snippet_commands1) {
	$snippet =~ /labelbase=.*:(.+:[^,\]]+)[,\]]/ ;
	$_ = $1;
	s/:/@/g ;
	print "\\\n\tCodeSamples/$subdir/$_.fcv ";
    }
}

print "\n\nEXTRACT = utilities/fcvextract.pl\n\n" ;

foreach $source (@fcvsources) {
    my @snippet_commands2 ;
    my $src_under_sub ;
    my $subdir ;
    my $snippet ;
    @snippet_commands2 = `grep -F $snippet_key $source` ;
    chomp @snippet_commands2 ;
    $src_under_sub = $source ;
    $source =~ m!(.*/[^/]+)/[^/]+! ;
    $subdir = $1 ;
#    print @snippet_commands ;
    foreach $snippet (@snippet_commands2) {
	$snippet =~ /labelbase=.*:(.+:[^,\]]+)[,\]]/ ;
	if (not defined $1) {
	    die("Oops! Please try \"make clean; make\".\n") ;
	}
	$_ = $1;
	s/:/@/g ;
	print "$subdir/$_.fcv: $src_under_sub \$\(EXTRACT\)\n";
    }
}
