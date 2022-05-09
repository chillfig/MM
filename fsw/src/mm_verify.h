/*************************************************************************
** File: mm_verify.h 
**
**   Copyright � 2007-2014 United States Government as represented by the
**   Administrator of the National Aeronautics and Space Administration.
**   All Other Rights Reserved.
**
**   This software was created at NASA's Goddard Space Flight Center.
**   This software is governed by the NASA Open Source Agreement and may be
**   used, distributed and modified only pursuant to the terms of that
**   agreement.
**
** Purpose:
**   Contains CFS Memory Manager macros that run preprocessor checks
**   on mission configurable parameters
**
*************************************************************************/
#ifndef _mm_verify_
#define _mm_verify_

/*************************************************************************
** Includes
*************************************************************************/
#include "mm_mission_cfg.h"
#include "mm_platform_cfg.h"
#include "stdint.h"

/*************************************************************************
** Macro Definitions
*************************************************************************/

/*
** Maximum value of the subtype
*/
#if MM_CFE_HDR_SUBTYPE > UINT32_MAX
#error MM_CFE_HDR_SUBTYPE cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a file load to RAM memory
*/
#if MM_MAX_LOAD_FILE_DATA_RAM > UINT32_MAX
#error MM_MAX_LOAD_FILE_DATA_RAM cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a file load to EEPROM memory
*/
#if MM_MAX_LOAD_FILE_DATA_EEPROM > UINT32_MAX
#error MM_MAX_LOAD_FILE_DATA_EEPROM cannot exceed the uint32 maximum value
#endif

/*
**  Maximum number of bytes for an uninterruptable load
*/
#if MM_MAX_UNINTERRUPTIBLE_DATA < 1
#error MM_MAX_UNINTERRUPTIBLE_DATA cannot be less than 1
#elif MM_MAX_UNINTERRUPTIBLE_DATA > 255
#error MM_MAX_UNINTERRUPTIBLE_DATA should be less than 256 Bytes
#endif

/*
** Maximum number of bytes for a file dump from RAM memory
*/
#if MM_MAX_DUMP_FILE_DATA_RAM > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_RAM cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a file dump from EEPROM memory
*/
#if MM_MAX_DUMP_FILE_DATA_EEPROM > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_EEPROM cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a file dump of the symbol table
*/
#if MM_MAX_DUMP_FILE_DATA_SYMTBL > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_SYMTBL cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes that can be dumped per task cycle
*/
#if MM_MAX_DUMP_DATA_SEG > UINT32_MAX
#error MM_MAX_DUMP_DATA_SEG cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a fill to RAM memory
*/
#if MM_MAX_FILL_DATA_RAM > UINT32_MAX
#error MM_MAX_FILL_DATA_RAM cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes for a fill to EEPROM memory
*/
#if MM_MAX_FILL_DATA_EEPROM > UINT32_MAX
#error MM_MAX_FILL_DATA_EEPROM cannot exceed the uint32 maximum value
#endif

/*
** Maximum number of bytes that can be filled per task cycle
*/
#if MM_MAX_FILL_DATA_SEG > UINT32_MAX
#error MM_MAX_FILL_DATA_SEG cannot exceed the uint32 maximum value
#endif

/*
** Minimum size for max load file defaults
*/
#if MM_MAX_LOAD_FILE_DATA_RAM < 1
#error MM_MAX_LOAD_FILE_DATA_RAM cannot be less than 1
#endif

#if MM_MAX_LOAD_FILE_DATA_EEPROM < 1
#error MM_MAX_LOAD_FILE_DATA_EEPROM cannot be less than 1
#endif

/*
** Minimum size for max load data segment
*/
#if MM_MAX_LOAD_DATA_SEG < 4
#error MM_MAX_LOAD_DATA_SEG cannot be less than 4
#endif

/*
** Minimum size for max fill data segment
*/
#if MM_MAX_FILL_DATA_SEG < 4
#error MM_MAX_FILL_DATA_SEG cannot be less than 4
#endif

/*
**  Dump, load, and fill data segment sizes
*/
#if (MM_MAX_LOAD_DATA_SEG % 4) != 0
#error MM_MAX_LOAD_DATA_SEG should be longword aligned
#endif

#if (MM_MAX_DUMP_DATA_SEG % 4) != 0
#error MM_MAX_DUMP_DATA_SEG should be longword aligned
#endif

#if (MM_MAX_FILL_DATA_SEG % 4) != 0
#error MM_MAX_FILL_DATA_SEG should be longword aligned
#endif

/*
** Optional MEM32 Configurable Parameters
*/
#ifdef MM_OPT_CODE_MEM32_MEMTYPE

#if (MM_MAX_LOAD_FILE_DATA_MEM32 % 4) != 0
#error MM_MAX_LOAD_FILE_DATA_MEM32 should be longword aligned
#endif

#if (MM_MAX_DUMP_FILE_DATA_MEM32 % 4) != 0
#error MM_MAX_DUMP_FILE_DATA_MEM32 should be longword aligned
#endif

#if (MM_MAX_FILL_DATA_MEM32 % 4) != 0
#error MM_MAX_FILL_DATA_MEM32 should be longword aligned
#endif

/* Maximum number of bytes for a file load to MEM32 memory */
#if MM_MAX_LOAD_FILE_DATA_MEM32 > UINT32_MAX
#error MM_MAX_LOAD_FILE_DATA_MEM32 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a file dump from MEM32 memory */
#if MM_MAX_DUMP_FILE_DATA_MEM32 > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_MEM32 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a fill to MEM32 memory */
#if MM_MAX_FILL_DATA_MEM32 > UINT32_MAX
#error MM_MAX_FILL_DATA_MEM32 cannot exceed the uint32 maximum value
#endif

#endif /* MM_OPT_CODE_MEM32_MEMTYPE */

/*
** Optional MEM16 Configurable Parameters
*/
#ifdef MM_OPT_CODE_MEM16_MEMTYPE

#if (MM_MAX_LOAD_FILE_DATA_MEM16 % 2) != 0
#error MM_MAX_LOAD_FILE_DATA_MEM16 should be word aligned
#endif

#if (MM_MAX_DUMP_FILE_DATA_MEM16 % 2) != 0
#error MM_MAX_DUMP_FILE_DATA_MEM16 should be word aligned
#endif

#if (MM_MAX_FILL_DATA_MEM16 % 2) != 0
#error MM_MAX_FILL_DATA_MEM16 should be word aligned
#endif

/* Maximum number of bytes for a fill to MEM16 memory */
#if MM_MAX_FILL_DATA_MEM16 > UINT32_MAX
#error MM_MAX_FILL_DATA_MEM16 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a file load to MEM16 memory */
#if MM_MAX_LOAD_FILE_DATA_MEM16 > UINT32_MAX
#error MM_MAX_LOAD_FILE_DATA_MEM16 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a file dump from MEM16 memory */
#if MM_MAX_DUMP_FILE_DATA_MEM16 > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_MEM16 cannot exceed the uint32 maximum value
#endif

#endif /* MM_OPT_CODE_MEM16_MEMTYPE */

/*
** Optional MEM8 Configurable Parameters
*/
#ifdef MM_OPT_CODE_MEM8_MEMTYPE

/* Maximum number of bytes for a fill to MEM8 memory */
#if MM_MAX_FILL_DATA_MEM8 > UINT32_MAX
#error MM_MAX_FILL_DATA_MEM8 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a file load to MEM8 memory */
#if MM_MAX_LOAD_FILE_DATA_MEM8 > UINT32_MAX
#error MM_MAX_LOAD_FILE_DATA_MEM8 cannot exceed the uint32 maximum value
#endif

/* Maximum number of bytes for a file dump from MEM8 memory */
#if MM_MAX_DUMP_FILE_DATA_MEM8 > UINT32_MAX
#error MM_MAX_DUMP_FILE_DATA_MEM8 cannot exceed the uint32 maximum value
#endif

#endif /* MM_OPT_CODE_MEM8_MEMTYPE */

#if MM_LOAD_WID_CRC_TYPE != CFE_MISSION_ES_DEFAULT_CRC
#error MM_LOAD_WID_CRC_TYPE must be a type supported by CFE_ES_CalculateCRC
#endif
#if MM_LOAD_FILE_CRC_TYPE != CFE_MISSION_ES_DEFAULT_CRC
#error MM_LOAD_FILE_CRC_TYPE must be a type supported by CFE_ES_CalculateCRC
#endif
#if MM_DUMP_FILE_CRC_TYPE != CFE_MISSION_ES_DEFAULT_CRC
#error MM_DUMP_FILE_CRC_TYPE must be a type supported by CFE_ES_CalculateCRC
#endif

#endif /*_mm_verify_*/

/************************/
/*  End of File Comment */
/************************/
