/* Copyright (c) 2001-2008 Timothy B. Terriberry
   Copyright (c) 2008-2009 Xiph.Org Foundation */
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stddef.h>
#include "entdec.h"
#include "os_support.h"
#include "arch.h"

void ec_byte_readinit(ec_byte_buffer *_b,unsigned char *_buf,ec_uint32 _bytes){
  _b->buf=_buf;
  _b->offs=_b->end_offs=0;
  _b->storage=_bytes;
}

int ec_byte_read(ec_byte_buffer *_b){
  return _b->offs<_b->storage?_b->buf[_b->offs++]:0;
}

unsigned char ec_byte_read_from_end(ec_byte_buffer *_b){
  return _b->end_offs<_b->storage?_b->buf[_b->storage-++(_b->end_offs)]:0;
}

ec_uint32 ec_dec_uint(ec_dec *_this,ec_uint32 _ft){
  unsigned ft;
  unsigned s;
  int      ftb;
  /*In order to optimize EC_ILOG(), it is undefined for the value 0.*/
  celt_assert(_ft>1);
  _ft--;
  ftb=EC_ILOG(_ft);
  if(ftb>EC_UINT_BITS){
    ec_uint32 t;
    ftb-=EC_UINT_BITS;
    ft=(unsigned)(_ft>>ftb)+1;
    s=ec_decode(_this,ft);
    ec_dec_update(_this,s,s+1,ft);
    t=s<<ftb|ec_dec_bits(_this,ftb);
    if(t<=_ft)return t;
    _this->error=1;
    return _ft;
  }
  else{
    _ft++;
    s=ec_decode(_this,(unsigned)_ft);
    ec_dec_update(_this,s,s+1,(unsigned)_ft);
    return s;
  }
}

int ec_dec_get_error(ec_dec *_this){
  return _this->error;
}
