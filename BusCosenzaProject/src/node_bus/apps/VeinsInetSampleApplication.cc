//
// Copyright (C) 2018 Christoph Sommer <sommer@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "../apps/VeinsInetSampleApplication.h"

#include "../apps/VeinsInetSampleMessage_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"

#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"


using namespace inet;

Define_Module(VeinsInetSampleApplication);

VeinsInetSampleApplication::VeinsInetSampleApplication()
{
}

bool VeinsInetSampleApplication::startApplication()
{
    auto callback = [this]() {

        // used for obtained the type vehicle
        std::string vehicle = mobility->getExternalId();

        auto interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        EV_INFO << "Interfaces available:" << endl;

        for (int i = 0; i < interfaceTable->getNumInterfaces(); i++) {
            auto ie = interfaceTable->getInterface(i);
            EV_INFO << "  - " << ie->getInterfaceName()
                    << " (" << (ie->isUp() ? "up" : "down") << ")" << endl;
        }

        auto wifi = interfaceTable->findInterfaceByName("wlan0");
        auto celular = interfaceTable->findInterfaceByName("cellular");

        // check if the name string start with "bus_cosenza."
        //if (vehicle.compare(0, 14, "buses_cosenza.") == 0) {
        if (vehicle.rfind("buses_cosenza.", 0) == 0) { // starts_with
            EV_INFO << "type vehicle: BUS " << vehicle << endl;
            isBus = true;

            //auto callback = [this]() {
            getParentModule()->getDisplayString().setTagArg("i", 1, "red");

            //traciVehicle->setSpeed(0);

            auto payload = makeShared<VeinsInetSampleMessage>();
            payload->setChunkLength(B(100));
            payload->setRoadId(traciVehicle->getRoadId().c_str());
            timestampPayload(payload);

            //auto packet = createPacket("accident");
            //packet->insertAtBack(payload);
            //sendPacket(std::move(packet));

            auto packet = createPacket("accident");
            packet->insertAtBack(payload);
            EV_INFO << "accident start: " << endl;


            if (wifi && celular) {
                if (wifi->isUp()) {
                    celular->setState(NetworkInterface::State::DOWN);
                    EV_INFO << "WiFi activo → NO usar interfaz celular" << endl;

                    //auto rt = getModuleFromPar<Ipv4RoutingTable>(par("routingTableModule"), this);
                    auto rt = check_and_cast<Ipv4RoutingTable*>(getModuleByPath("^.ipv4.routingTable"));

                    // Elimina cualquier default route previa
                    for (int i = rt->getNumRoutes() - 1; i >= 0; --i) {
                        auto route = rt->getRoute(i);
                        if (route->getDestination().isUnspecified()) {
                            rt->deleteRoute(route);
                        }
                    }

                    EV_INFO << "===== Routing Table (before update) =====" << endl;
                    for (int i = 0; i < rt->getNumRoutes(); i++) {
                        auto route = rt->getRoute(i);
                        EV_INFO << "[" << i << "] "
                                << "dest=" << route->getDestination() << "  "
                                << "netmask=" << route->getNetmask() << "  "
                                << "gw=" << (route->getGateway().isUnspecified() ? "<direct>" : route->getGateway().str()) << "  "
                                << "if=" << route->getInterfaceName() << endl;
                    }
                    EV_INFO << "========================================" << endl;

                    // Crea tu default route WLAN
                    auto wlan0 = interfaceTable->findInterfaceByName("wlan0");
                    auto def = new Ipv4Route();
                    def->setDestination(Ipv4Address("0.0.0.0"));
                    def->setNetmask(Ipv4Address("0.0.0.0"));
                    def->setGateway(Ipv4Address("192.168.0.1"));
                    def->setInterface(wlan0);
                    def->setSourceType(Ipv4Route::MANUAL);
                    rt->addRoute(def);

                    EV_INFO << "===== Routing Table (after update) =====" << endl;
                    for (int i = 0; i < rt->getNumRoutes(); i++) {
                        auto route = rt->getRoute(i);
                        EV_INFO << "[" << i << "] "
                                << "dest=" << route->getDestination() << "  "
                                << "netmask=" << route->getNetmask() << "  "
                                << "gw=" << (route->getGateway().isUnspecified() ? "<direct>" : route->getGateway().str()) << "  "
                                << "if=" << route->getInterfaceName() << endl;
                    }
                    EV_INFO << "========================================" << endl;


                } else {
                    celular->setState(NetworkInterface::State::UP);
                    EV_INFO << "WiFi disable → Using interfaz celular" << endl;

//                    auto rt = check_and_cast<Ipv4RoutingTable*>(getModuleByPath("^.ipv4.routingTable"));
//                    auto interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
//
//                    // limpiar default routes previas
//                    for (int i = rt->getNumRoutes() - 1; i >= 0; --i) {
//                        auto route = rt->getRoute(i);
//                        if (route->getDestination().isUnspecified()) {
//                            rt->deleteRoute(route);
//                        }
//                    }
//
//                    // agregar default route por celular
//                    auto celular = interfaceTable->findInterfaceByName("cellular");
//                    auto def = new Ipv4Route();
//                    def->setDestination(Ipv4Address("0.0.0.0"));
//                    def->setNetmask(Ipv4Address("0.0.0.0"));
//                    def->setGateway(Ipv4Address("10.0.0.1"));
//                    def->setInterface(celular);
//                    def->setSourceType(Ipv4Route::MANUAL);
//                    rt->addRoute(def);
//
//                    EV_INFO << "Default route added via CELLULAR (gw=10.0.0.1)" << endl;
                }
            }




            // Enviar al broadcast o unidifusión WLAN
            L3Address destAddr = L3AddressResolver().resolve("15.0.0.1");
            socket.sendTo(packet.release(), destAddr, 3000);


        }else{
            EV_INFO << "type vehicle: CAR " << vehicle << endl;
            isBus = false;
            if (celular) celular->setState(NetworkInterface::State::DOWN);
            if (wifi) wifi->setState(NetworkInterface::State::DOWN);
        }

        // host[0] should stop at t=20s
        //if (getParentModule()->getIndex() == 0) {
        //        }

    };
    timerManager.create(veins::TimerSpecification(callback).oneshotAt(SimTime(2.5, SIMTIME_S)));


    return true;
}

bool VeinsInetSampleApplication::stopApplication()
{
    return true;
}

VeinsInetSampleApplication::~VeinsInetSampleApplication()
{
}

void VeinsInetSampleApplication::processPacket(std::shared_ptr<inet::Packet> pk)
{
    auto payload = pk->peekAtFront<VeinsInetSampleMessage>();

    EV_INFO << "Received packet: " << payload << endl;

    getParentModule()->getDisplayString().setTagArg("i", 1, "green");

    traciVehicle->changeRoute(payload->getRoadId(), 999.9);

    if (haveForwarded) return;

    auto packet = createPacket("relay");
    packet->insertAtBack(payload);
    sendPacket(std::move(packet));

    haveForwarded = true;
}
