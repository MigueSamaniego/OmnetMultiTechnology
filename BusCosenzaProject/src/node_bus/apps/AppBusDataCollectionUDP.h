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

#pragma once

#include "../apps/veins_inet.h"
#include "../apps/VeinsInetApplicationBase.h"
#include "inet/common/INETDefs.h"
#include "inet/common/geometry/common/Coord.h"
#include "../sdcard/Buffers.h"

#include "omnetpp.h" // useful to cListener y simsignal_t

class VEINS_INET_API AppBusDataCollectionUDP : public veins::VeinsInetApplicationBase, public omnetpp::cListener {
protected:
    uint8_t SignStateConnectAP = 0;
    bool ConnectionToAP = false;
    bool ConnectionToAP_Pass = false;

    simsignal_t stateAssociationSignalId;



private:
    bool Vehicle_With_Interface = false;
    int coun_msg_sent = 0;
    omnetpp::cMessage* Control_Task_Timer = nullptr; // Usar omnetpp::cMessage
    omnetpp::cMessage* Control_GPS_Data = nullptr; // Usar omnetpp::cMessage
    omnetpp::cMessage* Control_Data_Emission = nullptr; // Usar omnetpp::cMessage
    omnetpp::cMessage* Control_Data_Vehicle = nullptr; // Usar omnetpp::cMessage

    SDCardBuffer sdcard;
    InputBuffer inputBuffer;
    OutputBuffer outputBuffer;

    inet::IInterfaceTable* interfaceTable = nullptr;
    inet::NetworkInterface* wifi = nullptr;
    inet::NetworkInterface* celular = nullptr;

    std::time_t startTime;
    static bool initialized;

    std::string buildDataString(const std::vector<double>& values);

    int getBufferBytes(const std::vector<std::string>& buffer) const;

    const double SDCARD_MAX_CAPACITY = 1250000.0; // maximun capacity of SDCard


    // observe variables in real time
    int sdcardCount = 0;
    int inputCount = 0;
    int outputCount = 0;

    // buffer occupation
    simsignal_t inputBufferSignal;
    simsignal_t outputBufferSignal;
    simsignal_t sdcardBufferSignal;

    simsignal_t sdcardPercentSignal;

protected:
    // ** MODIFICAR: Reemplazar el initialize simple por el de etapas **
    virtual void initialize(int stage) override; // Heredado de cSimpleModule
    // ** MODIFICAR: Agregar finish() si deseas desuscribirte explícitamente **
    virtual void finish() override;

    virtual bool startApplication() override;
    virtual bool stopApplication() override;
    virtual void processPacket(std::shared_ptr<inet::Packet> pk) override;
    virtual void sendDataToCloud();
    virtual void subscriptionSignalState();
    virtual void interfaceAvailable();
    virtual void handleMessage(cMessage *msg) override;
    virtual inet::Coord convertXYtoLatLon(inet::Coord pos);

    //virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, long l, cObject *details) override;
    //virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details) override;

public:
    AppBusDataCollectionUDP();
    ~AppBusDataCollectionUDP();
};
