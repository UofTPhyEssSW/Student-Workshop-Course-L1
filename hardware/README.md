# Course Hardware
<p>
There are two boards that are built by the students in the course. The first is the Analog Front End PCB with helps the student practise through hole soldering. The second is the Phyduino Pico PCB which continues the soldering training, learning how solder surface mount components.
</p>
<p>
Each board is designed using <a href="https://www.kicad.org/">KiCAD EDA</a> design software.
</p>

# Phyduino Pico PCB
<img src="https://github.com/UofTPhyEssSW/Student-Workshop-Course-L1/blob/main/images/Phyduino_Pico_RevC.png?raw=true" height="250px">
<p>
The Phyduino Pico is an Arduino compatible microcontroller board the can be programmed using the <a href="https://docs.arduino.cc/software/ide-v2">Arduino IDE</a>. The Phyduino Pico uses the <a href="https://www.raspberrypi.com/products/rp2040/">Raspberry PI Pico micorcontroller</a> (RP2040) with 2 ARM Cortex-M0+ processors (133MHz) and 264KB of RAM. All programming code is stored in an external 16MB flash chip that can be both programming memory and solid state drive. The microcontroller can be program in C/C++ or <a href="https://www.raspberrypi.com/documentation/microcontrollers/micropython.html">MicroPython</a>. This course only shows examples with C/C++ programming, however there are many tutorials online for programming the RP2040 with MicroPython. 
</p>
<p>
The Phyduino Pico board is power using the USB Power delivery through the USB-C connector. Power delivery is negotiated throught the CC lines on the USB-C connector using the <a href="https://www.st.com/en/interfaces-and-transceivers/stusb4500.html">STUSB4500</a> USB PD controller. The STUSB4500 is programmed to negotiated for 3 power profiles, +5.0V(default), +9.0V, and +20V. The controller will try to negotiate for the high one available. By default all power delivery devices must have at least one profile by default +5.0V. The USB-C connector is used to both power and communicate with the board. Power for the board is regulated through 2, <a href="https://www.ti.com/product/TPS54231?dcmp=dsproject&hqs=td&#doctype2">TPS54231</a> DC/DC converters that steps down the USB VBus voltage to +3.3V and +5.0V.The +3.3V DC/DC regulator is enabled when the USB VBus voltage is above 4.5V. The +5.0V regulator is enabled when USB VBus voltage is above 7.0V. Each power rail on the board the has a led to indicate if the power is active, red for VBus, yellow for +3.3V and green for +5.0V. This allows the user to visual assess which power rails are active. If the the green led is off the USB-PD controller could not negotiate a high power then the default +5.0V. Power is output from the board through J2. +VUSB on J2 is connected directly to the USB VBus if the connected shield board is cannot accommodate the voltage here concider using a linear regulator to being down the voltage required by the shield board. 
</p>
<p>
All connector IO's are compatible with <a href="https://content.arduino.cc/assets/A000066-full-pinout.pdf?_gl=1*1w20zu6*_ga*MjA4MzA3Mzg0OS4xNjc2NDAwMDM1*_ga_NEXN8H46L5*MTY4MzkxNDk5MS4yNC4xLjE2ODM5MTgwODAuMC4wLjA.">Arduino UNO Rev3</a> pin outs. The digital IO pins are +3.3V digital IO tolerant. Do not use shield boards that use IO voltage above +3.3V you will damage the RP2040 IO's. The Analog input pins are broken into 2 groups, internal (A0 - A2) and external (A3 - A5). The internal analog signals are connected to the RP2040 microcontroller's internal 12bit 500KSps ADC. The voltage reference for the RP2040 internal ADC is <b>3.3V</b> so the maximum measurable voltage is +3.3V. The external signals is connected to the Analog Devices <a href="https://www.analog.com/en/products/max11635.html">MAX11635</a> 12bit 300Ksps ADC. The voltage reference for the MAX11635 is 2.048V so the maximum measurable voltage is <b>2.048V</b>. Exceed any of this maximum voltage for any analog input group with damage either the RP2040 or MAX11635.
</p>

# Analog Front End PCB
<img src="https://github.com/UofTPhyEssSW/Student-Workshop-Course-L1/blob/main/images/AFE_Shield_RevC.png?raw=true" height="250px">
<p>

</p>