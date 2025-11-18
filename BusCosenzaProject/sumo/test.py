import traci
import traci.constants as tc

# -----------------------------------------------------
# CONFIGURACIÓN
# -----------------------------------------------------
SUMO_BINARY = "sumo-gui"   # o "sumo" si no quieres interfaz gráfica
SUMO_CONFIG = "cosenza.sumocfg"  # ruta a tu archivo .sumocfg
FLOW_PREFIX = "buses_cosenza"  # ID del flow definido en el .rou.xml
RADIUS = 30.0  # metros de radio de detección
# -----------------------------------------------------

# Iniciar SUMO
traci.start([SUMO_BINARY, "-c", SUMO_CONFIG])
print("Simulación iniciada.")

# Diccionario para almacenar los buses ya suscritos
subscribed_buses = {}

# Bucle principal de simulación
step = 0
while step < 20000:  # puedes ajustar el número de pasos
    traci.simulationStep()
    step += 1

    # Obtener lista actual de vehículos
    veh_ids = traci.vehicle.getIDList()

    # Detectar nuevos buses generados por el flow
    for vid in veh_ids:
        if vid.startswith(FLOW_PREFIX) and vid not in subscribed_buses:
            # Suscribirse al contexto del bus
            traci.vehicle.subscribeContext(
                vid,
                tc.CMD_GET_VEHICLE_VARIABLE,
                RADIUS,
                [tc.VAR_POSITION]
            )
            subscribed_buses[vid] = True
            print(f"🚌 Bus detectado y suscrito: {vid}")

    # Recuperar resultados de suscripciones activas
    for bus_id in list(subscribed_buses.keys()):
        nearby = traci.vehicle.getContextSubscriptionResults(bus_id)
        if nearby:
            print(f"\n🔍 Vehículos cerca de {bus_id} (radio {RADIUS} m):")
            for veh, data in nearby.items():
                if veh != bus_id:  # evitar incluir el mismo bus
                    pos = data[tc.VAR_POSITION]
                    print(f" - {veh} en posición {pos}")

    # Opcional: detener si no quedan vehículos
    if traci.simulation.getMinExpectedNumber() <= 0:
        print("No quedan vehículos. Fin de la simulación.")
        break

traci.close()
print("Simulation Finished.")
