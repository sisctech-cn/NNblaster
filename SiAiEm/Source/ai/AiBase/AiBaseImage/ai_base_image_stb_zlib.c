/**
 ******************************************************************************
 * file    ai_base_image_std_zlib.c
 * author  sunlingge
 * version V100
 * brief   the ai base image zlib driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
INT32_T g_ai_base_image_stb_zlength_base[31] = {
    3,4,5,6,7,8,9,10,11,13,
    15,17,19,23,27,31,35,43,51,59,
    67,83,99,115,131,163,195,227,258,0,0
};

INT32_T g_ai_base_image_stb_zlength_extra[31]= {
    0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0
};

INT32_T g_ai_base_image_stb_zdist_base[32] = {
    1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,
    257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577,0,0
};

INT32_T g_ai_base_image_stb_zdist_extra[32] = {
    0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13
};

UINT8_T g_ai_base_image_stb_zdefault_length[288] =
{
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
   8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, 9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,
   7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8
};
UINT8_T g_ai_base_image_stb_zdefault_distance[32] =
{
   5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5
};

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT8_T AiBaseImageStbZget8(AI_BASE_IMAGE_STB_ZBUF *z)
{
    UINT8_T temp;

    if (z->zbuffer >= z->zbuffer_end)
        return 0;

    temp = (*(z->zbuffer));
    (z->zbuffer)++;
    return temp;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbParseZlibHeader(AI_BASE_IMAGE_STB_ZBUF *a)
{
    int cmf   = AiBaseImageStbZget8(a);
    int cm    = cmf & 15;
    /* int cinfo = cmf >> 4; */
    int flg   = AiBaseImageStbZget8(a);

    if (((cmf*256+flg) % 31) != 0) {
        /* "bad zlib header","Corrupt PNG" zlib spec */
        return AI_ERROR_GENERAL;
    }
    if (flg & 32) {
        /* "no preset dict","Corrupt PNG" preset dictionary not allowed in png */
        return AI_ERROR_GENERAL;
    }
    if (cm != 8) {
        /* "bad compression","Corrupt PNG" DEFLATE required for png */
        return AI_ERROR_GENERAL;
    }

    /* window = 1 << (8 + cinfo)... but who cares, we fully buffer output */
    return AI_ERROR_SUCCESS;
}

void AiBaseImageStbFillBits(AI_BASE_IMAGE_STB_ZBUF *z)
{
    do {
        if (!(z->code_buffer < (1U << z->num_bits))) {
            return;
		}
        z->code_buffer |= (((UINT32_T)AiBaseImageStbZget8(z)) << z->num_bits);
        z->num_bits += 8;
    } while (z->num_bits <= 24);

    return;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbZreceive(AI_BASE_IMAGE_STB_ZBUF *z, INT32_T n)
{
    UINT32_T k;

    if (z->num_bits < n)
        AiBaseImageStbFillBits(z);
    k = z->code_buffer & ((1 << n) - 1);
    z->code_buffer >>= n;
    z->num_bits -= n;
    return k;
}

/**
 * brief    need to make room for n bytes.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbZexpand(AI_BASE_IMAGE_STB_ZBUF *z, UINT8_T *zout, INT32_T n)
{
    INT32_T cur, limit, old_limit;

    z->zout = zout;
    if (!z->z_expandable) {
        /* "output buffer limit","Corrupt PNG" */
        return AI_ERROR_GENERAL;
    }
    cur   = (INT32_T) (z->zout     - z->zout_start);
    limit = old_limit = (INT32_T) (z->zout_end - z->zout_start);
    while (cur + n > limit)
        limit *= 2;
    if (limit > AI_BASE_IMAGE_STB_ZLIB_OBUF_LENGTH_MAX) {
        /*output buffer limit */
        return AI_ERROR_GENERAL;
    }
    z->zout_start = z->zout_start;
    z->zout       = z->zout_start + cur;
    z->zout_end   = z->zout_start + limit;

    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImagesStbParseUncompressedBlock(AI_BASE_IMAGE_STB_ZBUF *a)
{
    UINT8_T header[4];
    INT32_T len,nlen,k;

    if (a->num_bits & 7) {
        /* discard */
        AiBaseImageStbZreceive(a, a->num_bits & 7);
    }
    /* drain the bit-packed data into header */
    k = 0;
    while (a->num_bits > 0) {
        /* suppress MSVC run-time check */
        header[k++] = (UINT8_T) (a->code_buffer & 255);
        a->code_buffer >>= 8;
        a->num_bits -= 8;
    }
    if (!(a->num_bits == 0)) {
        return AI_ERROR_GENERAL;
    }
    /* now fill header the normal way */
    while (k < 4) {
        header[k++] = AiBaseImageStbZget8(a);
    }
    len  = header[1] * 256 + header[0];
    nlen = header[3] * 256 + header[2];
    if (nlen != (len ^ 0xffff)) {
        /* "zlib corrupt","Corrupt PNG" */
        return AI_ERROR_GENERAL;
    }
    if (a->zbuffer + len > a->zbuffer_end) {
        /* "read past buffer","Corrupt PNG" */
        return AI_ERROR_GENERAL;
    }
    if (a->zout + len > a->zout_end) {
        if (AiBaseImageStbZexpand(a, a->zout, len) != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
    }
    memcpy(a->zout, a->zbuffer, len);
    a->zbuffer += len;
    a->zout += len;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbBitreverse16(INT32_T n)
{
    n = ((n & 0xAAAA) >>  1) | ((n & 0x5555) << 1);
    n = ((n & 0xCCCC) >>  2) | ((n & 0x3333) << 2);
    n = ((n & 0xF0F0) >>  4) | ((n & 0x0F0F) << 4);
    n = ((n & 0xFF00) >>  8) | ((n & 0x00FF) << 8);
    return n;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbBitReverse(INT32_T v, INT32_T bits)
{
    if (!(bits <= 16))
        return 0;
    /* to bit reverse n bits, reverse 16 and shift */
    /* e.g. 11 bits, bit reverse and shift away 5 */
    return AiBaseImageStbBitreverse16(v) >> (16-bits);
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbZbuildHuffman(AI_BASE_IMAGE_STB_ZHUFFMAN *z, UINT8_T *sizelist, INT32_T num)
{
    INT32_T i,k=0;
    INT32_T code, next_code[16], sizes[17];
    INT32_T s;
    INT32_T c;
    UINT16_T fastv;
    INT32_T j;

    // DEFLATE spec for generating codes
    memset(sizes, 0, (17*sizeof(INT32_T)));
    memset(z->fast, 0, (1 << AI_BASE_IMAGE_STB_ZFAST_BITS)*sizeof(UINT16_T));
    for (i=0; i < num; ++i) {
        ++sizes[sizelist[i]];
    }
    sizes[0] = 0;
    for (i=1; i < 16; ++i) {
        if (sizes[i] > (1 << i)) {
            /* "bad sizes", "Corrupt PNG" */
            return AI_ERROR_GENERAL;
        }
    }
    code = 0;
    for (i=1; i < 16; ++i) {
        next_code[i] = code;
        z->firstcode[i] = (UINT16_T) code;
        z->firstsymbol[i] = (UINT16_T) k;
        code = (code + sizes[i]);
        if (sizes[i]) {
            if (code-1 >= (1 << i)) {
                /* "bad codelengths","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
        }
        /* preshift for inner loop */
        z->maxcode[i] = code << (16-i);
        code <<= 1;
        k += sizes[i];
    }
    /* sentinel */
    z->maxcode[16] = 0x10000;
    for (i=0; i < num; ++i) {
        s = sizelist[i];
        if (s) {
            c = next_code[s] - z->firstcode[s] + z->firstsymbol[s];
            fastv = (UINT16_T) ((s << 9) | i);
            z->size [c] = (UINT8_T ) s;
            z->value[c] = (UINT16_T) i;
            if (s <= AI_BASE_IMAGE_STB_ZFAST_BITS) {
                j = AiBaseImageStbBitReverse(next_code[s],s);
                while (j < (1 << AI_BASE_IMAGE_STB_ZFAST_BITS)) {
                    z->fast[j] = fastv;
                    j += (1 << s);
                }
            }
            ++next_code[s];
        }
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
INT32_T AiBaseImageStbZhuffmanDecodeSlowpath(AI_BASE_IMAGE_STB_ZBUF *a, AI_BASE_IMAGE_STB_ZHUFFMAN *z)
{
    INT32_T b,s,k;

    /* not resolved by fast table, so compute it the slow way */
    /* use jpeg approach, which requires MSbits at top */
    k = AiBaseImageStbBitReverse(a->code_buffer, 16);
    for (s=AI_BASE_IMAGE_STB_ZFAST_BITS+1; ; ++s) {
        if (k < z->maxcode[s])
            break;
    }
    if (s == 16)  {
        /* invalid code! */
        return -1;
    }
    /* code size is s, so: */
    b = (k >> (16-s)) - z->firstcode[s] + z->firstsymbol[s];
    if (!(z->size[b] == s)) {
        return 0;
    }
    a->code_buffer >>= s;
    a->num_bits -= s;
    return z->value[b];
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbZhuffmanDecode(AI_BASE_IMAGE_STB_ZBUF *a, AI_BASE_IMAGE_STB_ZHUFFMAN *z)
{
    INT32_T b,s;

    if (a->num_bits < 16)
        AiBaseImageStbFillBits(a);
    b = z->fast[a->code_buffer & AI_BASE_IMAGE_STB_ZFAST_MASK];
    if (b) {
        s = b >> 9;
        a->code_buffer >>= s;
        a->num_bits -= s;
        return (b & 511);
    }
    return AiBaseImageStbZhuffmanDecodeSlowpath(a, z);
}


/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbComputeHuffmanCodes(AI_BASE_IMAGE_STB_ZBUF *a)
{
    UINT8_T length_dezigzag[19] = { 16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15 };
    AI_BASE_IMAGE_STB_ZHUFFMAN z_codelength;
    /* padding for maximum single op */
    UINT8_T lencodes[286+32+137];
    UINT8_T codelength_sizes[19];
    INT32_T i,n;
    INT32_T s;
    INT32_T c;
    INT32_T hlit  = AiBaseImageStbZreceive(a,5) + 257;
    INT32_T hdist = AiBaseImageStbZreceive(a,5) + 1;
    INT32_T hclen = AiBaseImageStbZreceive(a,4) + 4;
    INT32_T ntot  = hlit + hdist;
	UINT8_T fill;

    memset(codelength_sizes, 0, 19*sizeof(UINT8_T));
    for (i=0; i < hclen; ++i) {
        s = AiBaseImageStbZreceive(a,3);
        codelength_sizes[length_dezigzag[i]] = (UINT8_T)s;
    }
    if (AiBaseImageStbZbuildHuffman(&z_codelength, codelength_sizes, 19) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    n = 0;
    while (n < ntot) {
        c = AiBaseImageStbZhuffmanDecode(a, &z_codelength);
        if ((c < 0) || (c >= 19)) {
            /* "bad codelengths", "Corrupt PNG" */
            return AI_ERROR_GENERAL;
        }
        if (c < 16) {
            lencodes[n++] = (UINT8_T) c;
        } else {
            fill = 0;
            if (c == 16) {
                c = AiBaseImageStbZreceive(a,2)+3;
                if (n == 0) {
                    /* "bad codelengths", "Corrupt PNG" */
                    return AI_ERROR_GENERAL;
                }
                fill = lencodes[n-1];
            } else if (c == 17) {
                c = AiBaseImageStbZreceive(a,3)+3;
            } else {
                if (!(c == 18)) {
                    return AI_ERROR_GENERAL;
                }
                c = AiBaseImageStbZreceive(a,7)+11;
            }
            if (ntot - n < c) {
                /* "bad codelengths", "Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            memset(lencodes+n, fill, c);
            n += c;
        }
    }
    if (n != ntot) {
        /* "bad codelengths","Corrupt PNG" */
        return AI_ERROR_GENERAL;
    }
    if (AiBaseImageStbZbuildHuffman(&a->z_length, lencodes, hlit) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;
    if (AiBaseImageStbZbuildHuffman(&a->z_distance, lencodes+hlit, hdist) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbParseHuffmanBlock(AI_BASE_IMAGE_STB_ZBUF *a)
{
    UINT8_T *zout = a->zout;
    INT32_T z;
    UINT8_T *p;
    INT32_T len,dist;
    UINT8_T v;

    for(;;) {
        z = AiBaseImageStbZhuffmanDecode(a, &a->z_length);
        if (z < 256) {
            if (z < 0) {
                /* "bad huffman code","Corrupt PNG" error in huffman codes */
                return AI_ERROR_GENERAL;
            }
            if (zout >= a->zout_end) {
                if (AiBaseImageStbZexpand(a, zout, 1) != AI_ERROR_SUCCESS)
                    return AI_ERROR_GENERAL;
                zout = a->zout;
            }
            *zout++ = (char) z;
        } else {
            if (z == 256) {
                a->zout = zout;
                return AI_ERROR_SUCCESS;
            }
            z -= 257;
            len = g_ai_base_image_stb_zlength_base[z];
            if (g_ai_base_image_stb_zlength_extra[z])
                len += AiBaseImageStbZreceive(a, g_ai_base_image_stb_zlength_extra[z]);
            z = AiBaseImageStbZhuffmanDecode(a, &a->z_distance);
            if (z < 0) {
                /* "bad huffman code","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            dist = g_ai_base_image_stb_zdist_base[z];
            if (g_ai_base_image_stb_zdist_extra[z])
                dist += AiBaseImageStbZreceive(a, g_ai_base_image_stb_zdist_extra[z]);
            if (zout - a->zout_start < dist) {
                /* "bad dist","Corrupt PNG" */
                return AI_ERROR_GENERAL;
            }
            if (zout + len > a->zout_end) {
                if (AiBaseImageStbZexpand(a, zout, len) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
                zout = a->zout;
            }
            p = (UINT8_T *) (zout - dist);
            if (dist == 1) {
                /* run of one byte; common in images. */
                v = *p;
                if (len) {
                    do {
                        *zout++ = v;
                    } while (--len);
                }
            } else {
                if (len) {
                    do {
                        *zout++ = *p++;
                    } while (--len);
                }
            }
        }
    }
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbParseZlib(AI_BASE_IMAGE_STB_ZBUF *a, INT32_T parse_header)
{
    INT32_T final, type;

    if (parse_header) {
        if (AiBaseImageStbParseZlibHeader(a) != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
    }

    a->num_bits = 0;
    a->code_buffer = 0;
    do {
        final = AiBaseImageStbZreceive(a,1);
        type = AiBaseImageStbZreceive(a,2);
        if (type == 0) {
            if (AiBaseImagesStbParseUncompressedBlock(a) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
        } else if (type == 3) {
            return AI_ERROR_GENERAL;
        } else {
            if (type == 1) {
                /* use fixed code lengths */
                if (AiBaseImageStbZbuildHuffman(&a->z_length  , g_ai_base_image_stb_zdefault_length  , 288) != AI_ERROR_SUCCESS)
                    return AI_ERROR_GENERAL;
                if (AiBaseImageStbZbuildHuffman(&a->z_distance, g_ai_base_image_stb_zdefault_distance,  32) != AI_ERROR_SUCCESS)
                    return AI_ERROR_GENERAL;
            } else {
                if (AiBaseImageStbComputeHuffmanCodes(a) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
            }
            if (AiBaseImageStbParseHuffmanBlock(a) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
        }
    } while (!final);

    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbDoZlib(AI_BASE_IMAGE_STB_ZBUF *a, UINT8_T *obuf, INT32_T olen, INT32_T exp, INT32_T parse_header)
{
    a->zout_start = obuf;
    a->zout       = obuf;
    a->zout_end   = obuf + olen;
    a->z_expandable = exp;

    return AiBaseImageStbParseZlib(a, parse_header);
}

/**
 * brief    ai server app main.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbZlibDecodeMallocGuessizeHeaderflag(UINT8_T *buffer, INT32_T len, INT32_T initial_size, INT32_T *outlen, INT32_T parse_header, UINT8_T *out)
{
    AI_BASE_IMAGE_STB_ZBUF a;

    if (outlen == NULL) {
        return AI_ERROR_GENERAL;
    }

    if (initial_size > AI_BASE_IMAGE_STB_ZLIB_OBUF_LENGTH_MAX)
        return AI_ERROR_GENERAL;

    a.zbuffer = (UINT8_T *) buffer;
    a.zbuffer_end = (UINT8_T *)buffer + len;
    if (AiBaseImageStbDoZlib(&a, a.p, initial_size, 1, parse_header) == AI_ERROR_SUCCESS) {
        *outlen = (INT32_T) (a.zout - a.zout_start);
        memcpy(out, a.zout_start, *outlen);
        return AI_ERROR_SUCCESS;
    }

    *outlen = (INT32_T) (0);
    return AI_ERROR_GENERAL;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
void *AiBaseImageStbiwSbgrowf(void **arr, INT32_T increment, INT32_T itemsize)
{
	int m = 2*AI_BASE_IMAGE_STBIW_SBM(*arr)+increment;
	void *p = AI_BASE_IMAGE_STBIW_SBRAW(*arr);

	if (p) {
      if (AI_BASE_IMAGE_STBIW_SBM(*arr) == 0) {
		  ((INT32_T *)p)[1] = 0;
	  }
      *arr = (void *)((INT32_T *)p + 2);
      AI_BASE_IMAGE_STBIW_SBM(*arr) = m;
   }
   return *arr;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT8_T * AiBaseImageStbiwZlibFlushf(UINT8_T *data, UINT32_T *bitbuffer, INT32_T *bitcount)
{
	while (*bitcount >= 8) {
		AI_BASE_IMAGE_STBIW_SBPUSH(data, AI_BASE_IMAGE_STBIW_UCHAR(*bitbuffer));
		*bitbuffer >>= 8;
		*bitcount -= 8;
	}
	return data;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbiwZhash(UINT8_T *data)
{
	STBIW_UINT32 hash;
	
	hash = data[0] + (data[1] << 8) + (data[2] << 16);
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return (UINT32_T)hash;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
UINT32_T AiBaseImageStbiwZlibCountm(UINT8_T *a, UINT8_T *b, INT32_T limit)
{
   INT32_T i;

   for (i=0; ((i < limit) && (i < 258)); ++i) {
      if (a[i] != b[i]) 
		  break;
   }
   return i;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbiwZlibBitrev(INT32_T code, INT32_T codebits)
{
   INT32_T res=0;

   while (codebits--) {
      res = (res << 1) | (code & 1);
      code >>= 1;
   }
   return res;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbZlibCompress(UINT8_T *data, INT32_T data_len, INT32_T quality, UINT8_T *content, UINT32_T *content_len)
{
	UINT16_T lengthc[] = { 3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258, 259 };
	UINT8_T  lengtheb[]= { 0,0,0,0,0,0,0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4,  4,  5,  5,  5,  5,  0 };
	UINT16_T distc[]   = { 1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577, 32768 };
	UINT8_T  disteb[]  = { 0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13 };
	UINT32_T bitbuf=0;
	INT32_T i,j, bitcount=0;
	UINT8_T stbiw_zhash[AI_BASE_IMAGE_STBIW_ZHASH*sizeof(UINT8_T **)];
	UINT8_T stbiw_zhash_memory[AI_BASE_IMAGE_STBIW_ZHASH][2*sizeof(INT32_T)+AI_BASE_IMAGE_STBIW_ZHASH_MEMORY_LENGTH_MAX];
	UINT8_T ***hash_table = (UINT8_T ***)stbiw_zhash;
	UINT32_T pos;
	UINT32_T out_len_max;
	UINT8_T *out;
	INT32_T h;
	INT32_T best;
	UINT8_T *bestloc;
	UINT8_T **hlist;
	INT32_T n;
	INT32_T d;
	UINT32_T s1, s2;
	INT32_T blocklen;

	if (quality < 5) {
		quality = 5;
	}

	pos = 0;
	out_len_max = (*content_len);
	out = content;
	memset(content, 0, out_len_max);
	AI_BASE_IMAGE_STBIW_INIT(out);
	/* DEFLATE 32K window */
	AI_BASE_IMAGE_STBIW_SBPUSH(out, 0x78);  
	/* FLEVEL = 1 */
	AI_BASE_IMAGE_STBIW_SBPUSH(out, 0x5e); 
	/* BFINAL = 1 */  
	AI_BASE_IMAGE_STBIW_ZLIB_ADD(1,1);
	/* BTYPE = 1 -- fixed huffman */
	AI_BASE_IMAGE_STBIW_ZLIB_ADD(1,2);  

	memset((char *)stbiw_zhash_memory, 0, (AI_BASE_IMAGE_STBIW_ZHASH*(2*sizeof(INT32_T)+AI_BASE_IMAGE_STBIW_ZHASH_MEMORY_LENGTH_MAX)));
	for (i=0; i < AI_BASE_IMAGE_STBIW_ZHASH; ++i) {
		hash_table[i] = (UINT8_T **)(&(stbiw_zhash_memory[i]));
		AI_BASE_IMAGE_STBIW_INIT(((UINT8_T *)(hash_table[i])));
	}

	i=0;
	while (i < data_len-3) {
		/* hash next 3 bytes of data to be compressed */
		h = AiBaseImageStbiwZhash(data+i)&(AI_BASE_IMAGE_STBIW_ZHASH-1);
		best=3;
		bestloc = 0;
		hlist = hash_table[h];
		n = AI_BASE_IMAGE_STBIW_SBCOUNT(hlist);
		for (j=0; j < n; ++j) {
			if (hlist[j]-data > (i-32768)) { 
				/* if entry lies within window */
				d = AiBaseImageStbiwZlibCountm(hlist[j], data+i, data_len-i);
				if (d >= best) {
					best=d;
					bestloc=hlist[j];
				}
			}
		}
		/* when hash table entry is too long, delete half the entries */
		if ((hash_table[h]) && (AI_BASE_IMAGE_STBIW_SBN(hash_table[h]) == (2*quality))) {
			memcpy(hash_table[h], hash_table[h]+quality, sizeof(hash_table[h][0])*quality);
			AI_BASE_IMAGE_STBIW_SBN(hash_table[h]) = quality;
		}
		AI_BASE_IMAGE_STBIW_SBPUSH(hash_table[h],data+i);

		if (bestloc) {
			/* "lazy matching" - check match at *next* byte, and if it's better, do cur byte as literal */
			h = AiBaseImageStbiwZhash(data+i+1)&(AI_BASE_IMAGE_STBIW_ZHASH-1);
			hlist = hash_table[h];
			n = AI_BASE_IMAGE_STBIW_SBCOUNT(hlist);
			for (j=0; j < n; ++j) {
				if (hlist[j]-data > i-32767) {
					int e = AiBaseImageStbiwZlibCountm(hlist[j], data+i+1, data_len-i-1);
					if (e > best) { 
						/* if next match is better, bail on current match */
						bestloc = NULL;
						break;
					}
				}
			}
		}

		if (bestloc) {
			/* distance back */
			d = (INT32_T)(data+i - bestloc); 
			if (!(d <= 32767 && best <= 258)) {
				return AI_ERROR_GENERAL;
			}
			for (j=0; best > lengthc[j+1]-1; ++j) {
				/* nothing to do */
				;
			}
			AI_BASE_IMAGE_STBIW_ZLIB_HUFF(j+257);
			if (lengtheb[j]) AI_BASE_IMAGE_STBIW_ZLIB_ADD(best - lengthc[j], lengtheb[j]);
			for (j=0; d > distc[j+1]-1; ++j){
				/* nothing to do */
				;
			}
			AI_BASE_IMAGE_STBIW_ZLIB_ADD(AiBaseImageStbiwZlibBitrev(j,5),5);
			if (disteb[j]) AI_BASE_IMAGE_STBIW_ZLIB_ADD(d - distc[j], disteb[j]);
			i += best;
		} else {
			AI_BASE_IMAGE_STBIW_ZLIB_HUFFB(data[i]);
			++i;
		}
	}
	// write out final bytes
	for (;i < data_len; ++i) {
		AI_BASE_IMAGE_STBIW_ZLIB_HUFFB(data[i]);
	}
	/* end of block */
	AI_BASE_IMAGE_STBIW_ZLIB_HUFF(256); 
	// pad with 0 bits to byte boundary
	while (bitcount) {
		AI_BASE_IMAGE_STBIW_ZLIB_ADD(0,1);
	}

	for (i=0; i < AI_BASE_IMAGE_STBIW_ZHASH; ++i) {
		//(void) stbiw__sbfree(hash_table[i]);
	}
	//STBIW_FREE(hash_table);

	{
		/* compute adler32 on input */
		s1=1;
		s2=0;
		blocklen = (INT32_T) (data_len % 5552);
		j=0;
		while (j < data_len) {
			for (i=0; i < blocklen; ++i) {
				s1 += data[j+i];
				s2 += s1;
			}
			s1 %= 65521;
			s2 %= 65521;
			j += blocklen;
			blocklen = 5552;
		}
		AI_BASE_IMAGE_STBIW_SBPUSH(out, AI_BASE_IMAGE_STBIW_UCHAR(s2 >> 8));
		AI_BASE_IMAGE_STBIW_SBPUSH(out, AI_BASE_IMAGE_STBIW_UCHAR(s2));
		AI_BASE_IMAGE_STBIW_SBPUSH(out, AI_BASE_IMAGE_STBIW_UCHAR(s1 >> 8));
		AI_BASE_IMAGE_STBIW_SBPUSH(out, AI_BASE_IMAGE_STBIW_UCHAR(s1));
	}
	*content_len = AI_BASE_IMAGE_STBIW_SBN(out);
	/* make returned pointer freeable */
	memcpy(content, out, *content_len);

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
