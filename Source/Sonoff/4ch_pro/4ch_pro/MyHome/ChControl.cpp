//#include "stm32f0xx_hal_gpio.h"
#include "ChControl.h"
#include "main.h"

ChControl::ChControl(
      int a_ch_num
    )
{
  switch(a_ch_num)
  {
    case 1: {
      _key_pin = KEY_S1_Pin;
      _led_pin = LED1_Pin;
      _relay_pin = RELAY1_Pin;
      _pass_pin = PASS_RELAY_1_Pin;
      _esp_pin = ESP_KEY_1_Pin;
      
      _key_pin_port = KEY_S1_GPIO_Port;
      _led_pin_port = LED1_GPIO_Port;
      _relay_pin_port = RELAY1_GPIO_Port;
      _pass_pin_port = PASS_RELAY_1_GPIO_Port;
      _esp_pin_port = ESP_KEY_1_GPIO_Port;
    }break;
    case 2: {
      _key_pin = KEY_S2_Pin;
      _led_pin = LED2_Pin;
      _relay_pin = RELAY2_Pin;
      _pass_pin = PASS_RELAY_2_Pin;
      _esp_pin = ESP_KEY_2_Pin;
      
      _key_pin_port = KEY_S2_GPIO_Port;
      _led_pin_port = LED2_GPIO_Port;
      _relay_pin_port = RELAY2_GPIO_Port;
      _pass_pin_port = PASS_RELAY_2_GPIO_Port;
      _esp_pin_port = ESP_KEY_2_GPIO_Port;
    }break;
    case 3: {
      _key_pin = KEY_S3_Pin;
      _led_pin = LED3_Pin;
      _relay_pin = RELAY3_Pin;
      _pass_pin = PASS_RELAY_3_Pin;
      _esp_pin = ESP_KEY_3_Pin;
      
      _key_pin_port = KEY_S3_GPIO_Port;
      _led_pin_port = LED3_GPIO_Port;
      _relay_pin_port = RELAY3_GPIO_Port;
      _pass_pin_port = PASS_RELAY_3_GPIO_Port;
      _esp_pin_port = ESP_KEY_3_GPIO_Port;
    }break;
    case 4: {
      _key_pin = KEY_S4_Pin;
      _led_pin = LED4_Pin;
      _relay_pin = RELAY4_Pin;
      _pass_pin = PASS_RELAY_4_Pin;
      _esp_pin = ESP_KEY_4_Pin;
      
      _key_pin_port = KEY_S4_GPIO_Port;
      _led_pin_port = LED4_GPIO_Port;
      _relay_pin_port = RELAY4_GPIO_Port;
      _pass_pin_port = PASS_RELAY_4_GPIO_Port;
      _esp_pin_port = ESP_KEY_4_GPIO_Port;
    }break;
  }
  
  _ch_state = 0;
  _esp_value = 0;
	_key_value = 0;
}

void ChControl::Operate()
{
  int vkey = (int)HAL_GPIO_ReadPin(_key_pin_port, _key_pin);
  int vesp = (int)HAL_GPIO_ReadPin(_esp_pin_port, _esp_pin);
  if (!_esp_value && vesp)
  {
    _ch_state = !_ch_state;
		HAL_GPIO_WritePin(_led_pin_port, _led_pin, (GPIO_PinState)_ch_state); 
		HAL_GPIO_WritePin(_relay_pin_port, _relay_pin, (GPIO_PinState)_ch_state);
		HAL_GPIO_WritePin(_pass_pin_port, _pass_pin, (GPIO_PinState)_ch_state);
  }
	else if (!_key_value && vkey)
	{
		if (!_ch_state) HAL_GPIO_WritePin(_led_pin_port, _led_pin, (GPIO_PinState)_ch_state); // Set state from ESP only for off
		HAL_GPIO_WritePin(_relay_pin_port, _relay_pin, (GPIO_PinState)_ch_state);
		HAL_GPIO_WritePin(_pass_pin_port, _pass_pin, (GPIO_PinState)_ch_state);
	}
	
	_esp_value = vesp;
	_key_value = vkey;
}

void ChControl::SetState(int state)
{
  //HAL_GPIO_WritePin(LED4_GPIO_Port,LED4_Pin, (GPIO_PinState)1);
}