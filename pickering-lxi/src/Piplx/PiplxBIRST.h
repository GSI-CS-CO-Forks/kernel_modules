/*******************************************************************************
 *  Pickering switching module header file of ClientBridge for controlling
 *  cards in LXI device.
 *
 *  Copyright (C) 1988 - 2010, Pickering Interfaces ltd.
 *   
 *  Support: support@pickeringswitch.com
 *  Version: 1.5.6
 *  Supported OS: Windows, Linux, LabViewRT
 *
 * Licence:
 * This agreement is made between Pickering Interfaces Ltd ("Pickering") and you,
 * the person who makes use of Pickering software products ("You"). You must agree
 * all terms in this agreement in order to use Pickering software legally. If you
 * don't agree all terms in the agreement, please don't use Pickering software, and
 * delete all related files from your computer.
 * 
 * 1. OWNERSHIP: Pickering software is fully owned by Pickering, this license
 * agreement doesn't change the ownership.
 * 
 * 2. LICENSE: Pickering grants You the license to use Pickering software, free of
 * charge, if you accept all the conditions listed in this agreement. "Use" means
 * loading the product to CPU, memory, and/or other storages of your computer.
 * 
 * 3. CONDITIONS: To be licensed to use Pickering software, You must: 
 *    a) Not modify any part of Pickering software; 
 *    b) Agree to release Pickering from all liabilities caused directly or
 *       indirectly by using Pickering software; 
 *    c) Agree not to attempt to reverse engineer, de-compile or use any other
 *       tools to extract source code from Pickering Software.
 *       
 * 4. CONSEQUENTIAL LICENSES: Some functions of Pickering software requires
 * additional licenses to fully operate. Pickering accepts no responsibility for
 * the provision of said licenses.
 * 
 * 5. REDISTRIBUTION:. You may freely re-distribute Pickering software in any way 
 * unless explicitly stated to the contrary for a particular product.
 *
 *******************************************************************************/

/// \cond

#ifndef _PIPLXBIRST_H_
#define _PIPLXBIRST_H_

#include "Picmlx.h"

#ifdef WIN32
  #ifdef PIPLX_EXPORTS
    #define PIPLXBIRST_API __declspec(dllexport) __stdcall
  #else
    #define PIPLXBIRST_API __declspec(dllimport) __stdcall
  #endif
#elif LINUX // !WIN32
    #define PIPLXBIRST_API
#endif // LINUX

/// \endcond

/* BIRST topology codes */
enum
{
	BIRST_TOPOLOGY_NONE	= 0,	/* No BIRST capability */
	BIRST_TOPOLOGY_001	= 1,	/* BIRST Mk1 original topology. Example: 40-584-001 (v2.00, with BIRST) */

	/* BIRST Mk1: dual Y-buses, independent isolation; BIRST connects to first daughtercard
	   INBOARD of isolation switches. Example: 60-555 (using PC634r0, PC635r0) ### NOW OBSOLETED */
	BIRST_TOPOLOGY_002	= 2,

	/* BIRST Mk1: topology as potentially available in 60-600, BIRST connections segmented on both axes
	   (though 60-600 itself would use BIRST Mk2) */
	BIRST_TOPOLOGY_003	= 3,

	/* BIRST Mk1: BRIC-M (and similar configs) with BIRST "sub-unit" connections to its two Y-buses */
	BIRST_TOPOLOGY_004	= 4,

	/* BIRST Mk1: single Y-bus, automated isolation; BIRST connects to first daughtercard
	   INBOARD of isolation switches. Example: 60-554 (using PC628r0, PC629r0) ### NOW OBSOLETED */
	BIRST_TOPOLOGY_005	= 5,

	/* BIRST Mk1 - models 60-590-001 and 60-590-002, which have complex topologies involving multiple
	   sub-units. */
	BIRST_TOPOLOGY_006	= 6,	/* BIRST Mk1 - 60-590-001 */
	BIRST_TOPOLOGY_007	= 7,	/* BIRST Mk1 - 60-590-002 */

	/* BIRST Mk1 - model 40-566A: dual Y-buses with independent isolation, requiring
	   an external adapter to test the second Y-bus */
	BIRST_TOPOLOGY_008	= 8,	/* BIRST Mk1 - 40-566A */

	BIRST_TOPOLOGY_009	= 9,	/* BIRST Mk1 - as yet undefined */
	BIRST_TOPOLOGY_010	= 10,	/* BIRST Mk1 - as yet undefined */

	BIRST_TOPOLOGY_201	= 201,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_001 */
	BIRST_TOPOLOGY_202	= 202,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_002 */
	BIRST_TOPOLOGY_203	= 203,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_003 */
	BIRST_TOPOLOGY_204	= 204,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_004 */
	BIRST_TOPOLOGY_205	= 205,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_005 */
	BIRST_TOPOLOGY_206	= 206,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_006 */
	BIRST_TOPOLOGY_207	= 207,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_007 */
	BIRST_TOPOLOGY_208	= 208,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_008 */
	BIRST_TOPOLOGY_209	= 209,	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_009 */
	BIRST_TOPOLOGY_210	= 210	/* BIRST Mk2, otherwise similar to BIRST_TOPOLOGY_010 */
};

/* Logical bit definitions for BIRST control register #1 data DWORD. */
/* ### Ref Paul B 22/07/09.
   The relay numbering is as follows:
   RL1: OFF = calibration; ON = matrix access
   RL2: OFF = pole 1; ON = pole 2
   RL3: OFF = matrix 1; ON = matrix 2 (for pole 1)
   RL4: OFF = matrix 1; ON = matrix 2 (for pole 2)
   RL5-16 = Not specified at this time
*/
enum
{
	BIRST_CTRL_REV_POL		= 0x00000001UL,		/* Logical bit controlling current source polarity reverser */
	BIRST_CTRL_CONNECT		= 0x00000002UL,		/* Logical bit controlling BIRST circuit "RL1" */
	BIRST_CTRL_POLE2		= 0x00000004UL,		/* Logical bit controlling BIRST circuit "RL2" */
	BIRST_CTRL_POLE1_SUB2	= 0x00000008UL,		/* Logical bit controlling BIRST circuit "RL3" */
	BIRST_CTRL_POLE2_SUB2	= 0x00000010UL,		/* Logical bit controlling BIRST circuit "RL4" */

	/* Present hardware architecture supports up to 6 separate switch daughtercards,
	   each with its own primary selection relays (Steve E, 17/05/10).
	   They may be distributed across multiple sub-units - e.g. 60-600 dual matrix made from 4 daughtercards. */
	BIRST_CTRL_BLOCK1_PRI1	= 0x00000020UL,	/* Logical bit controlling physical block 1 primary connection relay 1 */
	BIRST_CTRL_BLOCK1_PRI2	= 0x00000040UL,	/* Logical bit controlling physical block 1 primary connection relay 2 */
	BIRST_CTRL_BLOCK2_PRI1	= 0x00000080UL,	/* Logical bit controlling physical block 2 primary connection relay 1 */
	BIRST_CTRL_BLOCK2_PRI2	= 0x00000100UL,	/* Logical bit controlling physical block 2 primary connection relay 2 */
	BIRST_CTRL_BLOCK3_PRI1	= 0x00000200UL,	/* Logical bit controlling physical block 3 primary connection relay 1 */
	BIRST_CTRL_BLOCK3_PRI2	= 0x00000400UL,	/* Logical bit controlling physical block 3 primary connection relay 2 */
	BIRST_CTRL_BLOCK4_PRI1	= 0x00000800UL,	/* Logical bit controlling physical block 4 primary connection relay 1 */
	BIRST_CTRL_BLOCK4_PRI2	= 0x00001000UL,	/* Logical bit controlling physical block 4 primary connection relay 2 */
	BIRST_CTRL_BLOCK5_PRI1	= 0x00002000UL,	/* Logical bit controlling physical block 5 primary connection relay 1 */
	BIRST_CTRL_BLOCK5_PRI2	= 0x00004000UL,	/* Logical bit controlling physical block 5 primary connection relay 2 */
	BIRST_CTRL_BLOCK6_PRI1	= 0x00008000UL,	/* Logical bit controlling physical block 6 primary connection relay 1 */
	BIRST_CTRL_BLOCK6_PRI2	= 0x00010000UL	/* Logical bit controlling physical block 6 primary connection relay 2 */
};

/* Legacy idents: only supported up to 2 unsegmented sub-units, each having one or two primary connection relays */
#define BIRST_CTRL_SUB1_PRI1	BIRST_CTRL_BLOCK1_PRI1
#define BIRST_CTRL_SUB1_PRI2	BIRST_CTRL_BLOCK1_PRI2
#define BIRST_CTRL_SUB2_PRI1	BIRST_CTRL_BLOCK2_PRI1
#define BIRST_CTRL_SUB2_PRI2	BIRST_CTRL_BLOCK2_PRI2

/* Sub-unit number used to get any card-level calibration values */
#define BIRST_SUB_NONE		0UL

/* Pole number used to get any sub-ubit level calibration values */
#define BIRST_POLE_NONE		0UL

/* BIRST Mk2: BIRST_ATTR_SUB_CURRENT values */
#define BIRST_MK2_CURRENT_UNDEFINED		0		/* Current not specified */
#define BIRST_MK2_CURRENT_LO			1		/* Low current (usually 10mA) */
#define BIRST_MK2_CURRENT_HI			2		/* High current (usually 100mA) */

/* BIRST Mk2: BIRST_ATTR_SUB_GAIN values */
#define BIRST_MK2_GAIN_UNDEFINED	0		/* Range not specified */
#define BIRST_MK2_GAIN_1			1		/* Range 1 (usually x10) */
#define BIRST_MK2_GAIN_2			2		/* Range 2 (usually x30) */
#define BIRST_MK2_GAIN_3			3		/* Range 3 (usually x100) */
#define BIRST_MK2_GAIN_4			4		/* Range 4 (usually x120) */

#ifdef __cplusplus
  extern "C" {
#endif

/**
 * <summary>
 *   Get card BIRST capabilities.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unity-based).</param>
 * <param name="Topology">Pointer to variable to receive topology code.</param>
 * <param name="Capabilities">Pointer to variable to receive capabilities flags.</param>
 * <param name="RefRes">Pointer tovariable to receive reference resistance value (in miliohms)</param>
 * <returns>Zero for success, or no-zero error code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstCapabilities(SESSION Sid,DWORD CardNum,DWORD *Topology,DWORD *Capabilities,DWORD *RefRes);

/**
 * <summary>
 *   Get sub-unit BIRST information.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unity-based).</param>
 * <param name="Poles">Pointer to variable to receive the number of poles.</param>
 * <param name="Axis1">Pointer to variable to receive first BIRST connection axis identifier ('X' or 'Y').</param> 
 * <param name="Line1">Pointer to variable to receive the first line of the above axis being utilised by BIRST.</param> 
 * <param name="Axis2">Pointer to variable to receive second BIRST connection axis identifier ('X' or 'Y').</param> 
 * <param name="Line2">Pointer to variable to receive the second line of the above axis being utilised by BIRST.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstInfo(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Poles,CHAR *Axis1,DWORD *Line1,CHAR *Axis2,DWORD *Line2);

/**
 * <summary>
 *   Set BIRST controls.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="Data">BIRST control settings.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstSetControls(SESSION Sid,DWORD CardNum,DWORD Data);

/**
 * <summary>
 *   Get setting of BIRST controls.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="Data">Pointer to variable to receive BIRST control settings.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstGetControls(SESSION Sid,DWORD CardNum,DWORD *Data);

/**
 * <summary>
 *   Make BIRST measurement.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unity-based).</param>
 * <param name="Value">Pointer to receive the measurement value.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstMeasurement(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Value);

/**
 * <summary>
 *   Write BIRST calibration values into EEPROM.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CarNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unit-based).</param>
 * <param name="Pole">The number of the switch pole.</param>
 * <param name="Offset">Offset in the object's BIRST calibration table at which to start (zero-based).</param>
 * <param name="NumValues">The number of calibration values to write. Also indicates size of Data array.</param>
 * <param name="Data">Pointer to an array of calibration values.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstWriteCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Pole,DWORD Offset,DWORD NumValues,DWORD *Data);

/**
 * <summary>
 *   Read BIRST calibration values from EEPROM.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CarNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unit-based).</param>
 * <param name="Pole">The number of the switch pole.</param>
 * <param name="Offset">Offset in the object's BIRST calibration table at which to start (zero-based).</param>
 * <param name="NumValues">The number of calibration values to write. Also indicates size of Data array.</param>
 * <param name="Data">Pointer to an array to receive the calibration values.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstReadCal(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD Pole,DWORD Offset,DWORD NumValues,DWORD *Data);

/**
 * <summary>
 *   Write BIRST calibration date information into EEPROM.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unity-based).</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstWriteCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub);

/**
 * <summary>
 *   Read BIRST calibration date information from EEPROM.
 * </summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Card number (unity-based).</param>
 * <param name="OutSub">Output sub-unit number (unity-based).</param>
 * <param name="Year">Pointer to variable to receive year of calibration.</param>
 * <param name="Day">Pointer to variable to receive day in year of calibration.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstReadCalDate(SESSION Sid,DWORD CardNum,DWORD OutSub,DWORD *Year,DWORD *Day);

/// Attribute codes for PIPLX_BirstGetAttribute()
#define BIRST_ATTR_CARD_TOPOLOGY			    1	///< Gets topology code.
#define BIRST_ATTR_CARD_REF_RES1			    2	///< Gets reference resistance #1 value, milliohms (e.g. 1000 = 1R0).
#define BIRST_ATTR_CARD_REF_RES2			    3	///< Gets reference resistance #2 value, milliohms (e.g. 100 = 0R1).
#define BIRST_ATTR_CARD_CAPABILITIES_REG1	4	///< Gets bit flags indicating BIRST control register #1 capabilities.
#define BIRST_ATTR_CARD_CAPABILITIES_REG2	5	///< Gets bit flags indicating BIRST control register #2 capabilities.
#define BIRST_ATTR_CARD_CAL_VALUES			  6	///< Gets the number of card-level calibration values.

#define BIRST_ATTR_SUB_NUM_POLES			  7	///< Gets the number of testable poles.
#define BIRST_ATTR_SUB_NUM_SEGMENTS			8	///< Gets the number of BIRST segments in the sub-unit.

/* It is assumed that, for example in 60-600-022 (32 x 8) *IF CONFIGURED FOR SEGMENTED BIRST*:
   BIRST logical segment 1 = (Y-seg1, X-seg1) = Y1 - Y4, X1 - X16;  BIRST connection = Y3, Y4
   BIRST logical segment 2 = (Y-seg1, X-seg2) = Y1 - Y4, X17 - X32; BIRST connection = Y3, Y4
   BIRST logical segment 3 = (Y-seg2, X-seg1) = Y5 - Y8, X1 - X16;  BIRST connection = Y7, Y8
   BIRST logical segment 4 = (Y-seg2, X-seg2) = Y5 - Y8, X17 - X32; BIRST connection = Y7, Y8
   BIRST logical segment 5 = unused
   BIRST logical segment 6 = unused
*/

#define BIRST_ATTR_SUB_CAL_VALUES	  9	  ///< Gets the number of sub-unit-level calibration values.
#define BIRST_ATTR_POLE_CAL_VALUES	10	///< Gets the number of pole-level calibration values.
#define BIRST_ATTR_SUB_CURRENT		  11	///< Gets the default test current (BIRST Mk2)
#define BIRST_ATTR_SUB_GAIN			    12	///< Gets the default gain range (BIRST Mk2) */

/// Sub-unit BIRST logical segment 1
#define BIRST_ATTR_SUB_SEG01_X_SIZE		  0x100		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG01_Y_SIZE		  0x101		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG01_X_MIN		  0x102		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG01_X_MAX		  0x103		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG01_Y_MIN		  0x104		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG01_Y_MAX		  0x105		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG01_BLOCK_NUM	0x106		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG01_AXIS1		  0x107		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG01_LINE1		  0x108		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG01_AXIS2		  0x109		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG01_LINE2		  0x10A		///< Gets line (numeric) reference of BIRST connection 2.

/// Sub-unit BIRST logical segment 2
#define BIRST_ATTR_SUB_SEG02_X_SIZE		  0x200		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG02_Y_SIZE		  0x201		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG02_X_MIN		  0x202		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG02_X_MAX		  0x203		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG02_Y_MIN		  0x204		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG02_Y_MAX		  0x205		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG02_BLOCK_NUM	0x206		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG02_AXIS1		  0x207		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG02_LINE1		  0x208		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG02_AXIS2		  0x209		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG02_LINE2		  0x20A		///< Gets line (numeric) reference of BIRST connection 2.

/// Sub-unit BIRST logical segment 3
#define BIRST_ATTR_SUB_SEG03_X_SIZE		  0x300		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG03_Y_SIZE		  0x301		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG03_X_MIN		  0x302		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG03_X_MAX		  0x303		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG03_Y_MIN		  0x304		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG03_Y_MAX		  0x305		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG03_BLOCK_NUM	0x306		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG03_AXIS1		  0x307		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG03_LINE1		  0x308		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG03_AXIS2		  0x309		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG03_LINE2		  0x30A		///< Gets line (numeric) reference of BIRST connection 2.

/// Sub-unit BIRST logical segment 4
#define BIRST_ATTR_SUB_SEG04_X_SIZE		  0x400		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG04_Y_SIZE		  0x401		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG04_X_MIN		  0x402		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG04_X_MAX		  0x403		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG04_Y_MIN		  0x404		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG04_Y_MAX		  0x405		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG04_BLOCK_NUM	0x406		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG04_AXIS1		  0x407		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG04_LINE1		  0x408		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG04_AXIS2		  0x409		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG04_LINE2		  0x40A		///< Gets line (numeric) reference of BIRST connection 2.

/// Sub-unit BIRST logical segment 5
#define BIRST_ATTR_SUB_SEG05_X_SIZE		  0x500		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG05_Y_SIZE		  0x501		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG05_X_MIN		  0x502		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG05_X_MAX		  0x503		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG05_Y_MIN		  0x504		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG05_Y_MAX		  0x505		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG05_BLOCK_NUM	0x506		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG05_AXIS1		  0x507		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG05_LINE1		  0x508		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG05_AXIS2		  0x509		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG05_LINE2		  0x50A		///< Gets line (numeric) reference of BIRST connection 2.

/// Sub-unit BIRST logical segment 6
#define BIRST_ATTR_SUB_SEG06_X_SIZE		  0x600		///< Gets segment X-size (columns).
#define BIRST_ATTR_SUB_SEG06_Y_SIZE		  0x601		///< Gets segment Y-size (rows).
#define BIRST_ATTR_SUB_SEG06_X_MIN		  0x602		///< Gets segment minimum X-line (column).
#define BIRST_ATTR_SUB_SEG06_X_MAX		  0x603		///< Gets segment maximum X-line (column).
#define BIRST_ATTR_SUB_SEG06_Y_MIN		  0x604		///< Gets segment minimum Y-line (row).
#define BIRST_ATTR_SUB_SEG06_Y_MAX		  0x605		///< Gets segment maximum Y-line (row).
#define BIRST_ATTR_SUB_SEG06_BLOCK_NUM	0x606		///< Gets BIRST physical block number.
#define BIRST_ATTR_SUB_SEG06_AXIS1		  0x607		///< Gets axis (alpha) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG06_LINE1		  0x608		///< Gets line (numeric) reference of BIRST connection 1.
#define BIRST_ATTR_SUB_SEG06_AXIS2		  0x609		///< Gets axis (alpha) reference of BIRST connection 2.
#define BIRST_ATTR_SUB_SEG06_LINE2		  0x60A		///< Gets line (numeric) reference of BIRST connection 2.

/**
 * <summary>Get a sub-unit BIRST attribute.</summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Pickering card reference of target (unity-based).</param>
 * <param name="SubNum">Sub-unit of target to access (unity-based).</param>
 * <param name="Out">TRUE for output sub-unit, FALSE for input sub-unit.</param>
 * <param name="AttrCode">Code indicating the attribute to be obtained.</param>
 * <param name="AttrValue">Pointer to variable to receive the value of the attribute.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstGetAttribute(SESSION Sid,DWORD CardNum,DWORD SubNum,BOOL Out,DWORD AttrCode,DWORD *AttrValue);

///< BIRST control register numbers.
enum
{
	BIRST_CTRL_REG1		= 1UL,
	BIRST_CTRL_REG2		= 2UL
};

/// Logical bit definitions for BIRST control register #2 data DWORD.
enum
{
	/* 08/07/10: additions for BIRST Mk2 (as in 40-551-001) */
	/* ### Independent control over the following pair may not really be necessary... */
	BIRST_CTRL2_HI_CURRENT_POS	= 0x00000001UL,	///< Logical bit controlling "RL5", 100mA+.
	BIRST_CTRL2_HI_CURRENT_NEG	= 0x00000002UL,	///< Logical bit controlling "RL6", 100mA-.
	BIRST_CTRL2_HI_RES_REF		  = 0x00000004UL,	///< Logical bit controlling "RL7", 1R0 ref.
	BIRST_CTRL2_LO_RES_REF		  = 0x00000008UL,	///< Logical bit controlling "RL8", 0R1 ref.
	BIRST_CTRL2_ANALOG_SW1		  = 0x00000010UL,	///< Logical bit controlling SW1, Matrix Access Point 1.
	BIRST_CTRL2_ANALOG_SW2		  = 0x00000020UL,	///< Logical bit controlling SW2, Matrix Access Point 1.
	BIRST_CTRL2_ANALOG_SW3		  = 0x00000040UL,	///< Logical bit controlling SW3, Matrix Access Point 2.
	BIRST_CTRL2_ANALOG_SW4		  = 0x00000080UL,	///< Logical bit controlling SW4, Matrix Access Point 2.
	BIRST_CTRL2_ANALOG_SW5		  = 0x00000100UL,	///< Logical bit controlling SW5, Diff. Amp. Gain Selection.
	BIRST_CTRL2_ANALOG_SW6		  = 0x00000200UL,	///< Logical bit controlling SW6, Diff. Amp. Gain Selection.
	BIRST_CTRL2_ANALOG_SW7		  = 0x00000400UL,	///< Logical bit controlling SW7, 100mA Calibration.
	BIRST_CTRL2_ANALOG_SW8		  = 0x00000800UL	///< Logical bit controlling SW8, 100mA Calibration.
};

/**
 * <summary>Set BIRST controls (extended).</summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Pickering card reference of target (unity-based).</param>
 * <param name="CtrlReg">BIRST control register number.</param>
 * <param name="Data">BIRST control settings for the given register.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstSetControlsEx(SESSION Sid,DWORD CardNum,DWORD CtrlReg,DWORD Data);

/**
 * <summary>Get BIRST controls (extended).</summary>
 * <param name="Sid">Handle of current session.</param>
 * <param name="CardNum">Pickering card reference of target (unity-based).</param>
 * <param name="CtrlReg">BIRST control register number.</param>
 * <param name="Data">Pointer to variable to receive BIRST control settings for the given register.</param>
 * <returns>Zero for success, or non zero erroror code.</returns>
 */
DWORD PIPLXBIRST_API PIPLX_BirstGetControlsEx(SESSION Sid,DWORD CardNum,DWORD CtrlReg,DWORD *Data);

#ifdef __cplusplus
  }
#endif

#endif // _PIPLXBIRST_H_
