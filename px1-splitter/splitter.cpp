/*
	colorfilter.cpp
	last mod: 2020-08-28
	author: altnoi
*/

#include "splitter.h"

static PF_Err GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_Err err = PF_Err_NONE;

	try {
		// Sweetie SDK‚ªŽg—p‰Â”\‚©check
		AEFX_SuiteScoper<DuckSuite1> dsP = AEFX_SuiteScoper<DuckSuite1>(	in_data,
																			kDuckSuite1,
																			kDuckSuiteVersion,
																			out_data,
																			"No Dock suite!! Build the Sweetie SDK sample in the AEGP folder and put it in the plug-in folder.");
	}
	catch (A_Err &thrown_err){
		thrown_err = 0;
	}

	out_data->my_version = PF_VERSION(	MAJOR_VERSION,
										MINOR_VERSION,
										BUG_VERSION,
										STAGE_VERSION,
										BUILD_VERSION);

	out_data->out_flags =	PF_OutFlag_WIDE_TIME_INPUT |
							PF_OutFlag_I_DO_DIALOG;

	out_data->out_flags2 = PF_OutFlag2_SUPPORTS_THREADED_RENDERING;

	return err;
}

static PF_Err ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_Err err = PF_Err_NONE;
	PF_ParamDef def;

	AEFX_CLR_STRUCT(def);

	PF_ADD_SLIDER(	CHECK_FRAME_NAME,
					CHECK_FRAME_MIN,
					CHECK_FRAME_MAX,
					CHECK_FRAME_MIN,
					CHECK_FRAME_MAX,
					CHECK_FRAME_DFLT,
					CHECK_FRAME_DISK_ID);
					
	AEFX_CLR_STRUCT(def);
	
	PF_ADD_SLIDER(	W_FRAME_NAME,
					FRAME_MIN,
					FRAME_MAX,
					FRAME_MIN,
					FRAME_MAX,
					FRAME_DFLT,
					W_FRAME_DISK_ID);
	
	AEFX_CLR_STRUCT(def);
	
	PF_ADD_SLIDER(	H_FRAME_NAME,
					FRAME_MIN,
					FRAME_MAX,
					FRAME_MIN,
					FRAME_MAX,
					FRAME_DFLT,
					H_FRAME_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER(	CHECK_LAYER_NAME,
					PF_LayerDefault_MYSELF,
					CHECK_LAYER_DISK_ID);

	out_data->num_params = CHECK_NUM_PARAMS;

	if (in_data->appl_id != 'PrMr') {
		AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(	in_data,
																									kPFEffectUISuite,
																									kPFEffectCustomUISuiteVersion1,
																									out_data);
		ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "testing no"));
	}
	return err;
}

static PF_Err Render(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_Err	err		=	PF_Err_NONE,
			err2	=	PF_Err_NONE;
	
	int32_t	num_channelsl = 0;
	PF_Rect halfsies = { 0,0,0,0 };
	PF_ParamDef checkout;
	PF_ChannelDesc desc;
	PF_ChannelRef ref;
	PF_ChannelChunk chunk;
	PF_Boolean found_depthPB;

	AEFX_CLR_STRUCT(checkout);

	if (in_data->appl_id != 'PrMr') {
		AEFX_SuiteScoper<PF_ChannelSuite1> csP = AEFX_SuiteScoper<PF_ChannelSuite1>(	in_data,
																						kPFChannelSuite1,
																						kPFChannelSuiteVersion1,
																						out_data);
		ERR(csP->PF_GetLayerChannelCount(	in_data->effect_ref,
											0,
											&num_channelsl));

		if (num_channelsl) {
			ERR(csP->PF_GetLayerChannelTypedRefAndDesc(	in_data->effect_ref,
														0,
														PF_ChannelType_DEPTH,
														&found_depthPB,
														&ref,
														&desc));

			ERR(csP->PF_CheckoutLayerChannel(	in_data->effect_ref,
												&ref,
												in_data->current_time,
												in_data->time_step,
												in_data->time_scale,
												desc.data_type,
												&chunk));

			ERR(csP->PF_CheckinLayerChannel(	in_data->effect_ref,
												&ref,
												&chunk));
		}
	}

	int w_div = params[W_FRAME]->u.sd.value;
	int h_div = params[H_FRAME]->u.sd.value;

	halfsies.top = halfsies.left = 0;
	halfsies.right = (short)(output->width / w_div);
	halfsies.bottom = (short)(output->height / h_div);

	int w_save = halfsies.right;
	int h_save = halfsies.bottom;

	ERR(PF_CHECKOUT_PARAM(	in_data,
							CHECK_LAYER,
							(in_data->current_time + params[CHECK_FRAME]->u.sd.value * in_data->time_step),
							in_data->time_step,
							in_data->time_scale,
							&checkout));

	if (!err) {
		if (checkout.u.ld.data) {
			ERR(PF_COPY(&checkout.u.ld,
				output,
				NULL,
				&halfsies));
		}
		else {
			// if there is no layer, Zero-alpha black
			ERR(PF_FILL(NULL, &halfsies, output));
		}

		for (int k = 1; k < h_div; k++) {
			halfsies.top = halfsies.bottom;
			halfsies.bottom = halfsies.top + h_save;

			if (!err) {
				ERR(PF_COPY(&params[CHECK_INPUT]->u.ld,
					output,
					NULL,
					&halfsies));
			}

		}

		halfsies.top = 0;
		halfsies.bottom = h_save;

		for (int i = 1; i < w_div; i++) {
			halfsies.left = halfsies.right;
			halfsies.right += w_save;

			if (!err) {

				ERR(PF_COPY(&params[CHECK_INPUT]->u.ld,
					output,
					NULL,
					&halfsies));
			}

			for (int k = 1; k < h_div; k++) {
				halfsies.top = halfsies.bottom;
				halfsies.bottom = halfsies.top + h_save;

				if (!err) {
					ERR(PF_COPY(&params[CHECK_INPUT]->u.ld,
						output,
						NULL,
						&halfsies));
				}

			}

			halfsies.top = 0;
			halfsies.bottom = h_save;
		}
	}

	ERR2(PF_CHECKIN_PARAM(in_data, &checkout));

	return err;
}

static PF_Err About(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_SPRINTF(out_data->return_msg,
		"%s, v%d.%d\r%s",
		NAME,
		MAJOR_VERSION,
		MINOR_VERSION,
		DESCRIPTION);

	return PF_Err_NONE;
}

static PF_Err PopDialog(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_Err err = PF_Err_NONE;

	PF_SPRINTF(out_data->return_msg,
		"This is test dialog\r Can you see me?");
	out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;

	return err;
}

extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite * inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"splitter", // Name
		"px1 splitter", // Match Name
		"px1 Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}

PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err		err = PF_Err_NONE;

	try {
		switch (cmd) {
		case PF_Cmd_ABOUT:
			err = About(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = GlobalSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_RENDER:
			err = Render(in_data, out_data, params, output);
			break;
		case PF_Cmd_DO_DIALOG:
			err = PopDialog(in_data, out_data, params, output);
			break;
		default:
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		// Never EVER throw exceptions into AE.
		err = thrown_err;
	}
	return err;
}
