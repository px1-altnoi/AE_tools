/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	Skeleton.h
*/

#pragma once

#ifndef CLRFIL_H
#define CLRFIL_H

#include "AEConfig.h"
#include "entry.h"

#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "AE_ChannelSuites.h"
#include "AE_EffectSuites.h"

#include "Param_Utils.h"
#include "AEFX_SuiteHelper.h"
#include "DockSuite.h"

#define MAJOR_VERSION	2
#define MINOR_VERSION	0
#define BUG_VERSION		0
#define STAGE_VERSION	PF_Stage_DEVELOP
#define BUILD_VERSION	0

#define NAME "TESTBED0001"
#define DESCRIPTION "THIS IS TESTBED0001.Beta2\rCopyright 2020\r\r altnoi."
#define CHECK_FRAME_NAME "Frame offset"
#define CHECK_LAYER_NAME "Layer to checkout"
#define W_FRAME_NAME "Width"
#define H_FRAME_NAME "Height"

enum {
	CHECK_INPUT = 0,
	CHECK_FRAME,
	W_FRAME,
	H_FRAME,
	CHECK_LAYER,
	CHECK_NUM_PARAMS
};

enum {
	CHECK_FRAME_DISK_ID = 1,
	CHECK_LAYER_DISK_ID, 
	W_FRAME_DISK_ID,
	H_FRAME_DISK_ID
};

#define CHECK_FRAME_MIN -100
#define CHECK_FRAME_MAX 100
#define CHECK_FRAME_DFLT 0

#define FRAME_MIN 1
#define FRAME_MAX 10
#define FRAME_DFLT 2

extern "C" {
	DllExport
		PF_Err EffectMain(
			PF_Cmd cmd,
			PF_InData* in_data,
			PF_OutData* out_data,
			PF_ParamDef* params[],
			PF_LayerDef* output);
}

#endif // CLRFIL_H