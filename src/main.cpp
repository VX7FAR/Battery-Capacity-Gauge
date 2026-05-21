#include<Arduino.h>
#include<Adafruit_INA219.h>
#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2);
Adafruit_INA219 ina(0x40);

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
    {4.2, 100},
    {3.9, 75},
    {3.7, 50},
    {3.5, 25},
    {3.0, 0}
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

int table_finder(float value){
    for(int i = 0;i<5;i++){
        if((value,1) == ocvtable[i][1]){
            return i;
        }
    }
}

void battery_processor(unsigned long delta){
    battery_voltage = ina.getBusVoltage_V();
    battery_ampere = ina.getCurrent_mA();
    usedcapacity += battery_ampere*(delta/3600000);
    capacityleft = 100.0 - (usedcapacity/capacity) * 100.0;
}

float amp_average(float A){
    avg_amp[0] = avg_amp[1];
    avg_amp[1] = avg_amp[2];
    avg_amp[2] = avg_amp[3];
    avg_amp[3] = A;
    return (avg_amp[0] + avg_amp[1] + avg_amp[2] + avg_amp[3])/4;
}

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