/*
 * Copyright (C), 2000-2004 by the monit project group.
 * All Rights Reserved.
 *
 * 2005, Modified by Marius M. M.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "include/gnotify-backend.h"
#include "include/base64.h"


/**
 *  Implementation of base64 encoding/decoding. 
 *
 *  @author Jan-Henrik Haukeland, <hauk@tildeslash.com>
 *
 *  @version \$Id: base64.c,v 1.1 2005/05/01 14:50:50 devilx Exp $
 *
 *  @file
 */



/* ------------------------------------------------------------------ Public */



/**
 * Base64 encode and return size data in 'src'. The caller must free the
 * returned string.
 * @param size The size of the data in src
 * @param src The data to be base64 encode
 * @return encoded string otherwise NULL
 */
gchar *encode_base64(gint size, guchar *src) 
{

  gint i;
  gchar *out, *p;

  if(!src)
    return NULL;

  if(!size)
    size= strlen((gchar *)src);
    
  out= calloc(sizeof(gchar), size*4/3+4);

  p= out;
    
  for(i=0; i<size; i+=3) {
      
    guchar b1=0, b2=0, b3=0, b4=0, b5=0, b6=0, b7=0;
      
    b1 = src[i];
      
    if(i+1<size)
      b2 = src[i+1];
      
    if(i+2<size)
      b3 = src[i+2];
      
    b4= b1>>2;
    b5= ((b1&0x3)<<4)|(b2>>4);
    b6= ((b2&0xf)<<2)|(b3>>6);
    b7= b3&0x3f;
      
    *p++= encode(b4);
    *p++= encode(b5);
      
    if(i+1<size) {
      *p++= encode(b6);
    } else {
      *p++= '=';
    }
      
    if(i+2<size) {
      *p++= encode(b7);
    } else {
      *p++= '=';
    }

  }

  return out;

}


/**
 * Decode the base64 encoded string 'src' into the memory pointed to by
 * 'dest'. The dest buffer is <b>not</b> NUL terminated.
 * @param dest Pointer to memory for holding the decoded string.
 * Must be large enough to recieve the decoded string.
 * @param src A base64 encoded string.
 * @return TRUE (the length of the decoded string) if decode
 * succeeded otherwise FALSE.
 */
gint decode_base64(guchar *dest, const gchar *src) 
{

  if(src && *src) {
  
    guchar *p= dest;
    gint k, l= strlen(src)+1;
    guchar *buf= malloc(l);

    
    /* Ignore non base64 chars as per the POSIX standard */
    for(k=0, l=0; src[k]; k++) {
      
      if(is_base64(src[k])) {
	
	buf[l++]= src[k];
	
      }
      
    } 
    
    for(k=0; k<l; k+=4) {
      
      gchar c1='A', c2='A', c3='A', c4='A';
      guchar b1=0, b2=0, b3=0, b4=0;
      
      c1= buf[k];
      
      if(k+1<l) {
	
	c2= buf[k+1];
	
      }
      
      if(k+2<l) {
	
	c3= buf[k+2];
	
      }
      
      if(k+3<l) {
	
	c4= buf[k+3];
	
      }
      
      b1= decode(c1);
      b2= decode(c2);
      b3= decode(c3);
      b4= decode(c4);
      
      *p++=((b1<<2)|(b2>>4) );
      
      if(c3 != '=') {
	
	*p++=(((b2&0xf)<<4)|(b3>>2) );
	
      }
      
      if(c4 != '=') {
	
	*p++=(((b3&0x3)<<6)|b4 );
	
      }
      
    }
    
    g_free(buf);
    
    return(p-dest);

  }

  return FALSE;
  
}


/* ----------------------------------------------------------------- Private */


/**
 * Base64 encode one byte
 */
gchar encode(guchar u) {

  if(u < 26)  return 'A'+u;
  if(u < 52)  return 'a'+(u-26);
  if(u < 62)  return '0'+(u-52);
  if(u == 62) return '+';
  
  return '/';

}


/**
 * Decode a base64 character
 */
guchar decode(gchar c) {
  
  if(c >= 'A' && c <= 'Z') return(c - 'A');
  if(c >= 'a' && c <= 'z') return(c - 'a' + 26);
  if(c >= '0' && c <= '9') return(c - '0' + 52);
  if(c == '+')             return 62;
  
  return 63;
  
}


/**
 * Return TRUE if 'c' is a valid base64 character, otherwise FALSE
 */
gint is_base64(gchar c) {

  if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
     (c >= '0' && c <= '9') || (c == '+')             ||
     (c == '/')             || (c == '=')) {
    
    return TRUE;
    
  }
  
  return FALSE;

}
