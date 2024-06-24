#pragma once
#include <assert.h>

namespace CPPWheels
{
    //ref:https://blog.csdn.net/qq_41782149/article/details/107368192
    // memcpy源码
    void* memcpy(void* dst, const void* src, size_t len)
    {
        if (NULL == dst || NULL == src) {
            return NULL;
        }

        void* ret = dst;

        if (dst <= src || (char*)dst >= (char*)src + len) {
            //没有内存重叠，从低地址开始复制
            while (len--) {
                *(char*)dst = *(char*)src;
                dst = (char*)dst + 1;
                src = (char*)src + 1;
            }
        }
        else {
            //有内存重叠，从高地址开始复制
            src = (char*)src + len - 1;
            dst = (char*)dst + len - 1;
            while (len--) {
                *(char*)dst = *(char*)src;
                dst = (char*)dst - 1;
                src = (char*)src - 1;
            }
        }
        return ret;
    }

    //自定义memcpy
    void* Memcpy(void* dst, const void* src, size_t size)
    {
        char* psrc;  //源地址
        char* pdst;  //目标地址

        if (NULL == dst || NULL == src)
        {
            return NULL;
        }
        //源地址在前，对应上述情况2，需要自后向前拷贝
        if ((src < dst) && (char*)src + size > (char*)dst)
        {
            psrc = (char*)src + size - 1;
            pdst = (char*)dst + size - 1;
            while (size--)
            {
                *pdst-- = *psrc--;
            }
        }
        else    //源地址在后，对应上述第一种情况，直接逐个拷贝*pdst++=*psrc++即可
        {
            psrc = (char*)src;
            pdst = (char*)dst;
            while (size--)
            {
                *pdst++ = *psrc++;
            }
        }
        return pdst;
    }

}