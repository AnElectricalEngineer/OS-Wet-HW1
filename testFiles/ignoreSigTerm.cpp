#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main()
{	
	struct sigaction act = {0};
	act.sa_handler = SIG_IGN;
        sigaction(SIGTERM, &act, NULL);
	
	

	int i = 0, delay = 1, count = 10000;
	
	for(; i < count; i++)
	{
		if(i % 5 == 0)
		{
			printf("%d\n", i);
		}
		sleep(delay);
	}
	return 0;
}

