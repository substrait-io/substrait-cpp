#!/bin/perl -w
# SPDX-License-Identifier: Apache-2.0

# Renames package declarations for protobuffers from substrait to substrait.proto.
# This allows us to modify where the generated C++ have their definitions without
# affecting existing customers of the protobuf.

while (<>) {
	s|^(package substrait)|$1.proto|;
	s!^(import "substrait/)(.*proto\")!$1proto/$2!;
	s|substrait\.extensions|substrait.proto.extensions|g;
	print;
}
