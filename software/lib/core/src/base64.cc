#include <stdio.h>
#include <stdlib.h>
#include <string>

// Orignal code from  https://sourceforge.net/projects/base64/
// LICENCE: MIT
// AUTHOR: Bob Trower 2001/08/04

// Modified for jade daq 2018/07/30

/*
** Translation Table as described in RFC1113
*/
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
void encodeblock( unsigned char *in, unsigned char *out, int len )
{
  out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
  out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
  out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
  out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

/*
** encode
**
** base64 encode a stream adding padding and line breaks as per spec.
*/
int base64_btoa(const std::string &in_str, std::string &out_str, int linesize = 0)
{
  const unsigned char *in_chars_beg = (const unsigned char*)(in_str.c_str());
  const unsigned char *in_chars_end = (const unsigned char*)(in_chars_beg + in_str.size());
  const unsigned char *in_chars_pos = in_chars_beg;
  out_str.clear();
  out_str.reserve(in_str.size() + in_str.size()/2);
  
  unsigned char in[3];
  unsigned char out[4];
  int i, len, blocksout = 0;
  int retcode = 0;

  *in = (unsigned char) 0;
  *out = (unsigned char) 0;
  /* while( feof( infile ) == 0 ) { */
  while( in_chars_pos < in_chars_end ) {
    len = 0;
    for( i = 0; i < 3; i++ ) {
      /* in[i] = (unsigned char) getc( infile ); */
      if(in_chars_pos < in_chars_end)
        in[i] = *in_chars_pos;
      else
        in[i] = -1;
      ++in_chars_pos;
      /* if( feof( infile ) == 0 ) { */
      if( in_chars_pos < in_chars_end ) {
        len++;
      }
      else {
        in[i] = (unsigned char) 0;
      }
    }
    if( len > 0 ) {
      encodeblock( in, out, len );
      /* for( i = 0; i < 4; i++ ) { */
        /* if( putc( (int)(out[i]), outfile ) == EOF ){ */
        /*   if( ferror( outfile ) != 0 )      { */
        /*     perror( b64_message( B64_FILE_IO_ERROR ) ); */
        /*     retcode = B64_FILE_IO_ERROR; */
        /*   } */
        /*   break; */
        /* } */
      /* } */
      out_str.append((const char*)out, 4);
      blocksout++;          
    }
    /* if( blocksout >= (linesize/4) || feof( infile ) != 0 ) { */
    if(linesize && (blocksout >= (linesize/4) || in_chars_pos >= in_chars_end)) {
      if( blocksout > 0 ) {
        /* fprintf( outfile, "\n" ); */
        out_str.push_back('\n');
      }
      blocksout = 0;
    }
  }
  return( retcode );
}

/*
** decodeblock
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
void decodeblock( unsigned char *in, unsigned char *out )
{   
  out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
  out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
  out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
int base64_atob( const std::string &in_str, std::string &out_str )
{
  unsigned char *in_chars_beg = (unsigned char*)(in_str.c_str());
  unsigned char *in_chars_end = (unsigned char*)(in_chars_beg + in_str.size());
  unsigned char *in_chars_pos = in_chars_beg;
  out_str.clear();
  out_str.reserve(in_str.size());
  
  int retcode = 0;
  unsigned char in[4];
  unsigned char out[3];
  int v;
  int i, len;

  *in = (unsigned char) 0;
  *out = (unsigned char) 0;
  /* while( feof( infile ) == 0 ) { */
  while( in_chars_pos < in_chars_end ) {
    /* for( len = 0, i = 0; i < 4 && feof( infile ) == 0; i++ ) { */
    for( len = 0, i = 0; i < 4 && in_chars_pos < in_chars_end; i++ ) {
      v = 0;
      /* while( feof( infile ) == 0 && v == 0 ) { */
      while( in_chars_pos < in_chars_end && v == 0 ) {
        /* v = getc( infile ); */
        if(in_chars_pos < in_chars_end)
          v = *in_chars_pos;
        else
          v = -1;

        ++in_chars_pos;

        /* if( feof( infile ) == 0 ) { */
        if( in_chars_pos < in_chars_end ) {
          v = ((v < 43 || v > 122) ? 0 : (int) cd64[ v - 43 ]);
          if( v != 0 ) {
            v = ((v == (int)'$') ? 0 : v - 61);
          }
        }
      }
      /* if( feof( infile ) == 0 ) { */
      if( in_chars_pos < in_chars_end ) {
        len++;
        if( v != 0 ) {
          in[ i ] = (unsigned char) (v - 1);
        }
      }
      else {
        in[i] = (unsigned char) 0;
      }
    }
    if( len > 0 ) {
      decodeblock( in, out );
      /* for( i = 0; i < len - 1; i++ ) { */
      /*   if( putc( (int) out[i], outfile ) == EOF ){ */
      /*     if( ferror( outfile ) != 0 )      { */
      /*       perror( b64_message( B64_FILE_IO_ERROR ) ); */
      /*       retcode = B64_FILE_IO_ERROR; */
      /*     } */
      /*     break; */
      /*   } */
      /* } */
      if(len>1)
        out_str.append((const char*)out, len-1 );
    }
  }
  return( retcode );
}

