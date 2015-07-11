/*
* Copyright 2008 Free Software Foundation, Inc.
*
* This software is distributed under multiple licenses; see the COPYING file in the main directory for licensing information for this specific distribuion.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#ifndef __RADIO_DEVICE_H__
#define __RADIO_DEVICE_H__

#include <string>
#include <vector>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define GSMRATE       1625e3/6

/** a 64-bit virtual timestamp for radio data */
typedef unsigned long long TIMESTAMP;

/** A class to handle a USRP rev 4, with a two RFX900 daughterboards */
class RadioDevice {

  public:
  /* Available transport bus types */
  enum TxWindowType { TX_WINDOW_USRP1, TX_WINDOW_FIXED };

  /* Radio interface types */
  enum RadioInterfaceType { NORMAL, RESAMP_64M, RESAMP_100M, RESAMP_30_72M, DIVERSITY };

  static RadioDevice *make(size_t sps, size_t chans = 1,
                           bool diversity = false, double offset = 0.0);

  /** Initialize the USRP */
  virtual int open(const std::string &args = "", bool extref = false)=0;

  virtual ~RadioDevice() { }

  /** Start the USRP */
  virtual bool start()=0;

  /** Stop the USRP */
  virtual bool stop()=0;

  /** Get the Tx window type */
  virtual enum TxWindowType getWindowType()=0;

  /** Enable thread priority */
  virtual void setPriority(float prio = 0.5) = 0;

  /**
	Read samples from the radio.
	@param buf preallocated buf to contain read result
	@param len number of samples desired
	@param overrun Set if read buffer has been overrun, e.g. data not being read fast enough
	@param timestamp The timestamp of the first samples to be read
	@param underrun Set if radio does not have data to transmit, e.g. data not being sent fast enough
	@param RSSI The received signal strength of the read result
	@return The number of samples actually read
  */
  virtual int readSamples(std::vector<short *> &bufs, int len, bool *overrun,
                          TIMESTAMP timestamp = 0xffffffff, bool *underrun = 0,
                          unsigned *RSSI = 0) = 0;
  /**
        Write samples to the radio.
        @param buf Contains the data to be written.
        @param len number of samples to write.
        @param underrun Set if radio does not have data to transmit, e.g. data not being sent fast enough
        @param timestamp The timestamp of the first sample of the data buffer.
        @param isControl Set if data is a control packet, e.g. a ping command
        @return The number of samples actually written
  */
  virtual int writeSamples(std::vector<short *> &bufs, int len, bool *underrun,
                           TIMESTAMP timestamp, bool isControl = false) = 0;

  /** Update the alignment between the read and write timestamps */
  virtual bool updateAlignment(TIMESTAMP timestamp)=0;

  /** Set the transmitter frequency */
  virtual bool setTxFreq(double wFreq, size_t chan = 0) = 0;

  /** Set the receiver frequency */
  virtual bool setRxFreq(double wFreq, size_t chan = 0) = 0;

  /** Returns the starting write Timestamp*/
  virtual TIMESTAMP initialWriteTimestamp(void)=0;

  /** Returns the starting read Timestamp*/
  virtual TIMESTAMP initialReadTimestamp(void)=0;

  /** returns the full-scale transmit amplitude **/
  virtual double fullScaleInputValue()=0;

  /** returns the full-scale receive amplitude **/
  virtual double fullScaleOutputValue()=0;

  /** sets the receive chan gain, returns the gain setting **/
  virtual double setRxGain(double dB, size_t chan = 0) = 0;

  /** gets the current receive gain **/
  virtual double getRxGain(size_t chan = 0) = 0;

  /** return maximum Rx Gain **/
  virtual double maxRxGain(void) = 0;

  /** return minimum Rx Gain **/
  virtual double minRxGain(void) = 0;

  /** sets the transmit chan gain, returns the gain setting **/
  virtual double setTxGain(double dB, size_t chan = 0) = 0;

  /** return maximum Tx Gain **/
  virtual double maxTxGain(void) = 0;

  /** return minimum Tx Gain **/
  virtual double minTxGain(void) = 0;

  /** Return internal status values */
  virtual double getTxFreq(size_t chan = 0) = 0;
  virtual double getRxFreq(size_t chan = 0) = 0;
  virtual double getSampleRate()=0;
  virtual double numberRead()=0;
  virtual double numberWritten()=0;

};

#endif
