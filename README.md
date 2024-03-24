## Yet another volume mixer, but with some cool features

 - 5 volume faders
 - 5 mute buttons, that can double as HID buttons to trigger hotkeys at the OS level, each button is also kitted with an LED indicator
 - 1 master volume pot
 - An OLED screen that displays random monochrome 'gif' animations and give feedback on the volume levels
 - Volume levels disappear after 5 seconds so you can see your beautiful animations 
 
 Here's mine:
![enter image description here](https://lh3.googleusercontent.com/d/1_TZlOmNoFWG34ps8rrM8VcQQW-vLfLuF)
![enter image description here](https://lh3.googleusercontent.com/d/1GUtkFXpiVqL1wwbwS1nNojc_Z4so38ck)
![enter image description here](https://lh3.googleusercontent.com/d/1jgsPeNd-aBUdyYfLh-Hz6YVNfXeDxwVh)

## Parts list

 - Teensy 4.0
 - Linear faders: 10K, 60 mm: (https://www.amazon.co.uk/gp/product/B07PNDLKQQ/ref=ppx_yo_dt_b_search_asin_image?ie=UTF8&psc=1)
 - Master volume pot: 
https://www.amazon.co.uk/gp/product/B0734K6WYP/ref=ppx_yo_dt_b_search_asin_image?ie=UTF8
- OLED screen: 
https://www.amazon.co.uk/gp/product/B08LYL7QFQ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1
- Clear keycaps:
https://www.amazon.co.uk/gp/product/B08831GTXS/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
- M3 inserts:
https://www.amazon.co.uk/gp/product/B089GNXKKV/ref=ppx_yo_dt_b_asin_title_o06_s03?ie=UTF8&psc=1
- Fader sliders:
https://www.amazon.co.uk/gp/product/B07W78R5T9/ref=ppx_yo_dt_b_asin_title_o06_s03?ie=UTF8
- Proto board - HOLE SPACING IS IMPORTANT
https://www.amazon.co.uk/gp/product/B08151V9TS/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&th=1
- Red LEDs:
https://www.amazon.co.uk/gp/product/B00WX39R3S/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
- Large Pot control knob:
https://www.amazon.co.uk/gp/product/B089D4H55W/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1
- M2 nylon standoffs
https://www.amazon.co.uk/Spacers-Standoff-Assortment-Threaded-Circuit/dp/B09F9PHKJ6/ref=sr_1_5?sr=8-5
-rubber feet
-M2 countersank screws

## Assembly
![enter image description here](https://lh3.googleusercontent.com/d/18Hy5zogNRrKffHtVxrYmNcZI293UQcSA)
Fix the OLD screen to inside of the box with M2 screws and female nylon standoffs, the master pot is held in place with it's provided nut.
The faders are also held in place with M2 screws.

In the case of the teensy 4.0 all of the analogue pots run off of 3v, the OLED screen and the LEDs are running off of the 5v pin.

The Teensy has to be soldered to a protoboard with the correct holes and mounted at the correct height on the box cover
![](https://lh3.googleusercontent.com/d/1vrPGRh92qhTeGF_LfVyaFMzFTth23Wle)
This allows for the correct clearance for the USB port 
 ![](https://lh3.googleusercontent.com/d/1_hQhUJD9XMJX1ZsyLLC9t6KC_nhxW6XC)
The microcontroller is mounted to the lid via m2 nylon screws and standoffs, add rubber feet so the screws don't make contact with your desk
![](https://lh3.googleusercontent.com/d/17M93ZikbJTOgK1h-U_qBmDIkZhYRYE_d)

Pinouts:
| Pin | Function |
|--|--|
| A0 | Master volume dial |
| A1 | Fader 1 |
| A6 | Fader 2 |
| A3 | Fader 3 |
| A7 | Fader 4 |
| A8 | Fader 5 |
| A4 | SDA OLED screen|
| A5 | SCL OLED screen |
| D8 | Mute button 1 |
| D9 | Mute button 2 |
| D11 | Mute button 3 |
| 10 | Mute button 4 |
| D4 | Mute LED 1 |
| D5 | Mute LED 2 |
| D2 | Mute LED 3 |
| D3 | Mute LED 4 |
| D6 | Mute LED 5 |

