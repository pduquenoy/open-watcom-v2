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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include <stddef.h>    /* NULL value */
#ifdef __OS2_PM__
    #define INCL_PM
    #define INCL_WINFRAMEMGR
    #define INCL_NLS
    #define INCL_GPILCIDS
    #define INCL_GPIPRIMITIVES
    #include <wos2.h>
    #include "winexprt.h"
#elif defined( __WINDOWS__ ) || defined( __NT__ )
    #define INCLUDE_TOOLHELP_H
    #define INCLUDE_COMMDLG_H
    #define OEMRESOURCE
    #include <wwindows.h>
#endif
#include "guimem.h"


#define CUIMemAlloc(s)      GUIMemAlloc(s)
#define CUIMemFree(p)       GUIMemFree(p)
#define CUIMemRealloc(p,s)  GUIMemRealloc(p,s)

