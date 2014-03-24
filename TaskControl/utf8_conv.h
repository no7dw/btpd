#ifndef __UTF8_CONV_H__
#define __UTF8_CONV_H__

#ifdef __cplusplus
extern "C" {
#endif

        /* convert a arbitrary filename to a utf-8 charset filename */
    extern int utf8_convert(char *src, char* dest, int dest_len);

#ifdef __cplusplus
}
#endif

#endif
