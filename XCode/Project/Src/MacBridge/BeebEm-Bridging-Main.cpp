//
//  BeebEm-Bridging-Main.cpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//

#include "BeebEm-Bridging-Main.hpp"


//main.cpp
int mainInit();
int mainStep();
int mainEnd();

#include <stdio.h>
#include <sys/syslimits.h>
#include <unistd.h>

extern int __argc;
extern char** __argv;

extern "C"
int beeb_main(long argc, char* argv[])
{
	// NEED TO TURN OFF SANDBOXING IN ENTITLEMENTS FILE TO GET LOCAL FOLDERS TO WORK
	// DON'T TURN OFF SANDBOXING - THIS IS UNSAFE - ALLOW USER FOLDERS

	__argc = (int)argc;
	__argv = argv;


// show the path
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	} else {
		perror("getcwd() error");
	}

	return (  mainInit() );
}



extern "C" void beeb_MainCpuLoop()
{
	mainStep();
}

extern "C" int beeb_end()
{
	fprintf(stderr, "Shutting Down ...\n");

	mainEnd();
	
	return(0);
} /* main */
