#!/usr/bin/perl
use strict;
use warnings;
use pinkpill::PinkPill;

my $peanuts_includes = 'peanuts/src peanuts/dependencies/boost_1_53_0 peanuts/dependencies/glsdk_0.4.4/glload/include '; 
my $pp = new PinkPill;
$pp->set_options(
    src_folders => 'src/main.cpp',
    inc_folders => $peanuts_includes . 'src',
    compiler_flags => '-std=c++11 -Wall -Wextra -Wno-unused-parameter -DPEANUTS_LINUX',
    build_folder => 'bin/',
    obj_folder => 'bin/obj',
    linker_flags => '-Lpeanuts/bin -Lpeanuts/dependencies/bin',
    link_libraries => 'peanuts X11 GL glload ',
    program_name => 'peanuts_demo',
);
$, = "\n";
print "\nError logs...\n" and print $pp->error_logs and print "\n\n" unless $pp->build;
