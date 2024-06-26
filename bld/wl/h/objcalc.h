/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Header file for salloc.c, objcalc.c and autogrp.c
*
****************************************************************************/


extern int          NumGroups;

/* in autogrp.c */

extern void             AutoGroup( void );
extern group_entry      *AllocGroup( const char *, group_entry ** );

/* in salloc.c */

extern void     NormalizeAddr( void );
extern void     AddSize( offset );
extern offset   CAlign( offset, unsigned );
extern void     Align( byte );
extern void     MAlign( byte );
extern void     StartMemMap( void );
extern void     ChkLocated( targ_addr *segadr, bool fixed);
extern void     NewSegment( seg_leader * );

/* in objcalc.c */

extern void     CheckClassOrder( void );
extern bool     IsCodeClass( const char *, size_t );
extern bool     IsConstClass( const char *, size_t );
extern bool     IsStackClass( const char *, size_t );
extern void     CalcAddresses( void );
extern void     AllocClasses( section * );
extern void     CombineSeg( seg_leader *, signed_32 *, offset, byte, byte );
extern void     ProcPubsSect( mod_entry *, section * );
extern void     DoPubsSect( section * );
extern void     SetSegFlags( xxx_seg_flags * );
extern void     CalcSegSizes( void );
extern void     ConvertToFrame( targ_addr *, segment, bool );
