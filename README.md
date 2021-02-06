# CovidDetector_2.2.0

Un proyecto para detectar y advertir del nivel de riesgo de contagio por COVID-19 basandonos
en la temperatura y las partes de CO2 por millón.

Cuando el nivel de CO2 supera las 700 partes por millón, el aparato emite una secuencia de 
dos pitidos y se enciende el led amarillo, cuando el aparato supera las 1500 partes de CO2
por millón, el aparato enciende el led rojo y emite una secuencia de cuatro pitidos, si 
pasados 60 segundos no se ha solucionado el problema de la concentración, el aparato emitirá 
nuevamente los pitidos correspondientes al nivel de peligro, en cualquier otro caso se enciende
el led verde.

Cuando el aparato detecta una temperatura en la que no se puede trabajar, menos de 14ºC y más
de 28ºC, se encenderá el led rojo, si la temperatura es moderadamente incómoda, 14-17ºC y 25-28ºC,
se encenderá el led amarillo, en temperaturas confortables para trabajar se encenderá el led verde.
