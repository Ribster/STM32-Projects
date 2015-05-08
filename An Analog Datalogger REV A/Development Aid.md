- [ ] AFE issues that need to be resolved..
- [ ] Implementation of recording menuitem AFE + recording button blinking
- [ ] AFE information of settings
- [x] Get and paste RTC time into SD file write
- [ ] Create folder and write data into folder
- [ ] Attach interrupt every second from RTC ; Trigger AFE read
- [ ] RTC edit time implementation
- [ ] RTC info -> settings show + date && time


Recording handling
-> start recording
	-> make new folder
	-> write info.txt file into the folder with the info of the recording
		information includes time and date
	-> start sampling and writing from AFE to SD card
	-> upon stopping recording, the current sampling gets finished, but no new one will start
	-> count the total samples and write it on the bottom of info.txt
	
Priorities:
- Get AFE working full duplex
- Get a record mode working for the AFE (Just start record stop record, no info)
- Write the correct data from AFE to SD card