//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


package inet.applications.tcpapp;

import inet.applications.contract.IApp;

//
// Accepts any number of incoming TCP connections, and discards whatever
// arrives on them. Compatible with both ~Ipv4 and ~Ipv6.
//
module TcpServerBusApp extends TcpServerHostApp
{
    parameters:
        serverThreadModuleType = default("inet.applications.tcpapp.TcpServerBusAppThread");
        @class(TcpServerBusApp);
        @display("i=block/sink");
        
        @signal[numMsgTCPReceiveServer](type="int");
        
        @signal[fresh_AoI_priority_1](type="double");
        @signal[fresh_AoI_priority_2](type="double");
        @signal[fresh_AoI_priority_3](type="double");
        @signal[fresh_AoI_priority_4](type="double");
        @signal[fresh_AoI_priority_wifi](type="double");
        
        
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none; checkSignals=false);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none; checkSignals=false);

		@statistic[numMsgTCPReceiveServer](title="TCP packet received on server"; source="numMsgTCPReceiveServer"; record=vector,count,last);
		
		@statistic[fresh_AoI_priority_1](title= "AoI1 [s]"; source="fresh_AoI_priority_1"; record=vector,last,mean);
		@statistic[fresh_AoI_priority_2](title= "AoI2 [s]"; source="fresh_AoI_priority_2"; record=vector,last,mean);
		@statistic[fresh_AoI_priority_3](title= "AoI3 [s]"; source="fresh_AoI_priority_3"; record=vector,last,mean);
		@statistic[fresh_AoI_priority_4](title= "AoI4 [s]"; source="fresh_AoI_priority_4"; record=vector,last,mean);
		@statistic[fresh_AoI_priority_wifi](title= "AoIW [s]"; source="fresh_AoI_priority_wifi"; record=vector,last,mean);
		
}

simple TcpServerBusAppThread
{
    parameters:
        @display("i=block/sink");
        @signal[packetReceived](type=inet::Packet);
        @statistic[packetReceived](title="packets received"; source=packetReceived; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[endToEndDelay](title="end-to-end delay"; source="dataAge(packetReceived)"; unit=s; record=histogram,weightedHistogram,vector; interpolationmode=none);
}

