/*
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//##############################################################################
//!\defgroup CRC CRC Library
//!
//! The CRC module provides support for CRC calculations. The CRC calculations
//! performed by this module are look up table driven.
//! This module only supports the CRC polynomial that is used to generate its
//! look up table. The default polynomial supported by this module it the CCITT
//! polynomial (0x1021).
//! Using lookup tables to calculate CRC's is the "Direct" Method of CRC
//! calculation. Using hardware to calculate CRC's is the "Indirect" Method. The
//! "Direct" and "Indirect" methods give different results. Through some
//! manipulation when calculating both "Direct" and "Indirect" CRC's, the
//! methods can generate the same results. Since the PIC32 has a CRC module to
//! calculate CRC's (Indirect Method), it is important to be aware how to get
//! these methods to give the same results. It is undesirable to have the CRC
//! modules to provide the manipulation necessary to produce a CRC consistent
//! result with the "Indirect" method as this would preclude building up CRC's
//! in multiple CRC caclulation. I.e. the CRC would need to be calculated in a
//! single block calculation.
//! Additional steps to get "Direct" and "Indirect" method CRC's to match:-
//! Direct method CRC calculations (look up tables):
//! The initial Seed should be run through the CRC calculation. The resulting
//! CRC value should then be used as the Seed for the CRC calculation over the
//! data. I.e. Perform a CRC calculation where 0 is the seed and the initial
//! Seed is the data. Then use the resulting value as the Seed for the
//! calculation over the actual data.
//! Indirect method CRC calculations (hardware based):
//! The Seed and data should be used to calculate a CRC as normal; however the
//! CRC should also be calculated over two additional bytes of 0 data. I.e.
//! Calculate a CRC using the Seed and data, then use the resulting CRC value as
//! the Seed for an additional CRC calculation over 16bits of 0.
//!
//! SystemConfig.h CRC driver configuration:-
//! No system configuration required.
//!
//! Driver Dependencies:-
//! Instrumentation.
//!
//!\note
//!
//!\author
//!
//!\version 0.0.0
//!
//!@{
//!\file CRC.h
//!\file CRC.c
//##############################################################################
#ifndef CRC_H
#define	CRC_H

#ifdef	__cplusplus
extern "C" {
#endif
//##############################################################################
// Include Files
//==============================================================================

//##############################################################################
// Public MACRO Definitions
//==============================================================================
//! CRC16_DEFAULT_SEED is the default seed to be used with the supported CRC
//! polynomial.
#define CRC16_DEFAULT_SEED      (0xffff)

//##############################################################################
// Public Type Definitions
//==============================================================================

//##############################################################################
// Public Function Prototypes
//==============================================================================
//==============================================================================
//! CRC_Init initialise the CRC module.
//!
//! \return Does not return any parameters.
//==============================================================================
void CRC_Init(void);

//==============================================================================
//! CalcCRC16 performs CRC calculations using look up tables to perform the CRC
//! calculation.
//!
//! \param crcSeed is the initial CRC value.
//! \param data is a pointer to the data to perform the CRC over.
//! \param len is the amount of data to perform the CRC over.
//!
//! \return Returns the calculated CRC.
//==============================================================================
uint16_t CalcCRC16( uint16_t crcSeed,
                    const void *buf,
                    uint16_t len);

//==============================================================================
//! CalcCRC16Begin initiates a CRC calculation. CalcCRC16Begin should be called
//! prior to calling CalcCRC16 when calculating a new CRC value. CalcCRC16Complete
//! should be called once the CRC calculation has been performed over all the
//! data i.e. all calls to CalcCRC16 has been made. The CalcCRC16Begin and
//! CalcCRC16Complete allows a CRC value to be produce that matches CRC's
//! produced by either in-direct or direct methods of CRC calculation.
//!
//! \param crcSeed is the initial CRC value.
//!
//! \return Returns the calculated CRC.
//==============================================================================
uint16_t CalcCRC16Begin(uint16_t crcSeed);

//==============================================================================
//! CalcCRC16Complete completes a CRC calculation. CalcCRC16Begin should be called
//! prior to calling CalcCRC16 when calculating a new CRC value. CalcCRC16Complete
//! should be called once the CRC calculation has been performed over all the
//! data i.e. all calls to CalcCRC16 has been made. The CalcCRC16Begin and
//! CalcCRC16Complete allows a CRC value to be produce that matches CRC's
//! produced by either in-direct or direct methods of CRC calculation.
//!
//! \param crcSeed is the initial CRC value.
//!
//! \return Returns the calculated CRC.
//==============================================================================
uint16_t CalcCRC16Complete(uint16_t crcSeed);


#ifdef	__cplusplus
}
#endif
#endif	/* CRC_H */
//!@}
