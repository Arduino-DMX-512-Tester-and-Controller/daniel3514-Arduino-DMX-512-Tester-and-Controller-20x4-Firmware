// ***************************************************************************************************************************
// ***************************************************************************************************************************
// **																														**
// **										Arduino DMX-512 Tester Controller												**
// **																														**
// **	- Firmware v0.9																										**
// **	- Hardware v0.3	- v0.4 																								**
// **																														**
// **	- Compilado en Arduino IDE v1.0.6																					**
// **		http://www.arduino.cc/en/Main/OldSoftwareReleases																**
// **	- Compilado para Arduino Mega 2560 R3																				**
// **		http://www.arduino.cc/en/Main/ArduinoBoardMega2560																**
// **	- Libreria Arduino cuatro universos DMX v0.3 - Deskontrol.net														**
// **		http://www.deskontrol.net/blog/libreria-arduino-cuatro-universos-dmx/)											**
// **	- Libreria LCD v1.2.1 - Francisco Malpartida																		**
// **		https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home													**
// **	- Simulacion en Proteus v7.7 SP2																					**
// **	- Simulacion en Proteus de Arduino - Microcontrolandos																**
// **		http://microcontrolandos.blogspot.mx/2012/12/arduino-componentes-para-o-proteus.html							**
// **																														**
// **	Autor:																												**
// **																														**
// **	Daniel Roberto Becerril Angeles																						**
// **	daniel3514@gmail.com																								**																										**
// **	https://github.com/daniel3514/Arduino-DMX-512-Tester-Controller/wiki												**
// **																														**
// **	Licenciamiento:																										**
// **																														**
// **	GNU General Pubic Licence Version 3																					**
// **		https://www.gnu.org/copyleft/gpl.html																			**
// **																														**
// ***************************************************************************************************************************
// ***************************************************************************************************************************

// Librerias
	#include <LiquidCrystal.h>	// libreria para LCD
	#include <Wire.h>
	#include <EEPROM.h>
	#include <string.h>
	#include <lib_dmx.h>  		// libreria DMX 4 universos deskontrol four universes DMX library  - http://www.deskontrol.net/blog
		
// DMX Library
	#define    DMX512	  (0)   // (250 kbaud - 2 to 512 channels) Standard USITT DMX-512
	//#define  DMX1024    (1)   // (500 kbaud - 2 to 1024 channels) Completely non standard - TESTED ok
	//#define  DMX2048    (2)   // (1000 kbaud - 2 to 2048 channels) called by manufacturers DMX1000K, DMX 4x or DMX 1M ???

// Puertos, variables
	// DMX
		int DMX_Data_Flux 		= 2;	// control de flujo de datos para dmx, 0 por default 
		int  DMX_Values [515];      	// array 1 de valores actuales DMX
		int  Canal_Actual 		= 1;
		byte Universo_Actual	= 0;
	// Botones cursor
		int  Boton_Up     		= 9; 
		int  Boton_Down   		= 12;	
		int  Boton_Left   		= 8;	
		int  Boton_Right  		= 10;	
		int  Boton_Center		= 11;	
		byte LCD_Col_Pos 		= 0;	// posicion en tiempo real de lcd
		byte LCD_Row_Pos 		= 0;	// posicion en tiempo real de lcd
		byte Cursor_Conf[4][20] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},		// config de posiciones de lcd Col Row
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
								   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};		
	// Botones Numerico Array
		int  Boton_Array_1		= 38;
		int  Boton_Array_2		= 40;
		int  Boton_Array_3		= 42;
		int  Boton_Array_4		= 44;
		int  Boton_Array_A		= 30;	
		int  Boton_Array_B		= 32;
		int  Boton_Array_C   	= 34;
		int  Boton_Array_D   	= 36;
		byte Boton_Calc 		= 17;	// valor calculado	# E * F, 17 sin valor calculado
		byte Num_Col_Pos  		= 0;	// posicion en tiempo real de lcd
		byte Num_Row_Pos 		= 0;	// posicion en tiempo real de lcd
		int  Num_Val			= 0;	// valor generado al calculo
		long Boton_Delay_Teclado = 100;	// delay de lectura de boton
	// Potenciometro
		int  Pot				= A15;	// entrada de potenciometro
	// LCD
		int  LCD_RS 			= 43;	// puertos de conexion de LCD
		int  LCD_E  			= 45;
		int  LCD_D4 			= 47;
		int  LCD_D5 			= 49;
		int  LCD_D6 			= 51;
		int  LCD_D7				= 53;
		LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);  //LCD setup
		int  Back_Light_PWM		= 13;	// salida para PWM de Back Light de LCD
		int  Contrast_PWM		= 4;	// salida para pwm de contraste de LCD
		byte Back_Light_On_Off	= 0;	// saber si esta encendida o apagada
	// EEPROM
		int BackLight_Add 	= 4094;		// direccion de eeprom
		int Contrast_Add	= 4095;		// direccion de eeprom

void setup() 
	{
		// DMX
			//pinMode(DMX_Data_Flux, 		OUTPUT);
		// Botones cursor
			pinMode(Boton_Up,      		INPUT_PULLUP);
			pinMode(Boton_Down,    		INPUT_PULLUP);
			pinMode(Boton_Left,    		INPUT_PULLUP);
			pinMode(Boton_Right,   		INPUT_PULLUP);
			pinMode(11,					INPUT_PULLUP);	//Boton_Center,  		INPUT_PULLUP);
		// Botones numerico
			pinMode(Boton_Array_1, 		OUTPUT);
			pinMode(Boton_Array_2, 		OUTPUT);
			pinMode(Boton_Array_3, 		OUTPUT);
			pinMode(Boton_Array_4,		OUTPUT);
			pinMode(Boton_Array_A, 		INPUT_PULLUP);
			pinMode(Boton_Array_B, 		INPUT_PULLUP);
			pinMode(Boton_Array_C, 		INPUT_PULLUP);
			pinMode(Boton_Array_D, 		INPUT_PULLUP);
			digitalWrite(Boton_Array_1, HIGH);
			digitalWrite(Boton_Array_2, HIGH);
			digitalWrite(Boton_Array_3, HIGH);
			digitalWrite(Boton_Array_4, HIGH);
		// LCD
			pinMode(LCD_RS, 			OUTPUT);
			pinMode(LCD_E,  			OUTPUT);
			pinMode(LCD_D7, 			OUTPUT);
			pinMode(LCD_D6, 			OUTPUT);
			pinMode(LCD_D5, 			OUTPUT);
			pinMode(LCD_D4, 			OUTPUT);
			pinMode(Back_Light_PWM,		OUTPUT);
			lcd.begin(20, 4);							//tamaño de LCD				
		// DMX
			ArduinoDmx0.set_tx_address(1);      		// poner aqui la direccion de inicio de DMX 
			ArduinoDmx0.set_tx_channels(512);   		// poner aqui el numero de canales a transmitir 
			ArduinoDmx0.init_tx(DMX512);        		// iniciar transmision universo 0, modo estandar DMX512
		// no conectados
			pinMode(5, OUTPUT);
			digitalWrite(5, LOW);
			pinMode(6, OUTPUT);
			digitalWrite(6, LOW);
			pinMode(7, OUTPUT);
			digitalWrite(7, LOW);
			pinMode(22, OUTPUT);
			digitalWrite(22, LOW);
			pinMode(23, OUTPUT);
			digitalWrite(23, LOW);
			pinMode(24, OUTPUT);
			digitalWrite(24, LOW);
			pinMode(25, OUTPUT);
			digitalWrite(25, LOW);
			pinMode(26, OUTPUT);
			digitalWrite(26, LOW);
			pinMode(27, OUTPUT);
			digitalWrite(27, LOW);
			pinMode(28, OUTPUT);
			digitalWrite(28, LOW);
			pinMode(29, OUTPUT);
			digitalWrite(29, LOW);
			pinMode(31, OUTPUT);
			digitalWrite(31, LOW);
			pinMode(33, OUTPUT);
			digitalWrite(33, LOW);
			pinMode(35, OUTPUT);
			digitalWrite(35, LOW);
			pinMode(37, OUTPUT);
			digitalWrite(37, LOW);
			pinMode(39, OUTPUT);
			digitalWrite(39, LOW);
			pinMode(41, OUTPUT);
			digitalWrite(41, LOW);
			pinMode(46, OUTPUT);
			digitalWrite(46, LOW);
			pinMode(48, OUTPUT);
			digitalWrite(48, LOW);
			pinMode(50, OUTPUT);
			digitalWrite(50, LOW);
			pinMode(52, OUTPUT);
			digitalWrite(52, LOW);
			pinMode(A0, OUTPUT);
			digitalWrite(A0, LOW);
			pinMode(A1, OUTPUT);
			digitalWrite(A1, LOW);
			pinMode(A2, OUTPUT);
			digitalWrite(A2, LOW);
			pinMode(A3, OUTPUT);
			digitalWrite(A3, LOW);
			pinMode(A4, OUTPUT);
			digitalWrite(A4, LOW);
			pinMode(A5, OUTPUT);
			digitalWrite(A5, LOW);
			pinMode(A6, OUTPUT);
			digitalWrite(A6, LOW);
			pinMode(A7, OUTPUT);
			digitalWrite(A7, LOW);
			pinMode(A8, OUTPUT);
			digitalWrite(A8, LOW);
			pinMode(A9, OUTPUT);
			digitalWrite(A9, LOW);
			pinMode(A10, OUTPUT);
			digitalWrite(A10, LOW);
			pinMode(A11, OUTPUT);
			digitalWrite(A11, LOW);
			pinMode(A12, OUTPUT);
			digitalWrite(A12, LOW);
			pinMode(A13, OUTPUT);
			digitalWrite(A13, LOW);
			pinMode(A14, OUTPUT);
			digitalWrite(A14, LOW);
	}    

void loop()
	{
		digitalWrite(2, HIGH);	// max 485 como salida
		Back_Light_Init();
		Contrast_Init();
		GUI_About();
		GUI_Memory_Init();
	}

void Back_Light_Init()
	{
		// ultimo estado del back light
		byte Back_Light_Value = EEPROM.read(BackLight_Add);
		analogWrite(Back_Light_PWM, Back_Light_Value);
		if (Back_Light_Value == 0)
			{
				Back_Light_On_Off = 0;
			}
		if (Back_Light_Value > 0)
			{
				Back_Light_On_Off = 1;
			}	
	}
	
void Contrast_Init()
	{
		// ultimo estado del comtrast
			byte Contrast_Value = EEPROM.read(Contrast_Add);
			if (Contrast_Value < 150)
				{
					Contrast_Value = 150;
				}
			analogWrite(Contrast_PWM, Contrast_Value);
	}
	
void Back_Light_En()
	{
		byte Back_Light_Value = EEPROM.read(513);				// lectura del ultimo valor desde la eeprom, 513 es donde se guarda el valor
		// encender
			if (Back_Light_On_Off == 0)							// si esta apagada encenderla
				{
					if (Back_Light_Value == 0)					// la encendemos de todos modos
						{
							analogWrite(Back_Light_PWM, 127);	// aqui el valor a encender en el caso que se haya establecido apagado el back light
						}
					if (Back_Light_Value > 0)
						{
							analogWrite(Back_Light_PWM, Back_Light_Value);	// encender con el valor de la eeprom
						}
					Back_Light_On_Off = 1;
					goto salida;
				}
		// apagar
			if (Back_Light_On_Off == 1)							// si esta encendida apagarla
				{
					analogWrite(Back_Light_PWM, 0);
					Back_Light_On_Off = 0;
				}
		salida:
			{
				delay(300);										// para impedir repeticion del comando
			}
	}
	
void GUI_About()
	{
		byte Firm_Ver_Ent = 0;
		byte Firm_Ver_Dec = 9;
		byte Hard_Ver_Ent = 0;
		byte Hard_Ver_Dec = 4;
		lcd.clear ();
		for(int numero = 0; numero <= 512; numero ++)	// efecto binario en lcd
			{
				lcd.print (numero, BIN);
			}
		lcd.clear ();
		lcd.setCursor(3, 0);
		lcd.print("Arduino DMX-512");
		lcd.setCursor(1, 1);
		lcd.print("Tester & Controller");
		// Firmware
			lcd.setCursor(0, 3);
			lcd.print("Firm v");
			lcd.print(Firm_Ver_Ent);
			lcd.print(".");
			lcd.print(Firm_Ver_Dec);
		// Hardware
			lcd.setCursor(11, 3);
			lcd.print("Hard v");
			lcd.print(Hard_Ver_Ent);
			lcd.print(".");
			lcd.print(Hard_Ver_Dec);
		lcd.setCursor(0, 0);
		lcd.blink();
		delay (3000);
		lcd.clear ();
		lcd.setCursor(3, 1);
		lcd.print("Open Hardware!");
		lcd.setCursor(0, 3);
		lcd.print("http://goo.gl/5nqJKt");
		lcd.setCursor(0, 0);
		delay(3000);  									//retardo de muestra de mensaje
		lcd.noBlink();
	}

void Multi_Matrix(int inicial)
	{
		// dibujar banco
			Numerico_Write (inicial, 13, 0);
			Numerico_Write (inicial + 14, 17, 0);
		// matrix 1
			Numerico_Write (DMX_Values[inicial], 1, 1);
			Numerico_Write (DMX_Values[inicial + 1], 5, 1);
			Numerico_Write (DMX_Values[inicial + 2], 9, 1);
			Numerico_Write (DMX_Values[inicial + 3], 13, 1);
			Numerico_Write (DMX_Values[inicial + 4], 17, 1);
		// matrix 2
			Numerico_Write (DMX_Values[inicial + 5], 1, 2);
			Numerico_Write (DMX_Values[inicial + 6], 5, 2);
			Numerico_Write (DMX_Values[inicial + 7], 9, 2);
			Numerico_Write (DMX_Values[inicial + 8], 13, 2);
			Numerico_Write (DMX_Values[inicial + 9], 17, 2);
		// matrix 3
			Numerico_Write (DMX_Values[inicial + 10], 1, 3);
			Numerico_Write (DMX_Values[inicial + 11], 5, 3);
			Numerico_Write (DMX_Values[inicial + 12], 9, 3);
			Numerico_Write (DMX_Values[inicial + 13], 13, 3);
			Numerico_Write (DMX_Values[inicial + 14], 17, 3);
	}
	
void GUI_Control_Matrix()
	{
		int Inicial = 1;
		Canal_Actual = 1;
		inicio:
			lcd.clear();
			lcd.setCursor (0, 0);
			lcd.print("c--- Mb  Ctr    -");
			lcd.setCursor (7, 0);
			if (Universo_Actual == 0)
				{
					lcd.print("-");
				}
			else
				{
					lcd.print(Universo_Actual);
				}
			Multi_Matrix (Inicial);
			// Cursor
				LCD_Col_Pos = 12;		// posicion de cursor
				LCD_Row_Pos = 0;		// posicion e cursor
			// configuracion de cursor	
				Cursor_Conf_Clear();	// limpiar array
			// Row 0
				Cursor_Conf[0][4]  = 1;	// Memory
				Cursor_Conf[0][8]  = 1;	// Unit
				Cursor_Conf[0][12] = 1;	// Banco Inicial
				Cursor_Conf[0][16] = 1;	// Banco Final
			// Row 1
				Cursor_Conf[1][0]  = 1;
				Cursor_Conf[1][4]  = 1;
				Cursor_Conf[1][8]  = 1;
				Cursor_Conf[1][12] = 1;
				Cursor_Conf[1][16] = 1;
			// Row 2
				Cursor_Conf[2][0]  = 1;
				Cursor_Conf[2][4]  = 1;
				Cursor_Conf[2][8]  = 1;
				Cursor_Conf[2][12] = 1;
				Cursor_Conf[2][16] = 1;
			// Row 3
				Cursor_Conf[3][0]  = 1;
				Cursor_Conf[3][4]  = 1;
				Cursor_Conf[3][8]  = 1;
				Cursor_Conf[3][12] = 1;
				Cursor_Conf[3][16] = 1;
			// navegar
		Banco:
				GUI_Navegar(1, Inicial);
			// Acciones
				// Memory
					if (LCD_Col_Pos == 4 &&  LCD_Row_Pos == 0)
						{
							GUI_Memory();
							goto inicio;
						}
				// Control
					if (LCD_Col_Pos == 8 &&  LCD_Row_Pos == 0)
						{
							GUI_Control_Options();
							goto inicio;
						}
				// Banco Inicial
					if (LCD_Col_Pos == 12 && LCD_Row_Pos == 0)
						{
							Num_Row_Pos = 0;
							Num_Col_Pos = 13;
							Num_Val = Inicial;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							if (Num_Val > 498)	// limite de matriz
								{
									Num_Val = 498;
								}
							if (Num_Val == 0)	// limite de matriz
								{
									Num_Val = 1;
								}
							Inicial = Num_Val;
							goto inicio;
						}
				// Banco Final
					if (LCD_Col_Pos == 16 && LCD_Row_Pos == 0)
						{
							Num_Row_Pos = 0;
							Num_Col_Pos = 17;
							if (Inicial == 1)
								Num_Val = 15;
							else	
								Num_Val = Inicial - 14;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							if (Num_Val > 512)		// limite de matriz
								{
									Inicial = 498;
									goto inicio;
								}
							if (Num_Val < 15)		// limite de matriz
								{
									Inicial = 1;
									goto inicio;
								}
							Inicial = Num_Val - 14;
							goto inicio;
						}
				// posicion 1
					if (LCD_Col_Pos == 0 && LCD_Row_Pos == 1)
						{
							Canal_Actual = Inicial;
							goto Salida_DMX;
						}
				// posicion 2
					if (LCD_Col_Pos == 4 && LCD_Row_Pos == 1)
						{
							Canal_Actual = Inicial + 1;
							goto Salida_DMX;
						}
				// posicion 3
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 1)
						{
							Canal_Actual = Inicial + 2;
							goto Salida_DMX;
						}
				// posicion 4
					if (LCD_Col_Pos == 12 && LCD_Row_Pos == 1)
						{
							Canal_Actual = Inicial + 3;
							goto Salida_DMX;
						}
				// posicion 5
					if (LCD_Col_Pos == 16 && LCD_Row_Pos == 1)
						{
							Canal_Actual = Inicial + 4;
							goto Salida_DMX;
						}
				// posicion 6
					if (LCD_Col_Pos == 0 && LCD_Row_Pos == 2)
						{
							Canal_Actual = Inicial + 5;
							goto Salida_DMX;
						}
				// posicion 7
					if (LCD_Col_Pos == 4 && LCD_Row_Pos == 2)
						{
							Canal_Actual = Inicial + 6;
							goto Salida_DMX;
						}
				// posicion 8
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
						{
							Canal_Actual = Inicial + 7;
							goto Salida_DMX;
						}
				// posicion 9
					if (LCD_Col_Pos == 12 && LCD_Row_Pos == 2)
						{
							Canal_Actual = Inicial + 8;
							goto Salida_DMX;
						}
				// posicion 10
					if (LCD_Col_Pos == 16 && LCD_Row_Pos == 2)
						{
							Canal_Actual = Inicial + 9;
							goto Salida_DMX;
						}
				// posicion 11
					if (LCD_Col_Pos == 0 && LCD_Row_Pos == 3)
						{
							Canal_Actual = Inicial + 10;
							goto Salida_DMX;
						}
				// posicion 12
					if (LCD_Col_Pos == 4 && LCD_Row_Pos == 3)
						{
							Canal_Actual = Inicial + 11;
							goto Salida_DMX;
						}
				// posicion 13
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
						{
							Canal_Actual = Inicial + 12;
							goto Salida_DMX;
						}
				// posicion 14
					if (LCD_Col_Pos == 12 && LCD_Row_Pos == 3)
						{
							Canal_Actual = Inicial + 13;
							goto Salida_DMX;
						}
				// posicion 15
					if (LCD_Col_Pos == 16 && LCD_Row_Pos == 3)
						{
							Canal_Actual = Inicial + 14;
							goto Salida_DMX;
						}
		Salida_DMX:
			Num_Row_Pos = LCD_Row_Pos;
			Num_Col_Pos = LCD_Col_Pos + 1;
			Num_Val = DMX_Values[Canal_Actual];		// para dejar el numero que estaba si no se cambia
			Numerico_Calc(1);
			if (Num_Val == 612)		// ubicar
				{
					Ubicar();
					Num_Col_Pos = Num_Col_Pos - 4;
				}
			if (Num_Val == 712)		// analogo
				{
					Analog_Read_DMX(Num_Col_Pos - 2, Num_Row_Pos);
					Num_Col_Pos = Num_Col_Pos - 4;
					goto Banco;
				}
			if (Num_Val > 255)
				{
					Num_Val = 255;
					Numerico_Write (255, Num_Col_Pos + 2, Num_Row_Pos);
				}
			ArduinoDmx0.TxBuffer[Canal_Actual - 1] = Num_Val;
			DMX_Values[Canal_Actual] = Num_Val;
			goto Banco;		
	}
	
void Cursor_Conf_Clear()
	{
		for (byte Conteo_Col = 0; Conteo_Col <= 19; Conteo_Col ++)
			{
				for (byte Conteo_Row = 0; Conteo_Row <= 3; Conteo_Row ++)
					{
						Cursor_Conf[Conteo_Row][Conteo_Col] = 0;
					}
			}
	}
	
void GUI_Navegar(byte matrix, int banco)
	{
		int Boton_Delay_Cursor = 300;			// delay de lectura de boton
		byte LCD_Col_Pos_Ant;					// saber el estado anterior para borrar cursor
		byte LCD_Row_Pos_Ant;					// saber el estado anterior para borrar cursor
		// guardar valor anterior de row col
			LCD_Col_Pos_Ant = LCD_Col_Pos;
			LCD_Row_Pos_Ant = LCD_Row_Pos;
		// Dibujar cursor
			lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
			lcd.print(">"); 													
		// navegacion
			Dibujar:
				byte Dibujar_Cursor = 0;		// saber si dibujar cursor para evitar repeticiones en lcd, 0 no dibujar, 1 dibujar >, 2 dibujar +
				// LCD Back Light *
					digitalWrite(Boton_Array_1, LOW);	// lectura linea 1
						if (digitalRead(Boton_Array_D) == LOW)
							{
								delay(Boton_Delay_Teclado);
								Back_Light_En();
							}
					digitalWrite(Boton_Array_1, HIGH);	// lectura linea 1
				// Left
					if (digitalRead(Boton_Left) == LOW)
						{
							delay (Boton_Delay_Cursor);
							byte Salida_Left = 0;
							byte LCD_Col_Pos_Temp = 0;
							LCD_Col_Pos_Temp = LCD_Col_Pos;
							while (Salida_Left == 0)
								{
									if (LCD_Col_Pos_Temp == 0)
										{
											LCD_Col_Pos_Temp = 20;
										}
											LCD_Col_Pos_Temp = LCD_Col_Pos_Temp - 1;
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos_Temp] == 1)
										{
											LCD_Col_Pos = LCD_Col_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Left = 1;
										}
								}
							goto Salida;
						}
				// Right
					if (digitalRead(Boton_Right) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Right = 0;
							byte LCD_Col_Pos_Temp = 0;
							LCD_Col_Pos_Temp = LCD_Col_Pos;
							while (Salida_Right == 0)
								{
									LCD_Col_Pos_Temp = LCD_Col_Pos_Temp + 1;
									if (LCD_Col_Pos_Temp >= 20)
										{
											LCD_Col_Pos_Temp = 0;	// regresar al cero
										}
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos_Temp] == 1)
										{
											LCD_Col_Pos = LCD_Col_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Right = 1;
										}
								}
							goto Salida;
						}
				// Down
					if (digitalRead(Boton_Down) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Down = 0;
							byte LCD_Row_Pos_Temp = 0;
							LCD_Row_Pos_Temp = LCD_Row_Pos;
							while (Salida_Down == 0)
								{
									LCD_Row_Pos_Temp = LCD_Row_Pos_Temp + 1;
									if (LCD_Row_Pos_Temp >= 4)
										{
											LCD_Row_Pos_Temp = 0;	// regresar al cero
										}
									if (Cursor_Conf[LCD_Row_Pos_Temp][LCD_Col_Pos] == 1)
										{
											LCD_Row_Pos = LCD_Row_Pos_Temp;
											Dibujar_Cursor = 1;
											Salida_Down = 1;
										}
								}
							goto Salida;
						}
				// Up
					if (digitalRead(Boton_Up) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Up = 0;
							byte LCD_Row_Pos_Temp;
							LCD_Row_Pos_Temp = LCD_Row_Pos;
							while (Salida_Up == 0)
								{
									if (LCD_Row_Pos_Temp <= 0)
										{
											LCD_Row_Pos_Temp = 4;
										}
									LCD_Row_Pos_Temp = LCD_Row_Pos_Temp - 1;
									if (Cursor_Conf[LCD_Row_Pos_Temp][LCD_Col_Pos] == 1)
										{
											Dibujar_Cursor = 1;
											LCD_Row_Pos = LCD_Row_Pos_Temp;
											Salida_Up = 1;
										}
								}
							goto Salida;
						}
				// Center
					if (digitalRead(Boton_Center) == LOW)
						{
							delay(Boton_Delay_Cursor);
							byte Salida_Center = 0;
							while (Salida_Center == 0)
								{
									if (Cursor_Conf[LCD_Row_Pos][LCD_Col_Pos] == 1)
										{
											Dibujar_Cursor = 2;	// dibujar +
											Salida_Center = 1;
										}
								}
						}
			Salida:
				// Dibujar Cursor
					if (Dibujar_Cursor > 0)
						{
							if (Dibujar_Cursor == 1)
								{
									// borra el anterior
										lcd.setCursor (LCD_Col_Pos_Ant, LCD_Row_Pos_Ant);
										lcd.print(" "); 
									// escribir >
										lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
										lcd.print(">");
									// matrix print posicion
										if (matrix == 1)
											{
												// banco inicial
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
															goto salida;
														}
												// banco  final
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
															goto salir;
														}
												// Memory
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
															goto salida;
														}
												// Unitary
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 0)
														{
															lcd.setCursor(1, 0);
															lcd.print("---");
															goto salida;
														}
												// banco 1
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco, 1, 0);
															goto salida;
														}
												// banco 2
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 1, 1, 0);
															goto salida;
														}
												// banco 3
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 2, 1, 0);
															goto salida;
														}
												// banco 4
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 3, 1, 0);
															goto salida;
														}
												// banco 5
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 1)
														{
															Numerico_Write (banco + 4, 1, 0);
															goto salida;
														}
												// banco 6
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 5, 1, 0);
															goto salida;
														}
												// banco 7
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 6, 1, 0);
															goto salida;
														}
												// banco 8
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 7, 1, 0);
															goto salida;
														}
												// banco 9
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 8, 1, 0);
															goto salida;
														}
												// banco 10
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 2)
														{
															Numerico_Write (banco + 9, 1, 0);
															goto salida;
														}
												// banco 11
													if (LCD_Col_Pos == 0 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 10, 1, 0);
															goto salida;
														}
												// banco 12
													if (LCD_Col_Pos == 4 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 11, 1, 0);
															goto salida;
														}
												// banco 13
													if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 12, 1, 0);
															goto salida;
														}
												// banco 14
													if (LCD_Col_Pos == 12 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 13, 1, 0);
															goto salida;
														}
												// banco 15
													if (LCD_Col_Pos == 16 && LCD_Row_Pos == 3)
														{
															Numerico_Write (banco + 14, 1, 0);
															goto salida;
														}
												// escribir guion de bancos
												salida:
													lcd.setCursor(16, 0);
													lcd.print("-");
												salir:
													{}
											}
								}
							else
								{
									// escribir +
										lcd.setCursor (LCD_Col_Pos, LCD_Row_Pos);
										lcd.print("+");
								}
			
							//salida
								LCD_Col_Pos_Ant = LCD_Col_Pos;
								LCD_Row_Pos_Ant = LCD_Row_Pos;
								if (Dibujar_Cursor == 2)
									{
										goto Salir;
									}
								else	
									{
										Dibujar_Cursor = 0;
									}
						}
			goto Dibujar;
			Salir: {}
	}

void GUI_Memory_Init()
	{
		inicio:
			int salir = 0;
			lcd.clear ();
			// Texto
				lcd.setCursor (0, 0);
				lcd.print("Initial Memory:");
				lcd.setCursor (2, 2);
				lcd.print("Empty");
				lcd.setCursor (9, 2);
				lcd.print("Load");
				lcd.setCursor (15, 2);
				lcd.print("Clear");
			// Cursor
				LCD_Col_Pos = 1;			// posicion de cursor
				LCD_Row_Pos = 2;
			// configuracion de cursor	
				Cursor_Conf_Clear();		// limpiar array
				// Acciones
					Cursor_Conf[2][1]  = 1;	// Empty
					Cursor_Conf[2][8]  = 1; // Load
					Cursor_Conf[2][14] = 1;	// Clear
			// navegar
				GUI_Navegar(0, 0);
			// Acciones
				// Load
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
						{
							salir = EEPROM_Load();
							if (salir == 1)
								{
									goto inicio;
								}
							GUI_Control_Options();
						}
				// Clear
					if (LCD_Col_Pos == 14 && LCD_Row_Pos == 2)
						{
							salir = EEPROM_Clear();
							if (salir == 1)
								{
									goto inicio;
								}
							GUI_Control_Options();
						}
				// Empty
					if (LCD_Col_Pos == 1 && LCD_Row_Pos == 2)
						{
							GUI_Control_Options();
						}
	}
	
int GUI_Memory_Bank(byte Opcion)
	{
		// regresa 1 si se selecciona salir, de lo contrario 0
		// recibe opcion para pintar en la pantalla lo que se selecciono
		int salir = 0;
		lcd.clear ();
		delay (200);	// retardo para no seleccionar inmediatamente la opcion del banco
		// Texto
			lcd.setCursor (6, 0);
			lcd.print("Memory Bank:");
			lcd.setCursor (1, 1);
			lcd.print("Bank1  Bank4  Bank7");
			lcd.setCursor (1, 2);
			lcd.print("Bank2  Bank5  Bank8");
			lcd.setCursor (1, 3);
			lcd.print("Bank3  Bank6  Exit");
			switch (Opcion)
				{
					case 1:
						lcd.setCursor (1, 0);
						lcd.print("Save");
						break;
					case 2:
						lcd.setCursor (1, 0);
						lcd.print("Load");
						break;
					case 3:
						lcd.setCursor (0, 0);
						lcd.print("Clear");
						break;
				}
			// Universo Actual
				lcd.setCursor (19, 0);
				if (Universo_Actual == 0)
					{
						lcd.print("-");
					}
				else
					{
						lcd.print(Universo_Actual);
					}
		// Cursor inicial
			LCD_Col_Pos = 0;			// posicion de cursor
			LCD_Row_Pos = 1;
		// configuracion de cursor	
			Cursor_Conf_Clear();		// limpiar array
			// Acciones
				Cursor_Conf[1][0]   = 1;	// Bank 1
				Cursor_Conf[2][0]   = 1;    // Bank 2
				Cursor_Conf[3][0]   = 1;	// Bank 3
				Cursor_Conf[1][7]   = 1;	// Bank 4
				Cursor_Conf[2][7]   = 1;	// Bank 5
				Cursor_Conf[3][7]   = 1;	// Bank 6
				Cursor_Conf[1][14]  = 1;	// Bank 7
				Cursor_Conf[2][14]  = 1;	// Bank 8
				Cursor_Conf[3][14]  = 1;	// Exit
		// navegar
			GUI_Navegar(0, 0);
		// Acciones
			// Bank 1
				if (LCD_Col_Pos == 0 && LCD_Row_Pos == 1)
					{
						Universo_Actual = 1;
						goto Salida;
					}
			// Bank 2
				if (LCD_Col_Pos == 0 && LCD_Row_Pos == 2)
					{
						Universo_Actual = 2;
						goto Salida;
					}
			// Bank 3
				if (LCD_Col_Pos == 0 && LCD_Row_Pos == 3)
					{
						Universo_Actual = 3;
						goto Salida;
					}
			// Bank 4
				if (LCD_Col_Pos == 7 && LCD_Row_Pos == 1)
					{
						Universo_Actual = 4;
						goto Salida;
					}
			// Bank 5
				if (LCD_Col_Pos == 7 && LCD_Row_Pos == 2)
					{
						Universo_Actual = 5;
						goto Salida;
					}
			// Bank 6
				if (LCD_Col_Pos == 7 && LCD_Row_Pos == 3)
					{
						Universo_Actual = 6;
						goto Salida;
					}
			// Bank 7
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 1)
					{
						Universo_Actual = 7;
						goto Salida;
					}
			// Bank 8
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 2)
					{
						Universo_Actual = 8;
						goto Salida;
					}
			// Exit
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 3)
					{
						salir = 1;
					}
		Salida:
			return salir;
	}
	
void GUI_Memory()
	{
		iniciar:
			lcd.clear ();
			// Texto
				lcd.setCursor (0, 0);
				lcd.print("Memory Options:   b");
				if (Universo_Actual == 0)
					{
						lcd.print("-");
					}
				else
					{
						lcd.print(Universo_Actual);
					}
				lcd.setCursor (1, 2);
				lcd.print("Save");
				lcd.setCursor (1, 3);
				lcd.print("Load");
				lcd.setCursor (7, 2);
				lcd.print("Empty");
				lcd.setCursor (7, 3);
				lcd.print("Clear");
				lcd.setCursor (14, 2);
				lcd.print("Black");
				lcd.setCursor (14, 3);
				lcd.print("Cancel");
			// Cursor
				LCD_Col_Pos = 0;			// posicion de cursor
				LCD_Row_Pos = 2;
			// configuracion de cursor	
				Cursor_Conf_Clear();		// limpiar array
				// Acciones
					Cursor_Conf[2][0]  = 1;	// Save
					Cursor_Conf[3][0]  = 1; // Load
					Cursor_Conf[3][6]  = 1;	// Clear
					Cursor_Conf[2][6]  = 1;	// Empty
					Cursor_Conf[2][13] = 1;	// Black Out
					Cursor_Conf[3][13] = 1;	// Cancel
			// navegar
		regresa:
				GUI_Navegar(0, 0);
			// Acciones
				// Load
					if (LCD_Col_Pos == 0 && LCD_Row_Pos == 3)
						{
							if (EEPROM_Load() == 1)
								{
									goto iniciar;
								}
							goto salida;
						}
				// Clear
					if (LCD_Col_Pos == 6 && LCD_Row_Pos == 3)
						{
							if (EEPROM_Clear() == 1)
								{
									goto iniciar;
								}
							goto salida;
						}
				// Save
					if (LCD_Col_Pos == 0 && LCD_Row_Pos == 2)
						{
							if (EEPROM_Save() == 1)
								{
									goto iniciar;
								}
							goto salida;
						}
				// Empty
					if (LCD_Col_Pos == 6 && LCD_Row_Pos == 2)
						{
							EEPROM_Empty();
							goto salida;
						}
				// Black Out
					if (LCD_Col_Pos == 13 && LCD_Row_Pos == 2)
						{
							Black_Out();
							goto regresa;
						}
				// Cancel
					if (LCD_Col_Pos == 3 && LCD_Row_Pos == 13)
						{

						}
		salida:
			{
			
			}
	}

void GUI_Secuencer()
	{
		// secuenciador de bancos guardados en eeprom
		int Delay_Secuencia = 1;
		int First_Bank = 1;
		int Final_Bank = 8;
		lcd.clear ();
		// Texto
			lcd.setCursor (0, 0);
			lcd.print("Secuencer Banks:  b");
			if (Universo_Actual == 0)
				{
					lcd.print("-");
				}
			else
				{
					lcd.print(Universo_Actual);
				}
			lcd.setCursor (4, 1);
			lcd.print("Delay 000x100=mS");
			lcd.setCursor (0, 2);
			lcd.print("FirstBank 000  Ctrl");
			lcd.setCursor (0, 3);
			lcd.print("FinalBank 000  Start");
		// Cursor
			LCD_Col_Pos = 9;			// posicion de cursor
			LCD_Row_Pos = 1;
		// configuracion de cursor	
			Cursor_Conf_Clear();		// limpiar array
			// Acciones
				Cursor_Conf[1][9]  = 1;	// Delay
				Cursor_Conf[2][9]  = 1; // First Bank
				Cursor_Conf[3][9]  = 1;	// Final Bank
				Cursor_Conf[2][14] = 1;	// Control
				Cursor_Conf[3][14] = 1;	// start
	inicio:	
		// navegar
			GUI_Navegar(0, 0);
		// Acciones
			// Delay
				if (LCD_Col_Pos == 9 && LCD_Row_Pos == 1)
					{
						Num_Row_Pos = 1;
						Num_Col_Pos = 10;
						Num_Val = Delay_Secuencia;					// para dejar el numero que estaba si no se cambia
						Numerico_Calc(0);
						if (Num_Val == 0)
							{
								Numerico_Write (1, 10, 1);
								Delay_Secuencia = 1;
								Numerico_Write (Delay_Secuencia, 10, 1);
							}
						else
							{
								Delay_Secuencia = Num_Val;			// por el multiplicador
							}
						goto inicio;
					}
			// First Bank
				if (LCD_Col_Pos == 9 && LCD_Row_Pos == 2)
					{
						Num_Row_Pos = 2;
						Num_Col_Pos = 10;
						Num_Val = First_Bank;						// para dejar el numero que estaba si no se cambia
						Numerico_Calc(0);
						First_Bank = Num_Val;
						if (Num_Val == 0)
							{
								Numerico_Write (1, 10, 2);
								First_Bank = 1;
								Numerico_Write (First_Bank, 10, 2);
							}
						if (Num_Val > 8)
							{
								Numerico_Write (8, 10, 2);
								First_Bank = 8;
							}
						goto inicio;
					}
			// Final Bank
				if (LCD_Col_Pos == 9 && LCD_Row_Pos == 3)
					{
						Num_Row_Pos = 3;
						Num_Col_Pos = 10;
						Num_Val = Final_Bank; 						// para dejar el numero que estaba si no se cambia
						Numerico_Calc(0);
						Final_Bank = Num_Val;
						if (Num_Val == 0)
							{
								Numerico_Write (1, 10, 2);
								Final_Bank = 1;
								Numerico_Write (Final_Bank, 10, 3);
							}
						if (Num_Val > 8)
							{
								Numerico_Write (8, 10, 2);
								Final_Bank = 8;
							}
						goto inicio;
					}
			// Control
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 2)
					{
						// limpiar salida dmx
						for(int Canal = 0; Canal <= 512; Canal ++)
							{
								ArduinoDmx0.TxBuffer[Canal] = 0; 		// salida a DMX
							}
						GUI_Control_Options ();
					}
			// start
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 3)
					{
						// LCD
							// lcd.blink();
							lcd.setCursor (14, 3);
							lcd.print("+");
							lcd.setCursor (15, 3);
							lcd.print("Stop ");
							lcd.setCursor (19, 3);
						// establecer reversa o adelante
							byte Adelante_Reversa = 0;				// 0 Adelante, 1 Reversa
							// adelante
								if (First_Bank < Final_Bank)
									{
										Adelante_Reversa = 0;
									}
							// reversa
								if (First_Bank > Final_Bank)
									{
										Adelante_Reversa = 1;
									}
						// establecer bancos a secuenciar
							byte Bancos [9] = {0, 0, 0, 0, 0, 0, 0, 0};
							if (Adelante_Reversa == 0)
								{
									for (byte Bank = 1; Bank <= 8; Bank ++)
										{
											if (Bank >= First_Bank && Bank <= Final_Bank)
												{
													Bancos[Bank] = 1;
												}
										}
								}
							if (Adelante_Reversa == 1)
								{
									for (byte Bank = 8; Bank >= 1; Bank --)
										{
											if (Bank <= First_Bank && Bank >= Final_Bank)
												{
													Bancos[Bank] = 1;
												}
										}
								}
						// Cargar Bancos en memoria
							byte Banco_1 [512];
							byte Banco_2 [512];
							byte Banco_3 [512];
							byte Banco_4 [512];
							byte Banco_5 [512];
							byte Banco_6 [512];
							byte Banco_7 [512];
							byte Banco_8 [512];
							// Banco 1
							if (Bancos [1] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_1[Canal] = EEPROM.read(Canal);             	// lectura desde EEPROM
										}
								}
							// Banco 2
							if (Bancos [2] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_2[Canal] = EEPROM.read(Canal + 512);          // lectura desde EEPROM
										}
								}
							// Banco 3
							if (Bancos [3] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_3[Canal] = EEPROM.read(Canal + 1024);          // lectura desde EEPROM
										}
								}
							// Banco 4
							if (Bancos [4] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_4[Canal] = EEPROM.read(Canal + 1536);          // lectura desde EEPROM
										}
								}
							// Banco 5
							if (Bancos [5] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_5[Canal] = EEPROM.read(Canal + 2048);          // lectura desde EEPROM
										}
								}
							// Banco 6
							if (Bancos [6] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_6[Canal] = EEPROM.read(Canal + 2560);          // lectura desde EEPROM
										}
								}
							// Banco 7
							if (Bancos [7] == 1)
								{
									for(int Canal = 0; Canal <= 512; Canal ++)
										{
											Banco_7[Canal] = EEPROM.read(Canal + 3072);          // lectura desde EEPROM
										}
								}
							// Banco 8
							if (Bancos [8] == 1)
								{
									for(int Canal = 0; Canal <= 510; Canal ++)					 // dos canales menos
										{
											Banco_8[Canal] = EEPROM.read(Canal + 3584);          // lectura desde EEPROM
										}
								}
						// Secuenciar
										if (Adelante_Reversa == 0)				// adelante
											{
												for (byte conteo = 1; conteo <= 8; conteo ++)
													{
														if (Bancos [conteo] = 1)
															{
																lcd.setCursor (19, 0);
																lcd.print(conteo);
																for (int canal = 0; canal <= 512; canal ++)
																	{
																		switch (conteo)
																			{
																				case 1:
																					ArduinoDmx0.TxBuffer[canal] = Banco_1[canal]; 	// salida a DMX
																					break;
																				case 2:
																					ArduinoDmx0.TxBuffer[canal] = Banco_2[canal]; 	// salida a DMX
																					break;
																				case 3:
																					ArduinoDmx0.TxBuffer[canal] = Banco_3[canal]; 	// salida a DMX
																					break;
																				case 4:
																					ArduinoDmx0.TxBuffer[canal] = Banco_4[canal]; 	// salida a DMX
																					break;
																				case 5:
																					ArduinoDmx0.TxBuffer[canal] = Banco_5[canal]; 	// salida a DMX
																					break;
																				case 6:
																					ArduinoDmx0.TxBuffer[canal] = Banco_6[canal]; 	// salida a DMX
																					break;
																				case 7:
																					ArduinoDmx0.TxBuffer[canal] = Banco_7[canal]; 	// salida a DMX
																					break;
																				case 8:
																					ArduinoDmx0.TxBuffer[canal] = Banco_8[canal]; 	// salida a DMX
																					break;
																			}
																		//delay
																			long delay_contar = Delay_Secuencia;
																			while (digitalRead(Boton_Center) == HIGH)				// lectura del boton centro
																				{	
																					for (long contar = 0; contar <= delay_contar; contar ++)
																						{
																							delayMicroseconds(100);
																						}
																					goto Delay_Salir;
																				}
																			delay(200);		// rebote de boton
																			goto inicio;
																		Delay_Salir:
																			{
																				
																			}
																	}
															}
													}
											}
										if (Adelante_Reversa == 1)				// reversa
											{
												for (byte conteo = 8; conteo >= 1; conteo --)
													{
														if (Bancos [conteo] = 1)
															{
																lcd.setCursor (19, 0);
																lcd.print(conteo);
																for (int canal = 0; canal <= 512; canal ++)
																	{
																		switch (conteo)
																			{
																				case 1:
																					ArduinoDmx0.TxBuffer[canal] = Banco_1[canal]; 	// salida a DMX
																					break;
																				case 2:
																					ArduinoDmx0.TxBuffer[canal] = Banco_2[canal]; 	// salida a DMX
																					break;
																				case 3:
																					ArduinoDmx0.TxBuffer[canal] = Banco_3[canal]; 	// salida a DMX
																					break;
																				case 4:
																					ArduinoDmx0.TxBuffer[canal] = Banco_4[canal]; 	// salida a DMX
																					break;
																				case 5:
																					ArduinoDmx0.TxBuffer[canal] = Banco_5[canal]; 	// salida a DMX
																					break;
																				case 6:
																					ArduinoDmx0.TxBuffer[canal] = Banco_6[canal]; 	// salida a DMX
																					break;
																				case 7:
																					ArduinoDmx0.TxBuffer[canal] = Banco_7[canal]; 	// salida a DMX
																					break;
																				case 8:
																					ArduinoDmx0.TxBuffer[canal] = Banco_8[canal]; 	// salida a DMX
																					break;
																			}
																		//delay
																			long delay_contar = Delay_Secuencia;
																			while (digitalRead(Boton_Center) == HIGH)				// lectura del boton centro
																				{	
																					for (long contar = 0; contar <= delay_contar; contar ++)
																						{
																							delayMicroseconds(100);
																						}
																					goto Delay_Salir_rev;
																				}
																			delay(200);		// rebote de boton
																			goto inicio;
																		Delay_Salir_rev:
																			{
																				
																			}
																	}
															}
													}
											}							
					}
	}

						// LCD
							// lcd.noBlink();
							// lcd.setCursor (14, 3);
							// lcd.print(">");
							// lcd.setCursor (15, 3);
							// lcd.print("Start");
						
					

	
void Black_Out()
	{
		lcd.setCursor (19, 2);
		lcd.blink();
		// limpiar universo
			for(int Canal = 1; Canal <= 512; Canal ++)
				{
					ArduinoDmx0.TxBuffer[Canal - 1] = 0; 
				}
		// esperar al boton centro para cancelar
			while (digitalRead(Boton_Center) == HIGH)
				{

				}
			delay(300);		// retardo de rebote de boton
		// regresar el universo a su lugar
			for(int Canal = 1; Canal <= 512; Canal ++)
				{
					ArduinoDmx0.TxBuffer[Canal - 1] = DMX_Values[Canal];
				}
		lcd.setCursor (13, 2);
		lcd.noBlink();
	}

int EEPROM_Save()
	{
		// guarda el universo en la eeprom
		// regresa 1 si se selecciona exit
		int cancel = 0;				// regresa 1 si se selecciona salir			
		int bank;					// regresa 1 si se selecciona salir
		int EEPROM_Add = 0;			// direccion de eeprom para universos
		bank = GUI_Memory_Bank(1);	// seleccinar banco
		if (bank == 1)
			{
				cancel = 1;
				goto salida;
			}
		lcd.clear ();
		lcd.setCursor (1, 1);
		lcd.print ("Memory Saving...");
		lcd.setCursor (1, 2);
		lcd.print ("Bank: ");
		lcd.setCursor (7, 2);
		lcd.print (Universo_Actual);
		lcd.setCursor (19, 3);
		lcd.blink();
		for(int Canal = 1; Canal <= 512; Canal ++)
			{
				// Escritura de universo EEPROM
					switch (Universo_Actual)
						{
							case 1:
								EEPROM_Add = 0 + Canal;
								break;
							case 2:
								EEPROM_Add = 512 + Canal;
								break;
							case 3:
								EEPROM_Add = 1024 + Canal;
								break;
							case 4:
								EEPROM_Add = 1536 + Canal;
								break;
							case 5:
								EEPROM_Add = 2048 + Canal;
								break;
							case 6:
								EEPROM_Add = 2560 + Canal;
								break;
							case 7:
								EEPROM_Add = 3072 + Canal;
								break;
							case 8:
								EEPROM_Add = 3584 + Canal;
								if (EEPROM_Add > 4093)
									{
										EEPROM_Add = 4093;
									}
								break;
						}
				EEPROM.write(EEPROM_Add, DMX_Values[Canal]);          		// lectura desde EEPROM
				//lcd.print (Canal, BIN);
			}
		lcd.clear ();
		lcd.setCursor (3, 1);
		lcd.print ("Memory Saved!");
		lcd.setCursor (3, 2);
		lcd.print ("Bank: ");
		lcd.print (Universo_Actual);
		lcd.setCursor (19, 3);
		delay (2000);
		lcd.noBlink();
		salida:
			return cancel;
	}
	
int EEPROM_Load()
	{
		// guarda los valores en la eeprom
		// regresa 1 si se selecciona exit
		int cancel = 0;				// regresa 1 si se selecciona salir			
		int bank;					// regresa 1 si se selecciona salir
		int EEPROM_Add = 0;			// seleccion de universo
		bank = GUI_Memory_Bank(2);	// seleccinar banco
		if (bank == 1)
			{
				cancel = 1;
				goto salida;
			}
		lcd.clear ();
		for(int Canal = 1; Canal <= 512; Canal ++)
			{
				// Escritura de universo EEPROM
					switch (Universo_Actual)
						{
							case 1:
								EEPROM_Add = 0 + Canal;
								break;
							case 2:
								EEPROM_Add = 512 + Canal;
								break;
							case 3:
								EEPROM_Add = 1024 + Canal;
								break;
							case 4:
								EEPROM_Add = 1536 + Canal;
								break;
							case 5:
								EEPROM_Add = 2048 + Canal;
								break;
							case 6:
								EEPROM_Add = 2560 + Canal;
								break;
							case 7:
								EEPROM_Add = 3072 + Canal;
								break;
							case 8:
								EEPROM_Add = 3584 + Canal;
								if (EEPROM_Add > 4093)
									{
										EEPROM_Add = 4093;
									}
								break;
						}
				DMX_Values[Canal] = EEPROM.read(EEPROM_Add);          	// lectura desde EEPROM
				ArduinoDmx0.TxBuffer[Canal - 1] = DMX_Values[Canal]; 	// salida a DMX
			}
		lcd.clear ();
		lcd.setCursor (3, 1);
		lcd.print ("Memory Loaded!");
		lcd.setCursor (3, 2);
		lcd.print ("Bank: ");
		lcd.print (Universo_Actual);
		lcd.setCursor (19, 3);
		lcd.blink();
		delay (2000);
		lcd.noBlink();
		salida:
			return cancel;
	}
	
void EEPROM_Empty()
	{
		// solo borra la ram
		// no hay retorno al menu anterior
		lcd.clear ();
		for(int Canal = 0; Canal <= 512; Canal ++)
			{
				DMX_Values[Canal] = 0;          		// lectura desde EEPROM
				ArduinoDmx0.TxBuffer[Canal] = 0; 		// salida a DMX
			}
		lcd.clear ();
		lcd.setCursor (3, 1);
		lcd.print ("Memory Emptied!");
		lcd.setCursor (3, 2);
		lcd.print ("Bank: RAM");
		lcd.setCursor (19, 3);
		lcd.blink();
		delay (2000);
		lcd.noBlink();
	}
	
int EEPROM_Clear()
	{
		// Pone en ceros la memoria
		// regresa 1 si se selecciona exit
		int cancel = 0;				// regresa 1 si se selecciona salir			
		int bank;					// regresa 1 si se selecciona salir
		int EEPROM_Add = 0;			// direccion de eeprom para universos
		bank = GUI_Memory_Bank(3);	// seleccinar banco
		if (bank == 1)
			{
				cancel = 1;
				goto salida;
			}
		lcd.clear ();
		lcd.setCursor (1, 1);
		lcd.print ("Memory Cleaning...");
		lcd.setCursor (1, 2);
		lcd.print ("Bank: ");
		lcd.setCursor (7, 2);
		lcd.print (Universo_Actual);
		lcd.setCursor (19, 3);
		lcd.blink();
		for(int Canal = 0; Canal <= 512; Canal ++)
			{
				DMX_Values[Canal] = 0;          		// lectura desde EEPROM
				ArduinoDmx0.TxBuffer[Canal] = 0; 		// salida a DMX
				// Escritura de universo EEPROM
					switch (Universo_Actual)
						{
							case 1:
								EEPROM_Add = 0 + Canal;
								break;
							case 2:
								EEPROM_Add = 512 + Canal;
								break;
							case 3:
								EEPROM_Add = 1024 + Canal;
								break;
							case 4:
								EEPROM_Add = 1536 + Canal;
								break;
							case 5:
								EEPROM_Add = 2048 + Canal;
								break;
							case 6:
								EEPROM_Add = 2560 + Canal;
								break;
							case 7:
								EEPROM_Add = 3072 + Canal;
								break;
							case 8:
								EEPROM_Add = 3584 + Canal;
								if (EEPROM_Add > 4093)
									{
										EEPROM_Add = 4093;
									}
								break;
						}
					EEPROM.write (EEPROM_Add, 0);		// escritura EEPROM
			}
		lcd.clear ();
		lcd.setCursor (2, 1);
		lcd.print ("Memory Cleaned!");
		lcd.setCursor (2, 2);
		lcd.print ("Bank: ");
		lcd.print (Universo_Actual);
		lcd.setCursor (19, 3);
		delay (2000);
		lcd.noBlink();
		salida:
			return cancel;
	}
	
void GUI_Control_Options()
	{
		// LCD
			lcd.clear ();
			lcd.setCursor (0, 0);
			lcd.print ("Control Options:  b");
			if (Universo_Actual == 0)
				{
					lcd.print ("-");
				}
			else
				{
					lcd.print (Universo_Actual);
				}
			lcd.setCursor (2, 1);
			lcd.print ("Unitary");
			lcd.setCursor (2, 2);
			lcd.print ("Matrix");
			lcd.setCursor (2, 3);
			lcd.print ("Chaser");
			lcd.setCursor (11, 3);
			lcd.print ("Multiply");
			lcd.setCursor (11, 1);
			lcd.print ("Config");
			lcd.setCursor (11, 2);
			lcd.print ("Secuencer");
		// Cursor
			LCD_Col_Pos = 1;				// posicion de cursor
			LCD_Row_Pos = 2;
		// configuracion de cursor	
			Cursor_Conf_Clear();			// limpiar array
		// Acciones
			Cursor_Conf[1][1]   = 1;	// Unitary
			Cursor_Conf[2][1]   = 1; 	// Matrix
			Cursor_Conf[3][1]   = 1; 	// Chaser
			Cursor_Conf[3][10]  = 1; 	// Multiply
			Cursor_Conf[1][10]  = 1; 	// Config
			Cursor_Conf[2][10]  = 1; 	// Secuencer
		// navegar
			GUI_Navegar(0, 0);
		// Acciones
			// Unitary
				if (LCD_Col_Pos == 1 && LCD_Row_Pos == 1)
					{
						GUI_Control_Unit();
					}
			// Matrix
				if (LCD_Col_Pos == 1 && LCD_Row_Pos == 2)
					{
						GUI_Control_Matrix();
					}
			// Chaser
				if (LCD_Col_Pos == 1 && LCD_Row_Pos == 3)
					{
						GUI_Control_Chaser();
					}
			// Multiply
				if (LCD_Col_Pos == 10 && LCD_Row_Pos == 3)
					{
						GUI_Control_Multiply();
					}
			// Config
				if (LCD_Col_Pos == 10 && LCD_Row_Pos == 1)
					{
						GUI_Config();
					}
			// Secuencer
				if (LCD_Col_Pos == 10 && LCD_Row_Pos == 2)
					{
						GUI_Secuencer();
					}	
	}

void GUI_Config()
	{
	Inicio:	
		byte Back_Light_Value = EEPROM.read(BackLight_Add);
		byte Contrast_Value = EEPROM.read(Contrast_Add);
		// GUI
			lcd.clear ();
			lcd.setCursor (0, 0);
			lcd.print ("LCD Config:");
			lcd.setCursor (15, 0);
			lcd.print ("About");
			lcd.setCursor (3, 1);
			lcd.print ("Back Light:");
			Numerico_Write(Back_Light_Value, 15, 1);
			lcd.setCursor (5, 2);
			lcd.print ("Contrast:");
			Numerico_Write(Contrast_Value, 15, 2);
			lcd.setCursor (15, 3);
			lcd.print ("Ctrl");
			
		// Cursor
			LCD_Col_Pos = 14;			// posicion de cursor
			LCD_Row_Pos = 2;			// posicion e cursor
		// configuracion de cursor	
			Cursor_Conf_Clear();		// limpiar array
		// Cursores
			Cursor_Conf[1][14]  = 1;	// Back Light Value
			Cursor_Conf[2][14]  = 1;	// Contrast Value
			Cursor_Conf[3][14]  = 1;	// control
			Cursor_Conf[0][14]  = 1;	// About
		// navegar
	Navegacion:
			GUI_Navegar(0, 0);
		// Acciones
			//Back Light Value
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 1)
					{
						Num_Row_Pos = 1;
						Num_Col_Pos = 15;
						Numerico_Calc (1);
						if (Num_Val == 712)
							{
								lcd.setCursor (14, 1);
								lcd.print("a");									// indicar que es analogo
								digitalWrite(Boton_Array_3, LOW);				// lectura linea 3
								lcd.blink();
								while (digitalRead(Boton_Array_D) == HIGH && digitalRead(Boton_Center) == HIGH) // enter y center para paro
									{
										Num_Val = analogRead(Pot);				// lectura desde el potenciometro
										Num_Val = Num_Val / 4;					// / 4 porque es de 12 bits
										Numerico_Write(Num_Val, 15, 1);
										analogWrite(Back_Light_PWM, Num_Val);
										delay(50);								// retardo de lectura
									}
								lcd.noBlink();
								digitalWrite(Boton_Array_3, HIGH);				// lectura linea 3
								delay(300);										// retraso para center	
								goto salida;
							}
						if (Num_Val > 255)
							{
								Num_Val = 255;
								Numerico_Write (255, 15, 1);
							}
						analogWrite(Back_Light_PWM, Num_Val);
						salida:
							// mecanismo para on off Enable
								if (Num_Val == 0)
									{
										Back_Light_On_Off = 0;
									}
								if (Num_Val > 0)
									{
										Back_Light_On_Off = 1;
									}
							EEPROM.write(BackLight_Add, Num_Val);				// guardar valor nuevo
							goto Navegacion;
					}
			//Contrast Value
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 2)
					{
						Num_Row_Pos = 2;
						Num_Col_Pos = 15;
						Numerico_Calc (1);
						if (Num_Val == 712)
							{
								lcd.setCursor (14, 2);
								lcd.print("a");									// indicar que es analogo
								digitalWrite(Boton_Array_3, LOW);				// lectura linea 3
								lcd.blink();
								while (digitalRead(Boton_Array_D) == HIGH && digitalRead(Boton_Center) == HIGH) // enter y center para paro
									{
										Num_Val = analogRead(Pot);				// lectura desde el potenciometro
										Num_Val = Num_Val / 4;					// / 4 porque es de 12 bits
										if (Num_Val > 149)						// limite menor de contraste LCD						
											{
												Numerico_Write(Num_Val, 15, 2);
												analogWrite(Contrast_PWM, Num_Val);
											}
										if (Num_Val < 149)
											{
												Numerico_Write(150, 15, 2);
											}
										delay(50);								// retardo de lectura
									}
								lcd.noBlink();
								digitalWrite(Boton_Array_3, HIGH);				// lectura linea 3
								delay(300);										// retraso para center	
								goto salir;
							}
						if (Num_Val > 255)
							{
								Num_Val = 255;
								Numerico_Write (255, 15, 2);
							}
						if (Num_Val < 150)
							{
								Num_Val = 150;									// limite menor de contraste LCD
								Numerico_Write (150, 15, 2);
							}
						analogWrite(Contrast_PWM, Num_Val);
						salir:	
							EEPROM.write(Contrast_Add, Num_Val);							// guardar valor nuevo
							goto Navegacion;
					}
			// Exit
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 3)
					{
						GUI_Control_Options();
						goto Navegacion;
					}
			// About
				if (LCD_Col_Pos == 14 && LCD_Row_Pos == 0)
					{
						GUI_About();
						while (digitalRead(Boton_Center) == HIGH)
							{
									// esperamos a que se precione enter
							}
						delay(300);	// retardo para el rebote del boton
						goto Inicio;
					}
			goto Navegacion;
	}

void GUI_Control_Multiply()
	{
		int  First_Channel = 1;
		long Multiply 	   = 0;
		long Quantity 	   = 0;
		int  Value         = 255;
		iniciar:
			// LCD
				lcd.clear ();
				lcd.setCursor (0, 0);
				lcd.print ("b  Value");
				lcd.setCursor (1, 0);
				if (Universo_Actual == 0)
					{
						lcd.print("-");
					}
				else
					{
						lcd.print(Universo_Actual);
					}
				lcd.setCursor (0, 1);
				lcd.print ("First CH      Ctrl");
				lcd.setCursor (0, 2);
				lcd.print ("Multiply      Memory");
				lcd.setCursor (0, 3);
				lcd.print ("Quantity      Apply");
				Numerico_Write (Value, 		   9, 0);
				Numerico_Write (First_Channel, 9, 1);
				Numerico_Write (Multiply,      9, 2);
				Numerico_Write (Quantity,      9, 3);
			// Cursor
				LCD_Col_Pos = 8;			// posicion de cursor
				LCD_Row_Pos = 0;
		Siguiente:	
			// configuracion de cursor		
				Cursor_Conf_Clear();		// limpiar array
			// Acciones
				Cursor_Conf[1][8]   = 1;	// first Channel
				Cursor_Conf[3][8]   = 1; 	// quantity
				Cursor_Conf[2][8]   = 1; 	// Multiply
				Cursor_Conf[1][13]  = 1; 	// control
				Cursor_Conf[2][13]  = 1; 	// Memory
				Cursor_Conf[3][13]  = 1; 	// apply
				Cursor_Conf[0][8]   = 1; 	// value
			// navegar
				GUI_Navegar(0, 0);
			// Acciones
				// Control
					if (LCD_Col_Pos == 13 && LCD_Row_Pos == 1)
						{
							GUI_Control_Options();
						}
				// Memory
					if (LCD_Col_Pos == 13 && LCD_Row_Pos == 2)
						{
							GUI_Memory();
							goto iniciar;
						}
				// Apply
					if (LCD_Col_Pos == 13 && LCD_Row_Pos == 3)
						{
							// Validar datos
								long valid = Multiply * Quantity;
								if (valid > 512)	// si no es valido
									{
										lcd.setCursor(13, 0);
										lcd.print("Error! ");
										goto Siguiente;
									}
							// calcular
								lcd.setCursor(13, 0);
								lcd.print("       ");
								lcd.setCursor(14, 3);
								lcd.print("Calc..");
								long canal;
								for (long conteo = 1; conteo <= Quantity; conteo++)
									{
										if (conteo == 1)
											{
												canal = First_Channel;
											}
										if (conteo != 1)
											{
												canal = canal + Multiply;
											}
										if (canal > 512)
											{
												canal = canal - Multiply;
												conteo = Quantity;
											}
										lcd.setCursor(17, 0);
										lcd.print(canal);
										delay (5);
										ArduinoDmx0.TxBuffer[canal - 1] = Value;
										DMX_Values[canal] = Value;
									}
								lcd.setCursor(13, 0);
								lcd.print("Ok!");
								lcd.setCursor(14, 3);
								lcd.print("Apply ");
								goto Siguiente;
						}
				// First Channel
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 1)
						{
							Num_Row_Pos = 1;
							Num_Col_Pos = 9;
							Num_Val = First_Channel;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							First_Channel = Num_Val;
							if (First_Channel == 0)
								{
									First_Channel = 1;
									Numerico_Write (First_Channel, 9, 1);
								}
							if (First_Channel > 512)
								{
									First_Channel = 512;
									Numerico_Write (First_Channel, 9, 1);
								}
						}
				// Multiply
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
						{
							Num_Row_Pos = 2;
							Num_Col_Pos = 9;
							Num_Val = Multiply;			// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							Multiply = Num_Val;
							if (Multiply == 0)
								{
									Multiply = 1;
									Numerico_Write (Multiply, 9, 2);
								}
							if (Multiply > 512)
								{
									Multiply = 512;
									Numerico_Write (Multiply, 9, 2);
								}
						}
				// Quantity
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
						{
							Num_Row_Pos = 3;
							Num_Col_Pos = 9;
							Num_Val = Quantity;			// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							Quantity = Num_Val;
							if (Quantity == 0)
								{
									Quantity = 1;
									Numerico_Write (Quantity, 9, 3);
								}
							if (Quantity > 512)
								{
									Quantity = 512;
									Numerico_Write (Quantity, 9, 3);
								}
						}
				// Value
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 0)
						{
							Num_Row_Pos = 0;
							Num_Col_Pos = 9;
							Num_Val = Value;			// para dejar el numero que estaba si no se cambia
							Numerico_Calc(1);
							Value = Num_Val;
							if (Value > 255)
								{
									Value = 255;
									Numerico_Write (Value, 9, 0);
								}
						}
			goto Siguiente;
	}
	
void GUI_Control_Chaser()
	{
		long Delay     = 1;
		long First 	   = 1;
		long Final 	   = 0;
			// LCD
				lcd.clear ();
				lcd.setCursor (0, 0);
				lcd.print ("ChaserCH ---");
				lcd.setCursor (3, 1);
				lcd.print ("Delay    x10=mS");
				lcd.setCursor (0, 2);
				lcd.print ("First CH       Ctrl");
				lcd.setCursor (0, 3);
				lcd.print ("Final CH       Start");
				Numerico_Write (Delay, 9, 1);
				Numerico_Write (First, 9, 2);
				Numerico_Write (Final, 9, 3);
			// Cursor
				LCD_Col_Pos = 8;			// posicion de cursor
				LCD_Row_Pos = 1;
		Siguiente:	
			// configuracion de cursor		
				Cursor_Conf_Clear();		// limpiar array
			// Acciones
				Cursor_Conf[1][8]   = 1;	// Delay
				Cursor_Conf[2][8]   = 1; 	// First
				Cursor_Conf[3][8]   = 1; 	// Final
				Cursor_Conf[2][14]  = 1; 	// control
				Cursor_Conf[3][14]  = 1; 	// Start
			// navegar
				GUI_Navegar(0, 0);
			// Acciones
				// Control
					if (LCD_Col_Pos == 14 && LCD_Row_Pos == 2)
						{
							// regresar el universo a su lugar
								for(int Canal = 1; Canal <= 512; Canal ++)
									{
										ArduinoDmx0.TxBuffer[Canal - 1] = DMX_Values[Canal];
									}
							GUI_Control_Options();
						}
				// Start
					if (LCD_Col_Pos == 14 && LCD_Row_Pos == 3)
						{
							long ciclo_longitud = 1300;							// numero multiplicador aprox para 1 seg
							long ciclo 			= 0;
							long Delay_Cont		= 0;
							int  canal 			= First;
							lcd.setCursor(15,3);
							lcd.print("Stop ");
							lcd.blink();
							// borrar canales previos
								for(int Canales = 0; Canales <= 512; Canales ++)
									{
										ArduinoDmx0.TxBuffer[Canales] = 0; 		// salida a DMX
									}
							while (digitalRead(Boton_Center) == HIGH)			// lectura del boton centro
								{
									ciclo = ciclo + 1;
									if (ciclo == ciclo_longitud)
										{
											Delay_Cont = Delay_Cont + 1;
											if (Delay_Cont == Delay)
												{
													if (canal > Final)
														{
															canal = First;
														}
													Numerico_Write (canal, 9, 0);
													// apagar lo anterior
														for (long contar = First; contar <= Final; contar ++)
															{
																ArduinoDmx0.TxBuffer[contar - 1] = 0;
															}
													// encender el siguiente
														ArduinoDmx0.TxBuffer[canal - 1] = 255;
													Delay_Cont = 0;
													canal = canal + 1;
												}
											ciclo = 0;
										}
								}
							lcd.noBlink();
							lcd.setCursor(15,3);
							lcd.print("Start");
							delay (300); 		// evita que le gane la descarga del capacitor
							goto Siguiente;
						}
				// Delay
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 1)
						{
							Num_Row_Pos = 1;
							Num_Col_Pos = 9;
							Num_Val = Delay;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							Delay = Num_Val;
						}
				// First
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
						{
							Num_Row_Pos = 2;
							Num_Col_Pos = 9;
							Num_Val = First;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							First = Num_Val;
							if (First == 0)
								{
									First = 1;
									Numerico_Write (First, 9, 2);
								}
							if (First > 512)
								{
									First = 512;
									Numerico_Write (First, 9, 2);
								}
						}
				// Final
					if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
						{
							Num_Row_Pos = 3;
							Num_Col_Pos = 9;
							Num_Val = Final;	// para dejar el numero que estaba si no se cambia
							Numerico_Calc(0);
							Final = Num_Val;
							if (Final == 0)
								{
									Final = 1;
									Numerico_Write (Final, 9, 3);
								}
							if (Final > 512)
								{
									Final = 512;
									Numerico_Write (Final, 9, 3);
								}
						}
			goto Siguiente;
	}
	
void Numerico_Write (int valor, int col, int row)
	{
		//posicionar el valor en los campos 000
			lcd.setCursor (col, row);
			lcd.print ("000");
		if (valor < 10)
			{
				lcd.setCursor (col + 2, row);
			}
		if (valor > 9 && valor < 100)
			{
				lcd.setCursor (col + 1, row);
			}
		if (valor > 99)
			{
				lcd.setCursor (col, row);
			}
		lcd.print (valor);
	}
	
void GUI_Control_Unit()
	{
	Inicio:	
		Canal_Actual = 1;
		// GUI
			lcd.clear ();
			lcd.setCursor (0, 0);
			lcd.print ("Unitary Control:  b");
			if (Universo_Actual == 0)
				{
					lcd.print("-");
				}
			else
				{
					lcd.print(Universo_Actual);
				}
			lcd.setCursor (14, 2);
			lcd.print ("Ctrl");
			lcd.setCursor (14, 3);
			lcd.print ("Memory");
			lcd.setCursor (0, 2);
			lcd.print ("Channel: 001");
			lcd.setCursor (2, 3);
			lcd.print ("Value:");
			Numerico_Write(DMX_Values[1], 9, 3);
		// Cursor
			LCD_Col_Pos = 8;		// posicion de cursor
			LCD_Row_Pos = 2;		// posicion e cursor
		// configuracion de cursor	
			Cursor_Conf_Clear();	// limpiar array
		// Cursores
			Cursor_Conf[2][8]  = 1;	// Channel
			Cursor_Conf[3][8]  = 1;	// Value
			Cursor_Conf[2][13] = 1; // Control
			Cursor_Conf[3][13] = 1;	// Memory
		// navegar
	Navegacion:
			GUI_Navegar(0, 0);
		// Acciones
			//Channel
				if (LCD_Col_Pos == 8 && LCD_Row_Pos == 2)
					{
						Numerico_Write(Canal_Actual, 9, 2);
						Num_Row_Pos = 2;
						Num_Col_Pos = 9;
						Num_Val = Canal_Actual;		// para dejar el numero que estaba si no se cambia
						Numerico_Calc (0);
						if (Num_Val > 512)
							{
								Num_Val = 512;
								Numerico_Write (512, 9, 2);
							}
						if (Num_Val < 1)
							{
								Num_Val = 1;
								Numerico_Write (1, 9, 2);
							}
						// mostrar valor actual del canal								
							Canal_Actual = Num_Val;
							Numerico_Write(DMX_Values[Canal_Actual], 9, 3);
					}
			// Value
				if (LCD_Col_Pos == 8 && LCD_Row_Pos == 3)
					{
						Num_Row_Pos = 3;
						Num_Col_Pos = 9;
						Num_Val = DMX_Values[Canal_Actual];		// para dejar el numero que estaba si no se cambia
						Numerico_Calc (1);
						if (Num_Val == 612)						// ubicar
							{
								Ubicar();
							}
						if (Num_Val == 712)						// analogo
							{
								Analog_Read_DMX(9, 3);
								goto Navegacion;
							}
						if (Num_Val > 255)
							{
								Num_Val = 255;
								Numerico_Write (255, 9, 3);
							}
						// Escribr valor en dmx
							ArduinoDmx0.TxBuffer[Canal_Actual - 1] = Num_Val;
							DMX_Values[Canal_Actual] = Num_Val;
						goto Navegacion;
					}
			// Memory
				if (LCD_Col_Pos == 13 && LCD_Row_Pos == 3)
					{
						GUI_Memory();
						goto Inicio;
					}
			// Control
				if (LCD_Col_Pos == 13 && LCD_Row_Pos == 2)
					{
						GUI_Control_Options();
					}
			goto Navegacion;
	}
	
void Ubicar()
	{
		digitalWrite(Boton_Array_4, LOW);
		while (digitalRead(Boton_Array_C) == HIGH && digitalRead(Boton_Center) == HIGH)
			{
				ArduinoDmx0.TxBuffer[Canal_Actual - 1] = 255;
				Numerico_Write (255, Num_Col_Pos - 2, Num_Row_Pos);
				delay (100);
				ArduinoDmx0.TxBuffer[Canal_Actual - 1] = 0;
				lcd.setCursor (Num_Col_Pos - 2, Num_Row_Pos);
				lcd.print ("   ");
				delay (100);
			}
		digitalWrite(Boton_Array_4, HIGH);
		delay(300); // rebote de boton enter
		lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
	}
	
void Numerico_Calc(byte value)
	{
	// escritura del numero desde el teclado numerico si value es 1 entra opcion de A 255 y B 0
		int 	Num_Val_Temp_1 	 = 0;
		int 	Num_Val_Temp_2 	 = 0;
		lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		lcd.print("___");
		lcd.blink();								// mostrar cursor
		//Num_Val = 0;
		Num_Col_Pos = Num_Col_Pos + 2;
		lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		// primer numero
			Numerico_Read();						// leer boton
			if (Boton_Calc <= 9)
				{
					lcd.print (Boton_Calc);			// escribir valor calculado
					lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
					Num_Val = Boton_Calc;			// valor calculado
					Num_Val_Temp_1 = Boton_Calc;	// valor temporal para el acarreo
				}
			if (Boton_Calc == 14)					// enter
				{
					goto Salida;					// num val = 0		
				}
			if (value == 1)							// si es un valor
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// ubicar
						{
							Num_Val = 612;
							goto Salida_Option;
						}
					if (Boton_Calc == 13)			// analogo
						{
							Num_Val = 712;
							goto Salida_Option;
						}
				}
		// segundo numero
			Numerico_Read();						// leer boton
			if (Boton_Calc == 14)					// enter
				{
					Num_Val = Num_Val_Temp_1;	
					goto Salida;					// num val = num val anterior
				}
			if (value == 1)							// si es un valor
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// ubicar
						{
							Num_Val = 612;
							goto Salida_Option;
						}
					if (Boton_Calc == 13)			// analogo
						{
							Num_Val = 712;
							goto Salida_Option;
						}
				}
			if (Boton_Calc <= 9)
				{
					// recorrer numero ya impreso
						Num_Col_Pos = Num_Col_Pos - 1;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Num_Val_Temp_1);
					// dibujar numero nuevo
						lcd.print (Boton_Calc);
						Num_Val_Temp_2 = Boton_Calc;
						Num_Col_Pos = Num_Col_Pos + 1;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
				}
		// Tercer numero
			Numerico_Read();						// leer boton
			if (Boton_Calc == 14)					// enter
				{	
					Num_Val = (Num_Val_Temp_1 * 10) + Num_Val_Temp_2;
					goto Salida;
				}
			if (value == 1)							// si es un valor
				{
					if (Boton_Calc == 10)			// 255
						{
							Num_Val = 255;
							goto Salida;
						}
					if (Boton_Calc == 11)			// 000
						{
							Num_Val = 0;
							goto Salida;
						}
					if (Boton_Calc == 12)			// ubicar
						{
							Num_Val = 612;
							goto Salida_Option;
						}
					if (Boton_Calc == 13)			// analogo
						{
							Num_Val = 712;
							goto Salida_Option;
						}
				}
			if (Boton_Calc <= 9)
				{
					// recorrer numero ya impreso
						Num_Col_Pos = Num_Col_Pos - 2;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Num_Val_Temp_1);
						lcd.print (Num_Val_Temp_2);
					// dibujar numero nuevo
						Num_Col_Pos = Num_Col_Pos + 2;
						lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
						lcd.print (Boton_Calc);
						Num_Val = Boton_Calc + (Num_Val_Temp_1 * 100) + (Num_Val_Temp_2 * 10);
				}
		Salida:
			lcd.noBlink();
			// pintar los ceros antes del numero
				Numerico_Write (Num_Val, Num_Col_Pos - 2, Num_Row_Pos);
				Num_Col_Pos = Num_Col_Pos - 4;
			// regresar el cursor a su ligar
				lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
		Salida_Option:
			{
				lcd.noBlink();
				lcd.setCursor (Num_Col_Pos, Num_Row_Pos);
				delay(300); // rebote de boton
			}
	}

void Numerico_Read()
	{
		long Num_Barrido_Time = 5;			// tiempo entre barrido de keys
		Boton_Calc = 17;					// limpiar valor para lectura
		while (Boton_Calc == 17)			// valor calculado	# E * F, 17 sin valor calculado
			{	
					// Barrido
						// Linea 1
							digitalWrite(Boton_Array_1, LOW);	// lectura linea 1
							// Lectura 1A = 1
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 1;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 1B = 6
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 4;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 1C = 7
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 7;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 1D = * = 15
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 15;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_1, HIGH);	// lectura linea 1
							
						// Linea 2
							digitalWrite(Boton_Array_2, LOW);	// lectura linea 2
							// Lectura 2A = 2
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 2;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 2B = 5
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 5;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 2C = 8
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 8;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 2D = 0
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 0;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_2, HIGH);	// lectura linea 2
							
						// Linea 3
							digitalWrite(Boton_Array_3, LOW);	// lectura linea 3
							// Lectura 3A = 3
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 3;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 3B = 4
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 6;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 3C = 9
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 9;
										delay(Boton_Delay_Teclado);
									}	
								delay(Num_Barrido_Time);
							// Lectura 3D = 14
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 14;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_3, HIGH);	// lectura linea 3
						
						// Linea 4
							digitalWrite(Boton_Array_4, LOW);	// lectura linea 4
							// Lectura 4A = 10
								if (digitalRead(Boton_Array_A) == LOW)
									{
										Boton_Calc = 10;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4B = 11
								if (digitalRead(Boton_Array_B) == LOW)
									{
										Boton_Calc = 11;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4C = 12
								if (digitalRead(Boton_Array_C) == LOW)
									{
										Boton_Calc = 12;
										delay(Boton_Delay_Teclado);
									}
								delay(Num_Barrido_Time);
							// Lectura 4D = 13
								if (digitalRead(Boton_Array_D) == LOW)
									{
										Boton_Calc = 13;
										delay(Boton_Delay_Teclado);
									}
							delay(Num_Barrido_Time);
							digitalWrite(Boton_Array_4, HIGH);	// lectura linea 4
							
							// Cursor center "enter"
								if (digitalRead(Boton_Center) == LOW)
									{
										Boton_Calc = 14;
										delay(Boton_Delay_Teclado);
									}
			}
	}
	
void Analog_Read_DMX(byte col, byte row)
	{
	// escritura del numero desde el potenciometro aplica a dmx
		int read;
		lcd.setCursor (col - 1, row);
		lcd.print("a");									// indicar que es analogo
		digitalWrite(Boton_Array_3, LOW);				// lectura linea 3
		lcd.blink();
		int valores = 0;
		while (digitalRead(Boton_Array_D) == HIGH && digitalRead(Boton_Center) == HIGH) // enter y center para paro
			{
				read = analogRead(Pot);					// lectura desde el potenciometro
				read = read / 4;						// / 4 porque es de 12 bits
				delay(50);
				if (valores != read)
					{
						Numerico_Write(read, col, row);
						ArduinoDmx0.TxBuffer[Canal_Actual - 1] = read;
						DMX_Values[Canal_Actual] = read;
						valores = read;
					}
			}
		lcd.noBlink();
		digitalWrite(Boton_Array_3, HIGH);				// lectura linea 3										// retraso para center
		delay(300);										// delay para salir de la lectura analoga
	}