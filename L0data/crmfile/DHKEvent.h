#ifndef DHKEVENT_H
#define DHKEVENT_H
//
// DHKEvent.h --- defines class DHKEvent.
//
// 2005/6/4 SYZ
//
// Copyright (C) 2003-2005 by the CREAM project.  All rights reserved.
// 
// This file is part of CDAQ, the CREAM data acquisition and processing system.
// 
// This work may not be reproduced, displayed, modified or distributed
// without the express prior written permission of the copyright holder.
// For permission, contact Prof. Eun-suk Seo at seo@umd.edu.  The above
// copyright notice must appear in all copies and in supporting documentation.
// 
// The software is provided without any warranty, express or implied, including
// but not limited to the warranties of merchantability, fitness for a
// particular purpose and noninfringement.  In no event shall the author or
// the CREAM project group be liable for any claim, damages or other liability,
// whether in an action of contract, tort or otherwise, arising from, out of
// or in connection with the software or the use or other dealings in the
// software.
//
// $Log: DHKEvent.h,v $
// Revision 1.2  2012/03/21 20:41:34  dja1987
// T/BCD updates.
//
// Revision 1.1.1.1  2011/08/31 18:53:54  dja1987
// Importing SCDL bug fixes.
//
// Revision 1.2  2011/07/20 17:22:27  dja1987
// SCDL housekeeping code added
//
// Revision 1.1.1.1  2011/06/07 19:10:21  dja1987
// Includes both TRD and SCDL updates
//
// Revision 1.1.1.1  2010/06/11 15:14:31  spaced4
// CREAM6
//
// Revision 5.1  2009/03/02 04:42:01  creamop
// Pcm Encoder and TDRSS w/o SEI
//
// Revision 5.0  2009/02/02 03:05:20  creamop
// S.L 5.2 and FC3 both.
//
// Revision 1.1.1.1  2009/02/02 03:01:14  creamop
// hahaha
//
// Revision 5.0  2009/01/24 04:33:24  creamop
// S.L 5.2 Version
//
// Revision 1.1.1.1  2009/01/24 04:30:20  creamop
// hahaha
//
// Revision 5.0  2009/01/24 03:48:22  creamop
// S.L 5.2 Version.
//
// Revision 1.1.1.1  2009/01/24 03:44:34  creamop
// hahaha
//
// Revision 4.0  2009/01/20 08:13:49  creamop
// CREAM IV flight version.
//
// Revision 1.1.1.1  2009/01/20 08:12:10  creamop
// hahaha
//
// Revision 1.10  2007/04/02 17:46:25  juneki
// Increased hk channels to 1400
//
// Revision 1.9  2007/03/13 21:13:56  juneki
// increased DHK_NCH
//
// Revision 1.8  2006/03/24 14:42:38  syzinn
// Merged a branch post_flight2_changes
//
// Revision 1.7.2.1  2006/03/14 21:07:44  syzinn
// eight-byte timestamp
//
// Revision 1.7  2005/11/24 02:39:17  syzinn
// get timestamp in type double
//
// Revision 1.6  2005/08/08 22:54:16  syzinn
// Included methods for new trigger rates
//
// Revision 1.5  2005/08/04 02:48:42  syzinn
// print text version of the housekeeping data
//
// Revision 1.4  2005/07/06 21:22:21  cvs
// fixes
//
// Revision 1.3  2005/06/07 12:22:53  cvs
// first working version for new hk system
//
// Revision 1.2  2005/06/07 00:16:17  cvs
// still under modification
//
// Revision 1.1  2005/06/06 21:31:02  cvs
// new
//
//

//#define DHK_NCH 1024
//#define DHK_NCH 1200
//#define DHK_NCH 1400
#define DHK_NCH 2700 // Should be the same size as DHK_CBUFSIZ 18 July 2011

class CreamHKFile;
class DHKEvent {
friend class CreamHKFile;
public:
	enum {
		statusGreen = 0,
		statusOrange = 1,
		statusRed = 2,

		typeReal = 0,
		typeInteger = 1,
		typeLongInteger = 2,
		typeTimestamp = 3
	};

protected:
	struct _hk_item {
		struct {
			unsigned enabled	: 1;
			unsigned filled		: 1;
			unsigned status		: 2; // statusGreen, statusOrange, statusRed
			unsigned type		: 4;
		} info;

		union {
			double r4; // hk data in physical units
			int i4;
			uint8 u8;
			struct {
				unsigned	p; // 32 LSB padding
				timestamp4_t	v; // overlaps with t8
			} t4;
			timestamp8_t t8;
		} v;

		char name[8+1];
	} _data[DHK_NCH];

	// exclusive counts of channels within each range
	int _green_count;
	int _orange_count;
	int _red_count;

	inline void SetEnabled(unsigned k)
		{ _data[k].info.enabled = 1; }
	inline void SetStatus(unsigned k, unsigned v)
		{ _data[k].info.status = v; }

	inline void SetType(unsigned k, unsigned v) { _data[k].info.type = v; _data[k].info.filled = 1; }
	inline void SetName(unsigned k, char* name) { strncpy(_data[k].name, name, 8); }
	inline void SetDouble(unsigned k, double v) { _data[k].v.r4 = v; SetType(k, typeReal); }
	inline void SetReal(unsigned k, double v) { SetDouble(k, v); }
	inline void SetInteger(unsigned k, int v) { _data[k].v.i4 = v; SetType(k, typeInteger); }
	inline void SetLongInteger(unsigned k, uint8 v) { _data[k].v.u8 = v; SetType(k, typeLongInteger); }
	inline void SetTimestamp(unsigned k, timestamp4_t* v) { _data[k].v.t4.v = *v; SetType(k, typeTimestamp); }
	inline void SetTimestamp(unsigned k, timestamp4_t v) { _data[k].v.t4.v = v; SetType(k, typeTimestamp); }
	inline void SetTimestamp(unsigned k, timestamp8_t* v) { _data[k].v.t8 = *v; SetType(k, typeTimestamp); }
	inline void SetTimestamp(unsigned k, timestamp8_t v) { _data[k].v.t8 = v; SetType(k, typeTimestamp); }

public:
	DHKEvent(void);
	virtual ~DHKEvent(void);

	int Clear();

	// Status counts
	inline int GetNumberOfGreenChannel(void) { return _green_count; }
	inline int GetNumberOfOrangeChannel(void) { return _orange_count; }
	inline int GetNumberOfRedChannel(void) { return _red_count; }

	// per channel access
	inline int GetNumberOfChannel(void) { return DHK_NCH; }
	inline int GetType(unsigned k) { return _data[k].info.type; }
	inline double GetDouble(unsigned k) { return _data[k].v.r4; }
	inline int GetInteger(unsigned k) { return _data[k].v.i4; }
	inline uint8 GetLongInteger(unsigned k) { return _data[k].v.u8; }
	inline timestamp8_t* GetTimestamp(unsigned k) { return &(_data[k].v.t8); }
	inline int GetStatus(unsigned k) { return _data[k].info.status; }
	inline char* GetName(unsigned k) { return _data[k].name; }
	
	inline int IsEnabled(unsigned k)
		{ return (_data[k].info.enabled) ? -1 : 0; }
	inline int IsFilled(unsigned k)
		{ return (_data[k].info.enabled && _data[k].info.filled) ? -1 : 0; }

	// event timestamp
	inline timestamp8_t* GetEventTimestamp(void) { return GetTimestamp(0); }
	double GetEventTimeInSecond(void);

	inline int GetEventTimestampStatus(void) { return GetStatus(0); }

	// Trigger rates
	inline timestamp8_t* GetTRGTimestamp(void) { return GetTimestamp(10); }
	double GetTRGTimeInSecond(void);
	inline real4 GetTRGTotalRate(void) { return GetDouble(11); }
	inline real4 GetTRGExternalRate(void) { return GetDouble(12); }
	inline real4 GetTRGCalibrationRate(void) { return GetDouble(13); }
	inline real4 GetTRGEHiRate(void) { return GetDouble(14); }
	inline real4 GetTRGELoRate(void) { return GetDouble(15); }
	inline real4 GetTRGZCalibrationRate(void) { return GetDouble(16); }
	inline real4 GetTRGZHiRate(void) { return GetDouble(17); }
	inline real4 GetTRGCD1Rate(void) { return GetDouble(17); }
	inline real4 GetTRGCalorimeterRate(void) { return GetDouble(18); }
	inline real4 GetTRGCD2Rate(void) { return GetDouble(19); } //YONAS

	inline int GetTRGTimestampStatus(void) { return GetStatus(10); }
	inline int GetTRGTotalRateStatus(void) { return GetStatus(11); }
	inline int GetTRGExternalRateStatus(void) { return GetStatus(12); }
	inline int GetTRGCalibrationRateStatus(void) { return GetStatus(13); }
	inline int GetTRGEHiRateStatus(void) { return GetStatus(14); }
	inline int GetTRGELoRateStatus(void) { return GetStatus(15); }
	inline int GetTRGZCalibrationRateStatus(void) { return GetStatus(16); }
	inline int GetTRGZHiRateStatus(void) { return GetStatus(17); }
	inline int GetTRGCD1RateStatus(void) { return GetStatus(17); }
	inline int GetTRGCalorimeterRateStatus(void) { return GetStatus(18); }
	inline int GetTRGCD2RateStatus(void) { return GetStatus(19); } //YONAS

	// SW channels
	inline timestamp8_t* GetNIOTimestamp(void) { return GetTimestamp(20); }
	double GetNIOTimeInSecond(void);
	inline real4 GetNIOTotalRate(void) { return GetDouble(21); }
	inline real4 GetNIONetRate(void) { return GetDouble(22); }
	inline real4 GetBufferLevelCmdQ(void) { return GetDouble(23); }
	inline real4 GetBufferLevelHKQ(void) { return GetDouble(24); }
	inline real4 GetBufferLevelEvtQ(void) { return GetDouble(25); }
	inline real4 GetBufferLevelDatQ(void) { return GetDouble(26); }
	inline real4 GetBufferLevelDatQ2(void) { return GetDouble(27); }
	inline real4 GetBufferLevelPktQ0(void) { return GetDouble(28); }
	inline real4 GetBufferLevelPktQ1(void) { return GetDouble(29); }
	inline real4 GetBufferLevelSend(void) { return GetDouble(30); }
	inline real4 GetDiskSpaceLevel(void) { return GetDouble(30); }
	inline real4 GetFlash1Space(void) { return GetDouble(35); }
	inline real4 GetFlash2Space(void) { return GetDouble(36); }
	inline real4 GetFlash3Space(void) { return GetDouble(37); }
	inline real4 GetFlash4Space(void) { return GetDouble(38); }

	inline int GetNIOTimestampStatus(void) { return GetStatus(20); }
	inline int GetNIOTotalRateStatus(void) { return GetStatus(21); }
	inline int GetNIONetRateStatus(void) { return GetStatus(22); }
	inline int GetBufferLevelCmdQStatus(void) { return GetStatus(23); }
	inline int GetBufferLevelHKQStatus(void) { return GetStatus(24); }
	inline int GetBufferLevelEvtQStatus(void) { return GetStatus(25); }
	inline int GetBufferLevelDatQStatus(void) { return GetStatus(26); }
	inline int GetBufferLevelDatQ2Status(void) { return GetStatus(27); }
	inline int GetBufferLevelPktQ0Status(void) { return GetStatus(28); }
	inline int GetBufferLevelPktQ1Status(void) { return GetStatus(29); }
	inline int GetBufferLevelSendStatus(void) { return GetStatus(30); }
	inline int GetDiskSpaceLevelStatus(void) { return GetStatus(30); }
	inline int GetFlash1SpaceStatus(void) { return GetStatus(35); }
	inline int GetFlash2SpaceStatus(void) { return GetStatus(36); }
	inline int GetFlash3SpaceStatus(void) { return GetStatus(37); }
	inline int GetFlash4SpaceStatus(void) { return GetStatus(38); }

	// GPS
	inline timestamp8_t* GetGPSTimestamp(void) { return GetTimestamp(42); }
	double GetGPSTimeInSecond(void);
	inline int GetGPSHour(void) { return GetInteger(43); }
	inline int GetGPSMinute(void) { return GetInteger(44); }
	inline real4 GetGPSSecond(void) { return GetDouble(45); }
	inline real4 GetGPSLatitude(void) { return GetDouble(46); }
	inline real4 GetGPSLongitude(void) { return GetDouble(47); }
	inline real4 GetGPSAltitude(void) { return GetDouble(48); }
	inline real4 GetGPSVerticalVelocity(void) { return GetDouble(49); }
	inline real4 GetGPSHeading(void) { return GetDouble(50); }
	inline real4 GetGPSPitch(void) { return GetDouble(51); }
	inline real4 GetGPSRoll(void) { return GetDouble(52); }
	inline real4 GetGPSCourse(void) { return GetDouble(53); }
	inline real4 GetGPSGroundSpeed(void) { return GetDouble(54); }
	inline real4 GetGPSPDOP(void) { return GetDouble(55); }
	inline real4 GetGPSPhaseRMS(void) { return GetDouble(56); }
	inline real4 GetGPSBaselineRMS(void) { return GetDouble(57); }
	inline int GetGPSNumberOfSatellite(void) { return GetInteger(58); }
	inline int GetGPSResetFlag(void) { return GetInteger(59); }

	inline int GetGPSTimestampStatus(void) { return GetStatus(42); }
	inline int GetGPSHourStatus(void) { return GetStatus(43); }
	inline int GetGPSMinuteStatus(void) { return GetStatus(44); }
	inline int GetGPSSecondStatus(void) { return GetStatus(45); }
	inline int GetGPSLatitudeStatus(void) { return GetStatus(46); }
	inline int GetGPSLongitudeStatus(void) { return GetStatus(47); }
	inline int GetGPSAltitudeStatus(void) { return GetStatus(48); }
	inline int GetGPSVerticalVelocityStatus(void) { return GetStatus(49); }
	inline int GetGPSHeadingStatus(void) { return GetStatus(50); }
	inline int GetGPSPitchStatus(void) { return GetStatus(51); }
	inline int GetGPSRollStatus(void) { return GetStatus(52); }
	inline int GetGPSCourseStatus(void) { return GetStatus(53); }
	inline int GetGPSGroundSpeedStatus(void) { return GetStatus(54); }
	inline int GetGPSPDOPStatus(void) { return GetStatus(55); }
	inline int GetGPSPhaseRMSStatus(void) { return GetStatus(56); }
	inline int GetGPSBaselineRMSStatus(void) { return GetStatus(57); }
	inline int GetGPSNumberOfSatelliteStatus(void) { return GetStatus(58); }
	inline int GetGPSResetFlagStatus(void) { return GetStatus(59); }

	//BAD
	inline real4 GetXPosIner(void) { return GetDouble(43); }
	inline real4 GetYPosIner(void) { return GetDouble(44); }
	inline real4 GetZPosIner(void) { return GetDouble(45); }
	inline real4 GetV_XIner(void) { return GetDouble(46); }
	inline real4 GetV_YIner(void) { return GetDouble(47); }
	inline real4 GetV_ZIner(void) { return GetDouble(48); }
	inline real4 GetAttQ1In(void) { return GetDouble(49); }
	inline real4 GetAttQ2In(void) { return GetDouble(50); }
	inline real4 GetAttQ3In(void) { return GetDouble(51); }
	inline real4 GetAttQ4In(void) { return GetDouble(52); }
	inline real4 GetXPosECF(void) { return GetDouble(53); }
	inline real4 GetYPosECF(void) { return GetDouble(54); }
	inline real4 GetZPosECF(void) { return GetDouble(55); }
	inline real4 GetV_XECF(void) { return GetDouble(56); }
	inline real4 GetV_YECF(void) { return GetDouble(57); }
	inline real4 GetV_ZECF(void) { return GetDouble(58); }
	inline real4 GetAttQ1ECF(void) { return GetDouble(59); }
	inline real4 GetAttQ2ECF(void) { return GetDouble(60); }
	inline real4 GetAttQ3ECF(void) { return GetDouble(61); }
	inline real4 GetAttQ4ECF(void) { return GetDouble(62); }
	inline real4 GetAttQ1Lvh(void) { return GetDouble(63); }
	inline real4 GetAttQ2Lvh(void) { return GetDouble(64); }
	inline real4 GetAttQ3Lvh(void) { return GetDouble(65); }
	inline real4 GetAttQ4Lvh(void) { return GetDouble(66); }

	inline int GetXPosInerStatus(void) { return GetStatus(43); }
	inline int GetYPosInerStatus(void) { return GetStatus(44); }
	inline int GetZPosInerStatus(void) { return GetStatus(45); }
	inline int GetV_XInerStatus(void) { return GetStatus(46); }
	inline int GetV_YInerStatus(void) { return GetStatus(47); }
	inline int GetV_ZInerStatus(void) { return GetStatus(48); }
	inline int GetAttQ1InStatus(void) { return GetStatus(49); }
	inline int GetAttQ2InStatus(void) { return GetStatus(50); }
	inline int GetAttQ3InStatus(void) { return GetStatus(51); }
	inline int GetAttQ4InStatus(void) { return GetStatus(52); }
	inline int GetXPosECFStatus(void) { return GetStatus(53); }
	inline int GetYPosECFStatus(void) { return GetStatus(54); }
	inline int GetZPosECFStatus(void) { return GetStatus(55); }
	inline int GetV_XECFStatus(void) { return GetStatus(56); }
	inline int GetV_YECFStatus(void) { return GetStatus(57); }
	inline int GetV_ZECFStatus(void) { return GetStatus(58); }
	inline int GetAttQ1ECFStatus(void) { return GetStatus(59); }
	inline int GetAttQ2ECFStatus(void) { return GetStatus(60); }
	inline int GetAttQ3ECFStatus(void) { return GetStatus(61); }
	inline int GetAttQ4ECFStatus(void) { return GetStatus(62); }
	inline int GetAttQ1LvhStatus(void) { return GetStatus(63); }
	inline int GetAttQ2LvhStatus(void) { return GetStatus(64); }
	inline int GetAttQ3LvhStatus(void) { return GetStatus(65); }
	inline int GetAttQ4LvhStatus(void) { return GetStatus(66); }

	// timers
// 2005/11/10 Total and live times are reported in minutes.  Long
// integer timers are no longer supported.
//	inline uint8 GetTotalTime(void) { return GetLongInteger(60); }
//	inline uint8 GetLiveTime(void) { return GetLongInteger(61); }
	inline double GetTotalTimeInMinute(void) { return GetDouble(67); }
	inline double GetLiveTimeInMinute(void) { return GetDouble(68); }
	inline double GetTotalTimeInSecond(void) { return 60.0 * GetDouble(67); }
	inline double GetLiveTimeInSecond(void) { return 60.0 * GetDouble(68); }

	inline int GetTotalTimeStatus(void) { return GetStatus(67); }
	inline int GetLiveTimeStatus(void) { return GetStatus(68); }


	inline real4 GetSfcACurr(void) { return GetDouble(72); }
	inline real4 GetSfcATemp(void) { return GetDouble(73); }
	inline real4 GetSfcBCurr(void) { return GetDouble(74); }
	inline real4 GetSfcBTemp(void) { return GetDouble(75); }
	inline real4 GetHCBTemp(void) { return GetDouble(76); }
	inline real4 GetHCATemp(void) { return GetDouble(77); }
	inline real4 GetIIUCurr(void) { return GetDouble(78); }
	inline real4 GetXBoxCur(void) { return GetDouble(79); }
	inline real4 GetSfcRet(void) { return GetDouble(80); }

	inline int GetSfcACurrStatus(void) { return GetStatus(72); }
	inline int GetSfcATempStatus(void) { return GetStatus(73); }
	inline int GetSfcBCurrStatus(void) { return GetStatus(74); }
	inline int GetSfcBTempStatus(void) { return GetStatus(75); }
	inline int GetHCBTempStatus(void) { return GetStatus(76); }
	inline int GetHCATempStatus(void) { return GetStatus(77); }
	inline int GetIIUCurrStatus(void) { return GetStatus(78); }
	inline int GetXBoxCurStatus(void) { return GetStatus(79); }
	inline int GetSfcRetStatus(void) { return GetStatus(80); }

	// calorimeter
	inline int CALGetNumberOfChannel(void) { return 8; }

	inline double CALGetPowerSupplyLogicV(void) { return GetDouble(81); }
	inline double CALGetPowerSupplyMinus3o6V(void) { return GetDouble(82); }
	inline double CALGetPowerSupplyPlus3o6V(void) { return GetDouble(83); }
	inline double CALGetPowerSupplyPlus12V(void) { return GetDouble(84); }
	inline double CALGetSystemMinus3o6VCurrent(void) { return GetDouble(85); }
	inline double CALGetSystemPlus3o6VCurrent(void) { return GetDouble(86); }
	inline double CALGetSystemPlus12VCurrent(void) { return GetDouble(87); }
	inline double CALGetTriggerPlus3o6VCurrent(void) { return GetDouble(88); }
	inline double CALGetTemperature(unsigned k) { return GetDouble(92+k); }
	inline double CALGetHighVoltage(unsigned k) { return GetDouble(100+k); }
	inline double CALGetBiasVoltage(unsigned k) { return GetDouble(116+k); }
	inline double CALGetMinus3o6VCurrent(unsigned k) { return GetDouble(124+k); }
	inline double CALGetPlus3o6VCurrent(unsigned k) { return GetDouble(132+k); }
	inline double CALGetPlus12VCurrent(unsigned k) { return GetDouble(140+k); }

	inline int CALGetPowerSupplyLogicVStatus(void) { return GetStatus(81); }
	inline int CALGetPowerSupplyMinus3o6VStatus(void) { return GetStatus(82); }
	inline int CALGetPowerSupplyPlus3o6VStatus(void) { return GetStatus(83); }
	inline int CALGetPowerSupplyPlus12VStatus(void) { return GetStatus(84); }
	inline int CALGetSystemMinus3o6VCurrentStatus(void) { return GetStatus(85); }
	inline int CALGetSystemPlus3o6VCurrentStatus(void) { return GetStatus(86); }
	inline int CALGetSystemPlus12VCurrentStatus(void) { return GetStatus(87); }
	inline int CALGetTriggerPlus3o6VCurrentStatus(void) { return GetStatus(88); }
	inline int CALGetTemperatureStatus(unsigned k) { return GetStatus(92+k); }
	inline int CALGetHighVoltageStatus(unsigned k) { return GetStatus(100+k); }
	inline int CALGetBiasVoltageStatus(unsigned k) { return GetStatus(116+k); }
	inline int CALGetMinus3o6VCurrentStatus(unsigned k) { return GetStatus(124+k); }
	inline int CALGetPlus3o6VCurrentStatus(unsigned k) { return GetStatus(132+k); }
	inline int CALGetPlus12VCurrentStatus(unsigned k) { return GetStatus(140+k); }

	//BSD
	inline real4 GetBsdRet1(void) { return GetDouble(148); }
	inline real4 GetBsdRet2(void) { return GetDouble(149); }
	inline real4 GetBsdTQA(void) { return GetDouble(150); }
	inline real4 GetBsdTQB(void) { return GetDouble(151); }
	inline real4 GetBsdTQC(void) { return GetDouble(152); }
	inline real4 GetBsdTQD(void) { return GetDouble(153); }
	inline real4 GetBsdp5V(void) { return GetDouble(154); }
	inline real4 GetBsdp3o3V(void) { return GetDouble(155); }
	inline real4 GetBsdm5V(void) { return GetDouble(156); }
	inline real4 GetBsdm12V(void) { return GetDouble(157); }
	inline real4 GetBsdp1o5V(void) { return GetDouble(158); }
	inline real4 GetBsdSciT(void) { return GetDouble(159); }
	inline real4 GetBsdEncDT(void) { return GetDouble(160); }
	inline real4 GetBsdEncET(void) { return GetDouble(161); }
	inline real4 GetBsdHVQA1(void) { return GetDouble(162); }
	inline real4 GetBsdCurQA(void) { return GetDouble(163); }
	inline real4 GetBsdHVQA2(void) { return GetDouble(164); }
	inline real4 GetBsdCurQB(void) { return GetDouble(165); }
	inline real4 GetBsdHVQA3(void) { return GetDouble(166); }
	inline real4 GetBsdCurQC(void) { return GetDouble(167); }
	inline real4 GetBsdCurQD(void) { return GetDouble(168); }
	inline real4 GetBsdHVQB1(void) { return GetDouble(169); }
	inline real4 GetBsdHVQB2(void) { return GetDouble(170); }
	inline real4 GetBsdHVQB3(void) { return GetDouble(171); }
	inline real4 GetBsdHVQC1(void) { return GetDouble(172); }
	inline real4 GetBsdHVQC2(void) { return GetDouble(173); }
	inline real4 GetBsdHVQC3(void) { return GetDouble(174); }
	inline real4 GetBsdHVQD1(void) { return GetDouble(175); }
	inline real4 GetBsdHVQD2(void) { return GetDouble(176); }
	inline real4 GetBsdHVQD3(void) { return GetDouble(177); }
	inline real4 GetBsdMonT1(void) { return GetDouble(178); }
	inline real4 GetBsdMonT2(void) { return GetDouble(179); }

	inline int GetBsdRet1Status(void) { return GetStatus(148); }
	inline int GetBsdRet2Status(void) { return GetStatus(149); }
	inline int GetBsdTQAStatus(void) { return GetStatus(150); }
	inline int GetBsdTQBStatus(void) { return GetStatus(151); }
	inline int GetBsdTQCStatus(void) { return GetStatus(152); }
	inline int GetBsdTQDStatus(void) { return GetStatus(153); }
	inline int GetBsdp5VStatus(void) { return GetStatus(154); }
	inline int GetBsdp3o3VStatus(void) { return GetStatus(155); }
	inline int GetBsdm5VStatus(void) { return GetStatus(156); }
	inline int GetBsdm12VStatus(void) { return GetStatus(157); }
	inline int GetBsdp1o5VStatus(void) { return GetStatus(158); }
	inline int GetBsdSciTStatus(void) { return GetStatus(159); }
	inline int GetBsdEncDTStatus(void) { return GetStatus(160); }
	inline int GetBsdEncETStatus(void) { return GetStatus(161); }
	inline int GetBsdHVQA1Status(void) { return GetStatus(162); }
	inline int GetBsdCurQAStatus(void) { return GetStatus(163); }
	inline int GetBsdHVQA2Status(void) { return GetStatus(164); }
	inline int GetBsdCurQBStatus(void) { return GetStatus(165); }
	inline int GetBsdHVQA3Status(void) { return GetStatus(166); }
	inline int GetBsdCurQCStatus(void) { return GetStatus(167); }
	inline int GetBsdCurQDStatus(void) { return GetStatus(168); }
	inline int GetBsdHVQB1Status(void) { return GetStatus(169); }
	inline int GetBsdHVQB2Status(void) { return GetStatus(170); }
	inline int GetBsdHVQB3Status(void) { return GetStatus(171); }
	inline int GetBsdHVQC1Status(void) { return GetStatus(172); }
	inline int GetBsdHVQC2Status(void) { return GetStatus(173); }
	inline int GetBsdHVQC3Status(void) { return GetStatus(174); }
	inline int GetBsdHVQD1Status(void) { return GetStatus(175); }
	inline int GetBsdHVQD2Status(void) { return GetStatus(176); }
	inline int GetBsdHVQD3Status(void) { return GetStatus(177); }
	inline int GetBsdMonT1Status(void) { return GetStatus(178); }
	inline int GetBsdMonT2Status(void) { return GetStatus(179); }
	
	// TCRD
	inline real4 GetTcdTemp1(void) { return GetDouble(180); }
	inline real4 GetTcdTemp2(void) { return GetDouble(181); }
	inline real4 GetTcdTemp3(void) { return GetDouble(182); }
	inline real4 GetTcdTemp4(void) { return GetDouble(183); }
	inline real4 GetTcdBpY1A(void) { return GetDouble(184); }
	inline real4 GetTcdBpY1B(void) { return GetDouble(185); }
	inline real4 GetTcdBmY2A(void) { return GetDouble(186); }
	inline real4 GetTcdBmY2B(void) { return GetDouble(187); }
	inline real4 GetTcdBpY3A(void) { return GetDouble(188); }
	inline real4 GetTcdBpY3B(void) { return GetDouble(189); }
	inline real4 GetTcdBmY4A(void) { return GetDouble(190); }
	inline real4 GetTcdBmY4B(void) { return GetDouble(191); }

	inline int GetTcdTemp1Status(void) { return GetStatus(180); }
	inline int GetTcdTemp2Status(void) { return GetStatus(181); }
	inline int GetTcdTemp3Status(void) { return GetStatus(182); }
	inline int GetTcdTemp4Status(void) { return GetStatus(183); }
	inline int GetTcdBpY1AStatus(void) { return GetStatus(184); }
	inline int GetTcdBpY1BStatus(void) { return GetStatus(185); }
	inline int GetTcdBmY2AStatus(void) { return GetStatus(186); }
	inline int GetTcdBmY2BStatus(void) { return GetStatus(187); }
	inline int GetTcdBpY3AStatus(void) { return GetStatus(188); }
	inline int GetTcdBpY3BStatus(void) { return GetStatus(189); }
	inline int GetTcdBmY4AStatus(void) { return GetStatus(190); }
	inline int GetTcdBmY4BStatus(void) { return GetStatus(191); }

	//Thermistors
	inline real4 GetTherm1(void) { return GetDouble(196); }
	inline real4 GetTherm2(void) { return GetDouble(197); }
	inline real4 GetTherm3(void) { return GetDouble(198); }
	inline real4 GetTherm4(void) { return GetDouble(199); }
	inline real4 GetTherm5(void) { return GetDouble(200); }
	inline real4 GetTherm6(void) { return GetDouble(201); }
	inline real4 GetTherm7(void) { return GetDouble(202); }
	inline real4 GetTherm8(void) { return GetDouble(203); }
	inline real4 GetTherm9(void) { return GetDouble(204); }
	inline real4 GetTherm10(void) { return GetDouble(205); }
	inline real4 GetTherm11(void) { return GetDouble(206); }
	inline real4 GetTherm12(void) { return GetDouble(207); }
	inline real4 GetTherm13(void) { return GetDouble(208); }
	inline real4 GetTherm14(void) { return GetDouble(209); }
	inline real4 GetTherm15(void) { return GetDouble(210); }
	inline real4 GetTherm16(void) { return GetDouble(211); }
	inline real4 GetTherm17(void) { return GetDouble(212); }
	inline real4 GetTherm18(void) { return GetDouble(213); }

	inline int GetTherm1Status(void) { return GetStatus(196); }
	inline int GetTherm2Status(void) { return GetStatus(197); }
	inline int GetTherm3Status(void) { return GetStatus(198); }
	inline int GetTherm4Status(void) { return GetStatus(199); }
	inline int GetTherm5Status(void) { return GetStatus(200); }
	inline int GetTherm6Status(void) { return GetStatus(201); }
	inline int GetTherm7Status(void) { return GetStatus(202); }
	inline int GetTherm8Status(void) { return GetStatus(203); }
	inline int GetTherm9Status(void) { return GetStatus(204); }
	inline int GetTherm10Status(void) { return GetStatus(205); }
	inline int GetTherm11Status(void) { return GetStatus(206); }
	inline int GetTherm12Status(void) { return GetStatus(207); }
	inline int GetTherm13Status(void) { return GetStatus(208); }
	inline int GetTherm14Status(void) { return GetStatus(209); }
	inline int GetTherm15Status(void) { return GetStatus(210); }
	inline int GetTherm16Status(void) { return GetStatus(211); }
	inline int GetTherm17Status(void) { return GetStatus(212); }
	inline int GetTherm18Status(void) { return GetStatus(213); }

	//CAL Power Module ISS
	inline real4 GetCalp3o3V(void) { return GetDouble(214); }
	inline real4 GetCalp5o2V(void) { return GetDouble(215); }
	inline real4 GetCalp12V(void) { return GetDouble(216); }
	inline real4 GetCalm5o2V(void) { return GetDouble(217); }
	inline real4 GetCalTemp(void) { return GetDouble(218); }
	inline real4 GetCalCurrp5o2(unsigned k) { return GetDouble(219+k); }
	inline real4 GetCalCurrm5o2(unsigned k) { return GetDouble(233+k); }
	inline real4 GetCalCurrp12(unsigned k) { return GetDouble(247+k); }
	inline real4 GetCalGnd (void) { return GetDouble(261); }

	inline int GetCalp3o3VStatus(void) { return GetStatus(214); }
	inline int GetCalp5o2VStatus(void) { return GetStatus(215); }
	inline int GetCalp12VStatus(void) { return GetStatus(216); }
	inline int GetCalm5o2VStatus(void) { return GetStatus(217); }
	inline int GetCalTempStatus(void) { return GetStatus(218); }
	inline int GetCalCurrp5o2Status(unsigned k) { return GetStatus(219+k); }
	inline int GetCalCurrm5o2Status(unsigned k) { return GetStatus(233+k); }
	inline int GetCalCurrp12Status(unsigned k) { return GetStatus(247+k); }
	inline int GetCalGndStatus(void) { return GetStatus(261); }	

	//SCD12 Power Module ISS
	inline real4 GetSCD12p3o3V(void) { return GetDouble(262); }
	inline real4 GetSCD12p5o2V(void) { return GetDouble(263); }
	inline real4 GetSCD12p12V(void) { return GetDouble(264); }
	inline real4 GetSCD12m5o2V(void) { return GetDouble(265); }
	inline real4 GetSCD12Temp(void) { return GetDouble(266); }
	inline real4 GetSCD12Currp5o2(unsigned k) { return GetDouble(267+k); }
	inline real4 GetSCD12Currm5o2(unsigned k) { return GetDouble(281+k); }
	inline real4 GetSCD12Currp12(unsigned k) { return GetDouble(295+k); }
	inline real4 GetSCD12Gnd (void) { return GetDouble(309); }

	inline int GetSCD12p3o3VStatus(void) { return GetStatus(262); }
	inline int GetSCD12p5o2VStatus(void) { return GetStatus(263); }
	inline int GetSCD12p12VStatus(void) { return GetStatus(264); }
	inline int GetSCD12m5o2VStatus(void) { return GetStatus(265); }
	inline int GetSCD12TempStatus(void) { return GetStatus(266); }
	inline int GetSCD12Currp5o2Status(unsigned k) { return GetStatus(267+k); }
	inline int GetSCD12Currm5o2Status(unsigned k) { return GetStatus(281+k); }
	inline int GetSCD12Currp12Status(unsigned k) { return GetStatus(295+k); }
	inline int GetSCD12GndStatus(void) { return GetStatus(309); }


	//SCD34 Power Module ISS
	inline real4 GetSCD34p3o3V(void) { return GetDouble(310); }
	inline real4 GetSCD34p5o2V(void) { return GetDouble(311); }
	inline real4 GetSCD34p12V(void) { return GetDouble(312); }
	inline real4 GetSCD34m5o2V(void) { return GetDouble(313); }
	inline real4 GetSCD34Temp(void) { return GetDouble(314); }
	inline real4 GetSCD34Currp5o2(unsigned k) { return GetDouble(315+k); }
	inline real4 GetSCD34Currm5o2(unsigned k) { return GetDouble(329+k); }
	inline real4 GetSCD34Currp12(unsigned k) { return GetDouble(343+k); }
	inline real4 GetSCD34Gnd (void) { return GetDouble(357); }

	inline int GetSCD34p3o3VStatus(void) { return GetStatus(310); }
	inline int GetSCD34p5o2VStatus(void) { return GetStatus(311); }
	inline int GetSCD34p12VStatus(void) { return GetStatus(312); }
	inline int GetSCD34m5o2VStatus(void) { return GetStatus(313); }
	inline int GetSCD34TempStatus(void) { return GetStatus(314); }
	inline int GetSCD34Currp5o2Status(unsigned k) { return GetStatus(315+k); }
	inline int GetSCD34Currm5o2Status(unsigned k) { return GetStatus(329+k); }
	inline int GetSCD34Currp12Status(unsigned k) { return GetStatus(343+k); }
	inline int GetSCD34GndStatus(void) { return GetStatus(357); }


	//Temperature Sensors
	inline real4 GetTemp1(void) { return GetDouble(358); }
	inline real4 GetTemp2(void) { return GetDouble(359); }
	inline real4 GetTemp3(void) { return GetDouble(360); }
	inline real4 GetTemp4(void) { return GetDouble(361); }
	inline real4 GetTemp5(void) { return GetDouble(362); }
	inline real4 GetTemp6(void) { return GetDouble(363); }
	inline real4 GetTemp7(void) { return GetDouble(364); }
	inline real4 GetTemp8(void) { return GetDouble(365); }
	inline real4 GetTemp9(void) { return GetDouble(366); }
	inline real4 GetTemp10(void) { return GetDouble(367); }
	inline real4 GetTemp11(void) { return GetDouble(368); }
	inline real4 GetTemp12(void) { return GetDouble(369); }


	inline int GetTemp1Status(void) { return GetStatus(358); }
	inline int GetTemp2Status(void) { return GetStatus(359); }
	inline int GetTemp3Status(void) { return GetStatus(360); }
	inline int GetTemp4Status(void) { return GetStatus(361); }
	inline int GetTemp5Status(void) { return GetStatus(362); }
	inline int GetTemp6Status(void) { return GetStatus(363); }
	inline int GetTemp7Status(void) { return GetStatus(364); }
	inline int GetTemp8Status(void) { return GetStatus(365); }
	inline int GetTemp9Status(void) { return GetStatus(366); }
	inline int GetTemp10Status(void) { return GetStatus(367); }
	inline int GetTemp11Status(void) { return GetStatus(368); }
	inline int GetTemp12Status(void) { return GetStatus(369); }



	// hodoscope
	inline int HDSGetNumberOfChannel(void) { return 12; }

	inline double HDSGetPowerSupplyLogicV(void) { return GetDouble(122); }
	inline double HDSGetPowerSupplyMinus6V(void) { return GetDouble(123); }
	inline double HDSGetPowerSupplyPlus6V(void) { return GetDouble(124); }
	inline double HDSGetPowerSupplyPlus12V(void) { return GetDouble(125); }
	inline double HDSGetTemperature1(unsigned k) { return GetDouble(134+k); }
	inline double HDSGetTemperature2(unsigned k) { return GetDouble(146+k); }
	inline double HDSGetTemperature3(unsigned k) { return GetDouble(158+k); }
	inline double HDSGetHighVoltage(unsigned k) { return GetDouble(170+k); }
	inline double HDSGetBiasVoltage(unsigned k) { return GetDouble(182+k); }
	inline double HDSGetMinus6VCurrent(unsigned k) { return GetDouble(194+k); }
	inline double HDSGetPlus6VCurrent(unsigned k) { return GetDouble(206+k); }
	inline double HDSGetPlus12VCurrent(unsigned k) { return GetDouble(218+k); }

	inline int HDSGetPowerSupplyLogicVStatus(void) { return GetStatus(122); }
	inline int HDSGetPowerSupplyMinus6VStatus(void) { return GetStatus(123); }
	inline int HDSGetPowerSupplyPlus6VStatus(void) { return GetStatus(124); }
	inline int HDSGetPowerSupplyPlus12VStatus(void) { return GetStatus(125); }
	inline int HDSGetTemperature1Status(unsigned k) { return GetStatus(134+k); }
	inline int HDSGetTemperature2Status(unsigned k) { return GetStatus(146+k); }
	inline int HDSGetTemperature3Status(unsigned k) { return GetStatus(158+k); }
	inline int HDSGetHighVoltageStatus(unsigned k) { return GetStatus(170+k); }
	inline int HDSGetBiasVoltageStatus(unsigned k) { return GetStatus(182+k); }
	inline int HDSGetMinus6VCurrentStatus(unsigned k) { return GetStatus(194+k); }
	inline int HDSGetPlus6VCurrentStatus(unsigned k) { return GetStatus(206+k); }
	inline int HDSGetPlus12VCurrentStatus(unsigned k) { return GetStatus(218+k); }

	// SCD
	inline int SCDGetNumberOfChannel(void) { return 26; }

	inline double SCDGetPowerSupplyLogicV(void) { return GetDouble(230); }
	inline double SCDGetPowerSupplyMinus6o5V(void) { return GetDouble(231); }
	inline double SCDGetPowerSupplyPlus6o5V(void) { return GetDouble(232); }
	inline double SCDGetPowerSupplyPlus12V(void) { return GetDouble(233); }
	inline double SCDGetMotherBoardMinus6o5VCurrent(unsigned k) { return GetDouble(234+k); }
	inline double SCDGetMotherBoardPlus6o5VCurrent(unsigned k) { return GetDouble(236+k); }
	inline double SCDGetMotherBoardPlus12VCurrent(unsigned k) { return GetDouble(238+k); }
	inline double SCDGetTemperature(unsigned k) { return GetDouble(242+k); }
	inline double SCDGetBiasVoltage(unsigned k) { return GetDouble(268+k); }
	inline double SCDGetLogicCurrent(unsigned k) { return GetDouble(294+k); }
	inline double SCDGetAnalogPlus5VCurrent(unsigned k) { return GetDouble(346+k); }
	inline double SCDGetAnalogMinus5VCurrent(unsigned k) { return GetDouble(320+k); }
	
	inline int SCDGetPowerSupplyLogicVStatus(void) { return GetStatus(230); }
	inline int SCDGetPowerSupplyMinus6o5VStatus(void) { return GetStatus(231); }
	inline int SCDGetPowerSupplyPlus6o5VStatus(void) { return GetStatus(232); }
	inline int SCDGetPowerSupplyPlus12VStatus(void) { return GetStatus(233); }
	inline int SCDGetMotherBoardMinus6o5VCurrentStatus(unsigned k) { return GetStatus(234+k); }
	inline int SCDGetMotherBoardPlus6o5VCurrentStatus(unsigned k) { return GetStatus(236+k); }
	inline int SCDGetMotherBoardPlus12VCurrentStatus(unsigned k) { return GetStatus(238+k); }
	inline int SCDGetTemperatureStatus(unsigned k) { return GetStatus(242+k); }
	inline int SCDGetBiasVoltageStatus(unsigned k) { return GetStatus(268+k); }
	inline int SCDGetLogicCurrentStatus(unsigned k) { return GetStatus(294+k); }
	inline int SCDGetAnalogPlus5VCurrentStatus(unsigned k) { return GetStatus(320+k); }
	inline int SCDGetAnalogMinus5VCurrentStatus(unsigned k) { return GetStatus(346+k); }

	// Top SCD      // Added by YS  Aug 7, 2007
	inline double tSCDGetPowerSupplyLogicV(void) { return GetDouble(439); }
	inline double tSCDGetPowerSupplyMinus6o5V(void) { return GetDouble(440); }
	inline double tSCDGetPowerSupplyPlus6o5V(void) { return GetDouble(441); }
	inline double tSCDGetPowerSupplyPlus12V(void) { return GetDouble(442); }
	inline double tSCDGetMotherBoardMinus6o5VCurrent(unsigned k) { return GetDouble(443+k); }
	inline double tSCDGetMotherBoardPlus6o5VCurrent(unsigned k) { return GetDouble(445+k); }
	inline double tSCDGetMotherBoardPlus12VCurrent(unsigned k) { return GetDouble(447+k); }
	inline double tSCDGetTemperature(unsigned k) { return GetDouble(455+k); }
	inline double tSCDGetBiasVoltage(unsigned k) { return GetDouble(481+k); }
	inline double tSCDGetBiasCurrent(unsigned k) { return GetDouble(585+k); }
	inline double tSCDGetLogicVoltage(unsigned k) { return GetDouble(507+k); }
	inline double tSCDGetLogicCurrent(unsigned k) { return GetDouble(611+k); }
	inline double tSCDGetAnalogMinus5VVoltage(unsigned k) { return GetDouble(533+k); }
	inline double tSCDGetAnalogPlus5VVoltage(unsigned k) { return GetDouble(559+k); }
	inline double tSCDGetAnalogPlus5VCurrent(unsigned k) { return GetDouble(663+k); }
	inline double tSCDGetAnalogMinus5VCurrent(unsigned k) { return GetDouble(637+k); }


	//bscd
	inline double bSCDGetPowerSupplyLogicV(void) { return GetDouble(689); }
	inline double bSCDGetPowerSupplyMinus6o5V(void) { return GetDouble(690); }
	inline double bSCDGetPowerSupplyPlus6o5V(void) { return GetDouble(691); }
	inline double bSCDGetPowerSupplyPlus12V(void) { return GetDouble(692); }
	inline double bSCDGetMotherBoardMinus6o5VCurrent(unsigned k) { return GetDouble(693+k); }
	inline double bSCDGetMotherBoardPlus6o5VCurrent(unsigned k) { return GetDouble(665+k); }
	inline double bSCDGetMotherBoardPlus12VCurrent(unsigned k) { return GetDouble(667+k); }
	inline double bSCDGetTemperature(unsigned k) { return GetDouble(705+k); }
	inline double bSCDGetBiasVoltage(unsigned k) { return GetDouble(731+k); }
	inline double bSCDGetBiasCurrent(unsigned k) { return GetDouble(835+k); }
	inline double bSCDGetLogicVoltage(unsigned k) { return GetDouble(757+k); }
	inline double bSCDGetLogicCurrent(unsigned k) { return GetDouble(861+k); }
	inline double bSCDGetAnalogMinus5VVoltage(unsigned k) { return GetDouble(783+k); }
	inline double bSCDGetAnalogPlus5VVoltage(unsigned k) { return GetDouble(809+k); }
	inline double bSCDGetAnalogPlus5VCurrent(unsigned k) { return GetDouble(913+k); }
	inline double bSCDGetAnalogMinus5VCurrent(unsigned k) { return GetDouble(887+k); }

	//scdl2
	inline double SCDL2GetPowerSupplyLogicV(void) { return GetDouble(1368); }
	inline double SCDL2GetPowerSupplyMinus6o5V(void) { return GetDouble(1369); }
	inline double SCDL2GetPowerSupplyPlus6o5V(void) { return GetDouble(1370); }
	inline double SCDL2GetPowerSupplyPlus12V(void) { return GetDouble(1371); }
	inline double SCDL2GetMotherBoardMinus6o5VCurrent(unsigned k) { return GetDouble(1372+k); }
	inline double SCDL2GetMotherBoardPlus6o5VCurrent(unsigned k) { return GetDouble(1374+k); }
	inline double SCDL2GetMotherBoardPlus12VCurrent(unsigned k) { return GetDouble(1376+k); }
	inline double SCDL2GetTemperature(unsigned k) { return GetDouble(1384+k); }
	inline double SCDL2GetBiasVoltage(unsigned k) { return GetDouble(1410+k); }
	inline double SCDL2GetBiasCurrent(unsigned k) { return GetDouble(1514+k); }
	inline double SCDL2GetLogicVoltage(unsigned k) { return GetDouble(1436+k); }
	inline double SCDL2GetLogicCurrent(unsigned k) { return GetDouble(1540+k); }
	inline double SCDL2GetAnalogMinus5VVoltage(unsigned k) { return GetDouble(1462+k); }
	inline double SCDL2GetAnalogPlus5VVoltage(unsigned k) { return GetDouble(1488+k); }
	inline double SCDL2GetAnalogPlus5VCurrent(unsigned k) { return GetDouble(1592+k); }
	inline double SCDL2GetAnalogMinus5VCurrent(unsigned k) { return GetDouble(1566+k); }


	//  SCDL1   Added by David Angelaszek on 10 Sep 2010
	inline double SCDL1GetPowerSupplyLogicV(void) { return GetDouble(1118); }
	inline double SCDL1GetPowerSupplyMinus6o5V(void) { return GetDouble(1119); }
	inline double SCDL1GetPowerSupplyPlus6o5V(void) { return GetDouble(1120); }
	inline double SCDL1GetPowerSupplyPlus12V(void) { return GetDouble(1121); }
	inline double SCDL1GetMotherBoardMinus6o5VCurrent(unsigned k) { return GetDouble(1122+k); }
	inline double SCDL1GetMotherBoardPlus6o5VCurrent(unsigned k) { return GetDouble(1124+k); }
	inline double SCDL1GetMotherBoardPlus12VCurrent(unsigned k) { return GetDouble(1126+k); }
	inline double SCDL1GetTemperature(unsigned k) { return GetDouble(1134+k); }
	inline double SCDL1GetBiasVoltage(unsigned k) { return GetDouble(1160+k); }
	inline double SCDL1GetBiasCurrent(unsigned k) { return GetDouble(1264+k); }
	inline double SCDL1GetLogicVoltage(unsigned k) { return GetDouble(1186+k); }
	inline double SCDL1GetLogicCurrent(unsigned k) { return GetDouble(1290+k); }
	inline double SCDL1GetAnalogMinus5VVoltage(unsigned k) { return GetDouble(1212+k); }
	inline double SCDL1GetAnalogPlus5VVoltage(unsigned k) { return GetDouble(1238+k); }
	inline double SCDL1GetAnalogPlus5VCurrent(unsigned k) { return GetDouble(1342+k); }
	inline double SCDL1GetAnalogMinus5VCurrent(unsigned k) { return GetDouble(1316+k); }
	
	// TRD
	inline int TRDGetNumberOfPressure(void) { return 11; }
	inline int TRDGetNumberOfTemperature(void) { return 8; }

	inline double TRDGetMinus3o6VCurrent(void) { return GetDouble(372); }
	inline double TRDGetPlus3o6VCurrent(void) { return GetDouble(373); }
	inline double TRDGetHighVoltage(unsigned k) { return GetDouble(382+k); }
	inline double TRDGetHighVoltagePowerSupplyCurrent(unsigned k) { return GetDouble(385+k); }
	inline double TRDGetTemperature(unsigned k) { return GetDouble(388+k); }
	inline double TRDGetPressure(unsigned k) { return GetDouble(396+k); }

	inline int TRDGetPlus3o6VCurrentStatus(void) { return GetStatus(372); }
	inline int TRDGetMinus3o6VCurrentStatus(void) { return GetStatus(373); }
	inline int TRDGetHighVoltageStatus(unsigned k) { return GetStatus(382+k); }
	inline int TRDGetHighVoltagePowerSupplyCurrentStatus(unsigned k) { return GetStatus(385+k); }
	inline int TRDGetTemperatureStatus(unsigned k) { return GetStatus(388+k); }
	inline int TRDGetPressureStatus(unsigned k) { return GetStatus(396+k); }

	// TCD
	inline double TCDGetPowerSupplyMinus2V(void) { return GetDouble(407); }
	inline double TCDGetPowerSupplyMinus5V(void) { return GetDouble(408); }
	inline double TCDGetPowerSupplyPlus3o3V(void) { return GetDouble(409); }
	inline double TCDGetPowerSupplyAnalogPlus5V(void) { return GetDouble(410); }
	inline double TCDGetPowerSupplyPlus5V(void) { return GetDouble(411); }
	inline double TCDGetPowerSupplyPlus12V(void) { return GetDouble(412); }
	inline int TCDGetNumberOfTemperature(void) { return 5; }
	inline double TCDGetTemperature(unsigned k) { return GetDouble(413+k); }

	inline int TCDGetPowerSupplyMinus2VStatus(void) { return GetStatus(407); }
	inline int TCDGetPowerSupplyMinus5VStatus(void) { return GetStatus(408); }
	inline int TCDGetPowerSupplyPlus3o3VStatus(void) { return GetStatus(409); }
	inline int TCDGetPowerSupplyAnalogPlus5VStatus(void) { return GetStatus(410); }
	inline int TCDGetPowerSupplyPlus5VStatus(void) { return GetStatus(411); }
	inline int TCDGetPowerSupplyPlus12VStatus(void) { return GetStatus(412); }
	inline int TCDGetTemperatureStatus(unsigned k) { return GetStatus(413+k); }

	// TCD HV   // Added by YS Aug 7, 2007
	inline double TCDGetHighVoltage0_PXL(void) { return GetDouble(1079); }
	inline double TCDGetHighVoltage1_PXR(void) { return GetDouble(1080); }
	inline double TCDGetHighVoltage2_PYL(void) { return GetDouble(1081); }
	inline double TCDGetHighVoltage3_PYR(void) { return GetDouble(1082); }
	inline double TCDGetHighVoltage4_MXL(void) { return GetDouble(1083); }
	inline double TCDGetHighVoltage5_MXR(void) { return GetDouble(1084); }
	inline double TCDGetHighVoltage6_MYL(void) { return GetDouble(1085); }
	inline double TCDGetHighVoltage7_MYR(void) { return GetDouble(1086); }
	inline double TCDGetHighVoltage8_S3(void)  { return GetDouble(1087); }

	// CherCam    // Added by YS Aug 7, 2007
	inline double CERGetHighVoltage(unsigned k) { return GetDouble(939+k); }
	inline double CERGetHVModule28VSupplyCurrent(unsigned k) { return GetDouble(1039+k); }
	inline double CERGetHVModuleMinus3VSupplyCurrent(unsigned k) { return GetDouble(1041+k); }
	inline double CERGetHVControlBoard12VSupplyCurrent(unsigned k) { return GetDouble(1043+k); }
	inline double CERGetHVControlBoardM3VSupplyCurrent(unsigned k) { return GetDouble(1045+k); }
	inline double CERGetPowerSupplyAnalog5V() { return GetDouble(1047); }
	inline double CERGetPowerSupplyDigital5V() { return GetDouble(1048); }
	inline double CERGetPowerSupplyMinus3V() { return GetDouble(1049); }
	inline double CERGetPowerSupply12V() { return GetDouble(1050); }
	inline double CERGetPowerSupply28V() { return GetDouble(1051); }
	inline double CERGetSparsification5VCurrent() { return GetDouble(1052); }
	inline double CERGetMerger3VSupplyCurrent(unsigned k) { return GetDouble(1053+k); }
	inline double CERGetMergerMinus3VSupplyCurrent(unsigned k) { return GetDouble(1055+k); }
	inline double CERGetMerger2o5VSupplyCurrent(unsigned k) { return GetDouble(1057+k); }
	inline double CERGetMerger5VSupplyCurrent(unsigned k) { return GetDouble(1059+k); }
	inline double CERGetHKBoard12VSupplyCurrent() { return GetDouble(1061); }
	inline double CERGetHKBoardMinus3VSupplyCurrent() { return GetDouble(1061); }
	inline double CERGetTemperature(unsigned k) { return GetDouble(1065+k); }

	// misc
	inline double GetTemperatureCALMinusXMinusY(void) { return GetDouble(423); }
	inline double GetTemperatureCALMinusXPlusY(void) { return GetDouble(424); }
	inline double GetTemperatureCALPlusXMinusY(void) { return GetDouble(425); }
	inline double GetTemperatureCALPlusXPlusY(void) { return GetDouble(426); }
	inline double GetTemperatureSCDMinusXMinusY(void) { return GetDouble(427); }
	inline double GetTemperatureSCDMinusXPlusY(void) { return GetDouble(428); }
	inline double GetTemperatureSCDPlusXMinusY(void) { return GetDouble(429); }
	inline double GetTemperatureSCDPlusXPlusY(void) { return GetDouble(430); }
	inline double GetTemperatureS0S1MinusX(void) { return GetDouble(431); }
	inline double GetTemperatureS0S1PlusX(void) { return GetDouble(432); }
	inline double GetTemperatureCalibStack(void) { return GetDouble(433); }
	inline double GetTemperatureSparsStack(void) { return GetDouble(434); }
	inline double GetTemperatureTCRDMinusXMinusY(void) { return GetDouble(435); } //YONAS
	inline double GetTemperatureTCRDMinusXPlusY(void) { return GetDouble(436); } //YONAS
	inline double GetTemperatureTCRDPlusXMinusY(void) { return GetDouble(437); } //YONAS
	inline double GetTemperatureTCRDPlusXPlusY(void) { return GetDouble(438); } //YONAS


	inline int GetTemperatureCALMinusXMinusYStatus(void) { return GetStatus(423); }
	inline int GetTemperatureCALMinusXPlusYStatus(void) { return GetStatus(424); }
	inline int GetTemperatureCALPlusXMinusYStatus(void) { return GetStatus(425); }
	inline int GetTemperatureCALPlusXPlusYStatus(void) { return GetStatus(426); }
	inline int GetTemperatureSCDMinusXMinusYStatus(void) { return GetStatus(427); }
	inline int GetTemperatureSCDMinusXPlusYStatus(void) { return GetStatus(428); }
	inline int GetTemperatureSCDPlusXMinusYStatus(void) { return GetStatus(429); }
	inline int GetTemperatureSCDPlusXPlusYStatus(void) { return GetStatus(430); }
	inline int GetTemperatureS0S1MinusXStatus(void) { return GetStatus(431); }
	inline int GetTemperatureS0S1PlusXStatus(void) { return GetStatus(432); }
	inline int GetTemperatureCalibStackStatus(void) { return GetStatus(433); }
	inline int GetTemperatureSparsStackStatus(void) { return GetStatus(434); }
	inline int GetTemperatureTCRDMinusXMinusYStatus(void) { return GetStatus(435); } //YONAS
	inline int GetTemperatureTCRDMinusXPlusYStatus(void) { return GetStatus(436); }//YONAS
	inline int GetTemperatureTCRDPlusXMinusYStatus(void) { return GetStatus(437); } //YONAS
	inline int GetTemperatureTCRDPlusXPlusYStatus(void) { return GetStatus(438); } //YONAS

	// 
	void PrintText(FILE* fp, int =-1 );
	void PrintHTML(FILE* fp);
};

#endif // DHKEVENT_H
