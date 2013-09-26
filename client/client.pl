#!/usr/bin/perl

use strict;
use Net::MessageBus;
use Data::Dumper;


    my $MessageBus = Net::MessageBus->new(server => '129.27.9.27',
					  port=>15000,
                          group => 'backend',
                          sender => 'simon',
    username=>'john',
    password=>'1234',
    );


$MessageBus->subscribe_all;


 while (my $message = $MessageBus->next_message()) {
     print "Sender:".$message->sender()."\n";
        print $message->type()."\n";

     open(MYFILE, '>'.$message->sender());

	my $pl=$message->payload();
	
     print $pl->{'message'};
	print  MYFILE $pl->{'message'};
     close (MYFILE);

    }

#$MessageBus->subscribe_to_all();
