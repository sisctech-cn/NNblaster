/**
 ******************************************************************************
 * file    ai_base_float.c
 * author  sunlingge
 * version V100
 * brief   the ai base float driver.
 * description
 *	1. double: sign(63 bit), exponent(52-62 bit), fraction(00-51 bit)
 *	2. float:  sign(31 bit), exponent(23-30 bit), fraction(00-22 bit)
 *  3. fp16:   sign(15 bit), exponent(10-14 bit), fraction(00-09 bit)
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_BASE_FLOAT_FP16 == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16BuiltinClz(UINT32_T x) 
{ 
	INT32_T e; 

	if (x == 0) {
		return 32; 
	}
	e=31;
	/* 1111 1111 1111 1111 0000 0000 0000 0000 */
	if ((x&0xFFFF0000) > 0) { 
		e -=16; 
		x >>=16; 
	} 
	/* 0000 0000 0000 0000 1111 1111 0000 0000 */
	if ((x&0x0000FF00) > 0) { 
		e -= 8; 
		x >>= 8; 
	} 
	/* 0000 0000 0000 0000 0000 0000 1111 0000 */
	if ((x&0x000000F0) > 0)
	{ 
		e -= 4; 
		x >>= 4; 
	} 
	/* 0000 0000 0000 0000 0000 0000 0000 1100 */
	if ((x&0x0000000C) > 0)
	{ 
		e -= 2; 
		x >>= 2; 
	} 
	/* 0000 0000 0000 0000 0000 0000 0000 0010 */
	if ((x&0x00000002) > 0)
	{ 
		e -= 1; 
	} 
	return e; 
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16ClzU16(UINT16_T a)
{
    INT32_T r;
	UINT32_T b=a;

    if (b == 0) {
        r = 16;
    } else {
        r = AiBaseFloat16BuiltinClz(b) - 16;
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16ClzU32(UINT32_T a)
{
    INT32_T r;

    if (a == 0) {
        r = 32;
    } else {
        r = AiBaseFloat16BuiltinClz(a);
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16ClzU64(UINT64_T a)
{
    INT32_T r;

    if (a == 0) {
        r = 64;
    } else {
		if ((a >> 32) > 0) {
			r = AiBaseFloat16BuiltinClz((UINT32_T)(a >> 32));
		} else {
			r = 32 + AiBaseFloat16BuiltinClz((UINT32_T)(a & 0x00000000FFFFFFFF));
		}
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16BuiltinCtz(UINT32_T x) 
{ 
	INT32_T e; 

	if (x == 0) {
		return 32; 
	}
	e=31;
	/* 1111 1111 1111 1111 0000 0000 0000 0000 */
	if ((x&0x0000FFFF) > 0) { 
		e -=16; 
		x <<=16; 
	} 
	/* 0000 0000 0000 0000 1111 1111 0000 0000 */
	if ((x&0x00FF0000) > 0) { 
		e -= 8; 
		x <<= 8; 
	} 
	/* 0000 0000 0000 0000 0000 0000 1111 0000 */
	if ((x&0x000F0000) > 0)
	{ 
		e -= 4; 
		x <<= 4; 
	} 
	/* 0000 0000 0000 0000 0000 0000 0000 1100 */
	if ((x&0x000C0000) > 0)
	{ 
		e -= 2; 
		x <<= 2; 
	} 
	/* 0000 0000 0000 0000 0000 0000 0000 0010 */
	if ((x&0x00020000) > 0)
	{ 
		e -= 1; 
	} 
	return e; 
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16CtzU16(UINT16_T a)
{
    INT32_T r;
	UINT32_T b=a;

    if (b == 0) {
        r = 16;
    } else {
        r = AiBaseFloat16BuiltinCtz(b) - 16;
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16CtzU32(UINT32_T a)
{
    INT32_T r;

    if (a == 0) {
        r = 32;
    } else {
        r = AiBaseFloat16BuiltinCtz(a);
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16CtzU64(UINT64_T a)
{
    INT32_T r;

    if (a == 0) {
        r = 64;
    } else {
		if ((a & 0x00000000FFFFFFFF) > 0) {
			r = AiBaseFloat16BuiltinCtz((UINT32_T)(a & 0x00000000FFFFFFFF));
		} else {
			r = 32 + AiBaseFloat16BuiltinCtz((UINT32_T)(a >> 32));
		}
    }
    return r;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16Pstrcpy(INT8_T *buf, INT32_T buf_size, const INT8_T *str)
{
    INT32_T c;
    INT8_T *q = buf;

    if (buf_size <= 0)
        return;

    for(;;) {
        c = *str++;
        if (c == 0 || q >= buf + buf_size - 1)
            break;
        *q++ = c;
    }
    *q = '\0';
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* strcat and truncate. */
INT8_T *AiBaseFloat16Pstrcat(INT8_T *buf, INT32_T buf_size, const INT8_T *s)
{
    INT32_T len;
    len = (INT32_T)strlen(buf);
    if (len < buf_size)
        AiBaseFloat16Pstrcpy(buf + len, buf_size - len, s);
    return buf;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16Strstart(const INT8_T *str, const INT8_T *val, const INT8_T **ptr)
{
    const INT8_T *p, *q;
    p = str;
    q = val;
    while (*q != '\0') {
        if (*p != *q)
            return 0;
        p++;
        q++;
    }
    if (ptr)
        *ptr = p;
    return 1;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16DbufInit2(AI_BASE_FLOAT16_DYN_BUF *s, void *opaque, AI_BASE_FLOAT16_DYN_BUF_REALLOC_FUNC *realloc_func)
{
    memset(s, 0, sizeof(*s));
    s->opaque = opaque;
    s->realloc_func = realloc_func;
}

#if 0
/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
static void *AiBaseFloat16DbufDefaultRealloc(void *opaque, void *ptr, UINT32_T size)
{
    return realloc(ptr, size);
}
#endif

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16DbufInit(AI_BASE_FLOAT16_DYN_BUF *s)
{
    AiBaseFloat16DbufInit2(s, NULL, NULL);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
/* return < 0 if error */
INT32_T AiBaseFloat16DbufRealloc(AI_BASE_FLOAT16_DYN_BUF *s, UINT32_T new_size)
{
    UINT32_T size;
    UINT8_T *new_buf;

    if (new_size > s->allocated_size) {
        if (s->error)
            return -1;
        size = s->allocated_size * 3 / 2;
        if (size > new_size)
            new_size = size;

		if (new_size > 16) {
			AiBaseLogErrorCritial();
		}
        new_buf = s->buffer_s;
        if (!new_buf) {
            s->error = AI_BASE_FLOAT16_TRUE;
            return -1;
        }
        s->buf = new_buf;
        s->allocated_size = new_size;
    }
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16DbufWrite(AI_BASE_FLOAT16_DYN_BUF *s, UINT32_T offset, const UINT8_T *data, UINT32_T len)
{
    UINT32_T end;
    end = offset + len;
    if (AiBaseFloat16DbufRealloc(s, end))
        return -1;
    memcpy(s->buf + offset, data, len);
    if (end > s->size)
        s->size = end;
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16DbufPut(AI_BASE_FLOAT16_DYN_BUF *s, const UINT8_T *data, UINT32_T len)
{
    if (((s->size + len) > s->allocated_size)) {
        if (AiBaseFloat16DbufRealloc(s, s->size + len))
            return -1;
    }
    memcpy(s->buf + s->size, data, len);
    s->size += len;
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16DbufPutc(AI_BASE_FLOAT16_DYN_BUF *s, UINT8_T c)
{
    return AiBaseFloat16DbufPut(s, &c, 1);
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16DbufPutstr(AI_BASE_FLOAT16_DYN_BUF *s, const INT8_T *str)
{
    return AiBaseFloat16DbufPut(s, (const UINT8_T *)str, (UINT32_T)strlen(str));
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
INT32_T AiBaseFloat16DbufPrintf(AI_BASE_FLOAT16_DYN_BUF *s, const INT8_T *fmt, ...)
{
#if 0
    va_list ap;
    INT8_T buf[128];
    INT32_T len;
    
    va_start(ap, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (len < sizeof(buf)) {
        /* fast case */
        return AiBaseFloat16DbufPut(s, (UINT8_T *)buf, len);
    } else {
        if (AiBaseFloat16DbufRealloc(s, s->size + len + 1))
            return -1;
        va_start(ap, fmt);
        vsnprintf((INT8_T *)(s->buf + s->size), s->allocated_size - s->size,
                  fmt, ap);
        va_end(ap);
        s->size += len;
    }
#endif
    return 0;
}

/**
 * brief  	none.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
void AiBaseFloat16DbufFree(AI_BASE_FLOAT16_DYN_BUF *s)
{
    /* we test s->buf as a fail safe to avoid crashing if AiBaseFloat16DbufFree()
       is called twice */
    if (s->buf) {
        s->realloc_func(s->opaque, s->buf, 0);
    }
    memset(s, 0, sizeof(*s));
}



/*------------------------- End ---------------------------------------------*/
#endif

