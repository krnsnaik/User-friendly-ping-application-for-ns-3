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
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/v4ping.h"
#include "ns3/names.h"
#include <string>
#include <map>
#include <iterator>
#include <sstream>

using namespace std;
namespace ns3 {

    V4PingHelper::V4PingHelper (Ipv4Address remote)
    {
        m_factory.SetTypeId ("ns3::V4Ping");
        m_factory.Set ("Remote", Ipv4AddressValue (remote));
    }

    void  V4PingHelper::SetAttribute (std::string name, const AttributeValue &value)
    {
        m_factory.Set (name, value);
    }

    ApplicationContainer V4PingHelper::Install (Ptr<Node> node) const
    {
        return ApplicationContainer (InstallPriv (node));
    }

    ApplicationContainer V4PingHelper::Install (Ptr<Node> node, string s) const
    {

        // The below code will extract the options from the string and gets stored in map 
        string t, value;
	    map <char, int> iping;
        
        iping.insert(pair <char, int> ('c', 0));
        iping.insert(pair <char, int> ('i', 1));
        iping.insert(pair <char, int> ('q', 0));
        iping.insert(pair <char, int> ('t', 0));
        iping.insert(pair <char, int> ('v', 0));
        iping.insert(pair <char, int> ('w', 15));

	    map <char, int> :: iterator itr;
	    for(int i=0;s[i];i++)
	    {
		    if(s[i]=='-' || s[i]==' ') continue;
		    t+=s[i];
	    }
	
        for(int i=0;t[i];i++)
        {
            if(t[i]=='c' || t[i]=='t' || t[i]=='i' || t[i]=='w')
		    {
			    int j=i,k;
			    value="";
			    while(t[i+1]>='0' && t[i+1]<='9' && t[i+1])
			    {
				    value+=t[i+1];
				    i++;
			    }
			    stringstream str(value);
                if(value.length()==0)
                    NS_ABORT_MSG ("ping: bad value");
                else
                {
			        str >> k;
			        iping[t[j]] = k;
                }
		    }
		    else if(t[i]=='v' || t[i]=='q')
			    iping[t[i]] = 1;
		    else
			    NS_ABORT_MSG ("These are the only options available currently: [-qv] [-c count] [-i interval] [-t ttl] [-w deadline]");
	    }


        return ApplicationContainer (InstallPriv (node, iping['c'], iping['i'], iping['q'], iping['t'], iping['v'], iping['w']) );
    }

    ApplicationContainer V4PingHelper::Install (string nodeName) const
    {
        Ptr<Node> node = Names::Find<Node> (nodeName);
        return ApplicationContainer (InstallPriv (node));
    }

    ApplicationContainer V4PingHelper::Install (NodeContainer c) const
    {
        ApplicationContainer apps;
        for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
        {
            apps.Add (InstallPriv (*i));
        }

        return apps;
    }

    Ptr<Application> V4PingHelper::InstallPriv (Ptr<Node> node, int c, int i, int q, int t, int v, int w) const
    {
        Ptr<V4Ping> app = m_factory.Create<V4Ping> ();
        app->SetAttribute ("Count", UintegerValue (c));
        app->SetAttribute ("Ttl", UintegerValue (t));
        app->SetAttribute ("Verbose", BooleanValue (v) );     
        app->SetAttribute ("Quiet", BooleanValue (q) );     
        app->SetAttribute ("Interval", TimeValue (Seconds (i)) );
        Time deadline = Seconds (w);
        app->SetStartTime (Seconds (1.0));
        app->SetStopTime (deadline);
                
        node->AddApplication (app);
        return app;
    }

    Ptr<Application> V4PingHelper::InstallPriv (Ptr<Node> node) const
    {
        Ptr<V4Ping> app = m_factory.Create<V4Ping> ();
        
        node->AddApplication (app);
        return app;
    }

}
