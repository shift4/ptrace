#!/usr/bin/env perl

use utf8;
use 5.018;

my %cg;
my $infunc;

while(<>){
	my @matches = /^0+([^0][^ ]+)\s+<(.*)>/g;
	if($#matches > 0){
		$infunc = $matches[1];
		#say $infunc;
		next;
	}else{
		my @matches2 = /(\S+):.*callq.*?(\S+) <(.*)>/g;
		#say "@matches2" if $#matches2 >= 0;
		if($#matches2 >= 0){
			$cg{$infunc} .= "$matches2[0]=$matches2[1]=$matches2[2]:";
		}else{
			@matches2 = /(\S+):.*callq.*?(\S+)/g;
			#say "@matches2" if $#matches2 >= 0;
			$cg{$infunc} .= "$matches2[0]=0=$matches2[1]:" if $#matches2 >= 0;
		}
	}
	#print $func[0]," => ",$func[1],"\n" if $#func > 0;
}

foreach (keys(%cg)){
	my @call = split(":",$cg{$_});
	my $func = $_;
	foreach (@call){
		my @pair = split("=",$_);
		say "$pair[0] $func $pair[1] $pair[2]"
	}
}

my $max = 0;
my $maxfunc;
my %calls;
foreach (keys(%cg)){
	#say $_,"===>",$cg{$_};
	my @len = split(":",$cg{$_});
	if($#len > $max){
		$maxfunc = $_;
		$max = $#len;
	}
	foreach (@len){
		$calls{$_}++;
	}
}
$max = 0;
my $leaf = 0;
#say "max calling is ",$maxfunc;
while( my ($k,$v) = each(%calls)){
	#say $k," is  called ",$v," times";
	if($v > $max){
		$max = $v;
		$maxfunc = $k;
	}
	$leaf++ if $v == 1;
}
#say "max called is ",$maxfunc," called ",$max," times";
#say "there are ",$leaf,"  leaf funcs";
#say $cg{'main'};
