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
/* Keypad button analog Value
no button pressed 1023
nul     >400    select  <400
left    <350    down    <290
up      <190    right   <50
*/
    keypad_value = analogRead(keypad_pin);
    if (keypad_value < 50)
    return 'R';
    else if (keypad_value < 190)
    return 'D';
    else if (keypad_value < 290)
    return 'U';
    else if (keypad_value < 350)
    return 'L';
    else if (keypad_value < 400)
    return 'S';
    else if (keypad_value > 400)
    return 'N';
}

void mcubake_h::WaitBtnRelease()
{
    while (analogRead(keypad_pin) < 600) {}
}

/*************** VALUE CONTROL METHOD ***************/

float mcubake_h::getDHTTemp()
{
   float t = dht.readTemperature();
   if (isnan(t)) {
      return 0;
   } else {
      return t;
   }
}

float mcubake_h::getDHTHumid()
{
   float h = dht.readHumidity();
   if (isnan(h)) {
      return 0;
   } else {
      return h;
   }
}

float mcubake_h::getTemp()
{
    return temp;
}

float mcubake_h::getHumid()
{
    return humid;
}

void mcubake_h::setHumid()
{
    humid = getDHTHumid();
}

void mcubake_h::setTemp()
{
    temp = getDHTTemp();
}

void mcubake_h::writeEEPROM()
{
    EEPROM.write(0, eeprom_read>>8);
    EEPROM.write(1, eeprom_read&0xFF);
    eeprom_read = 0;
}

int mcubake_h::readEEPROM()
{
    int readeeprom = 0;
    int returnVal = 0;
    readeeprom |= EEPROM.read(0)<<8;
    readeeprom |= EEPROM.read(1)&0xFF;
    if (isnan(readeeprom)) {
      returnVal = DEFAULTHUMID;
    }
    else{
      returnVal = readeeprom;
    }
    return returnVal;
}

/*************** PAGE CONFIGURATION *******************/

void mcubake_h::init(void)
{
   lcd.begin(16, 2);              // start the library
   clearLcd();
   dht.begin();
}

void mcubake_h::clearLcd()
{
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.home();
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
        poz = 0;
      }
    }
    else if (btn_push == 'D')
    {
      poz--;
      if (poz < poz_min) {
          poz = 0;
      }
    }
    else if (btn_push == 'R')
    {
      eeprom_read++;
    }
    else if (btn_push == 'L')
    {
      eeprom_read--;
    }
    
    pozNew = poz;
    if (pozNew != pozOld) {
        pozOld = pozNew;
    }
}

void mcubake_h::indexPage()
{
    //Default page
    clearLcd();
    lcd.print("HUMID :");
    lcd.setCursor(7,0);
    lcd.print(getHumid(),2);
    lcd.setCursor(0,1);
    lcd.print("TEMP  :");
    lcd.setCursor(7,1);
    lcd.print(getTemp(),2);
  
    switch (poz)
    {
    case 0:
      break;
    case 1:
      clearLcd();
      lcd.print("INDEX PAGE");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 0;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      clearLcd();
      lcd.print("SET HUMIDITY");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 1;
      }
      break;
    case 3:
      clearLcd();
      lcd.print("ABOUT US");
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
    clearLcd();
    lcd.print("SW-VERSION : 1.0");
    lcd.setCursor(0,2);
    lcd.print("PAKCHONGAGROTECH");
  
    switch (poz)
    {
    case 0:
      break;
    case 1:
      clearLcd();
      lcd.print("ABOUT US");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 2;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      clearLcd();
      lcd.print("INDEX PAGE");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 0;
      }
      break;
    case 3:
      clearLcd();
      lcd.print("SET HUMIDITY");
      if (press == 1) {
        press = 0;
        poz = 0;
        page = 1;
      }
      break;
    }
}

void mcubake_h::setHumidPage()
{
    //Default page
    eeprom_read = readEEPROM();
    clearLcd();
    lcd.print("SET HUMIDITY :");
    lcd.setCursor(0,2);
    lcd.print(eeprom_read);
    lcd.setCursor(4,2);
    lcd.print("%");
  
    switch (poz)
    {
    case 0:
      break;
    case 1:
      clearLcd();
      lcd.print("SET HUMIDITY");
      if (press == 1) {       //ถ้าผู้ใช้กดปุ่ม SELECT
        press = 0;            //Reset ปุ่ม select รอกดรอบต่อไป
        poz = 0;              //Reset ปุ่ม up-down รอกดรอบต่อไป
        page = 1;             //Flage เปลี่ยนหน้า
      }
      break;
    case 2:
      clearLcd();
      lcd.print("ABOUT US");
      if (press == 1) {
        press = 0;
        poz = 0;
        writeEEPROM();
        page = 2;
      }
      break;
    case 3:
      clearLcd();
      lcd.print("INDEX PAGE");
      if (press == 1) {
        press = 0;
        poz = 0;
        writeEEPROM();
        page = 0;
      }
      break;
    }
}

mcubake_h MCUBAKE;
