#!/usr/bin/perl

use strict;
use Net::MessageBus;
use Data::Dumper;
use YAML::Tiny;

my $config = YAML::Tiny->read( $ENV{HOME}."/.config/netboard" ) or die "No config.";
my $MessageBus = Net::MessageBus->new(
	server => $config->[0]->{server},
	port => $config->[0]->{port},
	group => 'NetBoard',
	sender => $config->[0]->{sender},
	username => $config->[0]->{username},
	password=>$config->[0]->{password},
    );


my @data=<STDIN>;

my $tosend=substr(join("",@data),0,4096);

$MessageBus->send( type => 'alert', payload => { message => $tosend }  );
