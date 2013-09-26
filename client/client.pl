#!/usr/bin/perl

use strict;
use Getopt::Std;
use Net::MessageBus;
use Data::Dumper;

my %opt;
getopts ('bd:o:', \%opt);

my $MessageBus = Net::MessageBus->new(
  server => '129.27.9.27',
  port=>15000,
  group => 'backend',
  sender => 'simon',
  username=>'john',
  password=>'1234',
);

$MessageBus->subscribe_all;

while (my $message = $MessageBus->next_message()) {
  if ($opt{b}) { print "\007"; }
  print $message->sender()." sent ".$message->type().":\n";
  print $message->payload()->{'message'};
  chomp($message->payload()->{'message'});
  if ($opt{o}) {
    open(MYFILE, '>'.$opt{o});
  } else {
    $opt{d} = "" unless $opt{d};
    open(MYFILE, '>'.$opt{d}.'/'.$message->sender());
  }

  print  MYFILE $message->payload()->{'message'};
  close (MYFILE);
}

