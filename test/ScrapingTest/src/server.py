from flask import Response, Flask
from requests import get
import prometheus_client
from prometheus_client.core import CollectorRegistry
from prometheus_client import Summary, Counter, Histogram, Gauge
import time
import random

#RED: 
##la ip de este servicio sera 10.101.0.150
##la ip del servidor de la estacion sera 10.100.0.2
##la ip del equipo que contenga el cluster sera 10.100.0.100
##la mascara de red de todos sera 255.255.255.0

#Variables
app = Flask(__name__)
#definir el numero de rutas a las que se quiera ir a recojer los valores
urls = ["http://172.20.10.3:5000/"]
indice = [1,2]
respuestas = []
#Metricas
graphs = {}
for n in indice:
    graphs['temp_'+str(n)] = Gauge('temperature_in_celsius_'+str(n), 'Temperature in Celsius')
    graphs['press_'+str(n)] = Gauge('atmospheric_pressure_'+str(n), 'Atmostpheric Pressure')
    graphs['hum_'+str(n)] = Gauge('humidity_'+str(n), 'Percentage of Humidity')
    graphs['w_speed_'+str(n)] = Gauge('wind_speed_'+str(n), 'The speed of the wind in m/s')
    graphs['w_dir_'+str(n)] = Gauge('wind_direction_'+str(n), 'The direction of the wind in degrees')
    graphs['rain_'+str(n)] = Gauge('rain_precipitation_'+str(n), 'Rain precipitations in l/m^2')
    graphs['light_'+str(n)] = Gauge('light_level_'+str(n), 'Light level')


#Pagina base de flask
@app.route("/", methods=['GET'])
def hello():
    return "Hello World!" 

#Prueba de conexion a un url externa
#@app.route("/prueba")
#def get_data():
#    response = get('http://10.100.0.2/')
#    print(response.text)
#    return response.text

#El htlm que recoja tiene este formato:
##<html><body>1,10.5,885,0,0,0,0,0</body></html>
#Esto se traduce a:
##<html><body>id_temperatura_presion_humedad_velocidadviento_direccionviento_lluvia_luz</body></html>

#Paguina a la que 
@app.route("/metrics")
def requests():
    while True:
        #Por cada estacion una url 
        #for url in urls:
        #    response = get(url)
        #    datos=str(response.content).split(sep=",") 
        #    datos[0] = datos[0][2:]
        #    datos[7] = datos[7][:-5]
        #    respuestas.append(datos)
        #las distintas respuestas genera distintas metricas en funciond de datos[0] que es el id de la estacion
        #for datos in respuestas:
        #    graphs['temp_'+str(datos[0])].set(datos[1])
        #    graphs['press_'+str(datos[0])].set(datos[5])
        #    graphs['hum_'+str(datos[0])].set(datos[2])
        #    graphs['w_speed_'+str(datos[0])].set(datos[3])
        #    graphs['w_dir_'+str(datos[0])].set(datos[4])
        #    graphs['rain_'+str(datos[0])].set(datos[6])
        #    graphs['light_'+str(datos[0])].set(datos[7])

        #Codigo para forzar variables por separado
        #---
        for n in indice:
            graphs['temp_'+str(n)].set(random.randint(46, 50))
            graphs['press_'+str(n)].set(random.randint(885, 1077))
            graphs['hum_'+str(n)].set(random.randint(0,100))
            graphs['w_speed_'+str(n)].set(random.randint(0, 40))
            graphs['w_dir_'+str(n)].set(random.randint(0,360))
            graphs['rain_'+str(n)].set(random.randint(0, 3))
            graphs['light_'+str(n)].set(random.randint(0,100))
        #---
        
        #Cargamos los datos al cleinte de prometheus
        res = []
        for k,v in graphs.items():
            res.append(prometheus_client.generate_latest(v))
        return Response(res, mimetype="text/plain")