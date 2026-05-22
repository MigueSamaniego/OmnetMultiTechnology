//
// Copyright (C) 2009 Christoph Sommer <christoph.sommer@informatik.uni-erlangen.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/networklayer/configurator/ipv4/HostAutoConfiguratorM.h"
//src.node_bus.configurator.ipv4
#include <algorithm>

#include "inet/common/ModuleAccess.h"
#include "inet/common/lifecycle/ModuleOperations.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"

namespace inet {

Define_Module(HostAutoConfiguratorM);

void HostAutoConfiguratorM::initialize(int stage)
{
    OperationalBase::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        interfaceTable.reference(this, "interfaceTableModule", true);
    }
}

void HostAutoConfiguratorM::finish()
{
}

void HostAutoConfiguratorM::handleMessageWhenUp(cMessage *apMsg)
{
}

void HostAutoConfiguratorM::setupNetworkLayer()
{
    EV_INFO << "host auto configuration started bus" << std::endl;

    std::string interfaces = par("interfaces");
    Ipv4Address addressBase = Ipv4Address(par("addressBase").stringValue());
    Ipv4Address netmask = Ipv4Address(par("netmask").stringValue());
    Ipv4Address addressBaseWlan = Ipv4Address(par("addressBaseWlan").stringValue());
    Ipv4Address netmaskWlan = Ipv4Address(par("netmaskWlan").stringValue());
    std::string mcastGroups = par("mcastGroups").stdstringValue();
    EV_INFO << "INTERFACES: " << interfaces << std::endl;
    EV_INFO << "addresbase: " << addressBase << std::endl;
    EV_INFO << "netmask: " << netmask << std::endl;

    // get our host module
    cModule *host = getContainingNode(this);
    EV_INFO << "host ID : " << host->getId() << std::endl;
    Ipv4Address myAddress = Ipv4Address(addressBase.getInt() + uint32_t(host->getId()));
    Ipv4Address myAddressWlan = Ipv4Address(addressBaseWlan.getInt() + uint32_t(host->getId()));
    EV_INFO << "myAddress: " << myAddress << std::endl;
    // address test
    if (!Ipv4Address::maskedAddrAreEqual(myAddress, addressBase, netmask))
        throw cRuntimeError("Generated IP address is out of specified address range");

    if (!Ipv4Address::maskedAddrAreEqual(myAddressWlan, addressBaseWlan, netmaskWlan))
            throw cRuntimeError("Generated IP address Wlan is out of specified address range");

    // get our routing table
    IIpv4RoutingTable *routingTable = L3AddressResolver().getIpv4RoutingTableOf(host);

    // encuentra la interfaz wlan0
    NetworkInterface *wlanInterface = interfaceTable->findInterfaceByName("wlan0");
    if (!wlanInterface)
        throw cRuntimeError("No such interface 'wlan0'");

    // crear ruta por defecto
    Ipv4Route *defaultRoute = new Ipv4Route();
    defaultRoute->setDestination(Ipv4Address("0.0.0.0"));      // destino: cualquier IP
    defaultRoute->setNetmask(Ipv4Address("0.0.0.0"));          // máscara: 0.0.0.0
    defaultRoute->setGateway(Ipv4Address("192.168.0.1"));      // gateway: AP
    defaultRoute->setInterface(wlanInterface);                 // usar wlan0
    defaultRoute->setSourceType(Ipv4Route::MANUAL);

    // agregar ruta a la tabla
    routingTable->addRoute(defaultRoute);
    EV_INFO << "Default route via WLAN added: " << defaultRoute->str() << endl;

    Ipv4Route *localWlanRoute = new Ipv4Route();
    localWlanRoute->setDestination(Ipv4Address("192.168.0.0"));
    localWlanRoute->setNetmask(Ipv4Address("255.255.0.0"));
    localWlanRoute->setGateway(Ipv4Address::UNSPECIFIED_ADDRESS); // ruta directa
    localWlanRoute->setInterface(wlanInterface);
    localWlanRoute->setSourceType(Ipv4Route::MANUAL);
    routingTable->addRoute(localWlanRoute);
    EV_INFO << "Direct WLAN route added: " << localWlanRoute->str() << endl;

    EV_INFO << "===== Routing Table =====" << endl;
    for (int i = 0; i < routingTable->getNumRoutes(); i++) {
        const Ipv4Route *route = routingTable->getRoute(i);
        EV_INFO << "Route " << i
                << ": dest=" << route->getDestination().str()
                << "  netmask=" << route->getNetmask().str()
                << "  gw=" << route->getGateway().str()
                << "  interface=" << route->getInterfaceName()
                << endl;
    }
    EV_INFO << "=========================" << endl;
    if (!routingTable)
        throw cRuntimeError("No routing table found");

    // look at all interface table entries
    cStringTokenizer interfaceTokenizer(interfaces.c_str());
    const char *ifname;
    uint32_t loopbackAddr = Ipv4Address::LOOPBACK_ADDRESS.getInt();

    while ((ifname = interfaceTokenizer.nextToken()) != nullptr) {
        NetworkInterface *ie = interfaceTable->findInterfaceByName(ifname);
        EV_INFO << "val ie : " << ie << std::endl;
        if (!ie)
            throw cRuntimeError("No such interface '%s'", ifname);

        auto ipv4Data = ie->getProtocolDataForUpdate<Ipv4InterfaceData>();
        EV_INFO << "val ipv4Data : " << ipv4Data << std::endl;
        // assign IP Address to all connected interfaces
        if (ie->isLoopback()) {
            ipv4Data->setIPAddress(Ipv4Address(loopbackAddr++));
            ipv4Data->setNetmask(Ipv4Address::LOOPBACK_NETMASK);
            ipv4Data->setMetric(1);
            EV_INFO << "loopback interface " << ifname << " gets " << ipv4Data->getIPAddress() << "/" << ipv4Data->getNetmask() << std::endl;
            continue;
        }
        EV_INFO << "nombre interface usada : " << ifname << std::endl;
        if (!strcmp(ifname, "cellular")) {
            myAddress = Ipv4Address(addressBase.getInt() + uint32_t(host->getId()));
            EV_INFO << "interface cellular : " << myAddress << std::endl;
        }else if (!strcmp(ifname, "wlan0")) {
            myAddress = Ipv4Address(addressBaseWlan.getInt() + uint32_t(host->getId()));
            netmask = netmaskWlan;
            EV_INFO << "interface wlan OK : " << myAddress << std::endl;
        }

        EV_INFO << "interface " << ifname << " gets " << myAddress.str() << "/" << netmask.str() << std::endl;

        ipv4Data->setIPAddress(myAddress);
        ipv4Data->setNetmask(netmask);
        ie->setBroadcast(true);

        // associate interface with default multicast groups
        ipv4Data->joinMulticastGroup(Ipv4Address::ALL_HOSTS_MCAST);
        ipv4Data->joinMulticastGroup(Ipv4Address::ALL_ROUTERS_MCAST);

        // associate interface with specified multicast groups
        cStringTokenizer interfaceTokenizer(mcastGroups.c_str());
        const char *mcastGroup_s;
        while ((mcastGroup_s = interfaceTokenizer.nextToken()) != nullptr) {
            Ipv4Address mcastGroup(mcastGroup_s);
            ipv4Data->joinMulticastGroup(mcastGroup);
        }

    }
}

void HostAutoConfiguratorM::handleStartOperation(LifecycleOperation *operation)
{
    if (operation == nullptr) {
        // in initialize:
        for (int i = 0; i < interfaceTable->getNumInterfaces(); i++)
            interfaceTable->getInterface(i)->addProtocolData<Ipv4InterfaceData>();
    }
    setupNetworkLayer();
}

void HostAutoConfiguratorM::handleStopOperation(LifecycleOperation *operation)
{
}

void HostAutoConfiguratorM::handleCrashOperation(LifecycleOperation *operation)
{
}

} // namespace inet

