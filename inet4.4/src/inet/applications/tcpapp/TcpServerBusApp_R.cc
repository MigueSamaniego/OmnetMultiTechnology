//
// Copyright (C) 2004 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/applications/tcpapp/TcpServerBusApp_R.h"

#include "inet/common/ModuleAccess.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/common/lifecycle/NodeStatus.h"
#include "inet/common/packet/Message.h"
#include "inet/common/socket/SocketTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "inet/common/packet/chunk/BytesChunk.h"

namespace inet {

Define_Module(TcpServerBusApp_R);
Define_Module(TcpServerBusApp_RThread);

TcpServerBusApp_R::TcpServerBusApp_R()
{
}

TcpServerBusApp_R::~TcpServerBusApp_R()
{
}

void TcpServerBusApp_R::initialize(int stage)
{
    TcpServerHostApp::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        bytesRcvd = 0;
        WATCH(bytesRcvd);
        EV_INFO << "App SERVER with Re-Try msg from server"<<endl;
    }
}

void TcpServerBusApp_R::refreshDisplay() const
{
    EV_INFO << "TEST server refresh" << endl;
    ApplicationBase::refreshDisplay();

    char buf[160];
    sprintf(buf, "threads: %d\nrcvd: %ld bytes", socketMap.size(), bytesRcvd);
    getDisplayString().setTagArg("t", 0, buf);
}

void TcpServerBusApp_R::finish()
{
    EV_INFO << "TEST server finish" << endl;
    TcpServerHostApp::finish();

    recordScalar("bytesRcvd", bytesRcvd);
}

void TcpServerBusApp_R::removeBusThread(TcpServerThreadBase *thread)
{
    Enter_Method_Silent();

    // 1. Cerramos el socket
    if (thread->getSocket()) {
        thread->getSocket()->abort();
    }

    // 2. MAGIA: Usamos la función interna de INET para eliminar el hilo de la GUI y la RAM
    removeThread(thread);
}

void TcpServerBusApp_RThread::initialize(int stage)
{
    EV_INFO << "TEST thread initialize" << endl;
    TcpServerThreadBase::initialize(stage);

    if (stage == INITSTAGE_LOCAL) {
        bytesRcvd = 0;
        WATCH(bytesRcvd);
    }
}

void TcpServerBusApp_RThread::established()
{
    Enter_Method_Silent();

    EV_INFO << "TEST thread establish" << endl;
    bytesRcvd = 0;
    lastDataTime = simTime(); // Tiempo actual de la simulación

    // Crear y programar el primer disparo del timer
    timeoutMsg = new cMessage("watchdog-timer");
    scheduleAt(simTime() + 5.0, timeoutMsg);

    EV_INFO << "Conexión establecida. Watchdog iniciado para T+5s." << endl;
}

void TcpServerBusApp_RThread::dataArrived(Packet *pk, bool urgent)
{
    lastDataTime = simTime();

    // Extrac bytes received
    auto bytesChunk = pk->peekAllAsBytes();
    std::vector<uint8_t> bytes = bytesChunk->getBytes();
    std::string content(bytes.begin(), bytes.end());

    EV_INFO << "msg received, ready to replay: " << content << endl;

    // update statistics
    bytesRcvd += pk->getByteLength();
    sinkAppModule->bytesRcvd += pk->getByteLength();

    emit(packetReceivedSignal, pk);
    delete pk; // clear packet, its not more necessary

    // replay Echo
    if (sock && sock->getState() == TcpSocket::CONNECTED) {
        auto echoPacket = new Packet("ECHO_REPLY");
        auto payload = makeShared<BytesChunk>(std::vector<uint8_t>(content.begin(), content.end()));
        echoPacket->insertAtBack(payload);
        sock->send(echoPacket);
    }
}

void TcpServerBusApp_RThread::refreshDisplay() const
{
    EV_INFO << "TEST thread refresh" << endl;
    std::ostringstream os;
    os << (sock ? TcpSocket::stateName(sock->getState()) : "NULL_SOCKET") << "\nrcvd: " << bytesRcvd << " bytes";
    getDisplayString().setTagArg("t", 0, os.str().c_str());
}

void TcpServerBusApp_RThread::timerExpired(cMessage *msg)
{
    if (msg == timeoutMsg) {
        simtime_t tiempoInactivo = simTime() - lastDataTime;

        if (tiempoInactivo >= 4.9) {// time-out 1s
            EV_INFO << "WATCHDOG: Cerrando socket y destruyendo hilo..." << endl;

            timeoutMsg = nullptr;
            delete msg;

            // --- EL CAMBIO ESTÁ AQUÍ ---
            // Le pedimos al padre que nos elimine pasándole 'this' (nuestro propio hilo)
            sinkAppModule->removeBusThread(this);

            // ¡SÚPER IMPORTANTE!
            // El hilo acaba de ser borrado de la memoria en la línea anterior.
            // Debemos usar 'return;' para salir inmediatamente y evitar que C++
            // intente leer variables de un hilo que ya no existe (Segmentation Fault).
            return;
            // ---------------------------
        }
        else {
            scheduleAt(lastDataTime + 5.0, msg);
        }
    } else {
        delete msg;
    }
}

void TcpServerBusApp_RThread::handleMessage(cMessage *msg)
{
    // 1. Crucial: Informar al simulador que el contexto cambia al Thread
    Enter_Method_Silent();

    if (msg->isSelfMessage()) {
        // 2. Tomar posesión del mensaje para poder borrarlo o reprogramarlo
        take(msg);

        // 3. Ejecutar tu lógica de 5 segundos
        timerExpired(msg);
    }
    else {
        // Mensajes de control de TCP que la clase base sabe manejar
        TcpServerThreadBase::handleMessage(msg);
    }
}

TcpServerBusApp_RThread::~TcpServerBusApp_RThread()
{
    // Si el timer sigue pendiente en el calendario de eventos, lo cancelamos y borramos

    // clean msg only if msg exist
    if (timeoutMsg) {
        cancelAndDelete(timeoutMsg);
    }
}




} // namespace inet

