//
//  PixelSortTOP.cpp
//  PixelSortTOP
//
//  Created by Dominique Dautheribes on 2019-08-13.
//  Copyright © 2019 www.petitsapin.com - Dominique Dautheribes. All rights reserved.
//

#include <iostream>
#include "CppTOP.hpp"

PixelSortTOP::PixelSortTOP( const OP_NodeInfo* ) : m_mult(1.0f) {}

PixelSortTOP::~PixelSortTOP() {}

void PixelSortTOP::getGeneralInfo(TOP_GeneralInfo* ginfo, const OP_Inputs* inputs, void* reserved1 )
{
	ginfo->cookEveryFrame = true;
	ginfo->memPixelType = OP_CPUMemPixelType::RGBA32Float;
}

void PixelSortTOP::execute( TOP_OutputFormatSpecs* outputFormat, const OP_Inputs* inputs, TOP_Context* context, void *reserved1)
{
	int memLocation = 0;
	float*	mem = (float*)outputFormat->cpuPixelData[memLocation];
	OP_TOPInput const* input_ = inputs->getInputTOP(0);
	
	if (input_ != nullptr)
	{
		OP_TOPInputDownloadOptions options;
		options.downloadType = OP_TOPInputDownloadType::Instant;
		options.cpuMemPixelType = OP_CPUMemPixelType::RGBA32Float;
		options.verticalFlip = false;
		
		float const* src = (float const*)inputs->getTOPDataInCPUMemory(input_, &options);

		if (src != nullptr)
		{
			m_mult = (float)inputs->getParDouble("Color");
			for (int y = 0; y < input_->height; y++)
			{
				for (int x = 0; x < input_->width; x++)
				{
					int pixelIndex = (y * input_->width + x) * sizeof(float);
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

void	PixelSortTOP::setupParameters( OP_ParameterManager* manager, void* reserver1 )
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

void	PixelSortTOP::pulsePressed( char const* name, void* reserved1 )
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
		return new PixelSortTOP(info);
	}
	
	DLLEXPORT void DestroyTOPInstance( TOP_CPlusPlusBase* instance, TOP_Context* context )
	{
		delete (PixelSortTOP*)instance;
	}
}


