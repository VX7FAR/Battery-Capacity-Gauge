#include<Arduino.h>
#include<Adafruit_INA219.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2);       //Replace the address with correct one, this is just an example
Adafruit_INA219 ina(0x40);              //Replace the address with correct one, this is just an example

byte one_box[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

int ocvtable[][2] = {
    {4.13, 100},
    {4.03, 90},
    {3.92, 80},
    {3.83, 70},
    {3.75, 60},
    {3.70, 50},
    {3.66, 40},
    {3.63, 30},
    {3.59, 20},
    {3.53, 10},
    {3.43, 5},
    {3.34, 0}
};

//Battery info
float battery_voltage = 0.0;
float battery_ampere = 0.0;
float capacity = 3000.0;
float usedcapacity = 0.0;
float capacityleft = 0.0;
char battery_State = 'N';

//Time management
unsigned long ms = 0;
unsigned long previous_ms = 0;
unsigned long time_since_lastvoltage = 0;

//Misc..
float avg_amp[4] = {0,0,0,0};
float previous_voltage = 0.0;

void setup(){
    lcd.init();
    lcd.backlight();
    lcd.createChar(0,one_box);
    ina.begin();
    ina.setCalibration_32V_2A();
}

//Prints bar for display
void bar_printer(int num,int x,int y){
    int boxes = int(round(num/10));
    lcd.setCursor(x,y);
    for(int i =0;i<boxes;i++){
        lcd.write(0);
    }
    for(int i = 0; i< 10-boxes;i++){
        lcd.print(" ");
    }
}

//Estimates the capacity by combination of OCV and coulumb counting method
void idle_calculator(){
    float j = 0;
    for(int i=0;i<12;i++){
        if(abs(battery_voltage - ocvtable[i][0]) <= 0.01){
            j = ocvtable[i][1];
            break;
        }
        else{
            j = battery_voltage;
        }
    }
    capacityleft = battery_voltage * 0.9 + j *0.1;
}

//Reads data from INA219
void battery_processor(unsigned long delta){
    battery_voltage = ina.getBusVoltage_V();
    battery_ampere = ina.getCurrent_mA();
    usedcapacity += battery_ampere*(delta/3600000);
    capacityleft = 100.0 - (usedcapacity/capacity) * 100.0;
}

//Calculates average ampere based on last 4 readings
float amp_average(float A){
    avg_amp[0] = avg_amp[1];
    avg_amp[1] = avg_amp[2];
    avg_amp[2] = avg_amp[3];
    avg_amp[3] = A;
    return (avg_amp[0] + avg_amp[1] + avg_amp[2] + avg_amp[3])/4;
}

//Main Code
void loop(){
    previous_ms = ms;
    ms = millis();

    battery_processor(ms-previous_ms);

    if(amp_average(battery_ampere)<-20.0){
        battery_State = 'C';        //Charging
    }
    else if(amp_average(battery_ampere) > 20.0){
        battery_State = 'D';        //Discharging
    }
    else{
        battery_State = 'N';        //Neutral (almost equal charging and discharge)
    }

    if(abs(previous_voltage - battery_voltage) <= 0.02){
        time_since_lastvoltage += ms = time_since_lastvoltage; 
    }

    if(battery_State == 'N' && time_since_lastvoltage >= 5000){
        idle_calculator();
    }

    if(capacityleft<=0.0){
        capacityleft = 0.0;
    }
    if(capacityleft>=100.0){
        capacityleft = 100.0;
    }

    if(ms-previous_ms >= 200){
        bar_printer(int(capacityleft),0,0);
        lcd.setCursor(11,0);
        lcd.print(String(int(capacityleft)));
        lcd.setCursor(0,1);
        lcd.print(String(battery_ampere,2) + "mA");
        lcd.setCursor(10,1);
        lcd.print(String(battery_voltage,2) + "V");
    }
}