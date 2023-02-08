#!/bin/perl -w

while (<>) {
	s!^(#include.*"substrait/)(.*pb.h\")!$1proto/$2!;
	print;
}
