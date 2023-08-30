/*******************************************************************************
 Copyright (c) 2017-2018, Intel Corporation

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

     * Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of Intel Corporation nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/**
 * Implementation of Ethernet CRCs
 */
#include "crcr.h"
#include "crc_ether.h"

/**
 * Local data
 */
uint32_t ether_crc32_lut[256];
struct DECLARE_ALIGNED(crcr_pclmulqdq_ctx ether_crc32_clmul, 16) = {
        0xccaa009e,     /**< rk1 */
        0x1751997d0,    /**< rk2 */
        0xccaa009e,     /**< rk5 */
        0x163cd6124,    /**< rk6 */
        0x1f7011640,    /**< rk7 */
        0x1db710641     /**< rk8 */
};

/**
 * Implementation
 */

/**
 * @brief Initializes data structures for Ethernet crc32 calculations.
 *
 */
void EtherCrcInit(void)
{
        crcr32_init_lut(ETHERNET_CRC32_POLYNOMIAL, ether_crc32_lut);
}

