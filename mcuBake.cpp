/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  21/8/2018
 */
#include "mcuBake.h"
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "DHT.h"

LiquidCrystal lcd(8,9,4,5,6,7);
DHT dht(DHTPIN, DHTTYPE);

/*************** BUTTON CONFIG ***************/
char mcubake_h::ReadKeypad()
{
    keypad_value = analogRead(KEYPAD_PIN);
    if (keypad_value < 50)
      return 'R';
    else if (keypad_value > 50 && keypad_value < 250)
      return 'U';
    else if (keypad_value > 250 && keypad_value < 450)
      return 'D';
    else if (keypad_value > 450 && keypad_value < 650)
      return 'L';
    else if (keypad_value > 650 && keypad_value < 850)
      return 'S';
    else if (keypad_value > 1000)
      return 'N';
}

void mcubake_h::WaitBtnRelease()
{
    while (analogRead(KEYPAD_PIN) < 600) {}
}

/*************** VALUE CONTROL METHOD ***************/

int mcubake_h::getDHTTemp()
{
   int t = dht.readTemperature();
   if (isnan(t)) {
      return 0;
   } else {
      return t;
   }
}

int mcubake_h::getDHTHumid()
{
   int h = dht.readHumidity();
   if (isnan(h)) {
      return 0;
   } else {
      return h;
   }
}

void mcubake_h::setHumid()
{
    humid = getDHTHumid();
}

void mcubake_h::setTemp()
{
    temp = getDHTTemp();
}

int mcubake_h::getTemp()
{
    return temp;
}

int mcubake_h::getHumid()
{
    return humid;
}

void mcubake_h::writeEEPROM(int val)
{
    EEPROM.write(0, val>>8);
    EEPROM.write(1, val&0xFF);
}

int mcubake_h::readEEPROM()
{
    int readeeprom = 0;
    int returnVal = 0;
    readeeprom |= EEPROM.read(0)<<8;
    readeeprom |= EEPROM.read(1)&0xFF;
    if (readeeprom < 0) {
      return DEFAULTHUMID;
    }
    else{
      return readeeprom;
    }
}

void mcubake_h::setEEProm(int val)
{
    eeprom_read = val;
}

int mcubake_h::getEEProm()
{
    return eeprom_read;
}

/*************** PAGE CONFIGURATION *******************/

void mcubake_h::init(void)
{
   Serial.begin(9600);
   lcd.begin(16, 2);              // start the library
   lcd.setCursor(0,0);
   setEEProm(readEEPROM());
   pinMode(RELAYPIN, OUTPUT);
   dht.begin();
   setTemp();
   setHumid();
}

void mcubake_h::listenBtnPage()
{
    btn_push = ReadKeypad();
    WaitBtnRelease();
    /* ถ้ามีการกดปุ่ม จะเริ่มทำงาน */
    if (btn_push == 'S') //enter selected menu
    {
      press++;
      delay(250);
      if (press > press_max) {
        press = 0;
      }
    }
    else if (btn_push == 'U')
    {
      poz++;
      if (poz > poz_max) {
        poz = 1;
      }
    }
    else if (btn_push == 'D')
    {
      poz--;
      if (poz < poz_min) {
          poz = 1;
      }
    }
    else if (btn_push == 'R')
    {
      eeprom_read++;
      Serial.println(eeprom_read);
      setEEProm(eeprom_read);
    }
    else if (btn_push == 'L')
    {
      eeprom_read--;
      Serial.println(eeprom_read);
      setEEProm(eeprom_read);
    }
}

void mcubake_h::indexPage()
{
    //Default page
    switch (poz)
    {
    case 0:
      lcd.setCursor(0,0);
      lcd.print("HUMIDITY% : ");
      lcd.setCursor(12,0);
      lcd.print(getHumid());
      lcd.setCursor(0,1);
      lcd.print("TEMP(C)   : ");
      lcd.setCursor(12,1);
      lcd.print(getTemp());
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 1;
      }
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===");
      lcd.setCursor(0,1);
      lcd.print("1.INDEX PAGE    ");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 0;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("2.SET % HUMIDITY");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 1;
      }
      break;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===");
      lcd.setCursor(0,1);
      lcd.print("3.ABOUT US      ");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 2;
      }
      break;
    }
}

void mcubake_h::showAboutPage()
{
    //Default page
  
    switch (poz)
    {
    case 0:
      lcd.setCursor(0,0);
      lcd.print("SW-VERSION : 1.0");
      lcd.setCursor(0,2);
      lcd.print("PAKCHONGAGROTECH");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 3;
      }
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("1.INDEX PAGE    ");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 0;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("2.SET % HUMIDITY");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 1;
      }
      break;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("3.ABOUT US      ");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 2;
      }
      break;
    }
}

void mcubake_h::setHumidPage()
{
    //Default page
 
    switch (poz)
    {
    case 0:
      lcd.setCursor(0,0);
      lcd.print("%HUMIDITY : ");
      lcd.setCursor(12,0);
      lcd.print(getEEProm());
      lcd.setCursor(14,0);
      lcd.print("% ");
      lcd.setCursor(0,1);
      lcd.print("SELECT TO SAVE  ");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        writeEEPROM(eeprom_read);
        poz = 3;
      }
      break;
    case 1:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("3.ABOUT US      ");
      setEEProm(readEEPROM());
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 2;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("1.INDEX PAGE    ");
      setEEProm(readEEPROM());
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 0;
      }
      break;
    case 3:
      lcd.setCursor(0,0);
      lcd.print("  === MENU ===  ");
      lcd.setCursor(0,1);
      lcd.print("2.SET % HUMIDITY");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 1;
      }
      break;
    }
}

void mcubake_h::setRelayCtrl()
{
    if(getHumid() >= eeprom_read)
    {
        digitalWrite(RELAYPIN, HIGH);
    }
    else
    {
        digitalWrite(RELAYPIN, LOW);
    }
}

mcubake_h MCUBAKE;
