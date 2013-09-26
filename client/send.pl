#!/usr/bin/perl

use Net::MessageBus;
use Data::Dumper;


    my $MessageBus = Net::MessageBus->new(
	server => '129.27.9.27',
	port=>15000,
	group => 'backend',
	sender => 'simon',
	username=>'john',
	password=>'1234',
    );


my @data=<STDIN>;

my $tosend=substr(join("",@data),0,4096);

$MessageBus->send( type => 'alert', payload => { message => $tosend }  );
