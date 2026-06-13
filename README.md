# Battery-Capacity-Gauge

After I just got into electronics many people suggested me to make my own BMS but electronic components and PCB are expensive here which is why I thought I should try to make something like that but with arduino.
Sooo cant say it as a BMS but I made this Battery Capacity Gauge which is suited to work with single cell 18650 3.7v li-ion cells.
This is what your circuit should look like:
<img width="1092" height="672" alt="Schematic" src="https://github.com/user-attachments/assets/6c1da674-03d3-4f21-b492-f71ad0dc008b" />

Here is what each component does:
- Arduino UNO R3 - This is just a simple microcontroller and is suitable for learners and beginners.
- INA219 - It is responsible for measuring the voltage and current of the wire.
- 16x2 LCD with I2C - This is the screen where the data will be printed. In the future I will add more features so I will probably add another screen to it.
- TP4056 - This is the battery charger and can supply upto 1A of chargign current.
- MT3608 - This is a boost converter and since li-ion has less than 5v the converter makes sure that arduino is getting 5v
- Capacitors - I added these for smoothing out the current after the voltage is increased from MT3608. I used 10nF ceramic and 100uF electrolytic capacitors you can use any other if you feel like it.

This is what the case looks like:
<img width="1423" height="732" alt="Screenshot 2026-06-13 175605" src="https://github.com/user-attachments/assets/0505f1d1-9c2c-4b6d-af7b-13ca83f79787" />
The case is simple with not too much effects so that if you want you can modify the case as per you want. In the case there is a gap on the side that will have to connector pins or you can add anything else if it can fit and that where the battery will be connected from.
On the back there will be a rectangular slot for the port of TP4056
On the front the big gap is for the screen.
The frame at the front is to be printed seperately so that inside it can provide support to the LCD screen.

Extras:
- In the code the INA219 has been set to 32v 2A configuration however you can change it from the following commands give in the table. You just need to change it in line 56 and remove the permade configuration.

| Code | Volts | Ampere |
|----------|----------|----------|
| setCalibration_32V_2A()   | 32V   | 2A   |
| setCalibration_32V_1A()   | 32V   | 1A   |
| setCalibration_16V_400mA()   | 16V   | 400mA   |   

- For displaying the bar I have used a complete black box, to change it you can make your own custom byte and update it into one_box[]. To make the byte you can visit this website and make the design as you like: https://maxpromer.github.io/LCD-Character-Creator/
- If you wish to use another battery then update the <mark>capacity</mark> variable  to the mAh capacity of your battery and you might have to update the OCV table with the OCV table of your own battery. Also if the battery operates on very low current like less than 20mA then you might have to remove the ocmponents that determine battery status or make it very less to maybe 5 or 10 or whatever you like. Also you might have to change the TP4056 with something else since it cannot charge every cell.
