Internet Applications Module Documentation
------------------------------------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

The goal of this module is to hold all the Internet-specific applications,
and most notably some very specific applications (e.g., ping) or daemons (e.g., radvd).  Other non-Internet-specific applications such as packet generators
are contained in other modules.

Model Description
*****************

The source code for the new module lives in the directory ``src/internet-apps``.

Each application has its own goals, limitations and scope, which are briefly explained
in the following.

V4Ping
======

This app mimics a "ping" (ICMP Echo) using IPv4. The application allows the 
following attributes to be set:

* Remote address
* Verbose mode
* Packet size (default 56 bytes)
* Packet interval  (default 1 second)
* Max no of packets to be sent (default 0)
* set ttl (default 0)
* Quiet mode

Moreover, the user can access the measured rtt value (as a Traced Source).

Ping6
=====

This app mimics a "ping" (ICMP Echo) using IPv6. The application allows the 
following attributes to be set:

* Remote address
* Local address (sender address)
* Packet size (default 56 bytes)
* Packet interval  (default 1 second)
* Max number of packets to send

Radvd
=====

This app mimics a "RADVD" daemon. I.e., the daemon responsible for IPv6 routers 
advertisements. All the IPv6 routers should have a RADVD daemon installed.

The configuration of the Radvd application mimics the one of the radvd Linux program.

Examples and use
****************

All the applications are extensively used in the top-level ``examples`` 
directories. The users are encouraged to check the scripts therein to have a 
clear overview of the various options and usage tricks.

Iping 
=====
This is an user friendly applications with the following functionalities :
*  -c count. Stop after sending count ECHO_REQUEST packets. With deadline option, ping waits for count ECHO_REPLY packets, until the timeout expires.
*  -i interval. Wait interval seconds between sending each packet.  The default is to wait for one second between each packet normally, or not to wait in flood mode. Only super-user may  set  interval 			to values less than 0.2 seconds.
*  -q Quiet output. Nothing is displayed except the summary lines at startup time and when finished.
*  -s packetsize. Specifies the number of data bytes to be sent.  The default is 56, which translates into 64 ICMP data bytes when combined with the 8 bytes of ICMP header data.
*  -t ttl ping only.  Set the IP Time to Live.
*  -v  Verbose output.
*  -w deadline. Specify a timeout, in seconds, before ping exits regardless of how many packets have been sent or received. In this case ping does not stop after count packet are sent, it waits either
  		for deadline expire or until count probes are answered or for some error notification from network.

 
