import xml.etree.ElementTree as ET
import random


cantidad_total = 4000



# Define los tipos de vehículos y sus proporciones
vehicle_types = {
    "car_vip": 10,
    "car_private": 20,
    "Truck": 5,
    "bus_Flixbus": 5,
}
total_weight = sum(vehicle_types.values())

# Cargar el archivo de rutas generado
tree = ET.parse("cosenza3.rou.xml")
root = tree.getroot()

# Reasignar tipos de vehículos basados en la distribución
for vehicle in root.findall('vehicle'):
    vtype = random.choices(
        list(vehicle_types.keys()),
        weights=list(vehicle_types.values()),
        k=1
    )[0]
    vehicle.set("type", vtype)

# Guardar el archivo modificado
tree.write("distributed_vehicles.rou.xml")
