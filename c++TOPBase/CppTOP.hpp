//
//  PixelSortTOP.hpp
//  PixelSortTOP
//
//  Created by Dominique Dautheribes on 2019-08-13.
//  Copyright © 2019 www.petitsapin.com - Dominique Dautheribes. All rights reserved.
//

#ifndef CppTOP_hpp
#define CppTOP_hpp

#include "TOP_CPlusPlusBase.h"

class CppTOP : public TOP_CPlusPlusBase
{
public:
    CppTOP( const OP_NodeInfo* info );
    virtual ~CppTOP();
    
    virtual void    getGeneralInfo( TOP_GeneralInfo*, const OP_Inputs*, void* ) override;
    virtual void    execute( TOP_OutputFormatSpecs*, const OP_Inputs*, TOP_Context*, void* reserved1 ) override;
	virtual void	setupParameters( OP_ParameterManager* manager, void* reserver1 ) override;
	virtual void	pulsePressed( char const* name, void* reserved1 ) override;

private:
	float	m_mult;
};

#endif /* CppTOP_hpp */
