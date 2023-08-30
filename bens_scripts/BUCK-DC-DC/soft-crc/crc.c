/*******************************************************************************
 Copyright (c) 2009-2018, Intel Corporation

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
 * Implementation of CRC computation methods
 */
#include <cpuid.h>

#include "crc.h"
#include "crcr.h"
#include "crcext.h"
#include "crc_rnc.h"
#include "crc_wimax.h"
#include "crc_sctp.h"
#include "crc_tcpip.h"
#include "crc_ether.h"
#include "crc_cable.h"

/**
 * Global data
 *
 */

/**
 * Flag indicating availability of PCLMULQDQ instruction
 * Only valid after running CRCInit() function.
 */
int pclmulqdq_available = 0;

__m128i crc_xmm_be_le_swap128;

DECLARE_ALIGNED(const uint8_t crc_xmm_shift_tab[48], 16) = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

/**
 * Common macros
 */

/**
 * Common use local data
 */

/**
 * Common use function prototypes
 */

/**
 * ========================
 *
 * 8-bit LUT METHOD
 *
 * ========================
 */

/**
 * @brief Initializes look-up-table (LUT) for given 8 bit polynomial
 *
 * @param poly CRC polynomial
 * @param lut pointer to 256 x 8bits look-up-table to be initialized
 */
void crc8_init_lut(const uint8_t poly, uint8_t *lut)
{
        uint_fast32_t i, j;

        if (lut == NULL)
                return;

        for (j = 0; j < 256; j++) {
                uint_fast8_t crc = j;

                for (i = 0; i < 8; i++)
                        if (crc & 0x80)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;

                lut[j] = crc;
        }
}


/**
 * ========================
 *
 * 16-bit LUT METHOD
 *
 * ========================
 */

/**
 * @brief Initializes look-up-table (LUT) for given 16 bit polynomial
 *
 * @param poly CRC polynomial
 * @param lut pointer to 256 x 16bits look-up-table to be initialized
 *
 */
void crc16_init_lut(const uint16_t poly, uint16_t *lut)
{
        uint_fast32_t i;

        if (lut == NULL)
                return;

        for (i = 0; i < 256 ; i++) {
                uint16_t crc = 0;
                uint_fast32_t j;

                crc = (uint16_t)(i << 8);

                for (j = 0; j < 8; j++) {
                        if (crc & 0x8000)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                }

                lut[i] = crc;
        }
}

/**
 * ========================
 *
 * 32-bit LUT METHOD
 *
 * ========================
 */

/**
 * @brief Initializes look-up-table (LUT) for given 32 bit polynomial
 *
 * @param poly CRC polynomial
 * @param lut pointer to 256 x 32bits look-up-table to be initialized
 */
void crc32_init_lut(const uint32_t poly, uint32_t *lut)
{
        uint_fast32_t i, j;

        if (lut == NULL)
                return;

        for (i = 0; i < 256; i++) {
                uint_fast32_t crc = (i << 24);

                for (j = 0; j < 8; j++)
                        if (crc & 0x80000000L)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;

                lut[i] = crc;
        }
}

/**
 * =================================
 *
 * SLICE-BY-2 METHOD (16 bit CRC)
 *
 * =================================
 */

/**
 * @brief Initializes look up tables for slice-By-2 method.
 *
 * @param poly CRC polynomial
 * @param slice1 pointer to 256 x 16bits slice look-up-table 1 to be initialized
 * @param slice2 pointer to 256 x 16bits slice look-up-table 2 to be initialized
 *
 * @return New CRC value
 */
void crc16_init_slice2(const uint16_t poly,
                       uint16_t *slice1, uint16_t *slice2)
{
        uint_fast32_t i;

        if (slice1 == NULL || slice2 == NULL)
                return;

        for (i = 0; i < 256 ; i++) {
                uint16_t crc = (uint16_t)(i << 8);
                uint_fast32_t j;

                for (j = 0; j < 8; j++)
                        if (crc & 0x8000)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;

                slice1[i] = bswap2(crc);

                for (j = 0; j < 8; j++)
                        if (crc & 0x8000)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;

                slice2[i] = bswap2(crc);
        }
}


/**
 * =================================
 *
 * SLICE-BY-4 METHOD (32 bit CRC)
 *
 * =================================
 */

/**
 * @brief Initializes look up tables for slice-By-4 method.
 *
 * @param poly CRC polynomial
 * @param slice1 pointer to 256 x 32bits slice look-up-table 1 to be initialized
 * @param slice2 pointer to 256 x 32bits slice look-up-table 2 to be initialized
 * @param slice3 pointer to 256 x 32bits slice look-up-table 3 to be initialized
 * @param slice4 pointer to 256 x 32bits slice look-up-table 4 to be initialized
 *
 * @return New CRC value
 */
void crc32_init_slice4(const uint32_t poly,
                       uint32_t *slice1, uint32_t *slice2,
                       uint32_t *slice3, uint32_t *slice4)
{
        uint_fast32_t i, j;

        if (slice1 == NULL || slice2 == NULL ||
            slice3 == NULL || slice4 == NULL)
                return;

        for (i = 0; i < 256; i++) {
                uint_fast32_t crc = (i << 24);

                for (j = 0; j < 8; j++)
                        if (crc & 0x80000000L)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                slice1[i] = bswap4(crc);

                for (j = 0; j < 8; j++)
                        if (crc & 0x80000000L)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                slice2[i] = bswap4(crc);

                for (j = 0; j < 8; j++)
                        if (crc & 0x80000000L)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                slice3[i] = bswap4(crc);

                for (j = 0; j < 8; j++)
                        if (crc & 0x80000000L)
                                crc = (crc << 1) ^ poly;
                        else
                                crc <<= 1;
                slice4[i] = bswap4(crc);
        }
}

/**
 * ===================================================================
 *
 * CRC initialization
 *
 * ===================================================================
 */

/**
 * @brief Initializes CRC module.
 * @note It is mandatory to run it before using any of CRC API's.
 */
void CRCInit(void)
{
        static int is_initialized = 0;
        uint32_t reax = 0, rebx = 0, recx = 0, redx = 0;

        if (is_initialized)
                return;

        /**
         * Check support for SSE4.2 & PCLMULQDQ
         */
        __cpuid(1, reax, rebx, recx, redx);

        if ((recx & bit_SSE4_2) && (recx & bit_PCLMUL))
                pclmulqdq_available = 1;

        /**
         * Init BE <-> LE swap pattern for XMM registers
         */
        crc_xmm_be_le_swap128 = _mm_setr_epi32(0x0c0d0e0f, 0x08090a0b,
                                               0x04050607, 0x00010203);

        /**
         * Initialize CRC functions
         */
        FPHdrCrc7Init();
        FPHdrCrc11Init();
        FPDataCrc16Init();
        IUUPHdrCrc6Init();
        IUUPDataCrc10Init();
        LTECrcInit();
        SCTPCrc32cInit();
        WiMAXCrcInit();
        IPChecksumInit();
        EtherCrcInit();
	CableCrcInit();

        is_initialized = 1;
}
