/**
 ******************************************************************************
 * file    ai_base_image_stb_png.h
 * author  sunlingge
 * version V100
 * brief   the ai base image set top box png driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_STB_PNG_H
#define AI_BASE_IMAGE_STB_PNG_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_IMAGE_STB_PNG_TYPE(a,b,c,d)  			(((UINT32_T)(a) << 24) + ((UINT32_T)(b) << 16) + ((UINT32_T)(c) << 8) + (UINT32_T)(d))
/* (4096) is for alphabet, and 1m for 1080p, so we use the max 1080p value:1M */
#define AI_BASE_IMAGE_STB_PNG_IDATA_LENGTH_MAX			(1920*1080*3*2)
/* (3M) is for alphabet, and 8M is for 1080p, so we use the max 1080p value:8M */
#define AI_BASE_IMAGE_STB_PNG_OUT_LENGTH_MAX			(1920*1080*4)
#define AI_BASE_IMAGE_STB_PNG_FLIT_LENGTH_MAX			((1920*3+1)*1080)
#define AI_BASE_IMAGE_STB_PNG_LINE_BUFFER_LENGTH_MAX	((1920*3))
#define AI_BASE_IMAGE_STB_PNG_INT_MAX   				(0x7FFFFFFF)

#define AI_BASE_IMAGE_STB_F_NONE                        (0)
#define AI_BASE_IMAGE_STB_F_SUB                         (1)
#define AI_BASE_IMAGE_STB_F_UP                          (2)
#define AI_BASE_IMAGE_STB_F_AVG                         (3)
#define AI_BASE_IMAGE_STB_F_PAETH                       (4)
/* synthetic filters used for first scanline to avoid needing a dummy row of 0s */
#define AI_BASE_IMAGE_STB_F_AVG_FIRST                   (5)
#define AI_BASE_IMAGE_STB_F_PAETH_FIRST                 (6)

/* truncate int to byte without warnings */
#define AI_BASE_IMAGE_STB_BYTECAST(x)                   ((UINT8_T)((x) & 255))  
#define AI_BASE_IMAGE_STBIW_UCHAR(x)					((UINT8_T)((x) & 0xff))

#define AI_BASE_IMAGE_STBIW_WPNG4(o,a,b,c,d)			((o)[0]=AI_BASE_IMAGE_STBIW_UCHAR(a),(o)[1]=AI_BASE_IMAGE_STBIW_UCHAR(b),(o)[2]=AI_BASE_IMAGE_STBIW_UCHAR(c),(o)[3]=AI_BASE_IMAGE_STBIW_UCHAR(d),(o)+=4)
#define AI_BASE_IMAGE_STBIW_WP32(data,v)				AI_BASE_IMAGE_STBIW_WPNG4(data, (v)>>24,(v)>>16,(v)>>8,(v));
#define AI_BASE_IMAGE_STNIW_WPTAG(data,s)				AI_BASE_IMAGE_STBIW_WPNG4(data, s[0],s[1],s[2],s[3])

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_stb_pngchunk {
	/* length_max is used to recode the chunk max length in one picture */
	UINT32_T length_max;
    UINT32_T length;
    UINT32_T type;
} AI_BASE_IMAGE_STB_PNGCHUNK, *AI_BASE_IMAGE_STB_PNGCHUNK_PTR;

typedef struct ai_base_image_stb_png {
    AI_BASE_IMAGE_STB_CONTEXT *s;
    UINT8_T idata[AI_BASE_IMAGE_STB_PNG_IDATA_LENGTH_MAX];
    UINT32_T idata_length;
    UINT8_T expanded[AI_BASE_IMAGE_STB_ZLIB_OBUF_LENGTH_MAX];
    UINT32_T expanded_length;
    UINT8_T out[AI_BASE_IMAGE_STB_PNG_OUT_LENGTH_MAX];
    UINT32_T out_length;
    UINT8_T depth;
} AI_BASE_IMAGE_STB_PNG, *AI_BASE_IMAGE_STB_PNG_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseImageStbCheckPngHeader(AI_BASE_IMAGE_STB_CONTEXT *s);
ERROR_T AiBaseImageStbPngTest(AI_BASE_IMAGE_STB_CONTEXT *s);
ERROR_T AiBaseImageStbGetChunkHeader(AI_BASE_IMAGE_STB_CONTEXT *s, AI_BASE_IMAGE_STB_PNGCHUNK *c);
ERROR_T AiBaseImageStbMul2sizesValid(INT32_T a, INT32_T b);
ERROR_T AiBaseImageStbAddsizesValid(INT32_T a, INT32_T b);
ERROR_T AiBaseImageStbMad3sizesValid(INT32_T a, INT32_T b, INT32_T c, INT32_T add);
UINT32_T AiBaseImageStbMallocMad3(INT32_T a, INT32_T b, INT32_T c, INT32_T add);
ERROR_T AiBaseImageStbCreatePngImageRaw(AI_BASE_IMAGE_STB_PNG *a, UINT8_T *raw, UINT32_T raw_len, INT32_T out_n, UINT32_T x, UINT32_T y, INT32_T depth, INT32_T color);
ERROR_T AiBaseImageStbCreatePngImage(AI_BASE_IMAGE_STB_PNG *a, UINT8_T *image_data, UINT32_T image_data_len, INT32_T out_n, INT32_T depth, INT32_T color, INT32_T interlaced);
ERROR_T AiBaseImageStbParsePngFile(AI_BASE_IMAGE_STB_PNG *z, INT32_T scan, INT32_T req_comp);
ERROR_T AiBaseImageStbDoPng(AI_BASE_IMAGE_STB_PNG *p, INT32_T *x, INT32_T *y, INT32_T *n, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbPngLoad(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbWritePng(UINT8_T *filename, INT32_T x, INT32_T y, INT32_T comp, UINT8_T *data, INT32_T stride_bytes, UINT8_T *content, UINT32_T *content_len);



#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

