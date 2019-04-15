/**
 ******************************************************************************
 * file    ai_base_image_stb_png.c
 * author  sunlingge
 * version V100
 * brief   the ai base image set top box png driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
extern UINT8_T g_ai_base_image_stb_depth_scale_table[9];

/*------------------------- Variable Definition------------------------------*/
INT32_T g_ai_base_image_stb_unpremultiply_on_load = 0;
INT32_T g_ai_base_image_stb_de_iphone_flag = 0;

UINT8_T g_ai_base_image_first_row_filter[5] =
{
   AI_BASE_IMAGE_STB_F_NONE,
   AI_BASE_IMAGE_STB_F_SUB,
   AI_BASE_IMAGE_STB_F_NONE,
   AI_BASE_IMAGE_STB_F_AVG_FIRST,
   AI_BASE_IMAGE_STB_F_PAETH_FIRST
};

UINT8_T g_ai_base_image_stb_png_zlib[AI_BASE_IMAGE_STB_PNG_FLIT_LENGTH_MAX];
UINT8_T g_ai_base_image_stb_png_filt[AI_BASE_IMAGE_STB_PNG_FLIT_LENGTH_MAX];
UINT8_T g_ai_base_image_stb_png_line_buffer[AI_BASE_IMAGE_STB_PNG_LINE_BUFFER_LENGTH_MAX];

/* one time only deal one png */
AI_BASE_IMAGE_STB_PNG g_ai_base_image_stb_png;
/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbCheckPngHeader(AI_BASE_IMAGE_STB_CONTEXT *s)
{
    UINT8_T png_sig[8] = { 137,80,78,71,13,10,26,10 };
    INT32_T i;
    UINT8_T content;

    for (i=0; i < 8; ++i) {
        if (AiBaseImageStbGet8(s, &content) != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;
        if (content != png_sig[i])
            return AI_ERROR_GENERAL;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbPngTest(AI_BASE_IMAGE_STB_CONTEXT *s)
{
    ERROR_T result;

    result = AiBaseImageStbCheckPngHeader(s);
    AiBaseImageStbRewind(s);

    return result;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbGetChunkHeader(AI_BASE_IMAGE_STB_CONTEXT *s, AI_BASE_IMAGE_STB_PNGCHUNK *c)
{
    ERROR_T result;

    result = AiBaseImageStbGet32be(s, &(c->length));
    if (result != AI_ERROR_SUCCESS) {
        memset((char *)c, 0, sizeof(AI_BASE_IMAGE_STB_PNGCHUNK));
        return AI_ERROR_SUCCESS;
    }

	if (c->length > c->length_max) {
		c->length_max = c->length;
	}

    result = AiBaseImageStbGet32be(s, &(c->type));
    if (result != AI_ERROR_SUCCESS) {
    memset((char *)c, 0, sizeof(AI_BASE_IMAGE_STB_PNGCHUNK));
        return AI_ERROR_SUCCESS;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbMul2sizesValid(INT32_T a, INT32_T b)
{
    if ((a < 0) || (b < 0))
        return AI_ERROR_GENERAL;
    if (b == 0) {
        /* mul-by-0 is always safe */
        return AI_ERROR_SUCCESS;
    }
    /* portable way to check for no overflows in a*b */
    if (a <= AI_BASE_IMAGE_STB_PNG_INT_MAX/b)
        return AI_ERROR_SUCCESS;
    return AI_ERROR_GENERAL;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbAddsizesValid(INT32_T a, INT32_T b)
{
    if (b < 0)
        return AI_ERROR_GENERAL;
    /* now 0 <= b <= INT_MAX, hence also                        */
    /* 0 <= INT_MAX - b <= INTMAX.                              */
    /* And "a + b <= INT_MAX" (which might overflow) is the     */
    /* same as a <= INT_MAX - b (no overflow)                   */
    if ( a <= (INT_MAX - b))
        return AI_ERROR_SUCCESS;
    return AI_ERROR_GENERAL;
}

/**
 * brief  	returns 1 if "a*b*c + add" has no negative terms/factors and doesn't overflow.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbMad3sizesValid(INT32_T a, INT32_T b, INT32_T c, INT32_T add)
{
    if ((AiBaseImageStbMul2sizesValid(a, b)  == AI_ERROR_SUCCESS) &&
        (AiBaseImageStbMul2sizesValid(a*b, c) == AI_ERROR_SUCCESS) &&
        (AiBaseImageStbAddsizesValid(a*b*c, add) == AI_ERROR_SUCCESS)) {
        return AI_ERROR_SUCCESS;
    }
    return AI_ERROR_GENERAL;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbMallocMad3(INT32_T a, INT32_T b, INT32_T c, INT32_T add)
{
    if (AiBaseImageStbMad3sizesValid(a, b, c, add) != AI_ERROR_SUCCESS)
        return 0;
    return (a*b*c + add);
}

/**
 * brief  	create the png data from post-deflated data.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbPaeth(INT32_T a, INT32_T b, INT32_T c)
{
    INT32_T p = a + b - c;
    INT32_T pa = abs(p-a);
    INT32_T pb = abs(p-b);
    INT32_T pc = abs(p-c);
    if ((pa <= pb) && (pa <= pc))
        return a;
    if (pb <= pc) 
        return b;
    return c;
}

/**
 * brief  	create the png data from post-deflated data.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbCreatePngImageRaw(AI_BASE_IMAGE_STB_PNG *a, UINT8_T *raw, UINT32_T raw_len, INT32_T out_n, UINT32_T x, UINT32_T y, INT32_T depth, INT32_T color)
{
    INT32_T bytes = ((depth == 16) ? 2 : 1);
    AI_BASE_IMAGE_STB_CONTEXT *s = a->s;
    UINT32_T i,j;
    UINT32_T stride = x*out_n*bytes;
    UINT32_T img_len;
    UINT32_T img_width_bytes;
    INT32_T k;
    /* copy it into a local for later */
    INT32_T img_n = s->img_n;
    INT32_T output_bytes = out_n*bytes;
    INT32_T filter_bytes = img_n*bytes;
    INT32_T width = x;
    UINT8_T *cur;
    UINT8_T *prior;
    INT32_T filter;
    INT32_T nk;
    UINT8_T *in;
    UINT8_T scale;
    INT32_T q;
    UINT16_T *cur16;

    if (!((out_n == s->img_n) || (out_n == s->img_n+1))) {
        return AI_ERROR_GENERAL;
    }
    /* extra bytes to write off the end into */
    a->out_length = AiBaseImageStbMallocMad3(x, y, output_bytes, 0);
    if ((a->out_length) == 0) {
        /* "outofmem", "Out of memory" */
        return AI_ERROR_GENERAL;
    }

    img_width_bytes = (((img_n * x * depth) + 7) >> 3);
    img_len = (img_width_bytes + 1) * y;
    /* we used to check for exact match between raw_len and img_len on non-interlaced PNGs,     */
    /* but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),    */
    /* so just check for raw_len < img_len always.                                              */
    if (raw_len < img_len) {
        /* "not enough pixels","Corrupt PNG" */
        return AI_ERROR_GENERAL;
    }

    for (j=0; j < y; ++j) {
        cur = a->out + stride*j;
        filter = *raw++;

        if (filter > 4) {
            /* "invalid filter","Corrupt PNG" */
            return AI_ERROR_GENERAL;
        }

        if (depth < 8) {
            if (!(img_width_bytes <= x)) {
                return AI_ERROR_GENERAL;
            }
            /* store output to the rightmost img_len bytes, so we can decode in place */
            cur += x*out_n - img_width_bytes; 
            filter_bytes = 1;
            width = img_width_bytes;
        }
        /* bugfix: need to compute this after 'cur +=' computation above */
        prior = cur - stride; 

        /* if first row, use special filter that doesn't sample previous row */
        if (j == 0) { 
            filter = g_ai_base_image_first_row_filter[filter];
        }
        
        /* handle first byte explicitly */
        for (k=0; k < filter_bytes; ++k) {
            switch (filter) {
            case AI_BASE_IMAGE_STB_F_NONE:
                cur[k] = raw[k];
                break;
            case AI_BASE_IMAGE_STB_F_SUB:
                cur[k] = raw[k];
                break;
            case AI_BASE_IMAGE_STB_F_UP:
                cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + prior[k]);
                break;
            case AI_BASE_IMAGE_STB_F_AVG:
                cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + (prior[k]>>1));
                break;
            case AI_BASE_IMAGE_STB_F_PAETH:
                cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + AiBaseImageStbPaeth(0,prior[k],0));
                break;
            case AI_BASE_IMAGE_STB_F_AVG_FIRST:
                cur[k] = raw[k];
                break;
            case AI_BASE_IMAGE_STB_F_PAETH_FIRST:
                cur[k] = raw[k];
                break;
            }
        }

        if (depth == 8) {
            if (img_n != out_n) {
                /* first pixel */
                cur[img_n] = 255; 
            }
            raw += img_n;
            cur += out_n;
            prior += out_n;
        } else if (depth == 16) {
            if (img_n != out_n) {
                /* first pixel top byte */
                cur[filter_bytes]   = 255; 
                /* first pixel bottom byte */
                cur[filter_bytes+1] = 255; 
            }
            raw += filter_bytes;
            cur += output_bytes;
            prior += output_bytes;
        } else {
            raw += 1;
            cur += 1;
            prior += 1;
        }

        /* this is a little gross, so that we don't switch per-pixel or per-component */
        if (depth < 8 || img_n == out_n) {
            nk = (width - 1)*filter_bytes;
            switch (filter) {
                /* "none" filter turns into a memcpy here; make that explicit. */
                case AI_BASE_IMAGE_STB_F_NONE:
                    memcpy(cur, raw, nk);
                    break;
                case AI_BASE_IMAGE_STB_F_SUB:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + cur[k-filter_bytes]);
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_UP:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + prior[k]);
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_AVG:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + ((prior[k] + cur[k-filter_bytes])>>1));
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_PAETH:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + AiBaseImageStbPaeth(cur[k-filter_bytes],prior[k],prior[k-filter_bytes]));
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_AVG_FIRST:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + (cur[k-filter_bytes] >> 1));
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_PAETH_FIRST:
                    for (k=0; k < nk; ++k) {
                        cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + AiBaseImageStbPaeth(cur[k-filter_bytes],0,0));
                    }
                    break;
                default:
                    break;
            }
            raw += nk;
        } else {
            if (!(img_n+1 == out_n)) {
                return AI_ERROR_GENERAL;
            }
            switch (filter) {
                case AI_BASE_IMAGE_STB_F_NONE:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = raw[k];
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_SUB:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + cur[k- output_bytes]);
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_UP:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + prior[k]);
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_AVG:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + ((prior[k] + cur[k- output_bytes])>>1));
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_PAETH:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + AiBaseImageStbPaeth(cur[k- output_bytes],prior[k],prior[k- output_bytes]));
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_AVG_FIRST:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + (cur[k- output_bytes] >> 1));
                        }
                    }
                    break;
                case AI_BASE_IMAGE_STB_F_PAETH_FIRST:
                    for (i=x-1; i >= 1; --i, cur[filter_bytes]=255,raw+=filter_bytes,cur+=output_bytes,prior+=output_bytes) {
                        for (k=0; k < filter_bytes; ++k) {
                            cur[k] = AI_BASE_IMAGE_STB_BYTECAST(raw[k] + AiBaseImageStbPaeth(cur[k- output_bytes],0,0));
                        }
                    }
                    break;
            }

            /* the loop above sets the high byte of the pixels' alpha, but for      */
            /* 16 bit png files we also need the low byte set. we'll do that here.  */
            if (depth == 16) {
                cur = a->out + stride*j; // start at the beginning of the row again
                for (i=0; i < x; ++i,cur+=output_bytes) {
                    cur[filter_bytes+1] = 255;
                }
            }
        }
    }

    /* we make a separate pass to expand bits to pixels; for performance,       */
    /* this could run two scanlines behind the above code, so it won            */
    /* intefere with filtering but will still be in the cache.                  */
    if (depth < 8) {
        for (j=0; j < y; ++j) {
            cur = a->out + stride*j;
            in  = a->out + stride*j + x*out_n - img_width_bytes;
            /* unpack 1/2/4-bit into a 8-bit buffer. allows us to keep the common 8-bit path optimal at minimal cost for 1/2/4-bit */
            /* png guarante byte alignment, if width is not multiple of 8/4/2 we'll decode dummy trailing data that will be skipped in the later loop */
            /* scale grayscale values to 0..255 range */
            scale = (color == 0) ? g_ai_base_image_stb_depth_scale_table[depth] : 1; 

            /* note that the final byte might overshoot and write more data than desired.           */
            /* we can allocate enough data that this never writes out of memory, but it             */
            /* could also overwrite the next scanline. can it overwrite non-empty data              */
            /* on the next scanline? yes, consider 1-pixel-wide scanlines with 1-bit-per-pixel.     */
            /* so we need to explicitly clamp the final ones                                        */
            if (depth == 4) {
                for (k=x*img_n; k >= 2; k-=2, ++in) {
                    *cur++ = scale * ((*in >> 4)       );
                    *cur++ = scale * ((*in     ) & 0x0f);
                }
                if (k > 0) {
                    *cur++ = scale * ((*in >> 4)       );
                }
            } else if (depth == 2) {
                for (k=x*img_n; k >= 4; k-=4, ++in) {
                    *cur++ = scale * ((*in >> 6)       );
                    *cur++ = scale * ((*in >> 4) & 0x03);
                    *cur++ = scale * ((*in >> 2) & 0x03);
                    *cur++ = scale * ((*in     ) & 0x03);
                }
                if (k > 0) {
                    *cur++ = scale * ((*in >> 6)       );
                }
                if (k > 1) {
                    *cur++ = scale * ((*in >> 4) & 0x03);
                }
                if (k > 2) {
                    *cur++ = scale * ((*in >> 2) & 0x03);
                }
            } else if (depth == 1) {
                for (k=x*img_n; k >= 8; k-=8, ++in) {
                    *cur++ = scale * ((*in >> 7)       );
                    *cur++ = scale * ((*in >> 6) & 0x01);
                    *cur++ = scale * ((*in >> 5) & 0x01);
                    *cur++ = scale * ((*in >> 4) & 0x01);
                    *cur++ = scale * ((*in >> 3) & 0x01);
                    *cur++ = scale * ((*in >> 2) & 0x01);
                    *cur++ = scale * ((*in >> 1) & 0x01);
                    *cur++ = scale * ((*in     ) & 0x01);
                }
                if (k > 0) {
                    *cur++ = scale * ((*in >> 7)       );
                }
                if (k > 1) {
                    *cur++ = scale * ((*in >> 6) & 0x01);
                }
                if (k > 2) {
                    *cur++ = scale * ((*in >> 5) & 0x01);
                }
                if (k > 3) {
                    *cur++ = scale * ((*in >> 4) & 0x01);
                }
                if (k > 4) {
                    *cur++ = scale * ((*in >> 3) & 0x01);
                }
                if (k > 5) {
                    *cur++ = scale * ((*in >> 2) & 0x01);
                }
                if (k > 6) {
                    *cur++ = scale * ((*in >> 1) & 0x01);
                }
            }
            if (img_n != out_n) {
                /* insert alpha = 255   */
                cur = a->out + stride*j;
                if (img_n == 1) {
                    for (q=x-1; q >= 0; --q) {
                        cur[q*2+1] = 255;
                        cur[q*2+0] = cur[q];
                    }
                } else {
                    if (!(img_n == 3)) {
                        return AI_ERROR_GENERAL;
                    }
                    for (q=x-1; q >= 0; --q) {
                        cur[q*4+3] = 255;
                        cur[q*4+2] = cur[q*3+2];
                        cur[q*4+1] = cur[q*3+1];
                        cur[q*4+0] = cur[q*3+0];
                    }
                }
            }
        }
    } else if (depth == 16) {
        /* force the image data from big-endian to platform-native.     */
        /* this is done in a separate pass due to the decoding relying  */
        /* on the data being untouched, but could probably be done      */
        /* per-line during decode if care is taken.                     */
        cur = a->out;
        cur16 = (UINT16_T *)cur;

        for(i=0; i < x*y*out_n; ++i,cur16++,cur+=2) {
            *cur16 = (cur[0] << 8) | cur[1];
        }
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbCreatePngImage(AI_BASE_IMAGE_STB_PNG *a, UINT8_T *image_data, UINT32_T image_data_len, INT32_T out_n, INT32_T depth, INT32_T color, INT32_T interlaced)
{
    INT32_T bytes = ((depth == 16) ? 2 : 1);
    INT32_T out_bytes = out_n * bytes;
    UINT8_T *final;
    INT32_T p;
    INT32_T xorig[] = { 0,4,0,2,0,1,0 };
    INT32_T yorig[] = { 0,0,4,0,2,0,1 };
    INT32_T xspc[]  = { 8,8,4,4,2,2,1 };
    INT32_T yspc[]  = { 8,8,8,4,4,2,2 };
    INT32_T i,j,x,y;
    UINT32_T img_len;
    INT32_T out_y;
    INT32_T out_x;

    if (!interlaced) {
        return AiBaseImageStbCreatePngImageRaw(a, image_data, image_data_len, out_n, a->s->img_x, a->s->img_y, depth, color);
    }

    // de-interlacing
    final = a->out;
    a->out_length = AiBaseImageStbMallocMad3(a->s->img_x, a->s->img_y, out_bytes, 0);
    for (p=0; p < 7; ++p) {
        /* pass1_x[4] = 0, pass1_x[5] = 1, pass1_x[12] = 1 */
        x = (a->s->img_x - xorig[p] + xspc[p]-1) / xspc[p];
        y = (a->s->img_y - yorig[p] + yspc[p]-1) / yspc[p];
        if (x && y) {
            img_len = ((((a->s->img_n * x * depth) + 7) >> 3) + 1) * y;
            if (AiBaseImageStbCreatePngImageRaw(a, image_data, image_data_len, out_n, x, y, depth, color) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
            for (j=0; j < y; ++j) {
                for (i=0; i < x; ++i) {
                    out_y = j*yspc[p]+yorig[p];
                    out_x = i*xspc[p]+xorig[p];
                    memcpy(final + out_y*a->s->img_x*out_bytes + out_x*out_bytes, a->out + (j*x+i)*out_bytes, out_bytes);
                }
            }
            image_data += img_len;
            image_data_len -= img_len;
        }
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbComputeTransparency16(AI_BASE_IMAGE_STB_PNG *z, UINT16_T tc[3], INT32_T out_n)
{
    AI_BASE_IMAGE_STB_CONTEXT *s = z->s;
    UINT32_T i, pixel_count = s->img_x * s->img_y;
    UINT16_T *p = (UINT16_T*) z->out;

    /* compute color-based transparency, assuming we've      */
    /* already got 65535 as the alpha value in the output    */
    if (!((out_n == 2) || (out_n == 4))) {
        return AI_ERROR_GENERAL;
    }

    if (out_n == 2) {
        for (i = 0; i < pixel_count; ++i) {
            p[1] = ((p[0] == tc[0]) ? 0 : 65535);
            p += 2;
        }
    } else {
        for (i = 0; i < pixel_count; ++i) {
            if ((p[0] == tc[0]) && (p[1] == tc[1]) && (p[2] == tc[2])) {
                p[3] = 0;
            }
            p += 4;
        }
    }
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbComputeTransparency(AI_BASE_IMAGE_STB_PNG *z, UINT8_T tc[3], INT32_T out_n)
{
    AI_BASE_IMAGE_STB_CONTEXT *s = z->s;
    UINT32_T i, pixel_count = s->img_x * s->img_y;
    UINT8_T *p = z->out;

    /* compute color-based transparency, assuming we've  */
    /* already got 255 as the alpha value in the output  */
    if (!((out_n == 2) || (out_n == 4))) {
        return AI_ERROR_GENERAL;
    }

    if (out_n == 2) {
        for (i=0; i < pixel_count; ++i) {
            p[1] = (p[0] == tc[0] ? 0 : 255);
            p += 2;
        }
    } else {
        for (i=0; i < pixel_count; ++i) {
            if ((p[0] == tc[0]) && (p[1] == tc[1]) && (p[2] == tc[2])) {
                p[3] = 0;
            }
            p += 4;
        }
    }
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbDeIphone(AI_BASE_IMAGE_STB_PNG *z)
{
    AI_BASE_IMAGE_STB_CONTEXT *s = z->s;
    UINT32_T i, pixel_count = s->img_x * s->img_y;
    UINT8_T *p = z->out;
    UINT8_T t;
    UINT8_T a;
    UINT8_T half;

    if (s->img_out_n == 3) {  
        /* convert bgr to rgb */
        for (i=0; i < pixel_count; ++i) {
            t = p[0];
            p[0] = p[2];
            p[2] = t;
            p += 3;
        }
    } else {
        if (!(s->img_out_n == 4)) {
            return AI_ERROR_GENERAL;
        }
        if (g_ai_base_image_stb_unpremultiply_on_load) {
            /* convert bgr to rgb and unpremultiply */
            for (i=0; i < pixel_count; ++i) {
                a = p[3];
                t = p[0];
                if (a) {
                    half = a / 2;
                    p[0] = (p[2] * 255 + half) / a;
                    p[1] = (p[1] * 255 + half) / a;
                    p[2] = ( t   * 255 + half) / a;
                } else {
                    p[0] = p[2];
                    p[2] = t;
                }
                p += 4;
            }
        } else {
            /* convert bgr to rgb */
            for (i=0; i < pixel_count; ++i) {
                t = p[0];
                p[0] = p[2];
                p[2] = t;
                p += 4;
            }
        }
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	returns 1 if "a*b + add" has no negative terms/factors and doesn't overflow.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbMad2sizesValid(INT32_T a, INT32_T b, INT32_T add)
{
    if ((AiBaseImageStbMul2sizesValid(a, b) == AI_ERROR_SUCCESS) && 
        (AiBaseImageStbAddsizesValid(a*b, add) == AI_ERROR_SUCCESS)) {
        return AI_ERROR_SUCCESS;    
    }
    
    return AI_ERROR_GENERAL;
}

/**
 * brief  	mallocs with size overflow checking.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbMallocMad2(INT32_T a, INT32_T b, INT32_T add)
{
    if (AiBaseImageStbMad2sizesValid(a, b, add) != AI_ERROR_SUCCESS) {
        return 0;
    }
    
    return (UINT32_T)(a*b + add);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbExpandPngPalette(AI_BASE_IMAGE_STB_PNG *a, UINT8_T *palette, INT32_T len, INT32_T pal_img_n)
{
    UINT32_T i, pixel_count = a->s->img_x * a->s->img_y;
    UINT8_T *p, *temp_out, *orig = a->out;
    INT32_T n;

    a->out_length = AiBaseImageStbMallocMad2(pixel_count, pal_img_n, 0);
    if (a->out_length == 0) {
        /* "outofmem", "Out of memory" */
        return AI_ERROR_GENERAL;
    }

    /* between here and free(out) below, exitting would leak */
    p = a->out;
    temp_out = p;

    if (pal_img_n == 3) {
        for (i=0; i < pixel_count; ++i) {
            n = orig[i]*4;
            p[0] = palette[n  ];
            p[1] = palette[n+1];
            p[2] = palette[n+2];
            p += 3;
        }
    } else {
        for (i=0; i < pixel_count; ++i) {
            n = orig[i]*4;
            p[0] = palette[n  ];
            p[1] = palette[n+1];
            p[2] = palette[n+2];
            p[3] = palette[n+3];
            p += 4;
        }
    }
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbParsePngFile(AI_BASE_IMAGE_STB_PNG *z, INT32_T scan, INT32_T req_comp)
{
    UINT8_T palette[1024];
    UINT8_T pal_img_n=0;
    UINT8_T has_trans=0;
    UINT8_T tc[3];
    UINT16_T tc16[3];
    UINT32_T ioff=0;
    UINT32_T idata_limit=0;
    UINT32_T i;
    UINT32_T pal_len=0;
    INT32_T first=1;
    INT32_T k;
    UINT8_T interlace=0;
    UINT8_T color=0;
    INT32_T is_iphone=0;
    AI_BASE_IMAGE_STB_CONTEXT *s;
    AI_BASE_IMAGE_STB_PNGCHUNK c;
    UINT8_T comp;
    UINT8_T filter;
    UINT16_T tc16_temp;
    UINT32_T idata_limit_old;
    INT32_T res;
    UINT32_T raw_len;
    UINT32_T bpl;
	UINT32_T crc;

    s = z->s;
    z->expanded_length = 0;
    z->idata_length = 0;
    z->out_length = 0;

    if (AiBaseImageStbCheckPngHeader(s) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    if (scan == AI_BASE_IMAGE_STB_SCAN_TYPE) {
        return AI_ERROR_SUCCESS;
    }

	memset((char *)&c, 0, sizeof(AI_BASE_IMAGE_STB_PNGCHUNK));
    for (;;) {
        AiBaseImageStbGetChunkHeader(s, &c);
        switch (c.type) {
        case AI_BASE_IMAGE_STB_PNG_TYPE('C','g','B','I'):
            is_iphone = 1;
            AiBaseImageStbSkip(s, c.length);
            break;
        case AI_BASE_IMAGE_STB_PNG_TYPE('I','H','D','R'): {
            if (!first) {
                /* "multiple IHDR","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            first = 0;
            if (c.length != 13) {
                /* "bad IHDR len","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            AiBaseImageStbGet32be(s, &(s->img_x));
            if (s->img_x > (1 << 24)) {
                /* "too large","Very large image (corrupt?)" */
                return AI_ERROR_GENERAL;
            }
            AiBaseImageStbGet32be(s, &(s->img_y));
            if (s->img_y > (1 << 24)) {
                /* "too large","Very large image (corrupt?)" */
                return AI_ERROR_GENERAL;
            }
            AiBaseImageStbGet8(s, &(z->depth));
            if ((z->depth != 1) && (z->depth != 2) && (z->depth != 4) && (z->depth != 8) && (z->depth != 16)) {
                /* "1/2/4/8/16-bit only","PNG not supported: 1/2/4/8/16-bit only" */
                return AI_ERROR_GENERAL;
            }
            AiBaseImageStbGet8(s, &(color));
            if (color > 6) {
                /* "bad ctype","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if ((color == 3) && (z->depth == 16)) {
				/* "bad ctype","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			if (color == 3) {
				pal_img_n = 3;
			} else if (color & 1) {
				/* "bad ctype","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			AiBaseImageStbGet8(s, &(comp));
			if (comp) {
				/* "bad comp method","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			AiBaseImageStbGet8(s, &(filter));
			if (filter) {
				/* "bad filter method","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			AiBaseImageStbGet8(s, &(interlace));
			if (interlace > 1) {
				/* "bad interlace method","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			if ((!s->img_x) || (!s->img_y)) {
				/* "0-pixel image","Corrupt PNG" */
				return AI_ERROR_GENERAL;
			}
			if (!pal_img_n) {
				s->img_n = ((color & 2) ? 3 : 1) + ((color & 4) ? 1 : 0);
					if (((1 << 30) / s->img_x / s->img_n) < (s->img_y)) {
						/* "too large", "Image too large to decode" */
						return AI_ERROR_GENERAL;
					}
					if (scan == AI_BASE_IMAGE_STB_SCAN_HEADER) {
						return AI_ERROR_SUCCESS;
					}
				} else {
					/* if paletted, then pal_n is our final components, and */
					/* img_n is # components to decompress/filter.          */
					s->img_n = 1;
					if (((1 << 30) / s->img_x / 4) < (s->img_y)) {
						/* "too large","Corrupt PNG" */
						return AI_ERROR_GENERAL;
					}
					/* if SCAN_header, have to scan to see if we have a tRNS */
				}
				break;
			}
        case AI_BASE_IMAGE_STB_PNG_TYPE('P','L','T','E'):  {
            if (first) {
                /* "first not IHDR", "Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (c.length > 256*3) {
                /* "invalid PLTE","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            pal_len = c.length / 3;
            if (pal_len * 3 != c.length) {
                /* "invalid PLTE","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            for (i=0; i < pal_len; ++i) {
                AiBaseImageStbGet8(s, &(palette[i*4+0]));
                AiBaseImageStbGet8(s, &(palette[i*4+1]));
                AiBaseImageStbGet8(s, &(palette[i*4+2]));
                palette[i*4+3] = 255;
            }
            break;
        }
        case AI_BASE_IMAGE_STB_PNG_TYPE('t','R','N','S'): {
            if (first) {
                /* "first not IHDR", "Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (z->idata) {
                /* "tRNS after IDAT","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (pal_img_n) {
                if (scan == AI_BASE_IMAGE_STB_SCAN_HEADER) {
                    s->img_n = 4;
                    return AI_ERROR_SUCCESS;
                }
                if (pal_len == 0) {
                    /* "tRNS before PLTE","Corrupt PNG" */
                    return AI_ERROR_GENERAL;
                }
                if (c.length > pal_len) {
                    /* "bad tRNS len","Corrupt PNG" */
                    return AI_ERROR_GENERAL;
                }
                pal_img_n = 4;
                for (i=0; i < c.length; ++i) {
                    AiBaseImageStbGet8(s, &(palette[i*4+3]));
                }
            } else {
                if (!(s->img_n & 1)) {
                    /* "tRNS with alpha","Corrupt PNG" */
                    return AI_ERROR_GENERAL;
                }
                if (c.length != ((UINT32_T)s->img_n)*2) {
                    /* "bad tRNS len","Corrupt PNG" */
                    return AI_ERROR_GENERAL;
                }
                has_trans = 1;
                if (z->depth == 16) {
                    for (k = 0; k < s->img_n; ++k) {
                        /* copy the values as-is */
                        AiBaseImageStbGet16be(s, &(tc16[k]));
                    }
                } else {
                    for (k = 0; k < s->img_n; ++k) {
                        /* non 8-bit images will be larger */
                        AiBaseImageStbGet16be(s, &(tc16_temp));
                        tc[k] = ((UINT8_T)(tc16_temp & 255)) * g_ai_base_image_stb_depth_scale_table[z->depth];
                    }
                }
            }
            break;
        }
        case AI_BASE_IMAGE_STB_PNG_TYPE('I','D','A','T'): {
            if (first) {
                /* "first not IHDR", "Corrupt PNG") */
                return AI_ERROR_GENERAL;
            }
            if ((pal_img_n) && (!pal_len)) {
                /* "no PLTE","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (scan == AI_BASE_IMAGE_STB_SCAN_HEADER) {
                s->img_n = pal_img_n;
                return AI_ERROR_SUCCESS;
            }
            if ((INT32_T)(ioff + c.length) < (INT32_T)ioff) {
                return AI_ERROR_SUCCESS;
            }
            if (ioff + c.length > idata_limit) {
                idata_limit_old = idata_limit;
                if (idata_limit == 0) {
                    idata_limit = (c.length > 4096) ? c.length : 4096;
                }
                while ((ioff + c.length) > idata_limit) {
                    idata_limit *= 2;
                }
                if (idata_limit > AI_BASE_IMAGE_STB_PNG_IDATA_LENGTH_MAX) {
                    /* "outofmem", "Out of memory" */
                    return AI_ERROR_GENERAL;
                }
            }
            AiBaseImageStbGetn(s, z->idata+ioff, c.length, &res);
            if (res == 0) {
                /* "outofdata","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            ioff += c.length;
			z->idata_length += c.length;
            break;
        }
        case AI_BASE_IMAGE_STB_PNG_TYPE('I','E','N','D'): {
            if (first) {
                /* "first not IHDR", "Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (scan != AI_BASE_IMAGE_STB_SCAN_LOAD) {
                return AI_ERROR_GENERAL;
            }
            if (z->idata_length == 0) {
                /* "no IDAT","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            /* initial guess for decoded data size to avoid unnecessary reallocs */
            /* bytes per line, per component */
            bpl = (s->img_x * z->depth + 7) / 8;
            /* pixels */
            /* filter mode per row */
            raw_len = bpl * s->img_y * s->img_n + s->img_y ;
            AiBaseImageStbZlibDecodeMallocGuessizeHeaderflag((char *) z->idata, ioff, raw_len, (int *) &raw_len, !is_iphone, z->expanded);
            if (raw_len == 0) {
                /* zlib should set error */
                return AI_ERROR_GENERAL;
            }
            if (((req_comp == (s->img_n+1)) && (req_comp != 3) && (!pal_img_n)) || has_trans) {
                s->img_out_n = s->img_n+1;
            } else {
                s->img_out_n = s->img_n;
            }
            if (AiBaseImageStbCreatePngImage(z, z->expanded, raw_len, s->img_out_n, z->depth, color, interlace) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
            if (has_trans) {
                if (z->depth == 16) {
                    if (AiBaseImageStbComputeTransparency16(z, tc16, s->img_out_n) != AI_ERROR_SUCCESS) {
                        return AI_ERROR_GENERAL;
                    }
                } else {
                    if (AiBaseImageStbComputeTransparency(z, tc, s->img_out_n) != AI_ERROR_SUCCESS) {
                        return AI_ERROR_GENERAL;
                    }
                }
            }
            if ((is_iphone) && (g_ai_base_image_stb_de_iphone_flag) && (s->img_out_n > 2)) {
                if (AiBaseImageStbDeIphone(z) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
            }
            if (pal_img_n) {
                /* pal_img_n == 3 or 4 */
                /* record the actual colors we had */
                s->img_n = pal_img_n; 
                s->img_out_n = pal_img_n;
                if (req_comp >= 3) {
                    s->img_out_n = req_comp;
                }
                if (AiBaseImageStbExpandPngPalette(z, palette, pal_len, s->img_out_n) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
            } else if (has_trans) {
                /* non-paletted image with tRNS -> source image has (constant) alpha */
                ++s->img_n;
            }
            z->expanded_length = 0;
            return AI_ERROR_SUCCESS;
        }

        default:
            /* if critical, fail */
            if (first) {
                /* "first not IHDR", "Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if ((c.type & (1 << 29)) == 0) {
                /* "PNG not supported: unknown PNG chunk type" */
                return AI_ERROR_GENERAL;
            }
            AiBaseImageStbSkip(s, c.length);
            break;
        }
        /* end of PNG chunk, read and skip CRC */
        AiBaseImageStbGet32be(s, &crc);
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbDoPng(AI_BASE_IMAGE_STB_PNG *p, INT32_T *x, INT32_T *y, INT32_T *n, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, UINT8_T *content, UINT32_T *content_len)
{
    ERROR_T result = AI_ERROR_GENERAL;

    if (req_comp < 0 || req_comp > 4)
        return AI_ERROR_GENERAL;

    if (AiBaseImageStbParsePngFile(p, AI_BASE_IMAGE_STB_SCAN_LOAD, req_comp) != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

    if (p->depth < 8) {
        ri->bits_per_channel = 8;
    } else {
        ri->bits_per_channel = p->depth;
    }
    if ((req_comp) && (req_comp != p->s->img_out_n)) {
        if (ri->bits_per_channel == 8) {
            result = AiBaseImageStbConvertFormat((UINT8_T *)p->out, p->out_length, p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y, content, content_len);
        } else {
            result = AiBaseStbConvertFormat16((UINT16_T*)p->out, (p->out_length/sizeof(UINT16_T)), p->s->img_out_n, req_comp, p->s->img_x, p->s->img_y, content, content_len);
        }
        p->s->img_out_n = req_comp;
        if (result != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
    }
    *x = p->s->img_x;
    *y = p->s->img_y;
    if (n) {
        *n = p->s->img_n;
    }
    
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbPngLoad(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, UINT8_T *content, UINT32_T *content_len)
{
    AI_BASE_IMAGE_STB_PNG *p;
    ERROR_T result;

	p = &g_ai_base_image_stb_png;
    memset((char *)p, 0, sizeof(AI_BASE_IMAGE_STB_PNG));
    p->s = s;

    result = AiBaseImageStbDoPng(p, x,y,comp,req_comp, ri, content, content_len);
    return result;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT8_T AiBaseImageStbiwPaeth(INT32_T a, INT32_T b, INT32_T c)
{
   INT32_T p = a + b - c, pa = abs(p-a), pb = abs(p-b), pc = abs(p-c);

   if ((pa <= pb) && (pa <= pc)) {
	   return AI_BASE_IMAGE_STBIW_UCHAR(a);
   }
   if (pb <= pc) {
	   return AI_BASE_IMAGE_STBIW_UCHAR(b);
   }
   return AI_BASE_IMAGE_STBIW_UCHAR(c);
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbiwCrc32(UINT8_T *buffer, INT32_T len)
{
    UINT32_T crc_table[256] =
	{
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0eDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
	};

	UINT32_T crc = ~0u;
	INT32_T i;
	for (i=0; i < len; ++i)
	   crc = (crc >> 8) ^ crc_table[buffer[i] ^ (crc & 0xff)];
	return ~crc;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbiwWpcrc(UINT8_T **data, INT32_T len)
{
   UINT32_T crc = AiBaseImageStbiwCrc32(*data - len - 4, len+4);
   AI_BASE_IMAGE_STBIW_WP32(*data, crc);

   return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbWritePngToMem(UINT8_T *pixels, INT32_T stride_bytes, INT32_T x, INT32_T y, INT32_T n, UINT8_T *out, UINT32_T *out_len)
{
	INT32_T ctype[5] = { -1, 0, 4, 2, 6 };
	UINT8_T sig[8] = { 137,80,78,71,13,10,26,10 };
	UINT8_T *o;
	UINT8_T *zlib = g_ai_base_image_stb_png_zlib;
	UINT8_T *filt = g_ai_base_image_stb_png_filt;
	UINT8_T *line_buffer = g_ai_base_image_stb_png_line_buffer;
	INT32_T i,j,k,p;
	UINT32_T zlen;
	INT32_T mapping[] = { 0,1,2,3,4 };
	INT32_T firstmap[] = { 0,1,0,5,6 };
	INT32_T *mymap;
	INT32_T best, bestval;
	INT32_T type, est;
	UINT8_T *z;
	ERROR_T result;

	if (stride_bytes == 0) {
		stride_bytes = x * n;
	}

	for (j=0; j < y; ++j) {
		mymap = (j != 0) ? mapping : firstmap;
		best = 0;
		bestval = 0x7fffffff;
		for (p=0; p < 2; ++p) {
			for (k= (p?best:0); k < 5; ++k) { 
				/* TODO: clarity: rewrite this to go 0..5, and 'continue' the unwanted ones during 2nd pass */
				type = mymap[k];
				est=0;
				z = pixels + stride_bytes*j;
				for (i=0; i < n; ++i) {
					switch (type) {
						case 0: 
							line_buffer[i] = z[i]; 
							break;
						case 1: 
							line_buffer[i] = z[i]; 
							break;
						case 2: 
							line_buffer[i] = z[i] - z[i-stride_bytes]; 
							break;
						case 3: 
							line_buffer[i] = z[i] - (z[i-stride_bytes]>>1); 
							break;
						case 4: 
							line_buffer[i] = (UINT8_T) (z[i] - AiBaseImageStbiwPaeth(0,z[i-stride_bytes],0)); 
							break;
						case 5: 
							line_buffer[i] = z[i]; 
							break;
						case 6: 
							line_buffer[i] = z[i]; 
							break;
					}
				}
				for (i=n; i < x*n; ++i) {
					switch (type) {
						case 0: 
							line_buffer[i] = z[i]; 
							break;
						case 1: 
							line_buffer[i] = z[i] - z[i-n]; 
							break;
						case 2: 
							line_buffer[i] = z[i] - z[i-stride_bytes]; 
							break;
						case 3: 
							line_buffer[i] = z[i] - ((z[i-n] + z[i-stride_bytes])>>1); 
							break;
						case 4: 
							line_buffer[i] = z[i] - AiBaseImageStbiwPaeth(z[i-n], z[i-stride_bytes], z[i-stride_bytes-n]); 
							break;
						case 5: 
							line_buffer[i] = z[i] - (z[i-n]>>1); 
							break;
						case 6: 
							line_buffer[i] = z[i] - AiBaseImageStbiwPaeth(z[i-n], 0,0); 
							break;
					}
				}
				if (p) { 
					break;
				}
				for (i=0; i < x*n; ++i) {
					est += abs((UINT8_T) line_buffer[i]);
				}
				if (est < bestval) { 
					bestval = est; 
					best = k; 
				}
			}
		}
		/* when we get here, best contains the filter type, and line_buffer contains the data */
		filt[j*(x*n+1)] = (UINT8_T) best;
		memcpy(filt+j*(x*n+1)+1, line_buffer, x*n);
	}
	/* increase 8 to get smaller but use more memory */
	zlen = AI_BASE_IMAGE_STB_PNG_FLIT_LENGTH_MAX;
	result = AiBaseImageStbZlibCompress(filt, y*( x*n+1), 8, zlib, &zlen); 
	if (!zlib) {
		return AI_ERROR_GENERAL;
	}

	// each tag requires 12 bytes of overhead
	if ((8 + 12+13 + 12+zlen + 12) > (*out_len)) {
		return AI_ERROR_GENERAL;
	}
	*out_len = 8 + 12+13 + 12+zlen + 12;

	o=out;
	memcpy(o,sig,8); 
	o+= 8;
	/* header length */
	AI_BASE_IMAGE_STBIW_WP32(o, 13); 
	AI_BASE_IMAGE_STNIW_WPTAG(o, "IHDR");
	AI_BASE_IMAGE_STBIW_WP32(o, x);
	AI_BASE_IMAGE_STBIW_WP32(o, y);
	*o++ = 8;
	*o++ = AI_BASE_IMAGE_STBIW_UCHAR(ctype[n]);
	*o++ = 0;
	*o++ = 0;
	*o++ = 0;
	AiBaseImageStbiwWpcrc(&o,13);

	AI_BASE_IMAGE_STBIW_WP32(o, zlen);
	AI_BASE_IMAGE_STNIW_WPTAG(o, "IDAT");
	memcpy(o, zlib, zlen);
	o += zlen;
	AiBaseImageStbiwWpcrc(&o, zlen);

	AI_BASE_IMAGE_STBIW_WP32(o,0);
	AI_BASE_IMAGE_STNIW_WPTAG(o, "IEND");
	AiBaseImageStbiwWpcrc(&o,0);

	if (!(o == out + *out_len)) {
		return AI_ERROR_GENERAL;
	}

	return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbWritePng(UINT8_T *filename, INT32_T x, INT32_T y, INT32_T comp, UINT8_T *data, INT32_T stride_bytes, UINT8_T *content, UINT32_T *content_len)
{
	FILE *f;
	UINT8_T result;

	result = AiBaseImageStbWritePngToMem((UINT8_T *) data, stride_bytes, x, y, comp, content, content_len);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	f = fopen(filename, "wb");
	if (!f) { 
		return AI_ERROR_GENERAL; 
	}

	fwrite(content, 1, *content_len, f);
	fclose(f);

	return AI_ERROR_SUCCESS;
}


/*------------------------- End ---------------------------------------------*/
