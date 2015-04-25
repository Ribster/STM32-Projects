###__Application development__
<<<<<<< HEAD
- [ ] SD card detection on startup
- [ ] Init list peripherals on startup
- [ ] General file menu
- [ ] SD file menu
- [ ] Menu scrolling sideways on selected item


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
=======
- [ ] SD Filemenu
- [ ] General filemenu

>**SD Filemenu**: Overview of all the items on the SD card.
>> _Structure_:

>> Call menustructure every update of the screen; decide wich items need to be updated.

>> Pass the current update counter; pass the menu structure struct -> decide what menu needs to be rendered
>>
>>>>>>> origin/master



###Design notes
