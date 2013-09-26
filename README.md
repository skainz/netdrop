netdrop
=======

A network-enabled, shared clipboard.

Dependencies
------------

    # apt-get install libyaml-tiny-perl
    # cpan install Net::MessageBus

How to use
----------

netdrop is a client-server based mechanism for broadcasting blobs of data to
several consumers at once.  It was written to allow easy sending of
clipboard content to other machines in an office.

netdrop-server needs to be run on at least one host, and clients
need to be configured to talk to this server.

All machines willing to participate run a client.
And if you run "netdrop", STDIN will be sent to all consumers/clients.

