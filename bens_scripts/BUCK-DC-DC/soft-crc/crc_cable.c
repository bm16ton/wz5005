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
 * Cable X.25 CRC16
 */
#include "crcr.h"
#include "crc_cable.h"

uint32_t cable_crc16x25_lut[256];
DECLARE_ALIGNED(struct crcr_pclmulqdq_ctx cable_crc16x25_clmul, 16) = {
        0x189ae,        /**< rk1 */
        0x8e10,         /**< rk2 */
        0x189ae,        /**< rk5 */
        0x114aa,        /**< rk6 */
        0x11c581910,    /**< rk7 */
        0x10811         /**< rk8 */
};                      /**< CLMUL */

/**
 * @brief Initializes data structures for X.25 crc16 calculations.
 *
 */
void CableCrcInit(void)
{
        crcr32_init_lut(CRC16_X_25_POLYNOMIAL << 16, cable_crc16x25_lut);
}

