#ifndef BS_H
#define BS_H

/* available ioctl's: */

#define BSRESET         0x100   /* reset the board */
#define BSSETBANK       0x101   /* set active memory page */
#define BSGETBANK       0x102   /* get active memory page */
#define BSSETMODE       0x103   /* set active mode (rom/1-bit ram/4-bit ram) */
#define BSGETMODE       0x104   /* get active mode */
#define BSSETRAW        0x105   /* set raw io */
#define BSGETRAW        0x106   /* get raw io */
#define BSCURADDR       0x107   /* query current memory address */
#define BSVERSION       0x108   /* read card version */



#endif /* BS_H */
