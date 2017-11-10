#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("iPing");

int main (int argc, char *argv[])
{
	NS_LOG_INFO ("iPing - User friendly ping");

	string userArg ("-v");  //By defaut, output lines will be produced.

	CommandLine cmd;
	cmd.AddValue("userArg", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", userArg);
  	cmd.Parse (argc, argv);

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


	ApplicationContainer app;

	V4PingHelper ping = V4PingHelper (interfaces.GetAddress(1));

	app = ping.Install (c.Get(0), userArg);
	//app = ping.Install (c.Get(0));

	//app.Start (Seconds (1.0));       //These are not required as the starttime and stoptime are being set in v4ping-helper.cc***
	//app.Stop (Seconds (10.0));	   //However, if it is included here, it will just override those values***

	Packet::EnablePrinting ();

	pointToPoint.EnablePcapAll("Example");
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}
