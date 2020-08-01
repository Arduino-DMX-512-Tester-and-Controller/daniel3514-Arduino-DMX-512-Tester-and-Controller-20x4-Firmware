# **Arduino DMX-512 Tester and Controller**

## **Firmware - LCD 20x4**

[![version](https://img.shields.io/badge/version-0.0-brightgreen.svg)](CHANGELOG.md)
[![license](https://img.shields.io/badge/licence-GNU_GPL_v3.0-blue)](https://github.com/Arduino-DMX-512-Tester-and-Controller/Arduino-DMX-512-Tester-and-Controller-LCD-20x4-Firmware/blob/master/LICENCE.md)

![GitHub issues](https://img.shields.io/github/issues/Arduino-DMX-512-Tester-and-Controller/Arduino-DMX-512-Tester-and-Controller-LCD-20x4-Firmware)
![GitHub forks](https://img.shields.io/github/forks/Arduino-DMX-512-Tester-and-Controller/Arduino-DMX-512-Tester-and-Controller-LCD-20x4-Firmware)
![GitHub stars](https://img.shields.io/github/stars/Arduino-DMX-512-Tester-and-Controller/Arduino-DMX-512-Tester-and-Controller-LCD-20x4-Firmware)

***

Herramienta Open Hardware, para pruebas y control de iluminación de espectáculos a través del protocolo [DMX-512](http://es.wikipedia.org/wiki/Digital_Multiplex), ideal para hacer pruebas rápidas en instalaciones fijas o temporales de iluminación, sin la necesidad de instalar consolas de iluminación, interfaces o computadoras en ambientes al intemperie, hostiles o de difícil acceso.

***

### **Basado en:**

- [Arduino Mega 2560 Rev-3](http://www.arduino.cc/en/Main/ArduinoBoardMega2560)
- [Librería Arduino cuatro universos DMX v0.3 - Deskontrol](http://www.deskontrol.net/blog/libreria-arduino-cuatro-universos-dmx/)
- [Libreria LCD v1.2.1 - Francisco Malpartida](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home)
- [Simulación en Proteus de Arduino - Microcontrolandos](http://microcontrolandos.blogspot.mx/2012/12/arduino-componentes-para-o-proteus.html)
- [Encoder](http://www.robodosis.net/2013/01/encoder-rotativo-mecanico-con-pic16f877a_2.html)

***

- Esquemático y PCB en Proteus v8.0 SP1
- LCD de 4x20 con backlight y contraste controlado por software
- Alimentación desde USB, baterías o fuente de alimentación externa
- Encoder para navegacion entre menu
- Teclado numérico 4x4
- Interruptor de on/off
- Salida DMX desde bornera, XLR de 3 pin y XLR de 5 pin
- Leds de estado de salida DMX
- Simulación en Proteus v7.7 SP2 y v8.0 SP1
- Esquemático y PCB en Proteus v8.0 SP1
- Aclarar que para este proyecto se utilizó un Arduino Mega hecho en China
- Regulador para fuente externa, el arduino se calienta
- Bornera para salida de fuente regulada de 5V para uso general
- Led para salida de fuente regulada
- Botón de reset
- Opto acoplado a salida de dmx
- Convertidor de DC/DC para aislamiento de DMX
- SW de palanca para invertir polaridad de salida de DMX
- Jumper default eeprom en pin 9
- Driver para luz led como lampara
- SW para Key Light
- SW para Ext Light
- Espaciadores para fijación
- Leyendas de conexión
- Fuente externa, el arduino se calentaba

***

- Compilado en Arduino IDE v1.0.6
- Instalación de librerías DMX:
	- Instalar Arduino IDE v1.0.6 (carpeta .../Software/) (Windows 10 64 bits, ok!)
	- Instalar la libreri´a del encoder:
		- Menú Sketch, Importar librería, Add Library
		- Seleccionar el archivo Encoder.zip (carpeta .../Firmware/Documentacion/Librerias/)
		- Abrir
		- Mensaje: Library added to you libraries
	- Instalar la librería del LCD:
		- Menú Sketch, Importar librería, Add Library
		- Seleccionar el archivo LiquidCrystal_V1.2.1.zip (carpeta .../Firmware/Documentacion/Librerias/)
		- Abrir
		- Mensaje: Library added to you libraries
	- Instalar la librería del DMX:
		- Copiar la carpeta DMX desde .../Firmware/Documentacion/Librerias/DMX/Dmx/
		- Pegarla en la carpeta de .../Arduino/libraries/ el los archivos de programa (para el caso de Windows 10 64 bits, C:\Program Files (x86)\Arduino\libraries)
		- Reiniciar Arduino IDE
		- Revisar el menú Sketch, Importar librería..., Dmx
- Cargando el Firmware:
	- Seleccionar el firmware que vamos a cargar
	- Lo abrimos desde Arduino IDE
	- Menú Herramientas, Tarjeta, Arduino Mega 2560 or Mega ADK
	- Conectar el Arduino Mega al PC
	- Verificar que Windows lo dé de alta correctamente:
		- Panel de control\Sistema y seguridad\Sistema
		- Administrador de dispositivos
		- Puertos (COM y LPT)
		- Arduino Mega 2560 (COMx)
	- En Arduino IDE seleccionamos nuestro Arduino Mega:
		- Menú Herramientas, Puerto Serial, COMx
	- Clic en el icono Cargar...
	- Salen estos errores:
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: stk500v2_recv(): checksum error
		- avrdude: verification error, first mismatch at byte 0x0b60 0x50 != 0xd7
		- avrdude: verification error; content mismatch
    
***

### ![**Hardware**](https://github.com/Arduino-DMX-512-Tester-and-Controller/Arduino-DMX-512-Tester-and-Controller-LCD-20x4-Hardware)

- v0.0 a v0.2

***
