#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "FMManager.h"

#define NUM_ALLOC 2000
#define DEFAULT_SIZE 1024

using namespace std;

int g_BufferSize;
int g_NumOfAlloc;
clock_t g_StartTime,g_EndTime;

class testData
{
public:
    int id;
};

int main(int argc, char* argv[])
{
    cout << "usage : FMManager -s <bufferSize> -n <numOfAllocations>\n\n" ;
    g_BufferSize = DEFAULT_SIZE;
    g_NumOfAlloc = NUM_ALLOC;

    g_StartTime = clock();

    cout << "dbg :: Starting FMM with \n\tBufferSize : ";
    for(int i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-s") == 0)
        {
            g_BufferSize = atoi(argv[++i]);
        }

        if(strcmp(argv[i],"-n") == 0)
        {
            g_NumOfAlloc = atoi(argv[++i]);
        }
    }
    cout << g_BufferSize << "\t#ofAllocations : ";
    cout << g_NumOfAlloc << endl;

    cout << "\ndbg :: Allocating Buffer Space...";
    FMManager<testData>* td_FFM = new FMManager<testData>(g_BufferSize);
    assert(td_FFM->GetBufferStart() != NULL);
    cout << "Done!\n";

    g_EndTime = clock();

    double elapsedTime = (double)(g_EndTime - g_StartTime) / CLOCKS_PER_SEC;

	cout << "\nstat :: Time taken for test to complete: " << elapsedTime << " seconds\n";

    return 0;
}
