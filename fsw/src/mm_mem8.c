/*************************************************************************
** File: mm_mem8.c 
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
**   Provides CFS Memory Manager functions that are used
**   for the conditionally compiled MM_MEM8 optional memory type.
**
*************************************************************************/

/*************************************************************************
** Includes
*************************************************************************/
#include "mm_mem8.h"
#include "mm_app.h"
#include "mm_events.h"
#include "mm_utils.h"
#include <string.h>

/*
** The code in this file is optional.
** See mm_platform_cfg.h to set this compiler switch.
*/
#ifdef MM_OPT_CODE_MEM8_MEMTYPE

/*************************************************************************
** External Data
*************************************************************************/
extern MM_AppData_t MM_AppData;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Load memory from a file using only 8 bit wide writes            */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool MM_LoadMem8FromFile(uint32 FileHandle, const char *FileName, const MM_LoadDumpFileHeader_t *FileHeader,
                         cpuaddr DestAddress)
{
    uint32 i              = 0;
    int32  ReadLength     = 0;
    int32  PSP_Status     = CFE_PSP_SUCCESS;
    int32  BytesProcessed = 0;
    int32  BytesRemaining = FileHeader->NumOfBytes;
    uint8 *DataPointer8   = (uint8 *)DestAddress;
    uint8 *ioBuffer8      = (uint8 *)&MM_AppData.LoadBuffer[0];
    uint32 SegmentSize    = MM_MAX_LOAD_DATA_SEG;
    bool   Valid          = false;

    while (BytesRemaining != 0)
    {
        if (BytesRemaining < MM_MAX_LOAD_DATA_SEG)
        {
            SegmentSize = BytesRemaining;
        }

        /* Read file data into i/o buffer */
        if ((ReadLength = OS_read(FileHandle, ioBuffer8, SegmentSize)) != SegmentSize)
        {
            BytesRemaining = 0;
            CFE_EVS_SendEvent(MM_OS_READ_ERR_EID, CFE_EVS_EventType_ERROR,
                              "OS_read error received: RC = 0x%08X Expected = %d File = '%s'", (unsigned int)ReadLength,
                              (int)SegmentSize, FileName);
        }
        else
        {
            /* Load memory from i/o buffer using 8 bit wide writes */
            for (i = 0; i < SegmentSize; i++)
            {
                PSP_Status = CFE_PSP_MemWrite8((cpuaddr)DataPointer8, ioBuffer8[i]);
                if (PSP_Status == CFE_PSP_SUCCESS)
                {
                    DataPointer8++;
                }
                else
                {
                    /* CFE_PSP_MemWrite8 error */
                    BytesRemaining = 0;
                    CFE_EVS_SendEvent(MM_PSP_WRITE_ERR_EID, CFE_EVS_EventType_ERROR,
                                      "PSP write memory error: RC=0x%08X, Address=%p, MemType=MEM8",
                                      (unsigned int)PSP_Status, (void *)DataPointer8);
                    /* Stop load segment loop */
                    break;
                }
            }

            if (PSP_Status == CFE_PSP_SUCCESS)
            {
                BytesProcessed += SegmentSize;
                BytesRemaining -= SegmentSize;

                /* Prevent CPU hogging between load segments */
                if (BytesRemaining != 0)
                {
                    MM_SegmentBreak();
                }
            }
        }
    }

    /* Update last action statistics */
    if (BytesProcessed == FileHeader->NumOfBytes)
    {
        Valid                              = true;
        MM_AppData.HkPacket.LastAction     = MM_LOAD_FROM_FILE;
        MM_AppData.HkPacket.MemType        = MM_MEM8;
        MM_AppData.HkPacket.Address        = DestAddress;
        MM_AppData.HkPacket.BytesProcessed = BytesProcessed;
        strncpy(MM_AppData.HkPacket.FileName, FileName, OS_MAX_PATH_LEN);
    }

    return (Valid);

} /* end MM_LoadMem8FromFile */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Dump the requested number of bytes from memory to a file using  */
/* only 8 bit wide reads                                           */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool MM_DumpMem8ToFile(uint32 FileHandle, const char *FileName, const MM_LoadDumpFileHeader_t *FileHeader)
{
    bool   Valid          = true;
    int32  OS_Status      = OS_ERROR;
    int32  PSP_Status     = CFE_PSP_SUCCESS;
    uint32 i              = 0;
    uint32 BytesProcessed = 0;
    uint32 BytesRemaining = FileHeader->NumOfBytes;
    uint8 *DataPointer8   = (uint8 *)(FileHeader->SymAddress.Offset);
    uint8 *ioBuffer8      = (uint8 *)&MM_AppData.DumpBuffer[0];
    uint32 SegmentSize    = MM_MAX_DUMP_DATA_SEG;

    while (BytesRemaining != 0)
    {
        if (BytesRemaining < MM_MAX_DUMP_DATA_SEG)
        {
            SegmentSize = BytesRemaining;
        }

        /* Load RAM data into i/o buffer */
        for (i = 0; i < SegmentSize; i++)
        {
            if ((PSP_Status = CFE_PSP_MemRead8((cpuaddr)DataPointer8, &ioBuffer8[i])) == CFE_PSP_SUCCESS)
            {
                DataPointer8++;
            }
            else
            {
                /* CFE_PSP_MemRead8 error */
                Valid          = false;
                BytesRemaining = 0;
                CFE_EVS_SendEvent(MM_PSP_READ_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "PSP read memory error: RC=0x%08X, Src=%p, Tgt=%p, Type=MEM8",
                                  (unsigned int)PSP_Status, (void *)DataPointer8, (void *)&ioBuffer8[i]);
                /* Stop load i/o buffer loop */
                break;
            }
        }

        /* Check for error loading i/o buffer */
        if (PSP_Status == CFE_PSP_SUCCESS)
        {
            /* Write i/o buffer contents to file */
            if ((OS_Status = OS_write(FileHandle, ioBuffer8, SegmentSize)) == SegmentSize)
            {
                /* Update process counters */
                BytesRemaining -= SegmentSize;
                BytesProcessed += SegmentSize;

                /* Prevent CPU hogging between dump segments */
                if (BytesRemaining != 0)
                {
                    MM_SegmentBreak();
                }
            }
            else
            {
                /* OS_write error */
                Valid          = false;
                BytesRemaining = 0;
                CFE_EVS_SendEvent(MM_OS_WRITE_EXP_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "OS_write error received: RC = 0x%08X Expected = %d File = '%s'",
                                  (unsigned int)OS_Status, (int)SegmentSize, FileName);
            }
        }
    }

    if (Valid)
    {
        /* Update last action statistics */
        MM_AppData.HkPacket.LastAction = MM_DUMP_TO_FILE;
        MM_AppData.HkPacket.MemType    = MM_MEM8;
        MM_AppData.HkPacket.Address    = FileHeader->SymAddress.Offset;
        strncpy(MM_AppData.HkPacket.FileName, FileName, OS_MAX_PATH_LEN);
        MM_AppData.HkPacket.BytesProcessed = BytesProcessed;
    }

    return (Valid);

} /* end MM_DumpMem8ToFile */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Fill memory with the command specified fill pattern using only  */
/* 8 bit wide writes                                               */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool MM_FillMem8(cpuaddr DestAddress, const MM_FillMemCmd_t *CmdPtr)
{
    uint32 i              = 0;
    int32  PSP_Status     = CFE_PSP_SUCCESS;
    uint32 BytesProcessed = 0;
    uint32 BytesRemaining = CmdPtr->NumOfBytes;
    uint8  FillPattern8   = (uint8)CmdPtr->FillPattern;
    uint8 *DataPointer8   = (uint8 *)DestAddress;
    uint32 SegmentSize    = MM_MAX_FILL_DATA_SEG;
    bool   Result         = true;

    while (BytesRemaining != 0)
    {
        /* Set size of next segment */
        if (BytesRemaining < MM_MAX_FILL_DATA_SEG)
        {
            SegmentSize = BytesRemaining;
        }

        /* Fill next segment */
        for (i = 0; i < SegmentSize; i++)
        {
            PSP_Status = CFE_PSP_MemWrite8((cpuaddr)DataPointer8, FillPattern8);
            if (PSP_Status == CFE_PSP_SUCCESS)
            {
                DataPointer8++;
            }
            else
            {
                /* CFE_PSP_MemWrite8 error */
                BytesRemaining = 0;
                Result         = false;
                CFE_EVS_SendEvent(MM_PSP_WRITE_ERR_EID, CFE_EVS_EventType_ERROR,
                                  "PSP write memory error: RC=0x%08X, Address=%p, MemType=MEM8",
                                  (unsigned int)PSP_Status, (void *)DataPointer8);
                /* Stop fill segment loop */
                break;
            }
        }

        if (PSP_Status == CFE_PSP_SUCCESS)
        {
            /* Update process counters */
            BytesRemaining -= SegmentSize;
            BytesProcessed += SegmentSize;

            /* Prevent CPU hogging between fill segments */
            if (BytesRemaining != 0)
            {
                MM_SegmentBreak();
            }
        }
    }

    /* Update last action statistics */
    if (BytesProcessed == CmdPtr->NumOfBytes)
    {
        MM_AppData.HkPacket.LastAction     = MM_FILL;
        MM_AppData.HkPacket.MemType        = MM_MEM8;
        MM_AppData.HkPacket.Address        = DestAddress;
        MM_AppData.HkPacket.DataValue      = (uint32)FillPattern8;
        MM_AppData.HkPacket.BytesProcessed = BytesProcessed;
    }

    return Result;

} /* End MM_FillMem8 */

#endif /* MM_OPT_CODE_MEM8_MEMTYPE */

/************************/
/*  End of File Comment */
/************************/
