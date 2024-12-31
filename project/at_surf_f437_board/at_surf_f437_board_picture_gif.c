/**
  **************************************************************************
  * @file     at_surf_f437_board_gif.c
  * @brief    gif image display.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPretENTATIONS OF ANY KIND. ARTERY EXPretSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPretS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPretENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at_surf_f437_board_picture.h"

const uint16_t _aMaskTbl[16] =
{
  0x0000, 0x0001, 0x0003, 0x0007,
  0x000f, 0x001f, 0x003f, 0x007f,
  0x00ff, 0x01ff, 0x03ff, 0x07ff,
  0x0fff, 0x1fff, 0x3fff, 0x7fff,
};
const uint8_t _aInterlaceOffset[] = {8, 8, 4, 2};
const uint8_t _aInterlaceYPos  [] = {0, 4, 2, 1};

/**
  * @brief  lcd draw line
  * @param  x0: row coordinates starting vaule
  * @param  y0: column coordinates starting vaule
  * @param  len: lengh
  * @param  color: color for fill in lcd
  * @retval none
  */
void piclib_draw_hline(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color)
{
  if((len == 0)||(x0>lcddev.width)||(y0>lcddev.height))
    return;

  lcd_fill(x0, y0, x0 + len-1, y0, color);
}

/**
  * @brief  detecting gif headers.
  * @param  file: file pointer.
  * @retval 0: gif89a/87a.
  *         1/2: other.
  */
uint8_t gif_check_head(FIL *file)
{
  uint8_t gifversion[6];
  uint32_t readed;
  uint8_t res;
  res = f_read(file, gifversion, 6, (UINT*)&readed);
  if(res)return 1;
  if((gifversion[0] != 'G')||(gifversion[1] != 'I')||(gifversion[2] != 'F')||
  (gifversion[3] != '8')||((gifversion[4] != '7') && (gifversion[4] != '9'))||
  (gifversion[5] != 'a'))return 2;
  else return 0;
}

/**
  * @brief  convert RGB888 to RGB565
  * @param  ctb: RGB888 color array first address
  * @retval RGB565 color.
  */
uint16_t gif_getrgb565(uint8_t *ctb)
{
  uint16_t r, g, b;
  r = (ctb[0]>>3)&0X1F;
  g = (ctb[1]>>2)&0X3F;
  b = (ctb[2]>>3)&0X1F;
  return b + (g<<5) + (r<<11);
}

/**
  * @brief  read color chart.
  * @param  file: file pointer.
  *         gif: gif information.
  *         num: tbl size
  * @retval 0: success.
  *         other: fail.
  */
uint8_t gif_readcolortbl(FIL *file, gif89a_type *gif, uint16_t num)
{
  uint8_t rgb[3];
  uint16_t t;
  uint8_t res;
  uint32_t readed;
  for(t = 0; t<num; t ++ )
  {
    res = f_read(file, rgb, 3, (UINT*)&readed);
    if(res)return 1;
    gif->colortbl[t] = gif_getrgb565(rgb);
  }
  return 0;
}

/**
  * @brief  get logical screen description.
  * @param  file: file pointer.
  *         gif: gif information.
  * @retval 0: success.
  *         other: fail.
  */
uint8_t gif_getinfo(FIL *file, gif89a_type *gif)
{
  uint32_t readed;
  uint8_t res;
  res = f_read(file, (uint8_t*)&gif->gifLSD, 7, (UINT*)&readed);
  if(res)return 1;
  if(gif->gifLSD.flag&0x80)
  {
    gif->numcolors = 2<<(gif->gifLSD.flag&0x07);
    if(gif_readcolortbl(file, gif, gif->numcolors))return 1;
  }
  return 0;
}

/**
  * @brief  save global color chart.
  * @param  gif: gif information.
  * @retval none.
  */
void gif_savegctbl(gif89a_type *gif)
{
  uint16_t i = 0;
  for(i = 0; i<256; i ++ )gif->bkpcolortbl[i] = gif->colortbl[i];
}

/**
  * @brief  restore global color chart.
  * @param  gif: gif information.
  * @retval none.
  */
void gif_recovergctbl(gif89a_type *gif)
{
  uint16_t i = 0;
  for(i = 0; i<256; i ++ )gif->colortbl[i] = gif->bkpcolortbl[i];
}

/**
  * @brief  initialize lzw related parameters.
  * @param  gif: gif information.
  *         codesize: lzw code length.
  * @retval none.
  */
void gif_initlzw(gif89a_type *gif, uint8_t codesize)
{
  memset((uint8_t *)gif->lzw, 0, sizeof(lzw_info_type));
  gif->lzw->SetCodeSize  = codesize;
  gif->lzw->CodeSize     = codesize  +  1;
  gif->lzw->ClearCode    = (1 << codesize);
  gif->lzw->EndCode      = (1 << codesize)  +  1;
  gif->lzw->MaxCode      = (1 << codesize)  +  2;
  gif->lzw->MaxCodeSize  = (1 << codesize) << 1;
  gif->lzw->ReturnClear  = 1;
  gif->lzw->LastByte     = 2;
  gif->lzw->sp           = gif->lzw->aDecompBuffer;
}

/**
  * @brief  read a data block.
  * @param  file: file pointer.
  *         buf: data cache area.
  *         maxnum: maximum read-write data limit.
  * @retval cnt.
  */
uint16_t gif_getdatablock(FIL *file, uint8_t *buf, uint16_t maxnum)
{
  uint8_t cnt;
  uint32_t readed;
  uint32_t fpos;
  f_read(file, &cnt, 1, (UINT*)&readed);
  if(cnt)
  {
    if (buf)
    {
      if(cnt>maxnum)
      {
        fpos = f_tell(file);
        f_lseek(file, fpos + cnt);
        return cnt;
      }
      f_read(file, buf, cnt, (UINT*)&readed);
    }else
    {
      fpos = f_tell(file);
      f_lseek(file, fpos + cnt);
    }
  }
  return cnt;
}

/**
  * @brief  readextension.
  *         reads an extension block. one extension block can consist of several data blocks.
  *         if an unknown extension block occures, the routine failes.
  * @param  file: file pointer.
  *         gif: gif information.
  * @retval 0: success.
  *         other: fail.
  */
uint8_t gif_readextension(FIL *file, gif89a_type *gif, int *pTransIndex, uint8_t *pDisposal)
{
  uint8_t temp;
  uint32_t readed;
  uint8_t buf[4];
  f_read(file, &temp, 1, (UINT*)&readed);
  switch(temp)
  {
    case GIF_PLAINTEXT:
    case GIF_APPLICATION:
    case GIF_COMMENT:
      while(gif_getdatablock(file, 0, 256)>0);
      return 0;
    case GIF_GRAPHICCTL:
      if(gif_getdatablock(file, buf, 4) != 4)return 1;
        gif->delay = (buf[2]<<8)|buf[1];
      *pDisposal = (buf[0]>>2)&0x7;
      if((buf[0]&0x1) != 0)*pTransIndex = buf[3];
      f_read(file, &temp, 1, (UINT*)&readed);
       if(temp != 0)return 1;
      return 0;
  }
  return 1;
}

/**
  * @brief  retrieve the next lzw code from the lzw cache, each code containing 12 bits.
  * @param  file: file pointer.
  *         gif: gif information.
  * @retval 0: success.
  *         other: fail.
  */
int gif_getnextcode(FIL *file, gif89a_type *gif)
{
  int i, j, End;
  long Result;
  if(gif->lzw->ReturnClear)
  {
    /* The first code should be a clearcode. */
    gif->lzw->ReturnClear = 0;
    return gif->lzw->ClearCode;
  }
  End = gif->lzw->CurBit + gif->lzw->CodeSize;
  if(End >= gif->lzw->LastBit)
  {
    int Count;
    if(gif->lzw->GetDone)return-1; //Error
    gif->lzw->aBuffer[0] = gif->lzw->aBuffer[gif->lzw->LastByte-2];
    gif->lzw->aBuffer[1] = gif->lzw->aBuffer[gif->lzw->LastByte-1];
    if((Count = gif_getdatablock(file, &gif->lzw->aBuffer[2], 300)) == 0)gif->lzw->GetDone = 1;
    if(Count<0)return -1; //Error
    gif->lzw->LastByte = 2 + Count;
    gif->lzw->CurBit = (gif->lzw->CurBit-gif->lzw->LastBit) + 16;
    gif->lzw->LastBit = (2 + Count)*8;
    End = gif->lzw->CurBit + gif->lzw->CodeSize;
  }
  j = End>>3;
  i = gif->lzw->CurBit>>3;
  if(i == j)Result = (long)gif->lzw->aBuffer[i];
  else if(i + 1 == j)Result = (long)gif->lzw->aBuffer[i]|((long)gif->lzw->aBuffer[i + 1]<<8);
  else Result = (long)gif->lzw->aBuffer[i]|((long)gif->lzw->aBuffer[i + 1]<<8)|((long)gif->lzw->aBuffer[i + 2]<<16);
  Result = (Result>>(gif->lzw->CurBit&0x7))&_aMaskTbl[gif->lzw->CodeSize];
  gif->lzw->CurBit += gif->lzw->CodeSize;
  return(int)Result;
}

/**
  * @brief  get the next code for lzw.
  * @param  file: file pointer.
  *         gif: gif information.
  * @retval <0: error.
  *         -1: unsuccessful.
  *         -2:  terminator.
  */
int gif_getnextbyte(FIL *file, gif89a_type *gif)
{
  int i, Code, Incode;
  while((Code = gif_getnextcode(file, gif)) >= 0)
  {
    if(Code == gif->lzw->ClearCode)
    {
      //Corrupt GIFs can make this happen
      if(gif->lzw->ClearCode >= (1<<MAX_NUM_LWZ_BITS))return -1; //Error
      //Clear the tables
      memset((uint8_t*)gif->lzw->aCode, 0, sizeof(gif->lzw->aCode));
      for(i = 0; i<gif->lzw->ClearCode;  ++ i)gif->lzw->aPrefix[i] = i;
      //Calculate the'special codes' independence of the initial code size
      //and initialize the stack pointer
      gif->lzw->CodeSize = gif->lzw->SetCodeSize + 1;
      gif->lzw->MaxCodeSize = gif->lzw->ClearCode<<1;
      gif->lzw->MaxCode = gif->lzw->ClearCode + 2;
      gif->lzw->sp = gif->lzw->aDecompBuffer;
      //Read the first code from the stack after clear ingand initializing*/
      do
      {
        gif->lzw->FirstCode = gif_getnextcode(file, gif);
      }while(gif->lzw->FirstCode == gif->lzw->ClearCode);
      gif->lzw->OldCode = gif->lzw->FirstCode;
      return gif->lzw->FirstCode;
    }
    if(Code == gif->lzw->EndCode)return -2; //End code
    Incode = Code;
    if(Code >= gif->lzw->MaxCode)
    {
      *(gif->lzw->sp) ++= gif->lzw->FirstCode;
      Code = gif->lzw->OldCode;
    }
    while(Code >= gif->lzw->ClearCode)
    {
      *(gif->lzw->sp) ++= gif->lzw->aPrefix[Code];
      if(Code == gif->lzw->aCode[Code])return Code;
      if((gif->lzw->sp-gif->lzw->aDecompBuffer) >= sizeof(gif->lzw->aDecompBuffer))return Code;
      Code = gif->lzw->aCode[Code];
    }
    *(gif->lzw->sp) ++= gif->lzw->FirstCode = gif->lzw->aPrefix[Code];
    if((Code = gif->lzw->MaxCode)<(1<<MAX_NUM_LWZ_BITS))
    {
      gif->lzw->aCode[Code] = gif->lzw->OldCode;
      gif->lzw->aPrefix[Code] = gif->lzw->FirstCode;
       ++ gif->lzw->MaxCode;
      if((gif->lzw->MaxCode >= gif->lzw->MaxCodeSize) && (gif->lzw->MaxCodeSize<(1<<MAX_NUM_LWZ_BITS)))
      {
        gif->lzw->MaxCodeSize <<= 1;
         ++ gif->lzw->CodeSize;
      }
    }
    gif->lzw->OldCode = Incode;
    if(gif->lzw->sp>gif->lzw->aDecompBuffer)return *--(gif->lzw->sp);
  }
  return Code;
}

/**
  * @brief  This routine draws a GIF image from the current pointer which should point to a
  *         valid GIF data block. The size of the desired image is given in the image descriptor
  * @param  file: file pointer.
  *         gif: gif information.
  *         pDescriptor : Points to a IMAGE_DESCRIPTOR structure, which contains infos about size, colors and interlacing.
  *         x0, y0      : obvious.
  *         Transparency: color index which should be treated as transparent.
  *         Disposal    : contains the disposal method of the previous image. if disposal == 2, the transparent pixels
  *                       of the image are rendered with the background color.
  * @retval 0: success.
  *         other: fail.
  */
uint8_t gif_dispimage(FIL *file, gif89a_type *gif, uint16_t x0, uint16_t y0, int Transparency, uint8_t Disposal)
{
  uint32_t readed;
  uint8_t lzwlen;
  int Index, OldIndex, XPos, YPos, YCnt, Pass, Interlace, XEnd;
  int Width, Height, Cnt, ColorIndex;
  uint16_t bkcolor;
  uint16_t *pTrans;

  Width = gif->gifISD.width;
  Height = gif->gifISD.height;
  XEnd = Width + x0-1;
  bkcolor = gif->colortbl[gif->gifLSD.bkcindex];
  pTrans = (uint16_t*)gif->colortbl;
  f_read(file, &lzwlen, 1, (UINT*)&readed);
  gif_initlzw(gif, lzwlen); //Initialize the LZW stack with the LZW code size
  Interlace = gif->gifISD.flag&0x40;
  for(YCnt = 0, YPos = y0, Pass = 0; YCnt<Height; YCnt ++ )
  {
    Cnt = 0;
    OldIndex = -1;
    for(XPos = x0; XPos <= XEnd; XPos ++ )
    {
      if(gif->lzw->sp>gif->lzw->aDecompBuffer)Index = *--(gif->lzw->sp);
        else Index = gif_getnextbyte(file, gif);
      if(Index == -2)return 0; //Endcode
      if((Index<0)||(Index >= gif->numcolors))
      {
        //IfIndex out of legal range stop decompressing
        return 1; //Error
      }
      //If current index equals old index increment counter
      if((Index == OldIndex) && (XPos <= XEnd))Cnt ++ ;
       else
      {
        if(Cnt)
        {
          if(OldIndex != Transparency)
          {
            piclib_draw_hline(XPos-Cnt-1, YPos, Cnt + 1, *(pTrans + OldIndex));
          }else if(Disposal == 2)
          {
            piclib_draw_hline(XPos-Cnt-1, YPos, Cnt + 1, bkcolor);
          }
          Cnt = 0;
        }else
        {
          if(OldIndex >= 0)
          {
            if(OldIndex != Transparency)lcd_point_draw(XPos-1, YPos, *(pTrans + OldIndex));
             else if(Disposal == 2)lcd_point_draw(XPos-1, YPos, bkcolor);
          }
        }
      }
      OldIndex = Index;
    }
    if((OldIndex != Transparency)||(Disposal == 2))
    {
      if(OldIndex != Transparency)ColorIndex = *(pTrans + OldIndex);
        else ColorIndex = bkcolor;
       if(Cnt)
      {
        piclib_draw_hline(XPos-Cnt-1, YPos, Cnt + 1, ColorIndex);
      }else lcd_point_draw(XEnd, YPos, ColorIndex);
    }
    //Adjust YPos if image is interlaced
    if(Interlace)
    {
      YPos += _aInterlaceOffset[Pass];
      if((YPos-y0) >= Height)
      {
         ++ Pass;
        YPos = _aInterlaceYPos[Pass] + y0;
      }
    }else YPos ++ ;
  }
  return 0;
}

/**
  * @brief  restore to background color
  * @param  x: coordinate
  *         y: coordinate
  *         gif: gif information
  *         pimge: image description block information
  * @retval 0: success.
  *         other: fail.
  */
void gif_clear2bkcolor(uint16_t x, uint16_t y, gif89a_type *gif, ImageScreenDescriptor pimge)
{
  uint16_t x0, y0, x1, y1;
  uint16_t color = gif->colortbl[gif->gifLSD.bkcindex];
  if(pimge.width == 0||pimge.height == 0)return;
  if(gif->gifISD.yoff>pimge.yoff)
  {
    x0 = x + pimge.xoff;
    y0 = y + pimge.yoff;
    x1 = x + pimge.xoff + pimge.width-1; ;
    y1 = y + gif->gifISD.yoff-1;
    if(x0<x1 && y0<y1 && x1<320 && y1<320)lcd_fill(x0, y0, x1, y1, color);
  }
  if(gif->gifISD.xoff>pimge.xoff)
  {
    x0 = x + pimge.xoff;
    y0 = y + pimge.yoff;
    x1 = x + gif->gifISD.xoff-1; ;
    y1 = y + pimge.yoff + pimge.height-1;
    if(x0<x1 && y0<y1 && x1<320 && y1<320)lcd_fill(x0, y0, x1, y1, color);
  }
  if((gif->gifISD.yoff + gif->gifISD.height)<(pimge.yoff + pimge.height))
  {
       x0 = x + pimge.xoff;
    y0 = y + gif->gifISD.yoff + gif->gifISD.height-1;
    x1 = x + pimge.xoff + pimge.width-1; ;
    y1 = y + pimge.yoff + pimge.height-1;
    if(x0<x1 && y0<y1 && x1<320 && y1<320)lcd_fill(x0, y0, x1, y1, color);
  }
   if((gif->gifISD.xoff + gif->gifISD.width)<(pimge.xoff + pimge.width))
  {
       x0 = x + gif->gifISD.xoff + gif->gifISD.width-1;
    y0 = y + pimge.yoff;
    x1 = x + pimge.xoff + pimge.width-1; ;
    y1 = y + pimge.yoff + pimge.height-1;
    if(x0<x1 && y0<y1 && x1<320 && y1<320)lcd_fill(x0, y0, x1, y1, color);
  }
}

/**
  * @brief  draw a frame of gif image
  * @param  file: file pointer.
  *         gif: gif information
  *         x0, y0: starting coordinates displayed
  * @retval 0: success.
  *         other: fail.
  */
uint8_t gif_drawimage(FIL *file, gif89a_type *gif, uint16_t x0, uint16_t y0)
{
  uint32_t readed;
  uint8_t res, temp;
  uint16_t numcolors;
  ImageScreenDescriptor previmg;

  uint8_t Disposal;
  int TransIndex;
  uint8_t Introducer;
  TransIndex = -1;
  do
  {
    res = f_read(file, &Introducer, 1, (UINT*)&readed);
    if(res)return 1;
    switch(Introducer)
    {
      case GIF_INTRO_IMAGE:
        previmg.xoff = gif->gifISD.xoff;
        previmg.yoff = gif->gifISD.yoff;
        previmg.width = gif->gifISD.width;
        previmg.height = gif->gifISD.height;

        res = f_read(file, (uint8_t*)&gif->gifISD, 9, (UINT*)&readed);
        if(res)return 1;
        if(gif->gifISD.flag&0x80)
        {
          gif_savegctbl(gif);
          numcolors = 2<<(gif->gifISD.flag&0X07);
          if(gif_readcolortbl(file, gif, numcolors))return 1;
        }
        if(Disposal == 2)gif_clear2bkcolor(x0, y0, gif, previmg);
        gif_dispimage(file, gif, x0 + gif->gifISD.xoff, y0 + gif->gifISD.yoff, TransIndex, Disposal);
         while(1)
        {
          f_read(file, &temp, 1, (UINT*)&readed);
          if(temp == 0)break;
          readed = f_tell(file);
          if(f_lseek(file, readed + temp))break;
          }
        if(temp != 0)return 1; //Error
        return 0;
      case GIF_INTRO_TERMINATOR:
        return 2;
      case GIF_INTRO_EXTENSION:
        //Read image extension*/
        res = gif_readextension(file, gif, &TransIndex, &Disposal);
        if(res)return 1;
         break;
      default:
        return 1;
    }
  }while(Introducer != GIF_INTRO_TERMINATOR);
  return 0;
}

/**
  * @brief  display .GIF image.
  * @param  picture: image information structure.
  *         pname: image file path.
  *         x_pos: the x-axis displays the starting point.
  *         y_pos: the y-axis displays the starting point.
  *         width: display width.
  *         height: display width.
  * @retval ERROR: did not display successfully.
  *         SUCCESS: display completed successfully .
  */
error_status gif_decode(picture_type *picture, uint8_t *pname, uint16_t x_pos, uint16_t y_pos, uint16_t width, uint16_t height)
{
  uint16_t dtime = 0;
  uint16_t res = 0;

  error_status status = SUCCESS;

  /* display parameter initialization */
  picture->display_xpos = x_pos;
  picture->display_ypos = y_pos;

  /* malloc memory */
  picture->gif89a = at32_calloc(1, sizeof(gif89a_type));
  picture->gif89a->lzw = at32_calloc(1, sizeof(lzw_info_type));

  /* open the specified file */
  if(f_open(&picture->file, (const TCHAR*)pname, FA_READ)  != FR_OK)
  {
    status = ERROR;

    goto end;
  }

  if(gif_check_head(&picture->file))
  {
    status = ERROR;

    goto end;
  }

  if(gif_getinfo(&picture->file, picture->gif89a))
  {
    status = ERROR;

    goto end;
  }

  /* over size check */
  if(picture->gif89a->gifLSD.width>width||picture->gif89a->gifLSD.height>height)
  {
    status = ERROR;

    goto end;
  }
  else
  {
    x_pos = (width-picture->gif89a->gifLSD.width)/2 + x_pos;
    y_pos = (height-picture->gif89a->gifLSD.height)/2 + y_pos;
  }

  /* image decoding */
  while(res == 0)
  {
    res = gif_drawimage(&picture->file, picture->gif89a, x_pos, y_pos);

    if(picture->gif89a->gifISD.flag&0x80)
    {
      gif_recovergctbl(picture->gif89a);
    }

    if(picture->gif89a->delay)
    {
      dtime = picture->gif89a->delay;
    }
    else
    {
      dtime = 10;
    }

    while(dtime--)
    {
      delay_ms(10);
    }

    if(res == 2)
    {
      res = 0;
      break;
    }
  }

  end:

  /* close file */
  f_close(&picture->file);

  /* free memory */
  at32_free(picture->gif89a);
  at32_free(picture->gif89a->lzw);

  return status;
}
