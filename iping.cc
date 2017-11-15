#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cmath>
#include <climits>
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("iPing");

int
main (int argc, char *argv[])
{
	NS_LOG_INFO ("iPing - User friendly ping");

	double count = UINT_MAX;
	double ttl = 0;
	double interval = 1;
	double deadline = 15;
	double packetsize = 56;
	bool verbose = false;
	bool quiet = false;

	CommandLine cmd;
	cmd.AddValue("c", "Stops after sending specified number(count) of ECHO_REQUEST packets.", count);
	cmd.AddValue("i", "Specify the interval seconds between sending each packet.", interval);
	cmd.AddValue("t", "Set the IP Time to Live.", ttl);
	cmd.AddValue("w", "Specify a timeout, in seconds, before ping exits regardless  of  how  many  packets have  been sent or received.", deadline);
	cmd.AddValue("s", "Specifies the number of data bytes to be sent.", packetsize);
	cmd.AddValue("v", "Verbose output.", verbose);
	cmd.AddValue("q", "Quiet output.  Nothing is displayed except the summary lines at  startup  time  and when finished.", quiet);
	cmd.Parse (argc, argv);

	
  NS_ABORT_MSG_IF (count <= 0, "ping: bad number of packets to transmit.");
	NS_ABORT_MSG_IF (interval < 0, "ping: bad timing interval.");
	NS_ABORT_MSG_IF (ttl < 0, "ping: bad value.");
	NS_ABORT_MSG_IF (packetsize < 0, "ping: illegal negetive packet size.");
	NS_ABORT_MSG_IF (interval < 0.2 && interval >=0, "ping: cannot flood; minimal interval allowed for user is 200ms.");

	NodeContainer c;
	c.Create (2);

	PointToPointHelper pointToPoint;
	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
	pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

	NetDeviceContainer devices;
	devices = pointToPoint.Install (c);

	Ipv4AddressHelper address;
	address.SetBase ("10.1.1.0", "255.255.255.0");
	InternetStackHelper stack;
	stack.Install (c);
	Ipv4InterfaceContainer interfaces = address.Assign (devices);

	NS_LOG_INFO ("Create V4Ping Appliation");
	Ptr<V4Ping> app = CreateObject<V4Ping> ();

	app->SetAttribute ("Remote", Ipv4AddressValue (interfaces.GetAddress(1)));
	app->SetAttribute ("Verbose", BooleanValue (verbose) );
	app->SetAttribute ("Count", UintegerValue (floor(count)));
	app->SetAttribute ("Ttl", UintegerValue (floor(ttl)));     
	app->SetAttribute ("Quiet", BooleanValue (quiet) );     
	app->SetAttribute ("Interval", TimeValue (Seconds (interval)) );
	app->SetAttribute ("Size", UintegerValue (packetsize));

	c.Get(0)->AddApplication (app);
	app->SetStartTime (Seconds (1.0));
	app->SetStopTime (Seconds (1.0 + deadline));
	
	Packet::EnablePrinting ();

	pointToPoint.EnablePcapAll("Example");
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}
