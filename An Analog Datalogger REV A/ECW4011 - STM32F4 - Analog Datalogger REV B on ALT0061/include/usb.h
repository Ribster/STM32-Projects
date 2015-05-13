/*
 * usb.h
 *
 *  Created on: 12-apr.-2015
 *      Author: Robbe Van Assche
 */

#ifndef USB_H_
#define USB_H_

#include "stm32f4xx.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"

/*
 * The USB data must be 4 byte aligned if DMA is enabled. This macro handles
 * the alignment, if necessary (it's actually magic, but don't tell anyone).
 */
__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

void
initialize_USB(void);

#endif /* USB_H_ */
