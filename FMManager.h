#include <iostream>
#include <new>
#include <assert.h>

using namespace std;

typedef struct tag_Header
{
  char* bufferStart;
  unsigned bufferSize;
  void* head;
  unsigned numOfFreeBlocks;
  unsigned numOfAllocatedBlocks;
}Header;

typedef struct tag_Bookmark
{
    void* nextEmptyBlock;
}Bookmark;

template<typename T>
class FMManager
{
private:
    Header* m_head;
public:
    FMManager();
    FMManager(unsigned bufferSize);
    ~FMManager();

    char* GetBufferStart();
    T *	Add();
    void Remove(T *);
    unsigned Count();
	unsigned Capacity();

	bool IsEmpty();
	bool IsFull();
};

/** \brief
 *  FMManager default constructor
 *  Create the FMManager object. Initialize buffer to default size
 */
template<typename T>
FMManager<T>::FMManager()
{
    m_head->bufferStart =  NULL;
    m_head->head = NULL;
    cout << "\n\nERROR : Using the incorrect FMManager Constructor!!!\n";
}

/** \brief
 *  FMManager parameterized constructor
 *  Create the FMManager object. Initialize buffer to size provided as argument.
 * \param
 *  bufferSize - The size of the buffer that needs to be allocated.
 */
template<typename T>
FMManager<T>::FMManager(unsigned bufferSize)
{
    assert(sizeof(Bookmark) <= sizeof(T));

    m_head = (Header*)malloc(sizeof(Header));
    assert(m_head != NULL);

    m_head->bufferSize = bufferSize;
    m_head->bufferStart = (char*)malloc(sizeof(bufferSize * sizeof(char)));
    assert(m_head->bufferStart != NULL);

    m_head->numOfAllocatedBlocks = 0;
    m_head->numOfFreeBlocks = 0 ;
    m_head->head = m_head->bufferStart;
}

template<typename T>
FMManager<T>::~FMManager()
{
    m_head->bufferStart = NULL;
    delete m_head->bufferStart;

    m_head->head = NULL;
    delete m_head->head;

    m_head = NULL;
    delete m_head;
}

/** \brief
 *  Returns the start pointer of the buffer as char pointer.
 * \return
 *  char*
 */

template<typename T>
char* FMManager<T>::GetBufferStart()
{
    return m_head->bufferStart;
}

template<typename T>
unsigned FMManager<T>::Count()
{
    return m_head->numOfAllocatedBlocks;
}

template<typename T>
unsigned FMManager<T>::Capacity()
{
    return m_head->bufferSize/sizeof(T) ;
}

template<typename T>
bool FMManager<T>::IsEmpty()
{
    return (m_head->numOfAllocatedBlocks == 0);
}

template<typename T>
bool FMManager<T>::IsFull()
{
    return (m_head->numOfAllocatedBlocks == (m_head->bufferSize/sizeof(T)));
}

template <typename T>
T* FMManager<T>::Add()
{
    void* result = m_head->head;

    assert(m_head->numOfFreeBlocks >= 0);

    if(m_head->numOfFreeBlocks == 0)
    {
        /**< There are no empty blocks. Allocate the next available space and return */
        result = m_head->head;
        m_head->head = (void*)((char*)m_head->head + sizeof(T));
    }
    else
    {
        /**< There are empty blocks. So allocate at head and update head to next block. */
        Bookmark* curBlock = ((Bookmark*)(m_head->head));
        m_head->head = curBlock->nextEmptyBlock;
        m_head->numOfFreeBlocks--;
    }

    m_head->numOfAllocatedBlocks++;
    assert(result != NULL);
    return new(result) T();
}

template <typename T>
void FMManager<T>::Remove(T* obj)
{
    assert(obj != NULL);
    assert(m_head->numOfAllocatedBlocks >= 0);

    obj->~T();

    Bookmark* bkmk = (Bookmark*)((char*)obj + sizeof(Bookmark));
    bkmk->nextEmptyBlock = m_head->head ;
    m_head->head = (void*)bkmk;

    m_head->numOfAllocatedBlocks--;
    m_head->numOfFreeBlocks++;
}
