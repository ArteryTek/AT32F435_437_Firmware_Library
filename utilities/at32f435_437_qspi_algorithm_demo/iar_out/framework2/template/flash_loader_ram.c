#include "flash_loader.h"
#include "flash_loader_extra.h"

void strcopy(char *to, char* from);

uint32_t FlashInit(void *base_of_flash, uint32_t image_size,
                   uint32_t link_address, uint32_t flags,
                   int argc, char const *argv[])
{
//  strcopy(LAYOUT_OVERRIDE_BUFFER,"2 0x100,7 0x200,7 0x1000");
//  SET_BUFSIZE_OVERRIDE(&FlashBufferEnd-&FlashBufferStart+1);
//  SET_PAGESIZE_OVERRIDE(128);
//  return OVERRIDE_LAYOUT | OVERRIDE_BUFEND | OVERRIDE_PAGESIZE;
//  if (flags & FLAG_ERASE_ONLY)
//  {
//    perform full erase
//    return RESULT_ERASE_DONE;
//  }
  return RESULT_OK;
}

uint32_t FlashWrite(void *block_start,
                    uint32_t offset_into_block,
                    uint32_t count,
                    char const *buffer)
{
  char *to = (char*)block_start + offset_into_block;
  while (count--)
  {
    *to++ = *buffer++;
  }
  return 0;
}

uint32_t FlashErase(void *block_start,
                    uint32_t block_size)
{
  char *p = (char*)block_start;
  while (block_size--)
  {
    *p++ = 0;
  }
  return 0;
}

OPTIONAL_CHECKSUM
uint32_t FlashChecksum(void const *begin, uint32_t count)
{
  return Crc16((uint8_t const *)begin, count);
}

OPTIONAL_SIGNOFF
uint32_t FlashSignoff(void)
{
  return 0;
}

void strcopy(char *to, char* from)
{
  while (*to++ = *from++) ;
}



