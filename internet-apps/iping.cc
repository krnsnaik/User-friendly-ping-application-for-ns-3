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
using namespace std;

NS_LOG_COMPONENT_DEFINE ("iPing");

int main (int argc, char *argv[])
{
	NS_LOG_INFO ("iPing - User friendly ping");

	uint32_t count = 0;
	uint32_t ttl = 0;
	uint32_t interval = 1;
	uint32_t deadline = 15;
	uint32_t packetsize ;
	bool verbose = false;
	bool quiet = false;

	CommandLine cmd;
  	cmd.AddValue("c", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", count);
  	cmd.AddValue("i", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", interval);
  	cmd.AddValue("t", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", ttl);
  	cmd.AddValue("w", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", deadline);
  	cmd.AddValue("s", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", packetsize);
  	cmd.AddValue("v", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", verbose);
  	cmd.AddValue("q", "Pass arguments in this format:[-qv] [-c count] [-i interval] [-t ttl] [-w deadline]", quiet);
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


	NS_LOG_INFO ("Create V4Ping Appliation");
	Ptr<V4Ping> app = CreateObject<V4Ping> ();

  	Config::SetDefault ("ns3::V4Ping::Size",   UintegerValue (70));
	app->SetAttribute ("Remote", Ipv4AddressValue (interfaces.GetAddress(1)));
	app->SetAttribute ("Verbose", BooleanValue (verbose) );
	app->SetAttribute ("Count", UintegerValue (count));
	app->SetAttribute ("Ttl", UintegerValue (ttl));     
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
