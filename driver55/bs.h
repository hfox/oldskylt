#ifndef BS_H
#define BS_H

/* available ioctl's: */

#define BSRESET		0x100	/* reset the board */
#define BSSETBANK	0x101	/* set active memory page */
#define BSSELMEM	0x102	/* use ram/rom memory */
#define BSCURADDR	0x103	/* query current memory address */
#define BSVERSION	0x104	/* read card version */

#endif /* BS_H */
