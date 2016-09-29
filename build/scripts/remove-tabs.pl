#!/usr/bin/perl
# replace tabs
# usage: remove-tabs.pl fileName
while (<>)
{
  $_ =~ s/[\t+]/ /g;
  print;   
}
