//
//  PixelSortTOP.cpp
//  PixelSortTOP
//
//  Created by Dominique Dautheribes on 2019-08-13.
//  Copyright © 2019 www.petitsapin.com - Dominique Dautheribes. All rights reserved.
//

#include <iostream>
#include "CppTOP.hpp"

CppTOP::CppTOP( const OP_NodeInfo* ) : m_mult(1.0f) {}

CppTOP::~CppTOP() {}

void CppTOP::getGeneralInfo(TOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void* reserved1 )
{
	ginfo->cookEveryFrame = true;
	ginfo->memPixelType = OP_CPUMemPixelType::RGBA32Float;
}

void CppTOP::execute( TOP_OutputFormatSpecs* outputFormat, const OP_Inputs* inputs, TOP_Context* context, void *reserved1)
{
	int memLocation = 0;
	float*	mem = (float*)outputFormat->cpuPixelData[memLocation];
	OP_TOPInput const* inputTOP = inputs->getInputTOP(0);
	
	if (inputTOP != nullptr)
	{
		OP_TOPInputDownloadOptions options;
		options.downloadType = OP_TOPInputDownloadType::Instant;
		options.cpuMemPixelType = OP_CPUMemPixelType::RGBA32Float;
		options.verticalFlip = false;
		
		float const* src = (float const*)inputs->getTOPDataInCPUMemory(inputTOP, &options);

		if (src != nullptr)
		{
			m_mult = (float)inputs->getParDouble("Color");
			for (int y = 0; y < inputTOP->height; y++)
			{
				for (int x = 0; x < inputTOP->width; x++)
				{
					int pixelIndex = (y * inputTOP->width + x) * sizeof(float);
					mem[pixelIndex + 0] = src[pixelIndex + 0] * m_mult;
					mem[pixelIndex + 1] = src[pixelIndex + 1] * m_mult;
					mem[pixelIndex + 2] = src[pixelIndex + 2] * m_mult;
					mem[pixelIndex + 3] = src[pixelIndex + 3];
				}
			}
		}
	}
	
	outputFormat->newCPUPixelDataLocation = memLocation;
}

void	CppTOP::setupParameters( OP_ParameterManager* manager, void* reserver1 )
{
	// Colour multiplier
	{
		OP_NumericParameter np;
		np.name = "Color";
		np.label = " Color";
		np.defaultValues[0] = 1.0;
		np.minSliders[0] = 0.0;
		np.maxSliders[0] = 1.0;
		np.minValues[0] = 0.0;
		np.maxValues[0] = 1.0;
		np.clampMins[0] = true;
		np.clampMaxes[0] = true;
		
		OP_ParAppendResult res = manager->appendFloat(np);
		assert(res == OP_ParAppendResult::Success);
	}
}

void	CppTOP::pulsePressed( char const* name, void* reserved1 )
{
}

extern "C"
{
	DLLEXPORT void FillTOPPluginInfo( TOP_PluginInfo* info )
	{
		info->apiVersion = TOPCPlusPlusAPIVersion;
		info->customOPInfo.opType->setString("PixelSort");
		info->customOPInfo.opLabel->setString("Pixel Sort");
		info->customOPInfo.authorName->setString("petitsapin - Dominique Dautheribes");
		info->customOPInfo.authorEmail->setString("@");
		info->customOPInfo.minInputs = 0;
		info->customOPInfo.maxInputs = 1;
		info->executeMode = TOP_ExecuteMode::CPUMemWriteOnly;
	}
	
	DLLEXPORT TOP_CPlusPlusBase* CreateTOPInstance( const OP_NodeInfo* info, TOP_Context* context )
	{
		return new CppTOP(info);
	}
	
	DLLEXPORT void DestroyTOPInstance( TOP_CPlusPlusBase* instance, TOP_Context* context )
	{
		delete (CppTOP*)instance;
	}
}
