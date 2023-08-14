EESchema Schematic File Version 4
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Relay:FINDER-40.41 K?
U 1 1 601ECFF8
P 3200 2450
F 0 "K?" H 3630 2496 50  0000 L CNN
F 1 "FINDER-40.41" H 3450 2150 50  0000 L CNN
F 2 "Relay_THT:Relay_SPDT_Finder_40.41" H 4340 2410 50  0001 C CNN
F 3 "https://www.finder-relais.net/de/finder-relais-serie-40.pdf" H 3200 2450 50  0001 C CNN
	1    3200 2450
	1    0    0    -1  
$EndComp
$Comp
L Relay:DIPxx-1Axx-11x K?
U 1 1 601ED128
P 4650 2450
F 0 "K?" H 4980 2496 50  0000 L CNN
F 1 "RenaultRelay" H 4980 2405 50  0000 L CNN
F 2 "Relay_THT:Relay_StandexMeder_DIP_LowProfile" H 5000 2400 50  0001 L CNN
F 3 "https://standexelectronics.com/wp-content/uploads/datasheet_reed_relay_DIP.pdf" H 4650 2450 50  0001 C CNN
	1    4650 2450
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:Arduino_Leonardo A?
U 1 1 601ED546
P 8200 3050
F 0 "A?" H 8200 4228 50  0000 C CNN
F 1 "Arduino_Leonardo" H 8200 4137 50  0000 C CNN
F 2 "Module:Arduino_UNO_R3" H 8350 2000 50  0001 L CNN
F 3 "https://www.arduino.cc/en/Main/ArduinoBoardLeonardo" H 8000 4100 50  0001 C CNN
	1    8200 3050
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 601ED733
P 4850 1300
F 0 "#PWR?" H 4850 1150 50  0001 C CNN
F 1 "+12V" H 4865 1473 50  0000 C CNN
F 2 "" H 4850 1300 50  0001 C CNN
F 3 "" H 4850 1300 50  0001 C CNN
	1    4850 1300
	1    0    0    -1  
$EndComp
Text GLabel 4350 1550 0    50   Input ~ 0
UCH_Signal_12V
Connection ~ 4850 2050
Wire Wire Line
	4850 2050 4850 2150
Wire Wire Line
	4350 1550 4450 1550
NoConn ~ 3300 2150
$Comp
L power:GND #PWR?
U 1 1 601EE5F1
P 6100 4300
F 0 "#PWR?" H 6100 4050 50  0001 C CNN
F 1 "GND" H 6105 4127 50  0000 C CNN
F 2 "" H 6100 4300 50  0001 C CNN
F 3 "" H 6100 4300 50  0001 C CNN
	1    6100 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 1700 4450 1700
Wire Wire Line
	4450 1700 4450 1550
Wire Wire Line
	3400 2750 3400 2900
Wire Wire Line
	3400 2900 4000 2900
Wire Wire Line
	4000 2900 4000 2050
Wire Wire Line
	4000 2050 4450 2050
Wire Wire Line
	4450 2050 4450 2150
Text GLabel 4900 2850 2    50   Input ~ 0
12V_Indoor
Wire Wire Line
	4850 2750 4850 2850
Wire Wire Line
	4450 2750 4450 4250
Wire Wire Line
	4450 4250 5950 4250
Wire Wire Line
	8100 4250 8100 4150
Wire Wire Line
	6100 4300 6100 4250
Connection ~ 6100 4250
$Comp
L Isolator:SFH617A-1 U?
U 1 1 601F03D9
P 2350 3250
F 0 "U?" H 2350 3575 50  0000 C CNN
F 1 "SFH617A-1" H 2350 3484 50  0000 C CNN
F 2 "Package_DIP:DIP-4_W7.62mm" H 2150 3050 50  0001 L CIN
F 3 "http://www.vishay.com/docs/83740/sfh617a.pdf" H 2350 3250 50  0001 L CNN
	1    2350 3250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 601F05D4
P 1800 3150
F 0 "R?" V 1593 3150 50  0000 C CNN
F 1 "R" V 1684 3150 50  0000 C CNN
F 2 "" V 1730 3150 50  0001 C CNN
F 3 "~" H 1800 3150 50  0001 C CNN
	1    1800 3150
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 4250 4450 4250
Connection ~ 4450 4250
Wire Wire Line
	4850 1300 4850 1350
Wire Wire Line
	4850 1350 3000 1350
Wire Wire Line
	3000 1350 3000 2050
Connection ~ 4850 1350
Wire Wire Line
	4850 1350 4850 2050
Wire Wire Line
	3500 1700 3500 2150
Wire Wire Line
	1950 3150 2050 3150
$Comp
L Device:D D?
U 1 1 601F285F
P 2600 2450
F 0 "D?" V 2554 2529 50  0000 L CNN
F 1 "D" V 2645 2529 50  0000 L CNN
F 2 "" H 2600 2450 50  0001 C CNN
F 3 "~" H 2600 2450 50  0001 C CNN
	1    2600 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 2750 3000 2850
Wire Wire Line
	3000 2850 2600 2850
Wire Wire Line
	2600 2850 2600 2600
Wire Wire Line
	3000 2050 2600 2050
Wire Wire Line
	2600 2050 2600 2300
Connection ~ 3000 2050
Wire Wire Line
	3000 2050 3000 2150
Wire Wire Line
	2650 3150 3000 3150
Wire Wire Line
	3000 3150 3000 2850
Connection ~ 3000 2850
Wire Wire Line
	2650 3350 3000 3350
Wire Wire Line
	3000 3350 3000 4250
Wire Wire Line
	2050 3350 1850 3350
Wire Wire Line
	1850 3350 1850 4250
Wire Wire Line
	1850 4250 3000 4250
Connection ~ 3000 4250
Text GLabel 1450 3150 0    50   Input ~ 0
IndoorPowerSwitch
Text GLabel 7700 2850 0    50   Input ~ 0
IndoorPowerSwitch
$Comp
L Device:R R?
U 1 1 601F4E0C
P 5950 2500
F 0 "R?" H 6020 2546 50  0000 L CNN
F 1 "40k" H 6020 2455 50  0000 L CNN
F 2 "" V 5880 2500 50  0001 C CNN
F 3 "~" H 5950 2500 50  0001 C CNN
	1    5950 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 601F4E96
P 5950 3050
F 0 "R?" H 6020 3096 50  0000 L CNN
F 1 "20k" H 6020 3005 50  0000 L CNN
F 2 "" V 5880 3050 50  0001 C CNN
F 3 "~" H 5950 3050 50  0001 C CNN
	1    5950 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 2850 4900 2850
Wire Wire Line
	4450 1700 5950 1700
Wire Wire Line
	5950 1700 5950 2350
Connection ~ 4450 1700
Wire Wire Line
	5950 3200 5950 4250
Connection ~ 5950 4250
Wire Wire Line
	5950 4250 6100 4250
Wire Wire Line
	5950 2650 5950 2800
Text GLabel 6000 2800 2    50   Input ~ 0
IndoorPowerFactory5V
Wire Wire Line
	6000 2800 5950 2800
Connection ~ 5950 2800
Wire Wire Line
	5950 2800 5950 2900
Text GLabel 7700 2950 0    50   Input ~ 0
IndoorPowerFactory5V
Wire Wire Line
	4850 2050 8100 2050
Wire Wire Line
	6100 4250 8100 4250
Wire Notes Line
	1600 3500 3800 3500
Wire Notes Line
	3800 3500 3800 1850
Wire Notes Line
	3800 1850 1600 1850
Wire Notes Line
	1600 1850 1600 3500
Text Notes 1650 1950 0    50   ~ 0
Relay module
Text Notes 4200 1700 0    50   ~ 0
white
Text Notes 4250 2050 0    50   ~ 0
blue
Text Notes 3000 1800 0    50   ~ 0
Vin
Text Notes 3000 3600 0    50   ~ 0
GND
Wire Wire Line
	1650 3150 1450 3150
Text Notes 1500 3150 0    50   ~ 0
In
$EndSCHEMATC
