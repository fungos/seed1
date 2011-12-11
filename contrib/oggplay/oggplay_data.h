/*
   Copyright (C) 2003 Commonwealth Scientific and Industrial Research
   Organisation (CSIRO) Australia

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of CSIRO Australia nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * oggplay_data.h
 *
 * Shane Stephens <shane.stephens@annodex.net>
 */
#ifndef __OGGPLAY_DATA_H__
#define __OGGPLAY_DATA_H__

void
oggplay_data_initialise_list (OggPlayDecode *decode);

OggPlayErrorCode
oggplay_data_handle_theora_frame (OggPlayTheoraDecode *decode, 
                                  const yuv_buffer *buffer);

OggPlayErrorCode
oggplay_data_handle_audio_data (OggPlayDecode *decode, 
                                void *data, long samples, size_t samplesize);

OggPlayErrorCode
oggplay_data_handle_cmml_data(OggPlayDecode *decode, 
                              unsigned char *data, long size);

#ifdef HAVE_KATE
OggPlayErrorCode
oggplay_data_handle_kate_data(OggPlayKateDecode *decode,
                              const kate_event *ev);
#endif

#ifdef HAVE_TIGER
OggPlayErrorCode
oggplay_data_update_tiger(OggPlayKateDecode *decode,
                          int active, ogg_int64_t presentation_time,
                          OggPlayCallbackInfo *info);
#endif

void
oggplay_data_clean_list (OggPlayDecode *decode);

void
oggplay_data_free_list(OggPlayDataHeader *list);

void
oggplay_data_shutdown_list (OggPlayDecode *decode);
#endif
