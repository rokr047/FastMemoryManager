#include <iostream>
#include <new>
#include <assert.h>

#define DEBUG 0

using namespace std;

template<typename T>
class FMManager
{
private:
    char* bufferStart;

public:
    FMManager();
    FMManager(int bufferSize);
    ~FMManager();

    char* GetBufferStart();
};

/** \brief
 *  FMManager default constructor
 *  Create the FMManager object. Initialize buffer to default size
 */
template<typename T>
FMManager<T>::FMManager()
{
    bufferStart =  NULL;
    cout << "\n\nERROR : Using the incorrect FMManager Constructor!!!\n";
}

/** \brief
 *  FMManager parameterized constructor
 *  Create the FMManager object. Initialize buffer to size provided as argument.
 * \param
 *  bufferSize - The size of the buffer that needs to be allocated.
 */
template<typename T>
FMManager<T>::FMManager(int bufferSize)
{
    bufferStart = (char*)malloc(sizeof(bufferSize * sizeof(char)));
    assert(bufferStart != NULL);
}

template<typename T>
char* FMManager<T>::GetBufferStart()
{
    return bufferStart;
}
