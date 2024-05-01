//
//  BeebEm-Bridging-ExportFIles.cpp
//  BeebEm
//
//  Created by Commander Coder on 21/04/2024.
//

#include "BeebEm-Bridging-ExportFIles.hpp"
#include "beebemrcids.h"

#include "ExportFileDialog-mac.hpp"

// Dummy functions for building the BeebEm shim
	
extern "C" void beeb_ModalOK(Dialog* d)
{
	if (d!=NULL)
	{
		d->WM_COMMAND(IDOK);
	}
}
