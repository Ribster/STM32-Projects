/**
  ******************************************************************************
  * @file    storage.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-May-2012
  * @brief   This file includes the Storage (FatFs) driver for the STM320518-EVAL
  *          demo.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#include "stm32f0xx.h"
#include "stm32f0xx_spi.h"
#include "fatfs/stm320518_eval_spi_sd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** @addtogroup STM320518_EVAL_Demo
  * @{
  */

/** @defgroup STORAGE
  * @brief This file includes the Storage (FatFs) driver for the STM320518-EVAL
  *        demo.
  * @{
  */

/** @defgroup STORAGE_Private_Types
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Private_Defines
  * @{
  */


/**
  * @}
  */

/** @defgroup STORAGE_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Private_Variables
  * @{
  */
extern FATFS fs;
FILINFO fno;
DIR dir;
extern FIL F;
extern UINT BytesWritten;
extern UINT BytesRead;
extern const uint8_t SlidesCheck[];
extern uint8_t Buffer1[];
extern volatile uint32_t LCDType;
/**
  * @}
  */


/** @defgroup STORAGE_Private_FunctionPrototypes
  * @{
  */
/**
  * @}
  */

/** @defgroup STORAGE_Private_Functions
  * @{
  */

/**
  * @brief  SDCARD Initialisation for FatFs
  * @param  None
  * @retval Error status (0=> success, 1=> fail)
  */
uint32_t Storage_Init(void)
{
  uint8_t resp;
  
  /* SD Card Initialization */
  resp = SD_Init();
  if (resp != 0)
  {
    /* Some 64/128MB ST cards fail for the 1st time: workaround is to call SD_Init() twice */
    resp = SD_Init();
    if (resp != 0)
    {
      return 1;
    }
  }
  /****************** FatFs Volume Acess **************************************/
  if (f_mount(0, &fs))
  {
    return 3;
  }
  return 0;
}

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  Xpoz: specifies the LCD X (line) position.
  * @param  Ypoz: specifies the LCD Y (column) position. 
  * @param  BmpName: the file name to open
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char* BmpName)
{
  uint32_t index = 0, size = 0, width = 0, height = 0, i1 = 0;
  uint32_t BmpAddress;
  uint16_t Xpos2 = 0;
  uint16_t Ypos2 = 0;
  uint32_t Counter = 0;
  
  FIL F1;
  
  f_open (&F1, BmpName, FA_READ);
  f_read (&F1, Buffer1, 26, &BytesRead);
  
  BmpAddress = (uint32_t)Buffer1;
  
  /* Read bitmap size */
  size = *(uint16_t *) (BmpAddress + 2);
  size |= (*(uint16_t *) (BmpAddress + 4)) << 16;
  
  /* Get bitmap data address offset */
  index = *(uint16_t *) (BmpAddress + 10);
  index |= (*(uint16_t *) (BmpAddress + 12)) << 16;
  
  /* Read bitmap width */
  width = *(uint16_t *) (BmpAddress + 18);
  width |= (*(uint16_t *) (BmpAddress + 20)) << 16;
  
  /* Read bitmap height */
  height = *(uint16_t *) (BmpAddress + 22);
  height |= (*(uint16_t *) (BmpAddress + 24)) << 16;
  
  size = (size - index) / 2;
  
  /* Synchronize f_read right in front of the image data */
  f_read (&F1, Buffer1, index - 26, &BytesRead);
  
  
  /*Setup the window*/
  LCD_SetDisplayWindow(Xpoz, Ypoz, height, width);
  
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : decrement, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  if(LCDType == LCD_HX8347D)
  {
    LCD_WriteReg(LCD_REG_22, 0x68);
  }
  else
  {
    LCD_WriteReg(R3, 0x1008);
    LCD_WriteRAM_Prepare();
  }
  if(LCDType == LCD_HX8347D)
  {
    do
    {
      if (size*2 < 512)
      {
        i1 = size * 2;
      }
      else
      {
        i1 = 512;
      }
      size -= i1 / 2;
      
      
      f_read (&F1, Buffer1, i1, &BytesRead);
      
      
      for (index = 0; index < i1 / 2; index++)
      {
        /* Prepare to write to the LCD RAM */
        LCD_WriteRegIndex(LCD_REG_34);
        GPIOF->BRR = GPIO_Pin_4 ;
        SPI_SendData8(LCD_SPI, 0x72);
        while((SPI1->SR & SPI_I2S_FLAG_BSY) != RESET)
        {
        }
        /* Write RAM */
        LCD_WriteRAM(*(uint16_t *)BmpAddress);
        
        /* Set  the /CS control line */
        GPIOF->BSRR = GPIO_Pin_4 ;
        Counter++;
        
        /* Set Cursor */
        Xpos2 = (uint16_t)((Counter / width) + (240 - Xpoz));
        Ypos2 = (uint16_t)(((Counter % width)) + (320 - Ypoz));
        LCD_WriteReg(LCD_REG_2, Ypos2 >> 8);
        LCD_WriteReg(LCD_REG_3, Ypos2 & 0xFF);
        LCD_WriteReg(LCD_REG_6, 0x00);
        LCD_WriteReg(LCD_REG_7,Xpos2);
        
        BmpAddress += 2;  /*jump on next pixel*/
      }
      BmpAddress = (uint32_t)Buffer1;
    }while (size > 0);
  }
  else
  {
    do
    {
      if (size*2 < 512)
      {
        i1 = size * 2;
      }
      else
      {
        i1 = 512;
      }
      size -= i1 / 2;
      
      /* Set LCD control line(/CS) */
      LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
      
      f_read (&F1, Buffer1, i1, &BytesRead);
      
      LCD_WriteRAM_Prepare();
      for (index = 0; index < i1 / 2; index++)
      {
        LCD_WriteRAM(*(uint16_t *)BmpAddress);
        BmpAddress += 2;  /*jump on next pixel*/
      }
      BmpAddress = (uint32_t)Buffer1;
    }while (size > 0);
    
  }
  /* Set LCD control line(/CS) */
  LCD_CtrlLinesWrite(LCD_NCS_GPIO_PORT, LCD_NCS_PIN, Bit_SET);
  
  f_close (&F1);

  /* Set GRAM write direction and BGR = 1 */
  /* I/D = 01 (LCD_DIR_HORIZONTAL : increment, LCD_DIR_VERTICAL : decrement) */
  /* AM = 1 (address is updated in vertical writing direction) */
  if(LCDType == 0x47)
  {
    LCD_WriteReg(LCD_REG_22, 0x28);
  }
  else
  {
    LCD_WriteReg(LCD_REG_3, 0x1018);
  }
  LCD_SetCursor(0,0);
  
  return 1;
}

/**
  * @brief  Open a file and copy its content to a buffer
  * @param  BmpName: the file name to open
  * @param  FileLen: the File lenght
  * @retval Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen)
{
  if (f_mount(0, &fs))
  {
    return 1;
  }
  if (f_open (&F, BmpName, FA_READ))
  {
    return 2;
  }

  f_read (&F, Buffer1, 6, &BytesRead);

  if (Menu_Buffercmp((uint8_t *)SlidesCheck, (uint8_t *) Buffer1, 6) != 0)
  {
    return 3;
  }
  return 0;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  DirName: the Directory name to open
  * @param  Files: the file to open
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles (const char* DirName, char* Files[])
{
  uint32_t i = 0, j = 0;
  FRESULT res;

  f_mount(0, &fs);
  res = f_opendir(&dir, DirName);

  if (res == FR_OK)
  {
    i = strlen(DirName);
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      if (fno.fname[0] == '.') continue;

      if (!(fno.fattrib & AM_DIR))
      {
        do
        {
          i++;
        }
        while (fno.fname[i] != 0x2E);


        if (j < MAX_FILES)
        {
          if ((fno.fname[i + 1] == 'B') && (fno.fname[i + 2] == 'M') && (fno.fname[i + 3] == 'P'))
          {
            sprintf (Files[j], "%-11.11s", fno.fname);
            j++;
          }
        }
        i = 0;
      }
    }
  }

  f_mount (0, NULL);

  return j;
}

/**
  * @brief  List up to 25 file on the root directory with extension .WAV
  * @param  DirName: the Directory name to open
  * @param  Files: the file to open
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryWaveFiles (const char* DirName, char* Files[])
{
  uint32_t i = 0, j = 0, k = 0;
  FRESULT res;

  f_mount(0, &fs);
  res = f_opendir(&dir, DirName);

  if (res == FR_OK)
  {
    i = strlen(DirName);
    for (;;)
    {
      for(k = 0; k < 13; k++)
      {
        fno.fname[k] = ' ';
      }
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      if (fno.fname[0] == '.') continue;

      if (!(fno.fattrib & AM_DIR))
      {
        do
        {
          i++;
        }
        while (fno.fname[i] != 0x2E);
        if (j < MAX_FILES)
        {
          if ((fno.fname[i + 1] == 'W') && (fno.fname[i + 2] == 'A') && (fno.fname[i + 3] == 'V'))
          {
            sprintf (Files[j], "%-12.12s", fno.fname);
            j++;
          }
        }
        i = 0;
      }
    }
  }

  f_mount (0, NULL);

  return j;
}


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
