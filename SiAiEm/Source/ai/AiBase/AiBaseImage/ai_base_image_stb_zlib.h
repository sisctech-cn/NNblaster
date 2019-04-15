/**
 ******************************************************************************
 * file    ai_base_image_std_zlib.h
 * author  sunlingge
 * version V100
 * brief   the ai base image zlib driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_STD_ZLIB_H
#define AI_BASE_IMAGE_STD_ZLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
/* fast-way is faster to check than jpeg huffman, but slow way is slower */
/* accelerate all cases in default tables */
#define AI_BASE_IMAGE_STB_ZFAST_BITS                    (9)
#define AI_BASE_IMAGE_STB_ZFAST_MASK                    ((1 << AI_BASE_IMAGE_STB_ZFAST_BITS) - 1)

#define AI_BASE_IMAGE_STB_ZLIB_OBUF_LENGTH_MAX          (1920*1080*4+1080)
#define AI_BASE_IMAGE_STBIW_ZHASH						(16384)

#define AI_BASE_IMAGE_STBIW_SBRAW(a)					((INT32_T *)(a) - 2)
#define AI_BASE_IMAGE_STBIW_SBM(a)						(AI_BASE_IMAGE_STBIW_SBRAW(a)[0])
#define AI_BASE_IMAGE_STBIW_SBN(a)						(AI_BASE_IMAGE_STBIW_SBRAW(a)[1])

#define AI_BASE_IMAGE_STBIW_SBNEEDGROW(a,n)				(((AI_BASE_IMAGE_STBIW_SBM(a))==0) || (AI_BASE_IMAGE_STBIW_SBN(a)+n) >= AI_BASE_IMAGE_STBIW_SBM(a))
#define AI_BASE_IMAGE_STBIW_SBGROW(a,n)					AiBaseImageStbiwSbgrowf((void **)&(a), (n), sizeof(*(a)))
#define AI_BASE_IMAGE_STBIW_SBMAYBEGROW(a,n)			(AI_BASE_IMAGE_STBIW_SBNEEDGROW(a,(n)) ? AI_BASE_IMAGE_STBIW_SBGROW(a,n) : 0)
#define AI_BASE_IMAGE_STBIW_SBPUSH(a, v)				(AI_BASE_IMAGE_STBIW_SBMAYBEGROW(a,1), (a)[AI_BASE_IMAGE_STBIW_SBN(a)++] = (v))
#define AI_BASE_IMAGE_STBIW_SBCOUNT(a)					((a) ? AI_BASE_IMAGE_STBIW_SBN(a) : 0)
#define AI_BASE_IMAGE_STBIW_INIT(a)						(a = (UINT8_T *)((INT32_T *)a + 2)) 

#define AI_BASE_IMAGE_STBIW_ZLIB_FLUSH()				(out = AiBaseImageStbiwZlibFlushf(out, &bitbuf, &bitcount))
#define AI_BASE_IMAGE_STBIW_ZLIB_ADD(code,codebits)     (bitbuf |= (code) << bitcount, bitcount += (codebits), AI_BASE_IMAGE_STBIW_ZLIB_FLUSH())

#define AI_BASE_IMAGE_STBIW_ZLIB_HUFFA(b,c)				AI_BASE_IMAGE_STBIW_ZLIB_ADD(AiBaseImageStbiwZlibBitrev(b,c),c)
// default huffman tables
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFF1(n)				AI_BASE_IMAGE_STBIW_ZLIB_HUFFA(0x30 + (n), 8)
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFF2(n)				AI_BASE_IMAGE_STBIW_ZLIB_HUFFA(0x190 + (n)-144, 9)
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFF3(n)				AI_BASE_IMAGE_STBIW_ZLIB_HUFFA(0 + (n)-256,7)
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFF4(n)				AI_BASE_IMAGE_STBIW_ZLIB_HUFFA(0xc0 + (n)-280,8)
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFF(n)				((n) <= 143 ? AI_BASE_IMAGE_STBIW_ZLIB_HUFF1(n) : (n) <= 255 ? AI_BASE_IMAGE_STBIW_ZLIB_HUFF2(n) : (n) <= 279 ? AI_BASE_IMAGE_STBIW_ZLIB_HUFF3(n) : AI_BASE_IMAGE_STBIW_ZLIB_HUFF4(n))
#define AI_BASE_IMAGE_STBIW_ZLIB_HUFFB(n)				((n) <= 143 ? AI_BASE_IMAGE_STBIW_ZLIB_HUFF1(n) : AI_BASE_IMAGE_STBIW_ZLIB_HUFF2(n))

#define AI_BASE_IMAGE_STBIW_ZHASH_MEMORY_LENGTH_MAX		(512)
/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_stb_zhuffman {
    UINT16_T fast[1 << AI_BASE_IMAGE_STB_ZFAST_BITS];
    UINT16_T firstcode[16];
    INT32_T maxcode[17];
    UINT16_T firstsymbol[16];
    UINT8_T  size[288];
    UINT16_T value[288];
} AI_BASE_IMAGE_STB_ZHUFFMAN, *AI_BASE_IMAGE_STB_ZHUFFMAN_PTR;

typedef struct ai_base_image_stb_zbuf {
    UINT8_T *zbuffer;
    UINT8_T *zbuffer_end;
    INT32_T num_bits;
    UINT32_T code_buffer;

    UINT8_T *zout;
    UINT8_T *zout_start;
    UINT8_T *zout_end;
    INT32_T  z_expandable;

    AI_BASE_IMAGE_STB_ZHUFFMAN z_length;
    AI_BASE_IMAGE_STB_ZHUFFMAN z_distance;

    UINT8_T p[AI_BASE_IMAGE_STB_ZLIB_OBUF_LENGTH_MAX];
} AI_BASE_IMAGE_STB_ZBUF, *AI_BASE_IMAGE_STB_ZBUF_PTR;

typedef UINT32_T STBIW_UINT32;

/*------------------------- Function Definition -----------------------------*/
UINT8_T AiBaseImageStbZget8(AI_BASE_IMAGE_STB_ZBUF *z);
ERROR_T AiBaseImageStbParseZlibHeader(AI_BASE_IMAGE_STB_ZBUF *a);
void AiBaseImageStbFillBits(AI_BASE_IMAGE_STB_ZBUF *z);
UINT32_T AiBaseImageStbZreceive(AI_BASE_IMAGE_STB_ZBUF *z, INT32_T n);
ERROR_T AiBaseImageStbZexpand(AI_BASE_IMAGE_STB_ZBUF *z, UINT8_T *zout, INT32_T n);
ERROR_T AiBaseImagesStbParseUncompressedBlock(AI_BASE_IMAGE_STB_ZBUF *a);
INT32_T AiBaseImageStbBitreverse16(INT32_T n);
INT32_T AiBaseImageStbBitReverse(INT32_T v, INT32_T bits);
ERROR_T AiBaseImageStbZbuildHuffman(AI_BASE_IMAGE_STB_ZHUFFMAN *z, UINT8_T *sizelist, INT32_T num);
INT32_T AiBaseImageStbZhuffmanDecodeSlowpath(AI_BASE_IMAGE_STB_ZBUF *a, AI_BASE_IMAGE_STB_ZHUFFMAN *z);
INT32_T AiBaseImageStbZhuffmanDecode(AI_BASE_IMAGE_STB_ZBUF *a, AI_BASE_IMAGE_STB_ZHUFFMAN *z);
ERROR_T AiBaseImageStbComputeHuffmanCodes(AI_BASE_IMAGE_STB_ZBUF *a);
ERROR_T AiBaseImageStbParseHuffmanBlock(AI_BASE_IMAGE_STB_ZBUF *a);
ERROR_T AiBaseImageStbParseZlib(AI_BASE_IMAGE_STB_ZBUF *a, INT32_T parse_header);
ERROR_T AiBaseImageStbDoZlib(AI_BASE_IMAGE_STB_ZBUF *a, UINT8_T *obuf, INT32_T olen, INT32_T exp, INT32_T parse_header);
ERROR_T AiBaseImageStbZlibDecodeMallocGuessizeHeaderflag(UINT8_T *buffer, INT32_T len, INT32_T initial_size, INT32_T *outlen, INT32_T parse_header, UINT8_T *out);
ERROR_T AiBaseImageStbZlibCompress(UINT8_T *data, INT32_T data_len, INT32_T quality, UINT8_T *content, UINT32_T *content_len);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

