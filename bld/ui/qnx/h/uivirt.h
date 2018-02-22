/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
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


/*-

 uivirt.h -- interface class for display, mouse, keyboard.
        Note the separation of these is notational convenience
        It is unlikely you will ever want an X keyboard with
        a QNX Console.....

*/

#ifndef _UIVIRT_H_INCLUDED
#define _UIVIRT_H_INCLUDED

/*-
 The modules for each type....
*/

extern  bool    QnxWCheck(void);
extern  bool    ConsCheck(void);
extern  bool    TermCheck(void);
extern  bool    TInfCheck(void);

extern Display  ConsDisplay;
extern Display  TermDisplay;
extern Display  TInfDisplay;
extern Display  QnxWDisplay;

extern Keyboard ConsKeyboard;

extern Mouse    ConsMouse;
extern Mouse    TermMouse;

extern void     stopmouse(void);
extern void     stopkeyboard( void );
extern void     savekeyb(void);
extern void     restorekeyb(void);

#endif
