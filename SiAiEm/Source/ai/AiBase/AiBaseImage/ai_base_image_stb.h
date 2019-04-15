/**
 ******************************************************************************
 * file    ai_base_image_stb.h
 * author  sunlingge
 * version V100
 * brief   the ai base image set top box driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_STB_H
#define AI_BASE_IMAGE_STB_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_IMAGE_STB_CONTEXT_BUFFER_LENGTH		(128)

#define AI_BASE_IMAGE_STB_ORDER_RGB					(0)
#define AI_BASE_IMAGE_STB_ORDER_BGR					(1)

#define AI_BASE_IMAGE_STB_PNG_IMAGE					(1)
#define AI_BASE_IMAGE_STB_JPEG_IMAGE				(0)
#define AI_BASE_IMAGE_STB_BMP_IMAGE					(0)
#define AI_BASE_IMAGE_STB_GIF_IMAGE					(0)
#define AI_BASE_IMAGE_STB_PSD_IMAGE					(0)
#define AI_BASE_IMAGE_STB_PIC_IMAGE					(0)
#define AI_BASE_IMAGE_STB_PNM_IMAGE					(0)
#define AI_BASE_IMAGE_STB_HDR_IMAGE					(0)
#define AI_BASE_IMAGE_STB_TGA_IMAGE					(0)

#define AI_BASE_IMAGE_STB_SCAN_LOAD					(0)
#define AI_BASE_IMAGE_STB_SCAN_TYPE					(1)
#define AI_BASE_IMAGE_STB_SCAN_HEADER				(2)

/*------------------------- Type Definition----------------------------------*/

typedef struct ai_base_image_stb_io_callbacks {
    /* fill 'data' with 'size' bytes.  return number of bytes actually read */
    INT32_T  (*read)(void *user, UINT8_T *data, INT32_T size);
    /* skip the next 'n' bytes, or 'unget' the last -n bytes if negative */
    void     (*skip)(void *user, INT32_T n);
    /* returns nonzero if we are at end of file/data  */
    int      (*eof)(void *user);
} AI_BASE_IMAGE_STB_IO_CALLBACKS, *AI_BASE_IMAGE_STB_IO_CALLBACKS_PTR;

typedef struct ai_base_image_stb_context {
    UINT32_T img_x;
    UINT32_T img_y;
    INT32_T img_n;
    INT32_T img_out_n;

    AI_BASE_IMAGE_STB_IO_CALLBACKS io;
    void *io_user_data;

    INT32_T read_from_callbacks;
    INT32_T buflen;
    UINT8_T buffer_start[AI_BASE_IMAGE_STB_CONTEXT_BUFFER_LENGTH];

    UINT8_T *img_buffer;
    UINT8_T *img_buffer_end;
    UINT8_T *img_buffer_original;
    UINT8_T *img_buffer_original_end;
} AI_BASE_IMAGE_STB_CONTEXT,  *AI_BASE_IMAGE_STB_CONTEXT_PTR;

typedef struct ai_base_image_stb_result_info {
    INT32_T bits_per_channel;
    INT32_T num_channels;
    INT32_T channel_order;
} AI_BASE_IMAGE_STB_RESULT_INFO, *AI_BASE_IMAGE_STB_RESULT_INFO_PTR;

typedef struct ai_base_stb_struct {
    UINT8_T reserved;
} AI_BASE_IMAGE_STB, *AI_BASE_IMAGE_STB_PTR;

/*------------------------- Function Definition -----------------------------*/
INT32_T AiBaseImageStbStdioRead(void *user, UINT8_T *data, INT32_T size);
void AiBaseImageStbStdioSkip(void *user, INT32_T n);
INT32_T AiBaseImageStbStdioEof(void *user);
void AiBaseImageStbRefillBuffer(AI_BASE_IMAGE_STB_CONTEXT *s);
void AiBaseImageStbRewind(AI_BASE_IMAGE_STB_CONTEXT *s);
void AiBaseImageStbSkip(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T n);
void AiBaseImageStbStartCallbacks(AI_BASE_IMAGE_STB_CONTEXT *s, AI_BASE_IMAGE_STB_IO_CALLBACKS *c, void *user);
void AiBaseImageStbStartFile(AI_BASE_IMAGE_STB_CONTEXT *s, FILE *f);
ERROR_T AiBaseImageStbGet8(AI_BASE_IMAGE_STB_CONTEXT *s, UINT8_T *content);
ERROR_T AiBaseImageStbGet16be(AI_BASE_IMAGE_STB_CONTEXT *s, UINT16_T *content);
ERROR_T AiBaseImageStbGet32be(AI_BASE_IMAGE_STB_CONTEXT *s, UINT32_T *content);
ERROR_T AiBaseImageStbGetn(AI_BASE_IMAGE_STB_CONTEXT *s, UINT8_T *buffer, INT32_T n, INT32_T *res);
ERROR_T AiBaseImageStbLoadMain(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, AI_BASE_IMAGE_STB_RESULT_INFO *ri, INT32_T bpc, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbConvertFormat(UINT8_T *data, UINT32_T data_len, INT32_T img_n, INT32_T req_comp, UINT32_T x, UINT32_T y, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseStbConvertFormat16(UINT16_T *data, UINT32_T data_len, INT32_T img_n, INT32_T req_comp, UINT32_T x, UINT32_T y, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbLoadAndPostprocess8Bit(AI_BASE_IMAGE_STB_CONTEXT *s, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbLoadFromFile(FILE *f, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len);
ERROR_T AiBaseImageStbLoad(UINT8_T *filename, INT32_T *x, INT32_T *y, INT32_T *comp, INT32_T req_comp, UINT8_T *content, UINT32_T *content_len);


#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

