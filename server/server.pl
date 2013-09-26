#!/usr/bin/perl -w

use strict;

use Net::MessageBus::Server;
use YAML::Tiny;

my $config = YAML::Tiny->read( $ENV{HOME}."/.config/netboard" ) or die "No config.";

sub authenticate_method {
        my ($username, $password, $client_ip) = @_;
        
        return 1 if ($username eq $config->[0]->{server}->{username} && $password eq $config->[0]->{server}->{password});
        return 0;
    }


    my $MBServer = Net::MessageBus::Server->new(
                        address => $config->[0]->{server}->{address},
                        port    => $config->[0]->{server}->{port},
                        authenticate => \&authenticate_method,
                    );
                    
$MBServer->start();
