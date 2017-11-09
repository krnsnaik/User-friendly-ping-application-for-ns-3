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

/*static void PingRtt (string context, Time rtt)
{
  cout << context << " " << rtt << endl;
}*/
int main (int argc, char *argv[])
{
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


	ApplicationContainer apps;
	string S="-v -i 10 -c 10";

	V4PingHelper ping = V4PingHelper (interfaces.GetAddress (1));

	apps = ping.Install (c,S);
	//apps = ping.Install (c);

	apps.Start (Seconds (1.0));
	apps.Stop (Seconds (10.0));

	Packet::EnablePrinting ();

	pointToPoint.EnablePcapAll("Example");
	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}
