//
//  BeebEm-Bridging-Video.hpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//

#ifndef BeebEm_Bridging_Video_hpp
#define BeebEm_Bridging_Video_hpp

#include <stdio.h>

// Exposed to Renderer
struct CColour{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

void beeb_video(long height, long width, struct CColour buffer[]);


#endif /* BeebEm_Bridging_Video_hpp */
