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
* Description:  Librarian command line parsing (WLIB mode).
*
****************************************************************************/


#include "wlib.h"
#include <errno.h>
#include "wio.h"
#include "cmdlinew.h"

#include "clibext.h"


#define READ_BUFFER_SIZE    512

typedef struct {
    FILE    *fp;
    char    *buffer;
    size_t  size;
} getline_data;


static void SetPageSize( unsigned_16 new_size )
{
    unsigned int i;

    Options.page_size = MIN_PAGE_SIZE;
    for( i = 4; i < 16; i++ ) {
        if( new_size & (1 << i) ) {
            Options.page_size = 1 << i;
        }
    }
    if( Options.page_size < new_size ) {
        Options.page_size <<= 1;
    }
}

static const char *ParseCommand( const char *c )
{
    scan_ctrl       sctrl = SCTRL_SINGLE;
    const char      *start;
    operation       ops = 0;

    start = c;
    c = SkipWhite( c );
    switch( *c++ ) {
    case '-':
        ops = OP_DELETE;
        switch( *c ) {
        case '+':
            ops |= OP_ADD;
            sctrl = SCTRL_NORMAL;
            ++c;
            break;
#if defined(__UNIX__)
        case ':':
#else
        case '*':
#endif
            ops |= OP_EXTRACT;
            sctrl = SCTRL_NORMAL;
            ++c;
            break;
        }
        break;
    case '+':
        ops = OP_ADD;
        if( *c == '+' ) {
            ops |= OP_IMPORT;
            ++c;
            AddCommand( ops, &c, SCTRL_IMPORT );
            return( c );
        }
        if( *c == '-' ) {
            ops |= OP_DELETE;
            sctrl = SCTRL_NORMAL;
            ++c;
        }
        break;
#if defined(__UNIX__)
    case ':':
#else
    case '*':
#endif
        ops |= OP_EXTRACT;
        if( *c == '-' ) {
            ops |= OP_DELETE;
            sctrl = SCTRL_NORMAL;
            ++c;
        }
        break;
    default:
        FatalError( ERR_BAD_CMDLINE, start );
    }
    AddCommand( ops, &c, sctrl );
    return( c );
}

static const char *ParseOption( const char *c )
{
    unsigned long   page_size;
    const char      *start;
    const char      *endptr;
    ar_format       libformat;

    start = c++;
    c = SkipWhite( c );
    switch( tolower( *(unsigned char *)c++ ) ) {
    case '?':
        Usage();
        break;
    case 'b': //                       (don't create .bak file)
        Options.no_backup = true;
        break;
    case 'c': //                       (case sensitive)
        Options.respect_case = true;
        break;
    case 'd': // = <object_output_directory>
        if( Options.output_directory != NULL ) {
            FatalError( ERR_DUPLICATE_OPTION, start );
        }
        Options.output_directory = GetFilenameExt( &c, SCTRL_EQUAL, NULL );
        if( access( Options.output_directory, F_OK ) != 0 ) {
            FatalError( ERR_DIR_NOT_EXIST, Options.output_directory );
        }
        break;
    case 'i':
        switch( tolower( *(unsigned char *)c++ ) ) {
        case 'n':
            switch( tolower( *(unsigned char *)c++ ) ) {
            case 'n':
                Options.nr_ordinal = false;
                break;
            case 'o':
                Options.nr_ordinal = true;
                break;
            default:
                c = start;
                break;
            }
            break;
        case 'r':
            switch( tolower( *(unsigned char *)c++ ) ) {
            case 'n':
                Options.r_ordinal = false;
                break;
            case 'o':
                Options.r_ordinal = true;
                break;
            default:
                c = start;
                break;
            }
            break;
        case 'a':
            if( Options.processor != WL_PROC_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.processor = WL_PROC_AXP;
            break;
        case 'm':
            if( Options.processor != WL_PROC_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.processor = WL_PROC_MIPS;
            break;
        case 'p':
            if( Options.processor != WL_PROC_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.processor = WL_PROC_PPC;
            break;
        case 'i':
            if( Options.processor != WL_PROC_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.processor = WL_PROC_X86;
            break;
        case '6':
            if( Options.processor != WL_PROC_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.processor = WL_PROC_X64;
            break;
        case 'e':
            if( Options.filetype != WL_FTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.filetype = WL_FTYPE_ELF;
            break;
        case 'c':
            if( tolower( *(unsigned char *)c ) == 'l' ) {
                Options.coff_import_long = true;
                ++c;
            }
            if( Options.filetype != WL_FTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.filetype = WL_FTYPE_COFF;
            break;
        case 'o':
            if( Options.filetype != WL_FTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.filetype = WL_FTYPE_OMF;
            break;
        default:
            c = start;
            break;
        }
        break;
    case 'h':
        Usage();
        break;
    case 'l': // [ = <list_file_name> ]
        if( Options.list_contents ) {
            FatalError( ERR_DUPLICATE_OPTION, start );
        }
        Options.list_contents = true;
        Options.list_file = GetFilenameExt( &c, SCTRL_EQUAL, EXT_LST );
        break;
    case 'm': //                       (display C++ mangled names)
        Options.mangled = true;
        break;
    case 'o': // = <out_library_name>
        if( Options.output_name != NULL ) {
            FatalError( ERR_DUPLICATE_OPTION, start );
        }
        Options.output_name = GetFilenameExt( &c, SCTRL_EQUAL, EXT_LIB );
        break;
    case 'q': //                       (don't print header)
        Options.quiet = true;
        break;
    case 'v': //                       (print header)
        Options.quiet = false;
        break;
    case 'x': //                       (explode all objects in library)
        Options.explode = true;
#ifdef DEVBUILD
        Options.explode_count = 0;
        if( tolower( *(unsigned char *)c ) == 'n' ) {
            Options.explode_count = 1;
            ++c;
        }
        Options.explode_ext = GetFilenameExt( &c, SCTRL_EQUAL, EXT_OBJ );
        if( Options.explode_count ) {
            char    cn[20] = FILE_TEMPLATE_MASK;
            strcpy( cn + sizeof( FILE_TEMPLATE_MASK ) - 1, Options.explode_ext );
            Options.explode_ext = DupStr( cn );
        }
#else
        Options.explode_ext = GetFilenameExt( &c, SCTRL_EQUAL, NULL );
#endif
        break;
    case 'z':
        if( ( tolower( *(unsigned char *)c ) == 'l' ) && ( tolower( *(unsigned char *)( c + 1 ) ) == 'd' ) ) {
            c += 2;
            if( Options.strip_dependency ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.strip_dependency = true; //(strip dependency info)
            break;
        } else if( ( tolower( *(unsigned char *)c ) == 'l' ) && ( tolower( *(unsigned char *)( c + 1 ) ) == 'l' ) ) {
            c += 2;
            if( Options.strip_library ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.strip_library = true;  //(strip library info)
            break;
        } else if( Options.strip_expdef ) {
            FatalError( ERR_DUPLICATE_OPTION, start );
        }
        Options.strip_expdef = true;
        Options.export_list_file = GetFilenameExt( &c, SCTRL_EQUAL, NULL );
        break;
    case 't':
        if( tolower( *(unsigned char *)c ) == 'l' ) {
            ++c;
            Options.list_contents = true;
            Options.terse_listing = true; // (internal terse listing option)
        } else {
            Options.trim_path = true; //(trim THEADR pathnames)
        }
        break;
    case 'f':
        switch( tolower( *(unsigned char *)c++ ) ) {
        case 'm':
            if( Options.libtype != WL_LTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.libtype = WL_LTYPE_MLIB;
            Options.elf_found = true;
            break;
        case 'a':
            switch( tolower( *(unsigned char *)c++ ) ) {
            case 'b':
                libformat = AR_FMT_BSD;
                break;
            case 'c':
                libformat = AR_FMT_COFF;
                break;
            case 'g':
                libformat = AR_FMT_GNU;
                break;
            default:
                --c;
                libformat = AR_FMT_NONE;
                break;
            }
            if( Options.libtype != WL_LTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            } else if( libformat != AR_FMT_NONE ) {
                Options.ar_libformat = libformat;
            }
            Options.libtype = WL_LTYPE_AR;
            Options.coff_found = true;
            break;
        case 'o':
            if( Options.libtype != WL_LTYPE_NONE ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.libtype = WL_LTYPE_OMF;
            Options.omf_found = true;
            break;
        default:
            c = start;
            break;
        }
        break;
    case 'p':
        /*
         * following only used by OMF libary format
         */
        if( tolower( *(unsigned char *)c ) == 'a' ) {
            c++;
            if( Options.page_size ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            Options.page_size = (unsigned_16)-1;
        } else {
            if( Options.page_size ) {
                FatalError( ERR_DUPLICATE_OPTION, start );
            }
            c = SkipEqual( c );
            endptr = c;
            errno = 0;
            page_size = 0;
            if( isdigit( *(unsigned char *)c ) ) {
                page_size = strtoul( c, (char **)&endptr, 0 );
            }
            if( endptr == c || *endptr != '\0' ) {
                FatalError( ERR_BAD_CMDLINE, start );
            } else if( errno == ERANGE || page_size == 0 || page_size > MAX_PAGE_SIZE ) {
                FatalError( ERR_PAGE_RANGE );
            }
            c = endptr;
            SetPageSize( (unsigned_16)page_size );
        }
        break;
    case 'n': //                       (always create a new library)
        Options.new_library = true;
        break;
    case 's':
        Options.strip_line = true;
        break;
    default:
        c = start;
        break;
    }
    return( c );
}

static void my_getline_init( const char *name, getline_data *fd )
{
    fd->fp = fopen( name, "rb" );
    if( fd->fp == NULL ) {
        FatalError( ERR_CANT_OPEN, name, strerror( errno ) );
    }
    fd->size = READ_BUFFER_SIZE;
    fd->buffer = MemAlloc( READ_BUFFER_SIZE );
    if( fd->buffer == NULL ) {
        fd->size = 0;
    }
}

static char *my_getline( getline_data *fd )
{
    char    *p;
    size_t  len_used;
    size_t  maxlen;
    size_t  len;

    p = fd->buffer;
    maxlen = fd->size;
    len_used = 0;
    while( fgets( p, maxlen, fd->fp ) != NULL ) {
        len = strlen( p );
        if( len == 0 )
            continue;
        len_used += len;
        if( p[len - 1] == '\n' ) {
            break;
        }
        if( len < maxlen - 1 ) {
            break;
        }
        if( feof( fd->fp ) ) {
            break;
        }
        len = fd->size + READ_BUFFER_SIZE;
        p = MemAlloc( len );
        memcpy( p, fd->buffer, len_used + 1 );
        MemFree( fd->buffer );
        fd->buffer = p;
        fd->size = len;
        p += len_used;
        maxlen = len - len_used;
    }
    if( len_used ) {
        p = fd->buffer;
        if( p[len_used - 1] == '\n' ) {
            len_used--;
            p[len_used] = '\0';
        }
        if( len_used && p[len_used - 1] == '\r' ) {
            len_used--;
            p[len_used] = '\0';
        }
        return( p );
    }
    return( NULL );
}

static void my_getline_fini( getline_data *fd )
{
    fclose( fd->fp );
    MemFree( fd->buffer );
}

void ParseOneLineWlib( const char *c )
{
    const char  *start;

    for( ;; ) {
        c = SkipWhite( c );
        start = c;
        switch( *c ) {
#if !defined(__UNIX__)
        case '/':
            c = ParseOption( c );
            if( c == start )
                FatalError( ERR_BAD_OPTION, c[1] );
            break;
#endif

        case '-':
            if( CmdList == NULL && Options.input_name == NULL ) {
                c = ParseOption( c );
                if( c != start ) {
                    break;
                }
            }
#if !defined(__UNIX__)
        case '*':
#else
        case ':':
#endif
        case '+':
            c = ParseCommand( c );
            break;
        case '@':
            {
                const char  *old_cmd;
                char        *p;

                ++c;
                old_cmd = c;
                p = GetString( &c, SCTRL_SINGLE );
                if( p != NULL ) {
                    const char  *env;

                    env = WlibGetEnv( p );
                    MemFree( p );
                    if( env != NULL ) {
                        ParseOneLineWlib( env );
                        break;
                    }
                }
                c = old_cmd;
                p = GetFilenameExt( &c, SCTRL_NORMAL, EXT_CMD );
                if( p != NULL ) {
                    getline_data    fd;

                    my_getline_init( p, &fd );
                    MemFree( p );
                    while( (p = my_getline( &fd )) != NULL ) {
                        ParseOneLineWlib( p );
                    }
                    my_getline_fini( &fd );
                    break;
                }
                c = old_cmd;
            }
            break;
        case '\0':
        case '#':
            // comment - blow away line
            return;
        case '?':
            Usage();
            return;
        default:
            if( Options.input_name == NULL ) {
                Options.input_name = GetFilenameExt( &c, SCTRL_NORMAL, EXT_LIB );
            } else {
                AddCommand( OP_ADD | OP_DELETE, &c, SCTRL_SINGLE );
            }
            break;
        }
    }
}

