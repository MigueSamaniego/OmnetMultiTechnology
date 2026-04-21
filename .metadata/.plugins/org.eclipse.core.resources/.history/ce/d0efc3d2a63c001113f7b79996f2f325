// 
//                  Simu5G
//
// Authors: Giovanni Nardini, Giovanni Stea, Antonio Virdis (University of Pisa)
// 
// This file is part of a software released under the license included in file
// "license.pdf". Please read LICENSE and README files before using it.
// The above files and the present reference are part of the software itself, 
// and cannot be removed from it.
//
package simulations.BusCosenza.cars;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.inet.AdhocHost;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth10G;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211RadioMedium;
import inet.physicallayer.wireless.ieee80211.packetlevel.Ieee80211DimensionalRadioMedium;

import inet.node.ethernet.EthernetSwitch;

import org.car2x.veins.modules.world.annotations.AnnotationManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.visualizer.roads.RoadsCanvasVisualizer;
//import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.modules.obstacle.ObstacleControl;
import inet.visualizer.canvas.integrated.IntegratedCanvasVisualizer;

import simu5g.world.radio.LteChannelControl;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.PgwStandard;
import simu5g.common.binder.Binder;
import simu5g.nodes.eNodeB;
//import simu5g.nodes.Ue;
import src.node_bus.cars.BusGW;




import org.car2x.veins.subprojects.veins_inet.VeinsInetManager;

network Highway
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        @display("bgb=$playgroundSizeX,$playgroundSizeY");

    submodules:
        obstacles: ObstacleControl {
            @display("p=240,50");
        }
        //radioMedium: Ieee80211RadioMedium {
        //    @display("p=100,100");
        //}
        annotations: AnnotationManager {
            @display("p=260,50");
        }
        //        connectionManager: ConnectionManager {
        //            parameters:
        //                @display("p=150,0;i=abstract/multicast");
        //        }
        radioMedium: Ieee80211DimensionalRadioMedium {
            @display("p=100,100");
        }
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeX", 1525, "parent.playgroundSizeX");
                playgroundSizeY = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeY", 1525, "parent.playgroundSizeY");
                playgroundSizeZ = veins_eval_by_version(veins_omnetpp_buildnum(), "playgroundSizeZ", 1525, "parent.playgroundSizeZ");
                @display("p=50,388;i=misc/globe");
        }

        roadsCanvasVisualizer: RoadsCanvasVisualizer {
            @display("p=300,0");
        }
		
		visualizer: IntegratedCanvasVisualizer {
            @display("p=350,50;is=s");
        }

        //# Veins manager module
        veinsManager: VeinsInetManager {
            @display("p=50,227;is=s");
        }

        //# LTE modules
        channelControl: LteChannelControl {
            @display("p=50,25;is=s");
        }
        binder: Binder {
            @display("p=50,175;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=50.993748,258.7;is=s");
        }
        server: StandardHost {
            @display("p=660,136;is=n;i=device/server");
        }
        router: Router {
            @display("p=561,135;i=device/smallrouter");
        }
        
        //# Ethernet Switch
        switch: EthernetSwitch {
            @display("p=622,165;i=device/switch");
        }
        
        pgw: PgwStandard {
            @display("p=462,136;is=l");
        }
        eNodeB1: eNodeB {
            @display("p=156,136;is=vl");
        }
        eNodeB2: eNodeB {
            @display("p=391,313;is=vl");
        }
        eNodeB3: eNodeB {  // <-- new EnodeB
            @display("p=280,313;is=vl");
        }



        //# submodule vector to store all cars - will be filled dynamically during simulations
        bus[0]: BusGW;
        
        //# wireless AP
        ap[2]: StandardHost {
            @display("p=210,250;is=vs;i=veins/sign/yellowdiamond");
        }


    connections allowunconnected:
        server.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G <--> pgw.filterGate;
        router.ethg++ <--> Eth10G <--> switch.ethg++;
        pgw.pppg++ <--> Eth10G <--> eNodeB1.ppp;
        pgw.pppg++ <--> Eth10G <--> eNodeB2.ppp;
        // new EnodeB
        pgw.pppg++ <--> Eth10G <--> eNodeB3.ppp;

        //# X2 connections
        eNodeB1.x2++ <--> Eth10G <--> eNodeB2.x2++;
        // new connections between EnodeBs
        eNodeB1.x2++ <--> Eth10G <--> eNodeB3.x2++;
        eNodeB2.x2++ <--> Eth10G <--> eNodeB3.x2++;

        //router.pppg++ <--> Eth10G <--> ap[0].pppg++;
        //router.pppg++ <--> Eth10G <--> ap[1].pppg++;
        
        for i=0..1 {
            ap[i].ethg++ <--> Eth10G <--> switch.ethg++;
        }
        
}

