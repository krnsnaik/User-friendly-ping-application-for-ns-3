/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "v4ping-helper.h"
#include "ns3/v4ping.h"
#include "ns3/names.h"
#include "ns3/abort.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include<iostream>

using namespace std;
namespace ns3 {

V4PingHelper::V4PingHelper (Ipv4Address remote)
{
  m_factory.SetTypeId ("ns3::V4Ping");
  m_factory.Set ("Remote", Ipv4AddressValue (remote));
}

void 
V4PingHelper::SetAttribute (string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
V4PingHelper::Install (Ptr<Node> node) const
{
     
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
V4PingHelper::Install (string nodeName) const
{
   

  Ptr<Node> node = Names::Find<Node> (nodeName);
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
V4PingHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}
      
ApplicationContainer
V4PingHelper::Install (NodeContainer c,string s) const
{


        string params[100];
int ctr=0;
        cout<<s;

   

    for (unsigned int i = 0; i<s.length(); i++){
    if (s[i] == ' ')
        ctr++;
    else
        params[ctr] += s[i];
}

        for(int i=0;i<ctr;++i)
{
        cout<<params[i]<<endl;
}
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}
Ptr<Application>
V4PingHelper::InstallPriv (Ptr<Node> node) const
{
  Ptr<V4Ping> app = m_factory.Create<V4Ping> ();
  app->SetAttribute ("Verbose", BooleanValue (true) );
 Time interPacketInterval = Seconds (2.);
 app->SetAttribute ("Interval", TimeValue (interPacketInterval) );
  node->AddApplication (app);

  return app;
}

} // namespace ns3
