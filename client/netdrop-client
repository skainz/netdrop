#!/usr/bin/perl

use strict;
use Getopt::Std;
use Net::MessageBus;
use Data::Dumper;
use YAML::Tiny;

my %opt;
getopts ('bc:d:o:', \%opt);

my $config = YAML::Tiny->read($opt{c} or $ENV{HOME}."/.config/netboard") or die "No config.";
$opt{o} or $opt{o} = $config->[0]->{client}->{"output-file"};
my $MessageBus = Net::MessageBus->new(
  server => $config->[0]->{client}->{server},
  port => $config->[0]->{client}->{port},
  group => 'NetBoard',
  sender => $config->[0]->{client}->{sender},
  username => $config->[0]->{client}->{username},
  password=>$config->[0]->{client}->{password},
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

