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
 * Implementation of cable X.25 CRC16
 *
 */

#ifndef __CRC_CABLE_H__
#define __CRC_CABLE_H__

/**
 * CRC polynomials
 */
#define CRC16_X_25_POLYNOMIAL 0x1021UL

extern uint32_t cable_crc16x25_lut[256];
extern struct crcr_pclmulqdq_ctx cable_crc16x25_clmul;

/**
 * @brief Initializes data structures for cable X.25 crc16 calculations.
 *
 */
extern void CableCrcInit(void);

/**
 * @brief Calculates cable X.25 CRC16 using LUT method.
 *
 * @param data pointer to data block to calculate CRC for
 * @param data_len size of data block
 *
 * @return New CRC value
 */
__forceinline
uint16_t CableCrc16CalculateLUT(const uint8_t *data, uint32_t data_len)
{
        return (uint16_t)(~crcr32_calc_lut(data, data_len, 0xffff,
                                           cable_crc16x25_lut));
}

/**
 * @brief Calculates cable X.25 CRC16 using CLMUL method.
 *
 * @param data pointer to data block to calculate CRC for
 * @param data_len size of data block
 *
 * @return New CRC value
 */
__forceinline
uint16_t CableCrc16CalculateCLMUL(const uint8_t *data, uint32_t data_len)
{
        return (uint16_t)(~crcr32_calc_pclmulqdq(data, data_len, 0xffff,
                                                 &cable_crc16x25_clmul));
}

#endif /* __CRC_CABLE_H__ */
