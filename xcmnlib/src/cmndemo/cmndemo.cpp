/*
 * Copyright (c) 2017-2024 UbVideo
 *
 * The computer program contained herein contains proprietary
 * information which is the property of UbVideo.
 * The program may be used and/or copied only with the written
 * permission of UbVideo or in accordance with the
 * terms and conditions stipulated in the agreement/contract under
 * which the programs have been supplied.
 */
#include "utility/videotype.hpp"
#include "cppkit/os/ck_time_utils.h"
#include "rapidmedia/rapidmedia.hpp"
int main(int argc, char *argv[])
{
	int i = 0;
	RapidMedia rapid;
	rapid.Init(TRUE, "rtsp://192.168.22.134:554/Streaming/Channels/1?transportmode=unicast&profile=Profile_1", "admin", "admin", TRUE);
	rapid.StartRaw();
	while(1)
	{
		cppkit::ck_sleep(10);
		printf("cnt %d\n", i ++);
	}
	while(1)
	{
		
		if (i %2 == 0)
		{
			rapid.Start();
			rapid.StartRaw();
		}else
		{
			rapid.Stop();
			rapid.StopRaw();
		}
		cppkit::ck_sleep(10);
		printf("cnt %d\n", i ++);
	}
}
