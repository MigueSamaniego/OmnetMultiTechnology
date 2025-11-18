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

#include "AppBusDataCollection.h"
#include "../apps/VeinsInetSampleMessage_m.h"
#include "inet/common/ModuleAccess.h"
#include "inet/common/packet/Packet.h"
#include "inet/common/TagBase_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/transportlayer/contract/udp/UdpControlInfo_m.h"
//#include "inet/linklayer/ieee80211/mac/Ieee80211MacM.h"

#include "inet/common/Simsignals.h"

#include "inet/networklayer/ipv4/Ipv4RoutingTable.h"

// *****************************************************************
// IMPORTANTE: Definiciones necesarias para INET y OMNeT++
// *****************************************************************
using namespace inet;
using namespace omnetpp;

Define_Module(AppBusDataCollection);

// *****************************************************************
// Implementación de Constructor y Destructor
// *****************************************************************

AppBusDataCollection::AppBusDataCollection()
{
    // Inicializar la bandera de suscripción
    haveForwarded = false; // Usamos haveForwarded como bandera de 'suscripción hecha'
    controlTimer = nullptr;
    ConnectionToAP = false;
}

AppBusDataCollection::~AppBusDataCollection()
{
}

// *****************************************************************
// 1. LÓGICA DE INICIALIZACIÓN (Registro de Señales)
// *****************************************************************

void AppBusDataCollection::initialize(int stage)
{
    // Llamar a la inicialización base, es CRÍTICO en INET
    veins::VeinsInetApplicationBase::initialize(stage);

    // Solo registramos las señales en la etapa de aplicación
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        EV_INFO << "Inicializando AppBusDataCollection en stage " << stage << endl;

        // Obtener y registrar los IDs de las señales
//        l2AssociatedOldApSignalId = registerSignal("l2AssociatedOldApSignal");
//        l2AssociatedNewApSignalId = registerSignal("l2AssociatedNewApSignal");
//        l2DisassociatedSignalId = registerSignal("l2DisassociatedSignal");

        stateAssociationSignalId = registerSignal("stateAssociationSignal");

    }

    // El timer se iniciará en startApplication(), que llamará a controlInterface()
    // donde se intentará la suscripción.
}


// ----------------------------------------------------------------------
// 2. FIRMA PARA long (¡LA MÁS PROBABLE para este tipo de señales en INET!)
// ----------------------------------------------------------------------
void AppBusDataCollection::receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details)
{   EV_INFO << "firma long " << endl;
    if (signalID == stateAssociationSignalId) {
        EV_INFO << "🟢 Señal RECIBIDA (Long): Disociado (sin AP). Valor: " << l << " Fuente: " << source->getFullPath() << endl;
        ConnectionToAP = l;
    }
}

// *****************************************************************
// 3. FUNCIONES DE LA APLICACIÓN
// *****************************************************************

bool AppBusDataCollection::startApplication()
{
    EV_INFO << "START APPLICATION " << endl;

    // Iniciar el timer para la función de control (y suscripción)
    if (!controlTimer) {
        controlTimer = new cMessage("controlTimer");
        // Iniciamos el control en t=1s para dar tiempo a la inicialización dinámica.
        scheduleAt(simTime() + SimTime(0.1, SIMTIME_S), controlTimer);
    }
    return true;
}

bool AppBusDataCollection::stopApplication()
{
    if (controlTimer) {
        cancelAndDelete(controlTimer);
        controlTimer = nullptr;
    }

    // Llama a la función stopApplication base
    veins::VeinsInetApplicationBase::stopApplication();
    return true;
}

void AppBusDataCollection::finish()
{
    // Llama a la función finish base
    veins::VeinsInetApplicationBase::finish();

    // Desuscribirse de las señales (Buena práctica)
    if (haveForwarded) {
        cModule *wlanAgent = getParentModule()->findModuleByPath("wlan[0].agent");
        if (wlanAgent) {
             wlanAgent->unsubscribe(stateAssociationSignalId, this);
        }
    }
}

void AppBusDataCollection::processPacket(std::shared_ptr<inet::Packet> pk)
{
    // Lógica para procesar paquetes entrantes
}

void AppBusDataCollection::handleMessage(cMessage *msg)
{
    if (msg == controlTimer) {
        EV_INFO << "MSG DE CONTROL ARRIVED: Ejecutando controlInterface()" << endl;

        // 1. Intentar suscribirse (si no se ha hecho) y ejecutar la lógica de control.
        controlInterface();

        // 2. Reprogramar el timer (si es que quieres que controlInterface() se siga ejecutando cada 1s)
        if (msg) {
             scheduleAt(simTime() + SimTime(1, SIMTIME_S), controlTimer);
        }
    } else {
        // Llama al manejo de mensajes base para mensajes del framework (paquetes, etc.)
        veins::VeinsInetApplicationBase::handleMessage(msg);
    }
}

// *****************************************************************
// 4. FUNCIÓN DE CONTROL CON LÓGICA DE SUSCRIPCIÓN ROBUSTA
// *****************************************************************

void AppBusDataCollection::controlInterface(){

    EV_INFO << "START CONTROL FUNCTION:" << endl;

    if (!haveForwarded) {
        EV_INFO << "Intentando Suscribirse a señales de AGENT..." << endl;

        cModule *host = getParentModule();
        cModule *wlanAgent = nullptr;

        // 1. Obtener la ruta completa del MAC
        std::string macPath = host->getFullPath() + ".wlan[0].agent";
        EV_INFO << "path total: " << macPath << endl;
        wlanAgent = getSystemModule()->findModuleByPath(macPath.c_str());
        EV_INFO << "val wlanAgent: " << wlanAgent << endl;
        if (wlanAgent) {
            // Suscribirse al MÓDULO AGENT
//            wlanAgent->subscribe(l2AssociatedOldApSignalId, this);
//            wlanAgent->subscribe(l2AssociatedNewApSignalId, this);
//            wlanAgent->subscribe(l2DisassociatedSignalId, this);

            wlanAgent->subscribe(stateAssociationSignalId, this);

            EV_INFO << "🟢 Suscripción exitosa a señales de gestión de asociación en " << wlanAgent->getFullPath() << endl;
            haveForwarded = true; // Establecer la bandera
        } else {
            // Reintentará en el siguiente ciclo del timer (cada 1s)
            EV_WARN << "🟡 ADVERTENCIA: Módulo AGENT no encontrado. Reintentando en el siguiente ciclo." << endl;
        }
    }

    // --- TU LÓGICA DE VERIFICACIÓN DE INTERFACES ---

    // Si la suscripción fue exitosa, el resto del código es tu lógica original de chequeo de interfaces
    std::string vehicle = mobility->getExternalId();

    auto interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
//    EV_INFO << "Interfaces available:" << endl;
//
//    for (int i = 0; i < interfaceTable->getNumInterfaces(); i++) {
//        auto ie = interfaceTable->getInterface(i);
//        EV_INFO << "  - " << ie->getInterfaceName()
//                << " (" << (ie->isUp() ? "up" : "down") << ")" << endl;
//    }

    auto wifi = interfaceTable->findInterfaceByName("wlan0");
    auto celular = interfaceTable->findInterfaceByName("cellular");

    // check if the name string start with "bus_cosenza."
    if (vehicle.rfind("buses_cosenza.", 0) == 0) { // starts_with
        EV_INFO << "type vehicle: BUS " << vehicle << endl;

        getParentModule()->getDisplayString().setTagArg("i", 1, "red");

        auto payload = makeShared<VeinsInetSampleMessage>();
        payload->setChunkLength(B(100));
        payload->setRoadId(traciVehicle->getRoadId().c_str());
        timestampPayload(payload);

        auto packet = createPacket("accident");
        packet->insertAtBack(payload);

        if(ConnectionToAP){
            celular->setState(NetworkInterface::State::DOWN);

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

        }else {
            celular->setState(NetworkInterface::State::UP);
            EV_INFO << "WiFi disable → Using interfaz celular" << endl;

            auto rt = check_and_cast<Ipv4RoutingTable*>(getModuleByPath("^.ipv4.routingTable"));
            auto interfaceTable = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);

            // limpiar default routes previas
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

            // agregar default route por celular
            auto celular = interfaceTable->findInterfaceByName("cellular");
            auto def = new Ipv4Route();
            def->setDestination(Ipv4Address("0.0.0.0"));
            def->setNetmask(Ipv4Address("0.0.0.0"));
            def->setGateway(Ipv4Address("10.0.0.1"));
            def->setInterface(celular);
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

            EV_INFO << "Default route added via CELLULAR (gw=10.0.0.1)" << endl;

        }

        // Enviar al broadcast o unidifusión WLAN
        L3Address destAddr = L3AddressResolver().resolve("15.0.0.1");
        socket.sendTo(packet.release(), destAddr, 3000);

    }
    // --- FIN LÓGICA DE VERIFICACIÓN DE INTERFACES ---
}
