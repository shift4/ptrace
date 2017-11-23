#!/usr/bin/env perl
use 5.010;
while(<>){
	my @a = /^0*([^0][0-9a-f]+)\s+<(.*)>/g;
	say $a[0]," ",$a[1] if $#a >= 0;
}
