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
 
    map<char,uint32_t> setflags(string s)
    {
        map <char, uint32_t> iping;
        string t, value;

        // The below code will extract the options from the string and gets stored in map 
        iping.insert(pair <char, uint32_t> ('c', 0));
        iping.insert(pair <char, uint32_t> ('i', 1));
        iping.insert(pair <char, uint32_t> ('q', 0));
        iping.insert(pair <char, uint32_t> ('t', 0));
        iping.insert(pair <char, uint32_t> ('s', 56));
        iping.insert(pair <char, uint32_t> ('v', 0));
        iping.insert(pair <char, uint32_t> ('w', 15));

	    
	    for(int i=0;s[i];i++)
	    {
		    if(s[i]=='-' || s[i]==' ') continue;
		    t+=s[i];
	    }
	
        for(int i=0;t[i];i++)
        {
            if(t[i]=='c' || t[i]=='t' || t[i]=='i' || t[i]=='w' || t[i]=='s')
		    {
			    int j=i;
                uint32_t k;
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
                    if(k<0)
                        NS_ABORT_MSG ("ping: bad value");
                    iping[t[j]] = k;
                }
		    }
		    else if(t[i]=='v' || t[i]=='q')
			    iping[t[i]] = 1;
		    else
			    NS_ABORT_MSG ("These are the only options available currently: [-qv] [-c count] [-i interval] [-s size] [-t ttl] [-w deadline]");
	    }
        return iping;
    }

    ApplicationContainer V4PingHelper::Install (Ptr<Node> node) const
    {
        return ApplicationContainer (InstallPriv (node));
    }

    //Below function will be called if arguments are passed.
    ApplicationContainer V4PingHelper::Install (Ptr<Node> node, string s) const
    {
        map <char, uint32_t> iping = setflags(s);
        return ApplicationContainer (InstallPriv (node, iping) );
    }

    ApplicationContainer V4PingHelper::Install (string nodeName) const
    {
        Ptr<Node> node = Names::Find<Node> (nodeName);
        return ApplicationContainer (InstallPriv (node));
    }

    //Below function will be called if arguments are passed.
    ApplicationContainer V4PingHelper::Install (string nodeName,string s) const
    {
        Ptr<Node> node = Names::Find<Node> (nodeName);
        map <char, uint32_t> iping = setflags(s);
        return ApplicationContainer (InstallPriv (node, iping) );
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
    
    Ptr<Application> V4PingHelper::InstallPriv (Ptr<Node> node, map<char,uint32_t> iping) const
    {
        Ptr<V4Ping> app = m_factory.Create<V4Ping> ();
        app->SetAttribute ("Count", UintegerValue (iping['c']));
        app->SetAttribute ("Ttl", UintegerValue (iping['t']));
        app->SetAttribute ("Verbose", BooleanValue (iping['v']) );     
        app->SetAttribute ("Quiet", BooleanValue (iping['q']) );     
        app->SetAttribute ("Interval", TimeValue (Seconds (iping['i'])) );
        app->SetAttribute ("Size", UintegerValue (iping['s']));
        Time deadline = Seconds (iping['w']);
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
