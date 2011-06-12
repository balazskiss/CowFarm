#!/usr/bin/perl -wT

use Socket;
use IO::Handle;

my $port = 843;
my $proto = getprotobyname('tcp');

# start the server:
socket(Server, PF_INET, SOCK_STREAM, $proto) or die "socket: $!";
setsockopt(Server, SOL_SOCKET, SO_REUSEADDR, 1 ) or die "setsockopt: $!";
bind(Server,sockaddr_in($port,INADDR_ANY)) or die "bind: $!";
listen(Server,SOMAXCONN) or die "listen: $!";

Server->autoflush( 1 );

my $paddr;

$/ = "\0";      # reset terminator to null char

# listening loop.

for ( ; $paddr = accept(Client,Server); close Client) {
    Client->autoflush(1);
    my($port,$iaddr) = sockaddr_in($paddr);
    my $ip_address   = inet_ntoa($iaddr);
    my $name         = gethostbyaddr($iaddr,AF_INET) || $ip_address;
 
    my $line = <Client>;

    if ($line =~ /.*policy\-file.*/i) {
        print Client &xml_policy;
    }
}

sub xml_policy {
    my $str = qq(<cross-domain-policy><allow-access-from domain="*" to-ports="*" /></cross-domain-policy>\0);
    return $str;
}