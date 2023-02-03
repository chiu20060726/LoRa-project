# LoRa Emergency connection LITE  
use esp32 and LoRa module to build up emergency connection without WIFI  



## hardware
+ esp32(ESP-wroom-32)
+ LoRa module(E22 900T30D)
+ DHT22
+ RGB LED
+ breadboard
+ phone(android)

## gpio
3v3 and GND links on breadboard
+ UART2(esp32)  -->UART(LoRa)
+ 18(esp32)     -->AUX(LoRa)
+ 21(esp32)     -->M0(LoRa)
+ 19(esp32)     -->M1(LoRa)
+ VIN(esp32)    -->VCC(LoRa)
+ 14(esp32)     -->DAT(DHT22)
+ 27(esp32)     -->R(RGB LED)
+ 26(esp32)     -->G(RGB LED)
+ 25(esp32)     -->B(RGB LED)


## Contributors  
+ Addison      https://github.com/codingaddison
+ Chiu20060726 https://github.com/chiu20060726
+ maxdostick   https://github.com/maxdostick

### links
+ normal ver. https://github.com/codingaddison/LoRa-System/
