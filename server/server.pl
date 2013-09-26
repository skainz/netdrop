#!/usr/bin/perl -w

use strict;

use Net::MessageBus::Server;




sub authenticate_method {
        my ($username, $password, $client_ip) = @_;
        
        return 1 if ($username eq "john" && $password eq "1234");
        return 0;
    }


    my $MBServer = Net::MessageBus::Server->new(
                        address => '129.27.9.27',
                        port    => '15000',
                       # logger  => $logger,
                        authenticate => \&authenticate_method,
                    );
                    
    $MBServer->start();
    
 
