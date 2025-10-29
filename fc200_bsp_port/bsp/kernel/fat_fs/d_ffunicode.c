/******[Configuration Header]*****************************************//**
\file
\brief
  Module Title       : d_ffunicode

  Abstract           : Kernel FAT FS Unicode handling functions

  Software Structure : SRS References: 136T-2200-131000-001-D20 SWREQ-230
                       SDD References: 136T-2200-131000-001-D22 SWDES-556
                                                                SWDES-682
                                                                SWDES-683
                                                                SWDES-684
                                                                SWDES-685
                                                                SWDES-686
                                                                SWDES-687
                                                                SWDES-688
                                                                SWDES-689
                                                                SWDES-690
                                                                SWDES-691
                                                                SWDES-692
                                                                SWDES-693
                                                                SWDES-694
                                                                SWDES-695
                                                                SWDES-696
                                                                SWDES-697
                                                                SWDES-698
                                                                SWDES-699
\note
  CSC ID             : SWDES-76
*************************************************************************/

/***** Includes *********************************************************/

#include "d_ff.h"

/***** Constants ********************************************************/

#ifdef FF_USE_LFN	/* This module is blanked when non-LFN configuration */

/*------------------------------------------------------------------------*/
/* Code Conversion Tables                                                 */
/*------------------------------------------------------------------------*/

static const Uint16_t uc850u[] = {	/*  CP850(Latin 1) to Unicode conversion table */
	0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
	0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
	0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
	0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0, 0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
	0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
	0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE, 0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
	0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE, 0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
	0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8, 0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0
};

/***** Type Definitions *************************************************/

/***** Macros (Inline Functions) Definitions ****************************/

/***** Variables ********************************************************/

/***** Function Declarations ********************************************/

/***** Function Definitions *********************************************/

/*------------------------------------------------------------------------*/
/* OEM <==> Unicode conversions for static code page configuration        */
/* SBCS fixed code page                                                   */
/*------------------------------------------------------------------------*/

#if d_FAT_CODE_PAGE != 0 && d_FAT_CODE_PAGE < 900
Uint16_t d_FAT_Uni2oem (	/* Returns OEM code character, zero on error */
	Uint32_t	uni,	/* UTF-16 encoded character to be converted */
	Uint16_t	cp		/* Code page for the conversion */
)
{
	Uint16_t c = 0;
	const Uint16_t *p = uc850u;


	if (uni < 0x80u) {	/* ASCII? */
		c = (Uint16_t)uni;

	} else {			/* Non-ASCII */
		if ((uni < 0x10000u) && (cp == d_FAT_CODE_PAGE)) {	/* Is it in BMP and valid code page? */
			for (c = 0; (c < 0x80u) && (uni != p[c]); c++)
      {
      }
			c = (c + 0x80u) & 0xFFu;
		}
	}

	return c;
}

Uint16_t d_FAT_Oem2uni (	/* Returns Unicode character, zero on error */
	Uint16_t	oem,	/* OEM code to be converted */
	Uint16_t	cp		/* Code page for the conversion */
)
{
	Uint16_t c = 0;
	const Uint16_t *p = uc850u;


	if (oem < 0x80u) {	/* ASCII? */
		c = oem;

	} else {			/* Extended char */
		if (cp == d_FAT_CODE_PAGE) {	/* Is it a valid code page? */
			if (oem < 0x100u)
      {
        c = p[oem - 0x80u];
		  }
    }
	}

	return c;
}

#endif




/*------------------------------------------------------------------------*/
/* OEM <==> Unicode conversions for dynamic code page configuration       */
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
/* Unicode up-case conversion                                             */
/*------------------------------------------------------------------------*/

Uint32_t d_FAT_WToUpper (	/* Returns up-converted code point */
	Uint32_t uni		/* Unicode code point to be up-converted */
)
{
	Uint32_t unc = uni;
	if ((unc >= 0x61u) && (unc <= 0x7Au))
	{
		unc = unc & 0xFDu;
	}

	return unc;
}


#endif /* #if FF_USE_LFN */
