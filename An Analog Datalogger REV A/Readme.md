###__Application development__
- [ ] Initialization on OLED
- [ ] SD card detection on startup
- [ ] Init list peripherals on startup
- [ ] General file menu
- [ ] SD file menu
- [ ] Menu scrolling sideways on selected item

>**Initialization on OLED**

>> _List_:

>> - UART

>> - LEDS

>> - Pushbuttons

>> - Encoders

>> - Systick

>> - RTC

>> - AFE

>> - RNG

>> - SD

>> - SSD1306

>> - USB

>> - nRF

>**General Filemenu**

>> _List_:

>> - RTC

>> - SD Card

>> - USB

>> - AFE

>> _Structure_:

>> Indication of selected item by inverted text. If the line is too long, scroll.. Use boundaries of text block..

>**SD Filemenu**

>> _List_:

>> - SD Card Info

>> - File Structure

>> _Structure_:

>> Call menustructure every update of the screen update. Determine divide ratio for editing screen + manage menu structure

>> Pass the current update counter; pass the menu structure struct -> decide what menu needs to be rendered + scrolling selection of current item

>> Make 



###Design notes
