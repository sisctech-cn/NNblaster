/**
 ******************************************************************************
 * file    ai_base_image_stb.c
 * author  sunlingge
 * version V100
 * brief   the ai base image set top box driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_BASE_IMAGE_STB_IO_CALLBACKS g_ai_base_image_stb_stdio_callbacks = {
    AiBaseImageStbStdioRead,
    AiBaseImageStbStdioSkip,
    AiBaseImageStbStdioEof,
};

UINT8_T g_ai_base_image_stb_depth_scale_table[9] = { 0, 0xff, 0x55, 0, 0x11, 0,0,0, 0x01 };
INT32_T g_ai_base_image_stb_vertically_flip_on_load = 0;

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbStdioRead(void *user, UINT8_T *data, INT32_T size)
{
    return (INT32_T)fread(data,1,size,(FILE*) user);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbStdioSkip(void *user, INT32_T n)
{
    fseek((FILE*)user, n, SEEK_CUR);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseImageStbStdioEof(void *user)
{
    return feof((FILE*) user);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbRefillBuffer(AI_BASE_IMAGE_STB_CONTEXT *s)
{
    INT32_T n;

    n = (s->io.read)(s->io_user_data, (UINT8_T *)s->buffer_start, s->buflen);
    if (n == 0) {
        /* at end of file, treat same as if from memory, but need to handle case */
        /* where s->img_buffer isn't pointing to safe memory, e.g. 0-byte file */
        s->read_from_callbacks = 0;
        s->img_buffer = s->buffer_start;
        s->img_buffer_end = s->buffer_start+1;
        s->img_buffer[0] = 0;
    } else {
        s->img_buffer = s->buffer_start;
        s->img_buffer_end = s->buffer_start + n;
    }

    return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbRewind(AI_BASE_IMAGE_STB_CONTEXT *s)
{
    /* conceptually rewind SHOULD rewind to the beginning of the stream,            */
    /* but we just rewind to the beginning of the initial buffer, because           */
    /* we only use it after doing 'test', which only ever looks at at most 92 bytes */
    s->img_buffer = s->img_buffer_original;
    s->img_buffer_end = s->img_buffer_original_end;

    return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbSkip(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T n)
{
    INT32_T blen;

    if (n < 0) {
        s->img_buffer = s->img_buffer_end;
        return;
    }

    if (s->io.read) {
        blen = (int) (s->img_buffer_end - s->img_buffer);
        if (blen < n) {
            s->img_buffer = s->img_buffer_end;
            (s->io.skip)(s->io_user_data, n - blen);
            return;
        }
    }

    s->img_buffer += n;
    return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbStartCallbacks(AI_BASE_IMAGE_STB_CONTEXT *s, AI_BASE_IMAGE_STB_IO_CALLBACKS *c, void *user)
{
    memcpy((char *)&(s->io), (char *)c, sizeof(AI_BASE_IMAGE_STB_IO_CALLBACKS));
    s->io_user_data = user;
    s->buflen = AI_BASE_IMAGE_STB_CONTEXT_BUFFER_LENGTH;
    s->read_from_callbacks = 1;
    s->img_buffer_original = s->buffer_start;
    AiBaseImageStbRefillBuffer(s);
    s->img_buffer_original_end = s->img_buffer_end;

    return;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseImageStbStartFile(AI_BASE_IMAGE_STB_CONTEXT *s, FILE *f)
{
    AiBaseImageStbStartCallbacks(s, &g_ai_base_image_stb_stdio_callbacks, (void *) f);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbGet8(AI_BASE_IMAGE_STB_CONTEXT *s, UINT8_T *content)
{
    if ((s->img_buffer) < (s->img_buffer_end)) {
        *content = *((UINT8_T *)(s->img_buffer));
        s->img_buffer++;
        return AI_ERROR_SUCCESS;
    }

    if (s->read_from_callbacks) {
        AiBaseImageStbRefillBuffer(s);
        *content = *((UINT8_T *)(s->img_buffer));
        s->img_buffer++;
        return AI_ERROR_SUCCESS;
    }

    *content = 0;
    return AI_ERROR_SUCCESS;
}

ERROR_T AiBaseImageStbGet16be(AI_BASE_IMAGE_STB_CONTEXT *s, UINT16_T *content)
{
    UINT8_T low;
    UINT8_T high;
    UINT16_T temp;

    if (AiBaseImageStbGet8(s, &low) != AI_ERROR_SUCCESS) {
        *content = 0;
        return AI_ERROR_SUCCESS;
    }

    if (AiBaseImageStbGet8(s, &high) != AI_ERROR_SUCCESS) {
        *content = 0;
        return AI_ERROR_SUCCESS;
    }

    temp = low;
    temp = (temp << 8) + high;
    *content = temp;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbGet32be(AI_BASE_IMAGE_STB_CONTEXT *s, UINT32_T *content)
{
    UINT16_T low;
    UINT16_T high;
    UINT32_T temp;

    if (AiBaseImageStbGet16be(s, &low) != AI_ERROR_SUCCESS) {
        *content = 0;
        return AI_ERROR_SUCCESS;
    }

    if (AiBaseImageStbGet16be(s, &high) != AI_ERROR_SUCCESS) {
        *content = 0;
        return AI_ERROR_SUCCESS;
    }

    temp = low;
    temp = (temp << 16) + high;
    *content = temp;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbGetn(AI_BASE_IMAGE_STB_CONTEXT *s, UINT8_T *buffer, INT32_T n, INT32_T *res)
{
    INT32_T blen;
    INT32_T count;

    if (s->io.read) {
        blen = (INT32_T) (s->img_buffer_end - s->img_buffer);
        if (blen < n) {
            memcpy(buffer, s->img_buffer, blen);
            count = (s->io.read)(s->io_user_data, (char*) buffer + blen, n - blen);
            *res = (count == (n-blen));
            s->img_buffer = s->img_buffer_end;
            return AI_ERROR_SUCCESS;
        }
    }

    if (s->img_buffer+n <= s->img_buffer_end) {
        memcpy(buffer, s->img_buffer, n);
        s->img_buffer += n;
        *res = 1;
        return AI_ERROR_SUCCESS;
    }

    *res = 0;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbLoadMain(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, INT32_T bpc, UINT8_T *content, UINT32_T *content_len)
{
    /* make sure it's initialized if we add new fields */
    memset((char *)ri, 0, sizeof(AI_BASE_IMAGE_STB_RESULT_INFO));
    /* default is 8 so most paths don't have to be changed */
    ri->bits_per_channel = 8;
    /* all current input & output are this, but this is here so we can add BGR order */
    ri->channel_order = AI_BASE_IMAGE_STB_ORDER_RGB;
    ri->num_channels = 0;

#if (AI_BASE_IMAGE_STB_PNG_IMAGE == 1)
    if (AiBaseImageStbPngTest(s) == AI_ERROR_SUCCESS)  {
        return AiBaseImageStbPngLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_JPEG_IMAGE == 1)
    if (AiBaseImageStbJpegTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbJpegLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_BMP_IMAGE == 1)
    if (AiBaseImageStbBmpTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbBmpLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_GIF_IMAGE == 1)
    if (AiBaseImageStbGifTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbGifLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_PSD_IMAGE == 1)
    if (AiBaseImageStbPsdTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbPsdLoad(s,x,y,comp,req_comp, ri, bpc, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_PIC_IMAGE == 1)
    if (AiBaseImageStbPicTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbPicLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_PNM_IMAGE == 1)
    if (AiBaseImageStbPnmTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbPnmLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif
#if (AI_BASE_IMAGE_STB_HDR_IMAGE == 1)
    if (AiBaseImageStbHdrTest(s) == AI_ERROR_SUCCESS) {
        AiBaseImageStbHdrLoad(s, x,y,comp,req_comp, ri, (FLOAT32_T *)content, content_len);
        return AiBaseImageStbHdrToLdr((FLOAT32_T *)content, *x, *y, req_comp ? req_comp : *comp, content);
    }
#endif
#if (AI_BASE_IMAGE_STB_TGA_IMAGE == 1)
    // test tga last because it's a crappy test!
    if (AiBaseImageStbPsdTest(s) == AI_ERROR_SUCCESS) {
        return AiBaseImageStbGifLoad(s,x,y,comp,req_comp, ri, content, content_len);
    }
#endif

    return AI_ERROR_GENERAL;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT8_T AiBaseImageStbComputeY(INT32_T r, INT32_T g, INT32_T b)
{
   return (UINT8_T) (((r*77) + (g*150) +  (29*b)) >> 8);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbConvertFormat(UINT8_T *data, UINT32_T data_len, INT32_T img_n, INT32_T req_comp, UINT32_T x, UINT32_T y, UINT8_T *content, UINT32_T *content_len)
{
    INT32_T i,j;
    UINT8_T *good;
    UINT32_T good_len;
    UINT8_T *src;
    UINT8_T *dest;

    if (req_comp == img_n) {
		if (data_len > (*content_len)) {
			*content_len = 0;
			return AI_ERROR_GENERAL;
		}
        memcpy((char *)content, (char *)data, data_len);
        *content_len = data_len;
        return AI_ERROR_SUCCESS;
    }
    if (!((req_comp >= 1) && (req_comp <= 4))) {
        *content_len = 0;
        return AI_ERROR_GENERAL;
    }

    good_len = AiBaseImageStbMallocMad3(req_comp, x, y, 0);
    if ((good_len == 0) || (good_len > (*content_len))) {
        *content_len = 0;
        /* "outofmem", "Out of memory" */
        return AI_ERROR_GENERAL;
    }

    good = content;
    for (j=0; j < (int) y; ++j) {
        src  = data + j * x * img_n   ;
        dest = good + j * x * req_comp;

        /* convert source image with img_n components to one with req_comp components;  */
        /* avoid switch per pixel, so use switch per scanline and massive macros        */
        switch ((img_n*8)+(req_comp)) {
            case ((1)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 2) { 
                    dest[0]=src[0], dest[1]=255;                                     
                } 
                break;
            case ((1)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 3) { 
                    dest[0]=dest[1]=dest[2]=src[0];                                  
                } 
                break;
            case ((1)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 4) { 
                    dest[0]=dest[1]=dest[2]=src[0], dest[3]=255;                     
                } 
                break;
            case ((2)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 1) { 
                    dest[0]=src[0];                                                  
                } 
                break;
            case ((2)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 3) { 
                    dest[0]=dest[1]=dest[2]=src[0];                                  
                } 
                break;
            case ((2)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 4) { 
                    dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                  
                } 
                break;
            case ((3)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 4) { 
                    dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=255;        
                } 
                break;
            case ((3)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 1) { 
                    dest[0]=AiBaseImageStbComputeY(src[0],src[1],src[2]);                   
                } 
                break;
            case ((3)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 2) { 
                    dest[0]=AiBaseImageStbComputeY(src[0],src[1],src[2]), dest[1] = 255;    
                } 
                break;
            case ((4)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 1) { 
                    dest[0]=AiBaseImageStbComputeY(src[0],src[1],src[2]);                   
                } 
                break;
            case ((4)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 2) { 
                    dest[0]=AiBaseImageStbComputeY(src[0],src[1],src[2]), dest[1] = src[3]; 
                } 
                break;
            case ((4)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 3) { 
                    dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                    
                } 
                break;
            default: 
                return AI_ERROR_GENERAL;
        }
    }

    *content_len = good_len;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
UINT16_T AiBaseStbComputeY16(INT32_T r, INT32_T g, INT32_T b)
{
    return (UINT16_T) (((r*77) + (g*150) +  (29*b)) >> 8);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseStbConvertFormat16(UINT16_T *data, UINT32_T data_len, INT32_T img_n, INT32_T req_comp, UINT32_T x, UINT32_T y, UINT8_T *content, UINT32_T *content_len)
{
    INT32_T i,j;
    UINT16_T *good;
    UINT32_T good_len;
    UINT16_T *src;
    UINT16_T *dest;

    if (req_comp == img_n) {
		if (((data_len)*sizeof(UINT16_T)) > (*content_len)) {
			*content_len = 0;
			return AI_ERROR_GENERAL;
		}
        memcpy((char *)content, (char *)data, (data_len)*sizeof(UINT16_T));
        *content_len = (data_len)*sizeof(UINT16_T);
        return AI_ERROR_SUCCESS;
    }
    if (!((req_comp >= 1) && (req_comp <= 4))) {
        return AI_ERROR_GENERAL;
    }

    good_len = (req_comp * x * y * sizeof(UINT16_T));
    if ((good_len == 0) || (good_len > (*content_len))) {
        *content_len = 0;
        /* "outofmem", "Out of memory" */
        return AI_ERROR_GENERAL;
    }

    good = (UINT16_T *)content;
    for (j=0; j < (int) y; ++j) {
        src  = data + j * x * img_n   ;
        dest = good + j * x * req_comp;

        #define STBI__CASE(a,b)   case STBI__COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
        // convert source image with img_n components to one with req_comp components;
        // avoid switch per pixel, so use switch per scanline and massive macros
        switch ((img_n)*8+(req_comp)) {
            case ((1)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 2) { 
                    dest[0]=src[0], dest[1]=0xffff;                                     
                } 
                break;
            case ((1)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 3) { 
                    dest[0]=dest[1]=dest[2]=src[0];                                     
                } 
                break;
            case ((1)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 1, dest += 4) { 
                    dest[0]=dest[1]=dest[2]=src[0], dest[3]=0xffff;                     
                } 
                break;
            case ((2)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 1) { 
                    dest[0]=src[0];                                                     
                } 
                break;
            case ((2)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 3) { 
                    dest[0]=dest[1]=dest[2]=src[0];                                     
                } 
                break;
            case ((2)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 2, dest += 4) { 
                    dest[0]=dest[1]=dest[2]=src[0], dest[3]=src[1];                     
                } 
                break;
            case ((3)*8+(4)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 4) { 
                    dest[0]=src[0],dest[1]=src[1],dest[2]=src[2],dest[3]=0xffff;        
                } 
                break;
            case ((3)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 1) { 
                    dest[0]=AiBaseStbComputeY16(src[0],src[1],src[2]);                   
                } 
                break;
            case ((3)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 3, dest += 2) { 
                    dest[0]=AiBaseStbComputeY16(src[0],src[1],src[2]), dest[1] = 0xffff; 
                } 
                break;
            case ((4)*8+(1)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 1) { 
                    dest[0]=AiBaseStbComputeY16(src[0],src[1],src[2]);                   
                } 
                break;
            case ((4)*8+(2)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 2) { 
                    dest[0]=AiBaseStbComputeY16(src[0],src[1],src[2]), dest[1] = src[3]; 
                } 
                break;
            case ((4)*8+(3)): 
                for(i=x-1; i >= 0; --i, src += 4, dest += 3) { 
                    dest[0]=src[0],dest[1]=src[1],dest[2]=src[2];                       
                } 
                break;
			default:
                return AI_ERROR_GENERAL;
        }
    }

    *content_len = good_len;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbConvert16To8(UINT16_T *orig, INT32_T w, INT32_T h, INT32_T channels, UINT8_T *content, UINT32_T *content_len)
{
    INT32_T i;
    INT32_T img_len = w * h * channels;
    
    for (i = 0; i < img_len; ++i) {
        /* top half of each byte is sufficient approx of 16->8 bit scaling */
        content[i] = (UINT8_T)((orig[i] >> 8) & 0xFF); 
    }
    *content_len = img_len;
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbVerticalFlip(UINT8_T *image, INT32_T w, INT32_T h, INT32_T bytes_per_pixel)
{
    INT32_T row;
    size_t bytes_per_row = (size_t)w * bytes_per_pixel;
    UINT8_T temp[2048];
    UINT8_T *bytes = (UINT8_T *)image;
    UINT8_T *row0;
    UINT8_T *row1; 
    size_t bytes_left;
    size_t bytes_copy;

    for (row = 0; row < (h>>1); row++) {
        row0 = bytes + row*bytes_per_row;
        row1 = bytes + (h - row - 1)*bytes_per_row;
        /* swap row0 with row1 */
        bytes_left = bytes_per_row;
        while (bytes_left) {
            bytes_copy = (bytes_left < sizeof(temp)) ? bytes_left : sizeof(temp);
            memcpy(temp, row0, bytes_copy);
            memcpy(row0, row1, bytes_copy);
            memcpy(row1, temp, bytes_copy);
            row0 += bytes_copy;
            row1 += bytes_copy;
            bytes_left -= bytes_copy;
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
ERROR_T AiBaseImageStbLoadAndPostprocess8Bit(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len)
{
    AI_BASE_IMAGE_STB_RESULT_INFO ri;
    INT32_T channels;
    ERROR_T result;

    result = AiBaseImageStbLoadMain(s, x, y, comp, req_comp, &ri, 8, content, content_len);
    if (result != AI_ERROR_SUCCESS) {
        return result;
    }

    if (ri.bits_per_channel != 8) {
        if (!(ri.bits_per_channel == 16)) {
            return AI_ERROR_GENERAL;
        }
        result = AiBaseImageStbConvert16To8((UINT16_T *)content, *x, *y, (req_comp == 0 ? *comp : req_comp), content, content_len);
		if (result != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
        ri.bits_per_channel = 8;
    }

    /* TODO: move stbi__convert_format to here */

    if (g_ai_base_image_stb_vertically_flip_on_load) {
        channels = req_comp ? req_comp : *comp;
        AiBaseImageStbVerticalFlip(content, *x, *y, channels * sizeof(UINT8_T));
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
ERROR_T AiBaseImageStbLoadFromFile(FILE *f, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len)
{
    AI_BASE_IMAGE_STB_CONTEXT s;
    ERROR_T result;

    AiBaseImageStbStartFile(&s,f);
    result = AiBaseImageStbLoadAndPostprocess8Bit(&s,x,y,comp,req_comp, content, content_len);
    if (result == AI_ERROR_SUCCESS) {
        // need to 'unget' all the characters in the IO buffer
        fseek(f, - (int)(s.img_buffer_end - s.img_buffer), SEEK_CUR);
    }
    return result;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageStbLoad(UINT8_T *filename, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len)
{
    FILE *f;
    ERROR_T result;

    f = fopen(filename, "rb");
    if (!f) {
        return AI_ERROR_GENERAL;
	}
    result = AiBaseImageStbLoadFromFile(f,x,y,comp,req_comp, content, content_len);
    fclose(f);
    return result;
}


/*------------------------- End ---------------------------------------------*/
