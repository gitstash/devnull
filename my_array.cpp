#include <iostream>
using namespace std;
template<typename T>
class Array
{
    private:
        int mSize;
        T *mArray;
    public:
        Array(int size):mSize(size)
        {
            try{
                mArray = new T[size];
            }catch(bad_alloc &ba){
                cout << "Bad alloc, exiting..." << endl;
            }
        }
        ~Array()
        {
            delete [] mArray;
        }
        //Iterator class goes in here 
        class My_iterator
        {
            private:
                T *mCurPtr;
                int mCurIdx;
                int mMaxIdx;
            public:
                //c'tor
                My_iterator(T* curPtr, int curIdx, int maxIdx) : mCurPtr(curPtr), mCurIdx(curIdx), mMaxIdx(maxIdx)
                {}
                ~My_iterator()
                {}
                //will be doing *itr, lets overload * operator
                const T& operator*()
                {
                    return *mCurPtr;
                }
                //will be doing itr++, lets overload post increment oprtr
                My_iterator operator++(int UNUSED)
                {
                    while(*mCurPtr == *(mCurPtr + 1) && mCurIdx < mMaxIdx)
                    {
                        mCurPtr++;
                        mCurIdx++;
                    }
                    My_iterator itr = *this;
                    mCurPtr++;
                    mCurIdx++;
                    return itr;
                }
                //might need to do ++itr, lets overload pre increment oprtr
                My_iterator operator++()
                {
                    ++mCurPtr;
                    ++mCurIdx;
                    return *this;
                }
                bool operator==(My_iterator rhs)
                {
                    return mCurPtr == rhs.mCurPtr;
                }
                // itr != arr.end() required, lets overload != operator
                bool operator!=(My_iterator rhs)
                {
                    return mCurPtr != rhs.mCurPtr ;
                }
                //implement hasNext function
                bool hasNext()
                {
                    //Assumption: checks if the array has next unique element or not
                    while(*mCurPtr == *(mCurPtr + 1)  && (mCurIdx < mMaxIdx))
                    {
                        mCurPtr++;
                        mCurIdx++;
                    }
                    return (mCurIdx < mMaxIdx);
                }
                // Returns the next element. After this call mCurPtr  should point to next element
                T next()
                {
                    if(mCurIdx == mMaxIdx)
                    {
                        cout << "Already at end, returning the current value instead " << endl;
                        // TODO there should be special value to indicate end of the array ( its tricky given array is generic template type)
                        // or the next function prototype should be changed 
                        return *mCurPtr;
                    }
                    while(*mCurPtr == *(mCurPtr + 1)  && (mCurIdx < mMaxIdx))
                    {
                        mCurPtr++;
                        mCurIdx++;
                    }
                    mCurPtr++;
                    mCurIdx++;
                    return *mCurPtr; 
                }


        };
        //implement member functions of Array
        //need arr.begin() , lets implement begin function
        My_iterator begin()
        {
            return My_iterator(mArray, 0, mSize - 1);
        }
        //need arr.end(), lets implement end function
        My_iterator end()
        {
            return My_iterator(mArray+mSize, mSize, mSize - 1); // max valid idx is size -1 
        }
        // Need to access element using [], lets overload that
        T& operator[](const int& idx)
        {
            return *(mArray + idx);
        }

};

int main(void)
{
 #if 0
    // Test 1. Only two elements and repeat. hasNext should return false ( no next unique element after first )
    Array<int> arr(2);
    arr[0] = 2;
    arr[1] = 2;
    Array<int>::My_iterator itr = arr.begin();
    cout << "Has next(Unique)? " << (itr.hasNext() ? "yes" : "No") << endl;
#endif

#if 0
    // Test 2. Only two elements and unique. hasNext should return true 
    Array<int> arr(2);
    arr[0] = 2;
    arr[1] = 3;
    Array<int>::My_iterator itr = arr.begin();
    cout << "Has next(Unique)? " << (itr.hasNext() ? "yes" : "No") << endl;
    cout << "Print next element " << endl;
    cout << itr.next() << endl;
    //hasNext should return false 
    cout << "Has next(Unique)? " << (itr.hasNext() ? "yes" : "No") << endl;
#endif

#if 1
    //Test 3. Sample data provided in Question 
    Array<int> arr(12);
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 3;
    arr[4] = 3;
    arr[5] = 4;
    arr[6] = 4;
    arr[7] = 10;
    arr[8] = 13;
    arr[9] = 15;
    arr[10] = 15;
    arr[11] = 17;
    Array<int>::My_iterator itr = arr.begin();
    cout << "Current element " << *itr << endl;
    cout << "Has next(Unique)? " << (itr.hasNext() ? "yes" : "No") << endl;
#endif
    cout << "Print next element " << endl;
    cout << itr.next() << endl;
    cout << "Print next element " << endl;
    cout << itr.next() << endl;
    //returns unique elements only
    cout << "Printing current through last element " << endl;
    for(itr;itr!=arr.end(); itr++){
        cout << *itr << " " ;
    }
    cout << endl;
}
