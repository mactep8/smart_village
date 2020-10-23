#ifndef CHCONTROL
#define CHCONTROL
//#include "stm32f0xx_hal_gpio.h"
#include "main.h"

class ChControl
{
  private:
    int _key_pin;
    int _led_pin;
    int _relay_pin;
    int _pass_pin;
    int _esp_pin;
    
    GPIO_TypeDef *_key_pin_port;
    GPIO_TypeDef *_led_pin_port;
    GPIO_TypeDef *_relay_pin_port;
    GPIO_TypeDef *_pass_pin_port;
    GPIO_TypeDef *_esp_pin_port;
  
    int _ch_state;
    int _esp_value;
		int _key_value;
  
    void SetState(int state);
  public:
    ChControl(
      int a_ch_num
    );
    //void Init(int state);
    void Operate();
};

#endif /* CHCONTROL */
