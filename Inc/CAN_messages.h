#ifndef CAN_MESSAGES_H_
#define CAN_MESSAGES_H_


/* CAN-bus message ID specification */

#define MASK_ONES		0x7FF
#define MASK_ZEROES		0x000

#define FLTR_NULL		0x7FF

#define GLOB_DRIVE 		0x100
#define GLOB_BATT_STAT 	0x102
#define GLOB_BATT_PERC 	0x103

#define WROT_FL_STAT 	0x310
#define WROT_FL_ANGL 	0x311
#define WROT_FL_PID 	0x312
#define WROT_FR_STAT 	0x320
#define WROT_FR_ANGL 	0x321
#define WROT_FR_PID 	0x322
#define WROT_ML_STAT 	0x330
#define WROT_ML_ANGL 	0x331
#define WROT_ML_PID 	0x332
#define WROT_MR_STAT 	0x340
#define WROT_MR_ANGL 	0x341
#define WROT_MR_PID 	0x342
#define WROT_BL_STAT 	0x350
#define WROT_BL_ANGL 	0x351
#define WROT_BL_PID 	0x352
#define WROT_BR_STAT 	0x360
#define WROT_BR_ANGL 	0x361
#define WROT_BR_PID 	0x362

#define WDRW_FF_STAT 	0x400
#define WDRW_FF_FAULT 	0x401
#define WDRW_FL_STAT 	0x410
#define WDRW_FR_STAT	0x430
#define WDRW_ML_STAT	0x440
#define WDRW_MR_STAT	0x450
#define WDRW_BL_STAT	0x460
#define WDRW_BR_STAT	0x470

#define MAST_STAT		0x500
#define MAST_CMD		0x501
#define MAST_POS_PV		0x502
#define MAST_POS_SP		0x503




#endif
