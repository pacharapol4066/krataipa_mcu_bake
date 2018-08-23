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
  int temp = 0;
  int humid = 0;
  int eeprom_read = 0;
  int poz = 0;
  int poz_max = 3;
  int poz_min = 0;
  int page_max = 1;
  int page_min = 0;
  int keypad_value = 0;
  int keypad_value_old = 0;
  char btn_push;
  int press = 0;
  int press_max = 1;
  
  private:
  int getDHTTemp();
  int getDHTHumid();
  int getTemp();
  int getHumid();
  void setHumid();
  void setTemp();
  void writeEEPROM(int val);
  int readEEPROM();
  void setEEProm(int val);
  int getEEProm();
  void WaitBtnRelease();
  
  public:
  int page = 0;

  public:
  void init(void);
  char ReadKeypad();
  void listenBtnPage();
  void indexPage();
  void showAboutPage();
  void setHumidPage();
  void setRelayCtrl();
};

extern mcubake_h MCUBAKE;
#endif
