/*
 *  MODIFIED BY PACHARAPOL OUMOLARN
 *  21/8/2018
 */
#include "mcuBake.h"
#include "mcuBake_def.h"


//Setup Configuration
void setup()
{
#if DEBUGMODE
  Serial.begin(9600);
#endif
  MCUBAKE.init();
}

//Loop
void loop()
{
    MCUBAKE.listenBtnPage();
    MCUBAKE.setRelayCtrl();
    // page select
    switch (MCUBAKE.page) {
    case 0:
      MCUBAKE.indexPage();
      break;
    case 1:
      MCUBAKE.setHumidPage();
      break;
    case 2:
      MCUBAKE.showAboutPage();
      break;
    }
}
