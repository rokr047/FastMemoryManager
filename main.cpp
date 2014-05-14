#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "FMManager.h"

#define NUM_ALLOC 2000
#define DEFAULT_SIZE 1024

#define RUN_FMM_TEST 1
#define RUN_NEW_TEST 1
#define DEBUG 1

using namespace std;

unsigned g_BufferSize;
unsigned g_NumOfAlloc;
clock_t g_StartTime,g_EndTime;

class testData
{
public:
    int id;
    int random1;
    int random2;

    testData()
    {
        this->id = 0;
        this->random1 = 0;
        this->random2 = 0;
    }

    ~testData()
    {

    }

    void setData(int id)
    {
        this->id = id;
        this->random1 = id << 14;
        this->random2 = id << 4;
    }
};

int main(int argc, char* argv[])
{
    cout << "usage : FMManager -s <bufferSize> -n <numOfAllocations>\n\n" ;
    g_BufferSize = DEFAULT_SIZE;
    g_NumOfAlloc = NUM_ALLOC;

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

    cout << "dbg :: Size of the testData block :: " << sizeof(testData);
    cout << "\ndbg :: Buffer Capacity :: " << td_FFM->Capacity();
    cout << "\ndbg :: Number of objects currently in buffer :: " << td_FFM->Count();
    cout << "\ndbg :: Is buffer empty :: " << (td_FFM->IsEmpty() ? "Yes" : "No");
    cout << "\ndbg :: Is buffer full :: " << (td_FFM->IsFull() ? "Yes" : "No");
    cout << "\n\n";

    testData * s_Objs[g_NumOfAlloc];
    double elapsedTime;

#if RUN_NEW_TEST
    g_StartTime = clock();

    for(unsigned i=0;i < g_NumOfAlloc;i++)
    {
        s_Objs[i] = new testData();
        s_Objs[i]->setData(i);
    }

    #if DEBUG
    for(unsigned i=0;i < g_NumOfAlloc;i++)
    {
        cout << "obj "<<i<< "--id "<<s_Objs[i]->id << " random1 "<<s_Objs[i]->random1 << " random2 "<<s_Objs[i]->random2<<endl;
    }
    #endif // DEBUG

    for(unsigned i=0;i < g_NumOfAlloc; i++)
    {
        delete s_Objs[i];
    }

    g_EndTime = clock();

    elapsedTime= (double)(g_EndTime - g_StartTime) / CLOCKS_PER_SEC;

	cout << "\nstat :: Time taken for default new allocations to complete: " << elapsedTime << " seconds\n";
#endif // RUN_NEW_TEST



#if RUN_FMM_TEST
	g_StartTime = clock();

    cout << "Capacity : " << td_FFM->Capacity() << " #ofAlloc : " << g_NumOfAlloc << endl;
	assert(td_FFM->Capacity() >= g_NumOfAlloc);

    cout << "Adding elements...";
    for(unsigned i=0;i < g_NumOfAlloc;i++)
    {
        s_Objs[i] = td_FFM->Add();
        s_Objs[i]->setData(i);
    }
    cout << "Done.\n";

    #if DEBUG
    for(unsigned i=0;i < g_NumOfAlloc;i++)
    {
        cout << "obj "<<i<< "--id "<<s_Objs[i]->id << " random1 "<<s_Objs[i]->random1 << " random2 "<<s_Objs[i]->random2<<endl;
    }
    #endif // DEBUG

    cout << "\nRemoving elements...";
    for(unsigned i=0;i < g_NumOfAlloc; i++)
    {
        td_FFM->Remove(s_Objs[i]);
    }
    cout << "Done.";

    g_EndTime = clock();

    elapsedTime = (double)(g_EndTime - g_StartTime) / CLOCKS_PER_SEC;

	cout << "\nstat :: Time taken for FMManager allocations to complete: " << elapsedTime << " seconds\n";
#endif // RUN_FMM_TEST

    return 0;
}
