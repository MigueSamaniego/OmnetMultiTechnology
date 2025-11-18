En la misma carperta deben estar los archivos
	- randomTrip
	- ejemplo.net.xml (este archivo debe contener los tipos de vehiculos y conductores)


Generar vehiculos con el siguiente comando


	python3 randomTrips.py -n cosenza3.net.xml -r all_vehicles.rou.xml --begin 1 --end 200 -l --validate --period 0.1


usa el archivo "distribuir2" para generar diferentes tipos de vehiculos y conductores

	python3 .\distribuir2.py

el archivo generado se llama "distributed_vehicles.rou.xml" cambia el nombre y reemplaza en el proyecto