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
static void PingRtt (std::string context, Time rtt)
{
  std::cout << context << " " << rtt << std::endl;
}
int
main (int argc, char *argv[])
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

	V4PingHelper ping = V4PingHelper (interfaces.GetAddress (1));
	apps = ping.Install (c);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (3.0));

	Packet::EnablePrinting ();

Config::Connect ("/NodeList/*/ApplicationList/*/$ns3::V4Ping/Rtt",
                   MakeCallback (&PingRtt));



  pointToPoint.EnablePcapAll("Example");
  Simulator::Run ();
  Simulator::Destroy ();
return 0;
}
