/*
 * usb.c
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#include "usb.h"

void
initialize_USB(void){
	/* Setup USB */
	USBD_Init(&USB_OTG_dev,
	            USB_OTG_FS_CORE_ID,
	            &USR_desc,
	            &USBD_CDC_cb,
	            &USR_cb);
	VCP_send_str("Testing USB terminal!!!\r\n");
	VCP_send_str("And testing the newline...\r\n");
}
