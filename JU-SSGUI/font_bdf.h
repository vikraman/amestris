#ifndef FONT_BDF_H
#define FONT_BDF_H

struct fontinfo_tag
  {
  signed char *data;
  int table[96];
  int yofs,fh,oy;
  };
extern struct fontinfo_tag fonttimesR10,
               fonttimesR12,
               fonttimesR14,
               fonttimesR16,
               fonttimesR20,
               fonttimesR24,

	       fonttimesI10,
	       fonttimesI12,
	       fonttimesI14,
	       fonttimesI16,
	       fonttimesI20,
	       fonttimesI24,
	       
	       fontcour10,
	       fontcour12,
	       fontcour16,
	       fontcour20,
	       fontcour24,
	       
	       fontariali10,
	       fontariali14,
	       fontariali16,
	       fontariali24,
               
	       fontimpact12,
	       fontimpact16,
	       fontimpact20,
	       fontimpact24,

	       fontArialn12,
	       fontArialn14,
	       fontArialn16,
	       fontArialn20,
	       fontArialn24;
#endif	       

