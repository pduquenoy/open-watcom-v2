/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024      The Open Watcom Contributors. All Rights Reserved.
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


#ifndef winputdialog_class
#define winputdialog_class

#include "wdialog.hpp"
#include "wstring.hpp"

WCLASS WText;
WCLASS WEditBox;
WCLASS WFileDialog;
WCLASS WInputDialog  : public WDialog {
    public:
        WEXPORT WInputDialog( WWindow *parent, const char *text );
        WEXPORT ~WInputDialog();
        void WEXPORT setBrowse( const char *filter=NULL, bool multi_select=false );
        void WEXPORT okButton( WWindow * );
        void WEXPORT cancelButton( WWindow * );
        bool WEXPORT getInput( WString& reply, const char *prompt );
    private:
        void initialize( void );
        void browseButton( WWindow * );
    private:
        WText           *_prompt;
        const char      *_promptText;
        WEditBox        *_input;
        WString         *_reply;
        WFileDialog     *_browseDialog;
        bool            _multiSelect;
};

#endif
