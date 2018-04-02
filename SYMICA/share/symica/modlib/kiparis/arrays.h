#pragma once
#ifndef _ARRAYS_H_
#define _ARRAYS_H_

#include <string.h>
#include "kiparis/type.h"
#include "kiparis/struct.h"

class KipMatrix
{
  public:

    int32 razmMatrx;
    REAL **ppKMatr;

    KipMatrix(int32 _razmMatrx);
    ~KipMatrix();
};

class CDoubleArr
{
  public:
	 CDoubleArr() : pArr(0), RealSize(0), Size(0), GrowBy(100) {}
    ~CDoubleArr(){ delete []pArr; pArr = 0; RealSize = 0; Size = 0; GrowBy = 100;}

    REAL *pArr;
    int32 RealSize;
    int32 Size;
    int32 GrowBy;

    inline int32 GetSize() const { return RealSize; }
    inline void SetSize(int32 sz)
    {
      Size = RealSize = sz;
      pArr = new REAL[Size];
      memset(pArr,0,Size*sizeof(REAL));
    }

    void Add(REAL D);
    inline REAL GetAt(int32 index) const { return pArr[index]; }
    inline REAL operator[](int32 index) const { return GetAt(index); }
    inline void RemoveLast() { RealSize--; }
    inline void RemoveAll() { delete []pArr; pArr = 0; RealSize = 0; Size = 0; }
    inline void RemoveAllCl() { RealSize = 0; }
    inline void SetAt(int32 index,REAL D){ pArr[index] = D; }
    void Copy(CDoubleArr *pSArr,REAL D);
    void Copy(CDoubleArr *pSArr);
    inline void RemoveAt(int32 index)
    {
      int MoveCount = RealSize - index - 1;
      if(MoveCount != 0)
        memmove(&pArr[index],&pArr[index + 1],MoveCount*sizeof(REAL));
      RealSize -= 1;
    }

    inline void InsertAt(int32 nIndex,REAL Element)
    {
      if(RealSize+1 > Size)
      {
        Size += GrowBy;
        REAL *pNewArr = new REAL[Size];
        memcpy(pNewArr,pArr,RealSize*sizeof(REAL));
        delete []pArr;
        pArr = pNewArr;
      }

      memmove(&pArr[nIndex+1],&pArr[nIndex],(RealSize-nIndex)*sizeof(REAL));
      pArr[nIndex] = Element;
      RealSize++;
    }

    inline REAL* GetData() { return pArr; }
};

class CIntArr
{
  public:
	CIntArr() : pArr(0), RealSize(0), Size(0), GrowBy(100) {}
    ~CIntArr(){ delete []pArr; pArr = 0; RealSize = 0; Size = 0; GrowBy = 100;}

    int32 *pArr;
    int32 RealSize;
    int32 Size;
    int32 GrowBy;

    inline int32 GetSize() const { return RealSize; }

    void Add(int32 D);
    inline int32 GetAt(int32 index) const { return pArr[index]; }
    inline int32 operator[](int32 index) const { return GetAt(index); }
    inline void RemoveLast(){RealSize--;}
    inline void RemoveAll() { delete []pArr; pArr = 0; RealSize = 0; Size = 0; }
    inline void RemoveAllCl() { RealSize = 0; }
    inline void SetAt(int32 index,int32 D){pArr[index] = D;}
    inline void RemoveAt(int32 index,int32 Count)
    {
      int MoveCount = RealSize - index - Count;
      if(MoveCount != 0)
        memmove(&pArr[index],&pArr[index + Count],MoveCount*sizeof(int32));
      RealSize -= Count;
    }

    void Copy(CIntArr *pSArr,int32 D);
    void Copy(CIntArr *pSArr);
    void CopyRS(CIntArr *pSArr);
    inline void CopyEqu(CIntArr *pSArr)
    {
      RealSize = pSArr->RealSize;
      memcpy(pArr,pSArr->pArr,RealSize*sizeof(int32));
    }

    inline void InsertAt(int32 nIndex,int32 Element)
    {
      if(RealSize+1 > Size)
      {
        Size += GrowBy;
        int32 *pNewArr = new int32[Size];
        memcpy(pNewArr,pArr,RealSize*sizeof(int32));
        delete []pArr;
        pArr = pNewArr;
      }

      memmove(&pArr[nIndex+1],&pArr[nIndex],(RealSize-nIndex)*sizeof(int32));
      pArr[nIndex] = Element;
      RealSize++;
    }

    inline void InsertAtCl(int32 nIndex,int32 Element,int32 rs)
    {
      memmove(&pArr[nIndex+1],&pArr[nIndex],(rs-nIndex)*sizeof(int32));
      pArr[nIndex] = Element;
    }

    void Append(CIntArr *pSourArr);

    inline void SetSize(int32 sz)
    {
      Size = RealSize = sz;
      pArr = new int32[Size];
      memset(pArr,0,Size*sizeof(int32));
    }

    inline void SetSizeCapacity(int32 sz)
    {
      Size = sz;
      pArr = new int32[Size];
      memset(pArr,0,Size*sizeof(int32));
    }
};

class CPtrArr
{
  public:
	CPtrArr() : ppArr(0), RealSize(0), Size(0), GrowBy(100) {}
    ~CPtrArr(){ delete [](char*)ppArr; ppArr = 0; RealSize = 0; Size = 0; GrowBy = 100;}

    void **ppArr;
    int32 RealSize;
    int32 Size;
    int32 GrowBy;

    inline int32 GetSize() const { return RealSize; }
    inline void SetSize(int32 sz)
    {
      Size = RealSize = sz;
      ppArr = (void**)new char[Size*sizeof(void*)];
      memset(ppArr,0,Size*sizeof(void*));
    }

    void Add(void *pE);
    inline void* GetAt(int32 index) { return ppArr[index]; }
    inline void* operator[](int32 index) { return GetAt(index); }
    inline void RemoveLast(){RealSize--;}
    inline void RemoveAll() { delete [](char*)ppArr; ppArr = 0; RealSize = 0; Size = 0; }
    inline void RemoveAllCl() { RealSize = 0; }
    inline void SetAt(int32 index,void *pE){ppArr[index] = pE;}
    inline void RemoveAt(int32 index)
    {
      int MoveCount = RealSize - index - 1;
      if(MoveCount != 0)
        memmove(&ppArr[index],&ppArr[index+1],MoveCount*sizeof(void*));
      RealSize -= 1;
    }

    inline void InsertAt(int32 nIndex,void *pE)
    {
      if(RealSize+1 > Size)
      {
        Size += GrowBy;
        void **ppNewArr = (void**)new char[Size*sizeof(void*)];
        memcpy(ppNewArr,ppArr,RealSize*sizeof(void*));
        delete [](char*)ppArr;
        ppArr = ppNewArr;
      }

      memmove(&ppArr[nIndex+1],&ppArr[nIndex],(RealSize-nIndex)*sizeof(void*));
      ppArr[nIndex] = pE;
      RealSize++;
    }

    inline void InsertAtCl(int32 nIndex,void *pE,int32 rs)
    {
      memmove(&ppArr[nIndex+1],&ppArr[nIndex],(rs-nIndex)*sizeof(void*));
      ppArr[nIndex] = pE;
    }

    void Copy(CPtrArr *pSArr);
    void CopyRS(CPtrArr *pSArr);

    void Append(CPtrArr *pSourArr);
};


////////
class CIntNode;

class CIntList
{
  public:

    CIntList();
    ~CIntList();

    void Add(int32 Ind);

    CIntNode *pFirst;
    CIntNode *pLast;
};

class CIntNode
{
  public:

    CIntNode(int32 _Ind);
    ~CIntNode();

    CIntNode *pNext;

    int32 Ind;
};


////////
template<typename T>
class CPtrNodeKip
{
  public:

    CPtrNodeKip()
    {
      NKu.NKInd = 0;
      NKu.pNKPtr = 0;
      NKu.NKR = 0.0;

      pPrev = 0;
      pNext = 0;
    }
    ~CPtrNodeKip()
    {
    }

    union
    {
      int NKInd;
      T *pNKPtr;
      REAL NKR;
    } NKu;

    CPtrNodeKip<T> *pPrev;
    CPtrNodeKip<T> *pNext;
};

template<typename T>
class CPtrListKip
{
  public:

    CPtrListKip()
    {
      PtrListSz = 0;
      pFirst = 0;
      pLast = 0;
    }
    ~CPtrListKip()
    {
      RemoveNKAll();
    }

    CPtrNodeKip<T>* AddNK(int32 NKInd)
    {
      CPtrNodeKip<T> *pN = new CPtrNodeKip<T>;
      pN->NKu.NKInd = NKInd;

      pN->pPrev = pLast;

      if(pLast != 0)
      {
        pLast->pNext = pN;
        pLast = pN;
      }
      else
      {
        pFirst = pLast = pN;
      }

      PtrListSz++;

      return pN;
    }

    CPtrNodeKip<T>* AddNK(T *pV)
    {
      CPtrNodeKip<T> *pN = new CPtrNodeKip<T>;
      pN->NKu.pNKPtr = pV;

      pN->pPrev = pLast;

      if(pLast != 0)
      {
        pLast->pNext = pN;
        pLast = pN;
      }
      else
      {
        pFirst = pLast = pN;
      }

      PtrListSz++;

      return pN;
    }
    CPtrNodeKip<T>* AddNK(REAL R)
    {
      CPtrNodeKip<T> *pN = new CPtrNodeKip<T>;
      pN->NKu.NKR = R;

      pN->pPrev = pLast;

      if(pLast != 0)
      {
        pLast->pNext = pN;
        pLast = pN;
      }
      else
      {
        pFirst = pLast = pN;
      }

      PtrListSz++;

      return pN;
    }

    void RemoveNK(CPtrNodeKip<T> *pN)
    {
      if(pN->pPrev != 0)
        pN->pPrev->pNext = pN->pNext;

      if(pN->pNext != 0)
        pN->pNext->pPrev = pN->pPrev;

      if(pN == pFirst)
        pFirst = pN->pNext;

      if(pN == pLast)
        pLast = pN->pPrev;

      delete pN;

      PtrListSz--;
    }
    void RemoveNKAll()
    {
      int32 i = 0;

      CPtrNodeKip<T> *pN = pFirst;
      CPtrNodeKip<T> *pNext = 0;

      for(i=0;i < PtrListSz;i++)
      {
        pNext = pN->pNext;
        delete pN;
        pN = pNext;
      }

      PtrListSz = 0;
      pFirst = 0;
      pLast = 0;
    }

    int32 GetSize() const
    {
      return PtrListSz;
    }

    int32 PtrListSz;

    CPtrNodeKip<T> *pFirst;
    CPtrNodeKip<T> *pLast;
};

class MapPtrToPtrHelper
{
  public:

    MapPtrToPtrHelper(void *p1,void *p2);
    ~MapPtrToPtrHelper();

    void *pPtr1;
    void *pPtr2;
};

class MapPtrToPtr
{
  public:

    MapPtrToPtr();
    ~MapPtrToPtr();

    void AddPtrs(void *p1,void *p2);

    void RemovePtrsBy1(void *p1);
    void RemovePtrsBy2(void *p2);

    void RemoveAll();

    void* GetPtr1ByPtr2(void *p2);
    void* GetPtr2ByPtr1(void *p1);

    int32 GetSize() const;
    void* GetAtPtr1(int32 i);
    void* GetAtPtr2(int32 i);

    void ChangePtr1(void *p1Old,void *p1New);
    void ChangePtr2(void *p2Old,void *p2New);

    CPtrArr MapArr;
};

#endif // _ARRAYS_H_
