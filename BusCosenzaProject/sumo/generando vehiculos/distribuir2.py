import xml.etree.ElementTree as ET
import random
import time


cantidad_total = 1989
categorias_livianos = 10
categorias_pesados = 4


vehicle_livianos_pocentaje = 80

livianos_total = (vehicle_livianos_pocentaje*cantidad_total)//100

pesados_toal = ((100 - vehicle_livianos_pocentaje)*cantidad_total)//100



aggressive_pocentaje    = 10
impatient_pocentaje     = 5
distracted_pocentaje    = 10  
novice_pocentaje        = 2
cautious_pocentaje      = 5
early_pocentaje         = 4 
defensive_pocentaje     = 4
experienced_pocentaje   = 60


types_total = aggressive_pocentaje+impatient_pocentaje+distracted_pocentaje+novice_pocentaje+cautious_pocentaje+early_pocentaje+defensive_pocentaje+experienced_pocentaje

if types_total == 100:
    print("Ok")
else:
    while(1):
        print("error en la distribucion del tipos de vehiculos")
        time.sleep(3)
        
        
        
aggressive_total    = ((aggressive_pocentaje*livianos_total)//100)
impatient_total     = ((impatient_pocentaje*livianos_total)//100)
distracted_total    = ((distracted_pocentaje*livianos_total)//100)
novice_total        = ((novice_pocentaje*livianos_total)//100)
cautious_total      = ((cautious_pocentaje*livianos_total)//100)
early_total         = ((early_pocentaje*livianos_total)//100) 
defensive_total     = ((defensive_pocentaje*livianos_total)//100)
experienced_total   = ((experienced_pocentaje*livianos_total)//100)

     


vip_total           = 3
passenger_total     = 30
authority_total     = 2
private_total       = 30
motorcycle_total    = 3
#moped_total         = 5
taxi_total          = 10
evehicle_total      = 10
delivery_total      = 5
emergency_total     = 4
hov_total           = 5

bus_total           = 35  # estos son buses exteriores a la simulacion tipo flixbus
coach_total         = 15
truck_total         = 30
trailer_total       = 20
#rail_total          = 10


    

# Define los tipos de vehículos y sus proporciones
vehicle_types = {
    "aggressive_driver_vip": ((categorias_livianos*aggressive_total)//vip_total),
    "impatient_driver_vip": ((categorias_livianos*impatient_total)//vip_total),
    "distracted_driver_vip": ((categorias_livianos*distracted_total)//vip_total),
    "novice_driver_vip": ((categorias_livianos*novice_total)//vip_total),
    "cautious_driver_vip": ((categorias_livianos*cautious_total)//vip_total),
    "early_driver_vip": ((categorias_livianos*early_total)//vip_total),
    "defensive_driver_vip": ((categorias_livianos*defensive_total)//vip_total),
    "experienced_driver_vip": ((categorias_livianos*experienced_total)//vip_total),
    
    "aggressive_driver_passenger": ((categorias_livianos*aggressive_total)//passenger_total),
    "impatient_driver_passenger": ((categorias_livianos*impatient_total)//passenger_total),
    "distracted_driver_passenger": ((categorias_livianos*distracted_total)//passenger_total),
    "novice_driver_passenger": ((categorias_livianos*novice_total)//passenger_total),
    "cautious_driver_passenger": ((categorias_livianos*cautious_total)//passenger_total),
    "early_driver_passenger": ((categorias_livianos*early_total)//passenger_total),
    "defensive_driver_passenger": ((categorias_livianos*defensive_total)//passenger_total),
    "experienced_driver_passenger": ((categorias_livianos*experienced_total)//passenger_total),

    "aggressive_driver_authority": ((categorias_livianos*aggressive_total)//authority_total),
    "impatient_driver_authority": ((categorias_livianos*impatient_total)//authority_total),
    "distracted_driver_authority": ((categorias_livianos*distracted_total)//authority_total),
    "novice_driver_authority": ((categorias_livianos*novice_total)//authority_total),
    "cautious_driver_authority": ((categorias_livianos*cautious_total)//authority_total),
    "early_driver_authority": ((categorias_livianos*early_total)//authority_total),
    "defensive_driver_authority": ((categorias_livianos*defensive_total)//authority_total),
    "experienced_driver_authority": ((categorias_livianos*experienced_total)//authority_total),
    
    "aggressive_driver_private": ((categorias_livianos*aggressive_total)//private_total),
    "impatient_driver_private": ((categorias_livianos*impatient_total)//private_total),
    "distracted_driver_private": ((categorias_livianos*distracted_total)//private_total),
    "novice_driver_private": ((categorias_livianos*novice_total)//private_total),
    "cautious_driver_private": ((categorias_livianos*cautious_total)//private_total),
    "early_driver_private": ((categorias_livianos*early_total)//private_total),
    "defensive_driver_private": ((categorias_livianos*defensive_total)//private_total),
    "experienced_driver_private": ((categorias_livianos*experienced_total)//private_total),    
        
    "aggressive_driver_motorcycle": ((categorias_livianos*aggressive_total)//motorcycle_total),
    "impatient_driver_motorcycle": ((categorias_livianos*impatient_total)//motorcycle_total),
    "distracted_driver_motorcycle": ((categorias_livianos*distracted_total)//motorcycle_total),
    "novice_driver_motorcycle": ((categorias_livianos*novice_total)//motorcycle_total),
    "cautious_driver_motorcycle": ((categorias_livianos*cautious_total)//motorcycle_total),
    "early_driver_motorcycle": ((categorias_livianos*early_total)//motorcycle_total),
    "defensive_driver_motorcycle": ((categorias_livianos*defensive_total)//motorcycle_total),
    "experienced_driver_motorcycle": ((categorias_livianos*experienced_total)//motorcycle_total),
    
    # "aggressive_driver_moped": ((categorias_livianos*aggressive_total)//moped_total),
    # "impatient_driver_moped": ((categorias_livianos*impatient_total)//moped_total),
    # "distracted_driver_moped": ((categorias_livianos*distracted_total)//moped_total),
    # "novice_driver_moped": ((categorias_livianos*novice_total)//moped_total),
    # "cautious_driver_moped": ((categorias_livianos*cautious_total)//moped_total),
    # "early_driver_moped": ((categorias_livianos*early_total)//moped_total),
    # "defensive_driver_moped": ((categorias_livianos*defensive_total)//moped_total),
    # "experienced_driver_moped": ((categorias_livianos*experienced_total)//moped_total),
    
    "aggressive_driver_taxi": ((categorias_livianos*aggressive_total)//taxi_total),
    "impatient_driver_taxi": ((categorias_livianos*impatient_total)//taxi_total),
    "distracted_driver_taxi": ((categorias_livianos*distracted_total)//taxi_total),
    "novice_driver_taxi": ((categorias_livianos*novice_total)//taxi_total),
    "cautious_driver_taxi": ((categorias_livianos*cautious_total)//taxi_total),
    "early_driver_taxi": ((categorias_livianos*early_total)//taxi_total),
    "defensive_driver_taxi": ((categorias_livianos*defensive_total)//taxi_total),
    "experienced_driver_taxi": ((categorias_livianos*experienced_total)//taxi_total),
    
    "aggressive_driver_evehicle": ((categorias_livianos*aggressive_total)//evehicle_total),
    "impatient_driver_evehicle": ((categorias_livianos*impatient_total)//evehicle_total),
    "distracted_driver_evehicle": ((categorias_livianos*distracted_total)//evehicle_total),
    "novice_driver_evehicle": ((categorias_livianos*novice_total)//evehicle_total),
    "cautious_driver_evehicle": ((categorias_livianos*cautious_total)//evehicle_total),
    "early_driver_evehicle": ((categorias_livianos*early_total)//evehicle_total),
    "defensive_driver_evehicle": ((categorias_livianos*defensive_total)//evehicle_total),
    "experienced_driver_evehicle": ((categorias_livianos*experienced_total)//evehicle_total),
    
    "aggressive_driver_delivery": ((categorias_livianos*aggressive_total)//delivery_total),
    "impatient_driver_delivery": ((categorias_livianos*impatient_total)//delivery_total),
    "distracted_driver_delivery": ((categorias_livianos*distracted_total)//delivery_total),
    "novice_driver_delivery": ((categorias_livianos*novice_total)//delivery_total),
    "cautious_driver_delivery": ((categorias_livianos*cautious_total)//delivery_total),
    "early_driver_delivery": ((categorias_livianos*early_total)//delivery_total),
    "defensive_driver_delivery": ((categorias_livianos*defensive_total)//delivery_total),
    "experienced_driver_delivery": ((categorias_livianos*experienced_total)//delivery_total),

    "aggressive_driver_emergency": ((categorias_livianos*aggressive_total)//emergency_total),
    "impatient_driver_emergency": ((categorias_livianos*impatient_total)//emergency_total),
    "distracted_driver_emergency": ((categorias_livianos*distracted_total)//emergency_total),
    "novice_driver_emergency": ((categorias_livianos*novice_total)//emergency_total),
    "cautious_driver_emergency": ((categorias_livianos*cautious_total)//emergency_total),
    "early_driver_emergency": ((categorias_livianos*early_total)//emergency_total),
    "defensive_driver_emergency": ((categorias_livianos*defensive_total)//emergency_total),
    "experienced_driver_emergency": ((categorias_livianos*experienced_total)//emergency_total),
    
    "aggressive_driver_hov": ((categorias_livianos*aggressive_total)//hov_total),
    "impatient_driver_hov": ((categorias_livianos*impatient_total)//hov_total),
    "distracted_driver_hov": ((categorias_livianos*distracted_total)//hov_total),
    "novice_driver_hov": ((categorias_livianos*novice_total)//hov_total),
    "cautious_driver_hov": ((categorias_livianos*cautious_total)//hov_total),
    "early_driver_hov": ((categorias_livianos*early_total)//hov_total),
    "defensive_driver_hov": ((categorias_livianos*defensive_total)//hov_total),
    "experienced_driver_hov": ((categorias_livianos*experienced_total)//hov_total),    
        
    "aggressive_driver_bus": ((categorias_livianos*aggressive_total)//bus_total),
    "impatient_driver_bus": ((categorias_livianos*impatient_total)//bus_total),
    "distracted_driver_bus": ((categorias_livianos*distracted_total)//bus_total),
    "novice_driver_bus": ((categorias_livianos*novice_total)//bus_total),
    "cautious_driver_bus": ((categorias_livianos*cautious_total)//bus_total),
    "early_driver_bus": ((categorias_livianos*early_total)//bus_total),
    "defensive_driver_bus": ((categorias_livianos*defensive_total)//bus_total),
    "experienced_driver_bus": ((categorias_livianos*experienced_total)//bus_total),    
        
    "aggressive_driver_coach": ((categorias_livianos*aggressive_total)//coach_total),
    "impatient_driver_coach": ((categorias_livianos*impatient_total)//coach_total),
    "distracted_driver_coach": ((categorias_livianos*distracted_total)//coach_total),
    "novice_driver_coach": ((categorias_livianos*novice_total)//coach_total),
    "cautious_driver_coach": ((categorias_livianos*cautious_total)//coach_total),
    "early_driver_coach": ((categorias_livianos*early_total)//coach_total),
    "defensive_driver_coach": ((categorias_livianos*defensive_total)//coach_total),
    "experienced_driver_coach": ((categorias_livianos*experienced_total)//coach_total),
        
    "aggressive_driver_truck": ((categorias_livianos*aggressive_total)//truck_total),
    "impatient_driver_truck": ((categorias_livianos*impatient_total)//truck_total),
    "distracted_driver_truck": ((categorias_livianos*distracted_total)//truck_total),
    "novice_driver_truck": ((categorias_livianos*novice_total)//truck_total),
    "cautious_driver_truck": ((categorias_livianos*cautious_total)//truck_total),
    "early_driver_truck": ((categorias_livianos*early_total)//truck_total),
    "defensive_driver_truck": ((categorias_livianos*defensive_total)//truck_total),
    "experienced_driver_truck": ((categorias_livianos*experienced_total)//truck_total),
    
    "aggressive_driver_trailer": ((categorias_livianos*aggressive_total)//trailer_total),
    "impatient_driver_trailer": ((categorias_livianos*impatient_total)//trailer_total),
    "distracted_driver_trailer": ((categorias_livianos*distracted_total)//trailer_total),
    "novice_driver_trailer": ((categorias_livianos*novice_total)//trailer_total),
    "cautious_driver_trailer": ((categorias_livianos*cautious_total)//trailer_total),
    "early_driver_trailer": ((categorias_livianos*early_total)//trailer_total),
    "defensive_driver_trailer": ((categorias_livianos*defensive_total)//trailer_total),
    "experienced_driver_trailer": ((categorias_livianos*experienced_total)//trailer_total),
    
    # "aggressive_driver_rail": ((categorias_livianos*aggressive_total)//rail_total),
    # "impatient_driver_rail": ((categorias_livianos*impatient_total)//rail_total),
    # "distracted_driver_rail": ((categorias_livianos*distracted_total)//rail_total),
    # "novice_driver_rail": ((categorias_livianos*novice_total)//rail_total),
    # "cautious_driver_rail": ((categorias_livianos*cautious_total)//rail_total),
    # "early_driver_rail": ((categorias_livianos*early_total)//rail_total),
    # "defensive_driver_rail": ((categorias_livianos*defensive_total)//rail_total),
    # "experienced_driver_rail": ((categorias_livianos*experienced_total)//rail_total),

}
total_weight = sum(vehicle_types.values())

# Cargar el archivo de rutas generado
tree = ET.parse("cosenza3.rou.xml")         # archivo para leer
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
tree.write("distributed_vehicles.rou.xml")      # nuevo archivo creado
