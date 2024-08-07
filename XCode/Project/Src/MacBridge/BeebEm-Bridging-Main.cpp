//
//  BeebEm-Bridging-Main.cpp
//  BeebEm
//
//  Created by Commander Coder on 10/04/2024.
//
#include "beebemrcids.h"


//main.cpp
int mainInit();
int mainStep();
int mainEnd();

#include <stdio.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <windows.h>
#include "Main.h"

extern "C"
int beeb_main(long argc, char* argv[])
{
	// NEED TO TURN OFF SANDBOXING IN ENTITLEMENTS FILE TO GET LOCAL FOLDERS TO WORK
	// DON'T TURN OFF SANDBOXING - THIS IS UNSAFE - ALLOW USER FOLDERS

	__argc = (int)argc;
	__argv = argv;

	// report the args
	int i;
	for (i = 0; i < __argc; ++i)
		fprintf(stderr, "Arg %d = %s\n", i, __argv[i]);

// show the path
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		printf("Current working dir: %s\n", cwd);
	} else {
		perror("getcwd() error");
	}

	return ( mainInit() );
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


extern "C" void beeb_Destroy()
{
	mainWin->Shutdown();
}

extern "C" void beeb_HandleCommand(unsigned int cmdID)
{
	mainWin->HandleCommand(ConvID2RC(cmdID));
}

#include <queue>
#include <mutex>


std::queue< std::tuple<UINT,int,long> > buffer;
std::mutex mtx;

extern "C" void beeb_consumer() {
  std::unique_lock<std::mutex> lock(mtx);
  if (buffer.empty()) {return;}
  auto data = buffer.front();
  buffer.pop();
   
  mainWin->AppProc(std::get<0>(data), std::get<1>(data), std::get<2>(data));
   
  lock.unlock();
}
