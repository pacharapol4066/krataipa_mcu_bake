/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  21/8/2018
 */
#ifndef _KTP_MCU_DEF_H_
#define _KTP_MCU_DEF_H_
#include "mcuBake_def.h"

class mcubake_h
{
  private:
  float temp = 0;
  float humid = 0;
  int eeprom_read = 0;
  int poz = 0;
  int poz_max = 3;
  int poz_min = -3;
  int pozOld = 0;
  int pozNew = 0;
  int page_max = 1;
  int page_min = 0;
  int keypad_pin = 0;   //A0
  int keypad_value = 0;
  int keypad_value_old = 0;
  char btn_push;
  int press = 0;
  int press_max = 1;
  
  private:
  float getDHTTemp();
  float getDHTHumid();
  float getTemp();
  float getHumid();
  void setHumid();
  void setTemp();
  void writeEEPROM();
  int readEEPROM();
  void WaitBtnRelease();
  void clearLcd();
  
  public:
  int page = 0;

  public:
  void init(void);
  char ReadKeypad();
  void listenBtnPage();
  void indexPage();
  void showAboutPage();
  void setHumidPage();
};

extern mcubake_h MCUBAKE;
#endif
