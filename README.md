## Desarrollo de aplicación IoT para monitorización de estaciones meteorológicas con esp32
<ul>
    <li>Fechas: Agosto 2023</li>
    <li>Descripción: Las estaciónes están montadas con un sistema embebido esp32 (programado con Arduino C) que centraliza los sensores de una estación. Se emplea este como cliente wifi para transmitir los datos a la arquitectura CNF, montada con Kubernetes. Esta arquitectura está compuesta de un pod de Prometheus que almacena los datos y de un pod de Grafana para visualizarlos. Además se implementa un sistema de alarmas utilizando el servidor SMTP de Google Mail. Los sensores de la estación miden las siguientes métricas: temperatura (DHT11), humedad(DHT11), nivel de luz(LDR), dirección del viento(rotary encoder), velocidad del aire(photo interrupter) y el nivel de la lluvia(sensor de nivel de agua).</li>
    <li>Herramientas utilizadas:</li>
<a href="https://grafana.com/" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/grafana/grafana-original.svg" alt="grafana" width="40" height="40"/> 
</a>
<a href="https://prometheus.io/" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/prometheus/prometheus-original.svg" alt="prometheus" width="40" height="40"/> 
</a>
<a href="https://kubernetes.io/" target="_blank" rel="noreferrer">
    <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/kubernetes/kubernetes-plain.svg" alt="kubernetes" width="40" height="40"/> 
</a>
</ul>
