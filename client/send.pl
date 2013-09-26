#!/usr/bin/perl

use strict;
use Net::MessageBus;
use Data::Dumper;
use YAML::Tiny;

my $config = YAML::Tiny->read( $ENV{HOME}."/.config/netboard" ) or die "No config.";
my $MessageBus = Net::MessageBus->new(
  server => $config->[0]->{client}->{server},
  port => $config->[0]->{client}->{port},
  group => 'NetBoard',
  sender => $config->[0]->{client}->{sender},
  username => $config->[0]->{client}->{username},
  password=>$config->[0]->{client}->{password},
);

my @data=<STDIN>;

my $tosend=substr(join("",@data),0,4096);

$MessageBus->send( type => 'alert', payload => { message => $tosend }  );
