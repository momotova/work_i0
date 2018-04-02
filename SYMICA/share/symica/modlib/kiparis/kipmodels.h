#pragma once
#ifndef _KIPMODELS_
#define _KIPMODELS_

#include "compat/compat_str.h"

#include "kiparis/struct.h"
#include "kiparis/type.h"
#include "kiparis/arrays.h"
#include "kiparis/kiparis_math_sse2.h"
#include "kiparis/paramt.h"

#include <utility>
#include <string>
#include <deque>
#include <vector>

#ifdef WIN
#pragma warning ( disable : 4711 )
#endif // WIN

////////  Механизм клонирования и таблиц реализуется по указанию из KIPARIS.

////////  Механизм таблиц автоматически реализуется через внутренний механизм клонирования.
////////  Также в механизме таблиц используется информация с прошлых расчетов для
////////  ускорения создания таблиц. Считывание из файла должно происходить только
////////  если групповые модели не изменились.

class DataDoc;
class CTypeModelInfo;
class CModClone;
class CTableClone;
class CTypeTable;
class PerRateDataDoc;
class PerRateModKip;
class ModKip;
class CTableModelFileBase;
class CTSInfoBase;
class TAB_DIM;
class CTableBase;

#if defined(_WIN32) || defined(__MINGW32__) 
#  define KIP_API_TYPE __stdcall
#else
#  define KIP_API_TYPE
#endif 

#define PARAMETER_ERR               999  // "Parameter incorrect"
#define PARAMETER_ERR_PROC_IN_MOD   -2
#define PARAMETER_ERR_RANGE         998  // "Parameter exceeds allowed limits. Set to lower limit by default"

#define NUMERICAL_DERIV_STEP 1e-12

////////  Константы для лианеризации экспоненты.
////////  34.0 - из BERKLI SPICE.
#define MAXEXP        5.8346174252745e+014
#define MIN_EXP       1.713908431e-15
#define EXP_THRESHOLD 34.0

//#define MAXEXP        5.1847055285871e+021
//#define MIN_EXP       1.9287498479639e-022
//#define EXP_THRESHOLD 50.0

#define TYPEMODEL_CLONE   0x00000001
#define TYPEMODEL_TABLE   0x00000002

//#define MOD_VERSION 250606
//#define MOD_VERSION 210411
//#define MOD_VERSION 011013
#define MOD_VERSION 210114

extern char* GetParamTChar(ParamT *pParT);
extern int32 GetParamTValue(const ParamT *pParT,REAL *destDouble);
extern int32 GetParamValueStr(const _TCHAR *source,REAL *destDouble);

#define SecToNanoSec 1e+9
#define NanoSecToSec 1e-9

typedef std::vector<int32> CNodeVec;
typedef std::pair<int32*,int32> Npair;
typedef std::pair<Npair,CNodeVec> NVpair;
typedef std::vector<NVpair> CNodeVec2;

void UpdateNodes(CNodeVec2 &addrNV,int32 &SIZE_OF_NODES_);

class CKT_OPTIONS
{
  public:

    CKT_OPTIONS()
    {
      temper = 0.0;
      tempNom = 0.0;
      bInputTnom = fALSE;
    }

    REAL temper;
    REAL tempNom;
    bOOL bInputTnom;

    REAL GetTnom(bOOL bSpectreModel)
    {
      if(fALSE == bInputTnom)
      {
        if(tRUE == bSpectreModel)
          return 27.0;
        else
          return 25.0;
      }
      else
        return tempNom;
    }
};

class PotNet
{
  public:

    PotNet();
    ~PotNet();

    void KIP_API_TYPE NewPotNet(int32 NumPot);
    void KIP_API_TYPE DelPotNet();

    REAL *pot;
};

struct ModControlOption
{
  ModControlOption()
  {
    ClearMCO();
  }
  ~ModControlOption()
  {
  }

  void ClearMCO()
  {
    integrationOrderPrev = 0;
    integrationOrder = 0;

    ppMCOToksAllMain = 0;

    MCORHSOnly = fALSE;

    MCOtnpl1 = 0;
    MCOtnplpr1 = 0;
    MCOtnplpr2 = 0;

    MCOStaticaTypeStatica = fALSE;

    MCOH = 0.0;

    MCOIntGKoeff = 0.0;

    MCOintOrdM = 0;

    MCOIsMultiThreadingEnable = fALSE;
  }

  int32 integrationOrderPrev;  //  Нужен для PSS анализа.
  int32 integrationOrder;

  REAL **ppMCOToksAllMain;

  bOOL MCORHSOnly;

  int32 MCOtnpl1;
  int32 MCOtnplpr1;
  int32 MCOtnplpr2;

  bOOL MCOStaticaTypeStatica;

  REAL MCOH;

  REAL MCOIntGKoeff;

  int32 *pMCOtn;
  REAL *pMCOBET;

  int32 MCOintOrdM;

  bOOL MCOIsMultiThreadingEnable;
};

class SubArr
{
  public:

    SubArr(int32 _MaxPinEl,int32 _MaxInnerPinEl,ModKip *_pSAElMod,bOOL YIOnly);
    ~SubArr();

    void KIP_API_TYPE SetZeroSA(bOOL All,bOOL YIOnly);

    void KIP_API_TYPE CopySA(SubArr *pSA,bOOL All,bOOL YIOnly,bOOL NeedIA);

////////  Блок памяти для всех массивов.
    char *pMemSA;

////////
    bOOL SAFilled;

////////
    int32 MaxPinEl;
    int32 MaxInnerPinEl;

    int32 dim;
    int32 dimInerNet;

    REAL **arrY;
    REAL **arrC;

    REAL *colI;
    REAL *colAbsI;

//    REAL *colQ;

    REAL *colY;  // conductance connected to ac source.
    REAL *colC;  // capacitance connected to ac source.(right parts of system with complex variables).
    REAL *colL;  // capacitance connected to ac source.(right parts of system with complex variables).
    REAL *colNd;
    REAL *colNrs;

    int32 *indArr;  //  Массив имен узлов.

    ModKip *pSAElMod;
};

#define PARAM_ERROR_EXCEPTION_CODE   0x20001001  //  2 - 0010 (00 - код тяжести,
                                                 //            1 т.к.- 0 - зарезервирован MICROSOFT,
                                                 //            0 - должен быть 0.)

class CParError
{
  public:

    CParError()
    {
      paramInd = -1;
      isIndGrp = -1;
    }

    int32 paramInd;
    int32 isIndGrp;  //  0 - ind, 1 - grp.
};

class CharStr
{
  public:

  char *pStr;

  CharStr(char *_pStr)
  {
    pStr = new char[strlen(_pStr)+1];
    strcpy(pStr,_pStr);
  }
  ~CharStr()
  {
    delete []pStr;
  }
};

struct ModSimInfo
{
  int32 num;  //  количество  выводов  подсхемы
  REAL *U;  //  массив напряжений на выводах ( входная инфа для кипариса )
  REAL *Q;  //  массив  зарядов на выводах  ( выходная инфа из кипариса )
  REAL *I;  //  массив  токов на выводах  ( выходная инфа из кипариса )
};

///
///  Интерфейс, предоставляющий вспомогательные функции для моделей.

class CKiparisMods
{
  public:

    CKiparisMods(){}
    virtual ~CKiparisMods(){}

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif

///  Вызывается при создании нового типа модели.
///  Регистрирует новый тип модели.
///  Возвращает 0 в случае успеха, иначе не 0.
    virtual int32 KIP_API_TYPE OnInitModelInfo(CTypeModelInfo *pModInfo) = 0;

///  Вызывается при создании новой модели.
///  Регистрирует новую модель.
    virtual void KIP_API_TYPE OnNewModel(int32 ModType,ModKip *pModel) = 0;

///
///  Возвращает численное имя узла по символьному имени.
    virtual int32 KIP_API_TYPE GetExtendNodeByCharName(Element *pEl,char *pNodeName) = 0;

///  Возвращает клон по параметрам модели.
///  Если не находит возвращает 0.
    virtual CModClone* KIP_API_TYPE GetCloneModel(int32 ModType,IndParName *pIndParEl,GrpParName *pGrpParEl) = 0;

///
///  Вызывается при создании новой подмодели. Создает клон.
    virtual CModClone* KIP_API_TYPE SetCloneModel(int32 ModType,IndParName *pIndParEl,GrpParName *pGrpParEl,ModKip *pModel,void *pSubModel) = 0;

///
///  Вызывается при подсоединении новой модели к клону. Извещает клон.
    virtual void KIP_API_TYPE AddCloneModel(CModClone *pCl,ModKip *pModel) = 0;

///
///  Вызывается при отсоединении модели от клона. Извещает клон.
    virtual void KIP_API_TYPE RemoveCloneModel(CModClone *pCl,ModKip *pModel) = 0;

///  Возвращает таблицу по параметрам модели.
///  Если не находит возвращает 0.
    virtual CTableClone* KIP_API_TYPE GetTableData(int32 ModType,IndParName *pIndParEl,GrpParName *pGrpParEl) = 0;

///
///  Вызывается при создании новой подмодели. Создает таблицу.
    virtual CTableClone* KIP_API_TYPE SetTableData(int32 ModType,IndParName *pIndParEl,GrpParName *pGrpParEl,void *pTableData) = 0;

///
///  Возвращает массив внутренних цепей.
    virtual CIntArr* KIP_API_TYPE NewInnerNets(int32 NumNets) = 0;

///
///  Очищает массив внутренних цепей.
    virtual void KIP_API_TYPE ClearInnerNets(CIntArr *pNetsArr) = 0;

///
///  Возвращает настройки.
    virtual CKT_OPTIONS* KIP_API_TYPE GetOption() = 0;

///
///  Возвращает настройки.
    virtual bOOL KIP_API_TYPE ReadOption(const char *name,REAL *dest) = 0;
    virtual REAL KIP_API_TYPE ReadOptionDef(const char *name,REAL def_val) = 0;

///
///  Выдача сообщения.
    virtual void KIP_API_TYPE WriteWarningMsg(int Elind,int ParamInd,int isIndGrp,int StrIndex) = 0;

    virtual void KIP_API_TYPE WriteWarningMsgStr(int Elind,const char *pStr) = 0;
    virtual void KIP_API_TYPE WriteWarningMsgStr2(int Elind,int ParamInd,int isIndGrp,const char *pStr) = 0;

///
///  Выдача сообщения об ошибке.
///  Если инфо о ошибке выдается прямо в модели, то необходимо вернуть -2 из InitParameters.
    virtual void KIP_API_TYPE WriteErrorMsgStr(int Elind,const char *pStr) = 0;
    virtual void KIP_API_TYPE WriteErrorMsgStr2(int Elind,int ParamInd,int isIndGrp,const char *pStr) = 0;

///  Для выдачи сообщений из VerilogA моделей.
    virtual void KIP_API_TYPE WriteWEMsgStr(int Elind,const char *pStr,...) = 0;

////////  Индексы зарядов и емкостей(индуктивностей) идут отдельно.

///
///  Возвращает проводимость и ток по заданной емкости или индуктивности.
    virtual REAL KIP_API_TYPE Integrate(REAL G,PotNet **pPhiNets,ModControlOption *pMCO,int32 NI1,int32 NI2,REAL *pRight,ModKip *pCurMod,int32 Ind) = 0;

///
///  Возвращает коэффициент интегрирования для получения проводимости.
    virtual REAL KIP_API_TYPE GetIntegrateGKoeff(ModControlOption *pMCO,ModKip *pCurMod) = 0;

///
///  Возвращает ток по заданному заряду.
    virtual REAL KIP_API_TYPE IntegrateQ(REAL Q,ModControlOption *pMCO,ModKip *pCurMod,int32 Ind) = 0;
    virtual REAL KIP_API_TYPE IntegrateQInNode(REAL Q,ModControlOption *pMCO,ModKip *pCurMod,int32 NodeInd) = 0;

///
///  Возвращает текущий индекс данных.
    virtual int32 KIP_API_TYPE GetRealTimeInd(int32 UserTimeInd) = 0;

///
///  Возвращает текущую частоту.
    virtual REAL KIP_API_TYPE GetACFreq() = 0;

///
///  Возвращает текущий значение счетчика итераций Ньютона.
    virtual int32 KIP_API_TYPE GetNewtonCounter() = 0;
    virtual int32 KIP_API_TYPE GetAbsNewtonCounter() = 0;

///
///  Возвращает текущее время моделирования.
    virtual REAL KIP_API_TYPE GetCurrentDynTime() = 0;
    virtual REAL KIP_API_TYPE GetCurrentDynTimeInSec() = 0;

///
///  Возвращает время конца моделирования.
    virtual REAL KIP_API_TYPE GetDynEndTime() = 0;
    virtual REAL KIP_API_TYPE GetDynEndTimeForPower() = 0;

///
///  Возвращает значение максимально возможного шага моделирования.
    virtual REAL KIP_API_TYPE GetDynMaxStep() = 0;

///
    virtual int32 KIP_API_TYPE GetMathTuningAccel() = 0;

///
    virtual int32 KIP_API_TYPE GetAnalysCode() = 0;

///
    virtual int32 KIP_API_TYPE GetTypeModByCh(const char *pMTN) = 0;

///
    virtual char* KIP_API_TYPE GetDesignName() = 0;

///
    virtual REAL KIP_API_TYPE GetNumStepForPowerStepping() = 0;

///
    virtual void KIP_API_TYPE SetNewtNonConv() = 0;

///
    virtual REAL KIP_API_TYPE GetVarObjMeanLastNewt(ModKip *pCurMod,int32 Ind) = 0;    //  obsolete.
    virtual REAL KIP_API_TYPE GetVarObjMeanLastStep(ModKip *pCurMod,int32 Ind) = 0;    //  obsolete.
    virtual void KIP_API_TYPE SetVarObjMean(REAL Mean,ModKip *pCurMod,int32 Ind) = 0;  //  obsolete.

///
    virtual bOOL KIP_API_TYPE IsRealsEqual(REAL p1,REAL p2) = 0;
    virtual bOOL KIP_API_TYPE IsRealsEqual2(REAL p1,REAL p2) = 0;

///
    virtual bOOL KIP_API_TYPE IsElCurrentlySwept(Element *pEl) = 0;

///
    virtual bOOL KIP_API_TYPE NeedNewJacobian() = 0;

///
    virtual REAL KIP_API_TYPE GetTimeStep(int32 UserTimeInd) = 0;

///
    virtual bOOL KIP_API_TYPE IsStatica() = 0;

///
    virtual int32 KIP_API_TYPE GetNumTimeStep() = 0;

///
    virtual REAL* KIP_API_TYPE GetCurrentNodeToks2(ModKip *pCurMod) = 0;
    virtual REAL* KIP_API_TYPE GetCurrentNodeQs(ModKip *pCurMod) = 0;

///
    virtual void KIP_API_TYPE RegisterFreePerRateModKip(ModKip *pCurMod,PerRateModKip *pPerRateModKip) = 0;
    virtual PerRateModKip* KIP_API_TYPE GetFreePerRateModKip(ModKip *pCurMod) = 0;

///
    enum TypeSetOfElems
    {
      AllElements,
      CurrentExistedElements
    };
    virtual Element* KIP_API_TYPE GetAndInitDependElement(ModKip *pCurMod,const char *pN,TypeSetOfElems Ts,int32 ElemType,bOOL NeedCurDepend,bOOL NeedOutToksThr) = 0;

///
    virtual bOOL KIP_API_TYPE IsInternalModelBypassAccessed() = 0;

///
    virtual int32 KIP_API_TYPE GetDigitSrcStateByCurrTime(const char *pSrc) = 0;
    virtual void KIP_API_TYPE SetDigitSrcStateKMS(const char *pSrc,int32 State,REAL Val) = 0;

///
    virtual void* KIP_API_TYPE ModSimulationStart(ModKip *pCurMod,char *pSbcktName) = 0;
    virtual void KIP_API_TYPE ModSimulationEnd(ModKip *pCurMod,void *pKern) = 0;
    virtual KipRetCode KIP_API_TYPE ModSimStatica(ModKip *pCurMod,void *pKern,ModSimInfo *pMSI) = 0;

///
    virtual std::string KIP_API_TYPE GetA2dD2aSrcName(ModKip *pCurMod, const char *pDigName) = 0;

///
    virtual REAL KIP_API_TYPE GetVPITimeUnit() = 0;

///
    virtual const char* KIP_API_TYPE GetKipOutputDir() = 0;

///
    virtual REAL KIP_API_TYPE GetKipGMinDC() = 0;
    virtual REAL KIP_API_TYPE GetKipGMin() = 0;
    virtual REAL KIP_API_TYPE GetKipGMinUsed() = 0;

    virtual bOOL KIP_API_TYPE IsRampUpPhase() = 0;
    virtual REAL KIP_API_TYPE RampUpUpdate(REAL Mean) = 0;

    virtual bOOL KIP_API_TYPE IsInitialStep() = 0;
    virtual bOOL KIP_API_TYPE IsFinalStep() = 0;

    virtual int32 KIP_API_TYPE FinishCreatePar(int32 code = 0) = 0;
    virtual void KIP_API_TYPE FinishSim(int32 code = 0) = 0;

    virtual int32 KIP_API_TYPE IsAnalysis(const char *pAnName) = 0;

    virtual CTableModelFileBase* KIP_API_TYPE GetTableModelFile(ModKip *pMod,const char *pTBfname,const char *pTBCS) = 0;

    virtual Element* KIP_API_TYPE GetNewWrapped(int32 MT,ElemName elName,IndParName *pIndParEl,GrpParName *pGrpParEl,int32 WrNumPins,Net **pWrCondNet) = 0;
    virtual void KIP_API_TYPE DeleteWrapped(Element *pWEl) = 0;

    virtual CTableBase* KIP_API_TYPE GetTable(int32 _TableID, TAB_DIM *pTD, int32 _dimen) = 0;

    virtual unsigned long long KIP_API_TYPE get_tick_count() = 0;

    virtual const char* KIP_API_TYPE GetModelName(const char *pN) = 0;

    virtual void KIP_API_TYPE KipSpinLockInt(void *pSPL) = 0;
    virtual void KIP_API_TYPE KipSpinUnLockInt(void *pSPL) = 0;

    virtual int32 KIP_API_TYPE GetRandomValue(void *pRandObj, int32 seed) = 0;

    virtual CTSInfoBase* KIP_API_TYPE GetTSInfo(ModKip *pMod,const char *pTSfname) = 0;

    virtual void ModelSpinLock(ModKip *pElMod) = 0;
    virtual void ModelSpinUnLock(ModKip *pElMod) = 0;
};

class CIntObjs
{
  public:

    CIntObjs()
    {
      pIntObjsMem = 0;

      pObjsArr = 0;

      pPRArr = 0;
    }

    ~CIntObjs()
    {
      if(pIntObjsMem != 0)
      {
        delete []pIntObjsMem;
        pIntObjsMem = 0;
      }

      pObjsArr = 0;

      pPRArr = 0;
/*
      int32 i = 0;

////////
      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);
          delete pDObjsArr;
        }
        delete pObjsArr;
        pObjsArr = 0;
      }

////////
      if(pPRArr != 0)
      {
        for(i=0;i < pPRArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pPRArr->GetAt(i);
          delete pDObjsArr;
        }
        delete pPRArr;
        pPRArr = 0;
      }
*/
    }

    void KIP_API_TYPE InitIntObjs(int32 NumObjs,int32 NumSteps,int32 NumStepsPr)
    {
      int32 i = 0;

      if(0 == NumObjs)
        return;

////////  Такая работа с памятью экономичнее.
      int32 sz = 0;
      if(NumSteps != 0)
      {
        sz += sizeof(CPtrArr);
        sz += NumObjs*sizeof(void*);
        sz += NumObjs*(sizeof(CDoubleArr) + NumSteps*sizeof(REAL));
      }

      if(NumStepsPr != 0)
      {
        sz += sizeof(CPtrArr);
        sz += NumObjs*sizeof(void*);
        sz += NumObjs*(sizeof(CDoubleArr) + NumStepsPr*sizeof(REAL));
      }

      if(sz != 0)
        pIntObjsMem = new char[sz];

      char *pIntObjsMemD = pIntObjsMem;
      if(NumSteps != 0)
      {
        pObjsArr = (CPtrArr*)pIntObjsMemD;
        pIntObjsMemD += sizeof(CPtrArr);

        pObjsArr->ppArr = (void**)pIntObjsMemD;
        pObjsArr->Size = pObjsArr->RealSize = NumObjs;
        pObjsArr->GrowBy = 0;
        memset(pObjsArr->ppArr,0,pObjsArr->Size*sizeof(void*));
        pIntObjsMemD += NumObjs*sizeof(void*);

        for(i=0;i < NumObjs;i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pIntObjsMemD;
          pIntObjsMemD += sizeof(CDoubleArr);

          pObjsArr->SetAt(i,pDObjsArr);

          pDObjsArr->pArr = (REAL*)pIntObjsMemD;
          pDObjsArr->Size = pDObjsArr->RealSize = NumSteps;
          pDObjsArr->GrowBy = 0;
          memset(pDObjsArr->pArr,0,pDObjsArr->Size*sizeof(REAL));
          pIntObjsMemD += NumSteps*sizeof(REAL);
        }
      }

      if(NumStepsPr != 0)
      {
        pPRArr = (CPtrArr*)pIntObjsMemD;
        pIntObjsMemD += sizeof(CPtrArr);

        pPRArr->ppArr = (void**)pIntObjsMemD;
        pPRArr->Size = pPRArr->RealSize = NumObjs;
        pPRArr->GrowBy = 0;
        memset(pPRArr->ppArr,0,pPRArr->Size*sizeof(void*));
        pIntObjsMemD += NumObjs*sizeof(void*);

        for(i=0;i < NumObjs;i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pIntObjsMemD;
          pIntObjsMemD += sizeof(CDoubleArr);

          pPRArr->SetAt(i,pDObjsArr);

          pDObjsArr->pArr = (REAL*)pIntObjsMemD;
          pDObjsArr->Size = pDObjsArr->RealSize = NumStepsPr;
          pDObjsArr->GrowBy = 0;
          memset(pDObjsArr->pArr,0,pDObjsArr->Size*sizeof(REAL));
          pIntObjsMemD += NumStepsPr*sizeof(REAL);
        }
      }
/*
////////
      if(NumSteps != 0)
      {
        pObjsArr = new CPtrArr;
        pObjsArr->SetSize(NumObjs);

        for(i=0;i < NumObjs;i++)
        {
          CDoubleArr *pDObjsArr = new CDoubleArr;
          pObjsArr->SetAt(i,pDObjsArr);

          pDObjsArr->SetSize(NumSteps);
        }
      }

      if(NumStepsPr != 0)
      {
        pPRArr = new CPtrArr;
        pPRArr->SetSize(NumObjs);

        for(i=0;i < NumObjs;i++)
        {
          CDoubleArr *pDObjsArr = new CDoubleArr;
          pPRArr->SetAt(i,pDObjsArr);

          pDObjsArr->SetSize(NumStepsPr);
        }
      }
*/
    }

    void KIP_API_TYPE SetIntObjsStartValue(CIntObjs *pIntObjsExtPrevSour,int32 PrevTimeInd,int32 CurTimeInd,int32 PrevTimePrInd,int32 CurTimePrInd)
    {
      int32 i = 0;

      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);
          CDoubleArr *pDObjsArrExtPS = (CDoubleArr*)pIntObjsExtPrevSour->pObjsArr->GetAt(i);

          pDObjsArr->SetAt(CurTimeInd,pDObjsArrExtPS->GetAt(PrevTimeInd));
        }
      }

      if(pPRArr != 0)
      {
        for(i=0;i < pPRArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pPRArr->GetAt(i);
          CDoubleArr *pDObjsArrExtPS = (CDoubleArr*)pIntObjsExtPrevSour->pPRArr->GetAt(i);

          pDObjsArr->SetAt(CurTimePrInd,pDObjsArrExtPS->GetAt(PrevTimePrInd));
        }
      }
    }

    void KIP_API_TYPE SetValueByValue(CIntObjs *pIntObjsExt)
    {
      int32 i = 0;
      int32 j = 0;

      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);
          CDoubleArr *pDObjsArrExt = (CDoubleArr*)pIntObjsExt->pObjsArr->GetAt(i);

          for(j=0;j < pDObjsArr->GetSize();j++)
            pDObjsArr->SetAt(j,pDObjsArrExt->GetAt(j));
        }
      }

      if(pPRArr != 0)
      {
        for(i=0;i < pPRArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pPRArr->GetAt(i);
          CDoubleArr *pDObjsArrExt = (CDoubleArr*)pIntObjsExt->pPRArr->GetAt(i);

          for(j=0;j < pDObjsArr->GetSize();j++)
            pDObjsArr->SetAt(j,pDObjsArrExt->GetAt(j));
        }
      }
    }

    void KIP_API_TYPE SetCurrentValueByValue(CIntObjs *pIntObjsExt,int32 CurTimeInd,int32 CurTimePrInd)
    {
      int32 i = 0;

      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);
          CDoubleArr *pDObjsArrExt = (CDoubleArr*)pIntObjsExt->pObjsArr->GetAt(i);

          pDObjsArr->SetAt(CurTimeInd,pDObjsArrExt->GetAt(CurTimeInd));
        }
      }

      if(pPRArr != 0)
      {
        for(i=0;i < pPRArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pPRArr->GetAt(i);
          CDoubleArr *pDObjsArrExt = (CDoubleArr*)pIntObjsExt->pPRArr->GetAt(i);

          pDObjsArr->SetAt(CurTimePrInd,pDObjsArrExt->GetAt(CurTimePrInd));
        }
      }
    }

    void KIP_API_TYPE InitIntObjsZeroes()
    {
      int32 i = 0;
      int32 j = 0;

      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);

          for(j=0;j < pDObjsArr->GetSize();j++)
            pDObjsArr->SetAt(j,0.0);
        }
      }

      if(pPRArr != 0)
      {
        for(i=0;i < pPRArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pPRArr->GetAt(i);

          for(j=0;j < pDObjsArr->GetSize();j++)
            pDObjsArr->SetAt(j,0.0);
        }
      }
    }

    void KIP_API_TYPE InitIntObjsByInd(int32 CurTimeInd)
    {
////////  Только для значений, не для производных.
      int32 i = 0;
      int32 j = 0;

      if(pObjsArr != 0)
      {
        for(i=0;i < pObjsArr->GetSize();i++)
        {
          CDoubleArr *pDObjsArr = (CDoubleArr*)pObjsArr->GetAt(i);

          REAL D = pDObjsArr->GetAt(CurTimeInd);

          for(j=0;j < pDObjsArr->GetSize();j++)
            pDObjsArr->SetAt(j,D);
        }
      }
    }

////////  Блок памяти для всех массивов.
    char *pIntObjsMem;

////////  Массив массивов значений с прошлых шагов.
    CPtrArr *pObjsArr;

////////  Массив массивов производных с прошлых шагов.
    CPtrArr *pPRArr;
};

class CIntObjsVars
{
  public:

    CIntObjsVars(){}
    ~CIntObjsVars(){}

    void KIP_API_TYPE InitIntObjs(int32 NumRealObjs,int32 NumIntObjs,int32 NumStrObjs,std::vector<int32> NumRealVObjs,std::vector<int32> NumIntVObjs,std::vector<int32> NumStrVObjs,int32 NumSteps)
    {
      int32 i = 0;
      int32 j = 0;

      IORealVars.resize(NumRealObjs);
      for(i=0;i < (int32)IORealVars.size();i++)
        IORealVars[i].resize(NumSteps);

      IOIntVars.resize(NumIntObjs);
      for(i=0;i < (int32)IOIntVars.size();i++)
        IOIntVars[i].resize(NumSteps);

      IOStrVars.resize(NumStrObjs);
      for(i=0;i < (int32)IOStrVars.size();i++)
        IOStrVars[i].resize(NumSteps);

      IORealVVars.resize(NumRealVObjs.size());
      for(i=0;i < (int32)IORealVVars.size();i++)
      {
        IORealVVars[i].resize(NumSteps);
        for(j=0;j < NumSteps;j++)
          IORealVVars[i][j].resize(NumRealVObjs[i]);
      }

      IOIntVVars.resize(NumIntVObjs.size());
      for(i=0;i < (int32)IOIntVVars.size();i++)
      {
        IOIntVVars[i].resize(NumSteps);
        for(j=0;j < NumSteps;j++)
          IOIntVVars[i][j].resize(NumIntVObjs[i]);
      }

      IOStrVVars.resize(NumStrVObjs.size());
      for(i=0;i < (int32)IOStrVVars.size();i++)
      {
        IOStrVVars[i].resize(NumSteps);
        for(j=0;j < NumSteps;j++)
          IOStrVVars[i][j].resize(NumStrVObjs[i]);
      }
    }

    void KIP_API_TYPE SetIntObjsStartValue(CIntObjsVars *pIntObjsExtPrevSour,int32 PrevTimeInd,int32 CurTimeInd)
    {
      int32 i = 0;

      for(i=0;i < (int32)IORealVars.size();i++)
        IORealVars[i][CurTimeInd] = pIntObjsExtPrevSour->IORealVars[i][PrevTimeInd];

      for(i=0;i < (int32)IOIntVars.size();i++)
        IOIntVars[i][CurTimeInd] = pIntObjsExtPrevSour->IOIntVars[i][PrevTimeInd];

      for(i=0;i < (int32)IOStrVars.size();i++)
        IOStrVars[i][CurTimeInd] = pIntObjsExtPrevSour->IOStrVars[i][PrevTimeInd];

      for(i=0;i < (int32)IORealVVars.size();i++)
        IORealVVars[i][CurTimeInd] = pIntObjsExtPrevSour->IORealVVars[i][PrevTimeInd];

      for(i=0;i < (int32)IOIntVVars.size();i++)
        IOIntVVars[i][CurTimeInd] = pIntObjsExtPrevSour->IOIntVVars[i][PrevTimeInd];

      for(i=0;i < (int32)IOStrVVars.size();i++)
        IOStrVVars[i][CurTimeInd] = pIntObjsExtPrevSour->IOStrVVars[i][PrevTimeInd];
    }

    void KIP_API_TYPE SetValueByValue(CIntObjsVars *pIntObjsExt)
    {
      int32 i = 0;
      int32 j = 0;

////////  Копировать только так, на случай если кто-то получил ссылку на i-ый вектор.
      for(i=0;i < (int32)IORealVars.size();i++)
        for(j=0;j < (int32)IORealVars[i].size();j++)
          IORealVars[i][j] = pIntObjsExt->IORealVars[i][j];

      for(i=0;i < (int32)IOIntVars.size();i++)
        for(j=0;j < (int32)IOIntVars[i].size();j++)
          IOIntVars[i][j] = pIntObjsExt->IOIntVars[i][j];

      for(i=0;i < (int32)IOStrVars.size();i++)
        for(j=0;j < (int32)IOStrVars[i].size();j++)
          IOStrVars[i][j] = pIntObjsExt->IOStrVars[i][j];

      for(i=0;i < (int32)IORealVVars.size();i++)
        for(j=0;j < (int32)IORealVVars[i].size();j++)
          IORealVVars[i][j] = pIntObjsExt->IORealVVars[i][j];

      for(i=0;i < (int32)IOIntVVars.size();i++)
        for(j=0;j < (int32)IOIntVVars[i].size();j++)
          IOIntVVars[i][j] = pIntObjsExt->IOIntVVars[i][j];

      for(i=0;i < (int32)IOStrVVars.size();i++)
        for(j=0;j < (int32)IOStrVVars[i].size();j++)
          IOStrVVars[i][j] = pIntObjsExt->IOStrVVars[i][j];
    }

    void KIP_API_TYPE SetCurrentValueByValue(CIntObjsVars *pIntObjsExt,int32 CurTimeInd)
    {
      int32 i = 0;

      for(i=0;i < (int32)IORealVars.size();i++)
        IORealVars[i][CurTimeInd] = pIntObjsExt->IORealVars[i][CurTimeInd];

      for(i=0;i < (int32)IOIntVars.size();i++)
        IOIntVars[i][CurTimeInd] = pIntObjsExt->IOIntVars[i][CurTimeInd];

      for(i=0;i < (int32)IOStrVars.size();i++)
        IOStrVars[i][CurTimeInd] = pIntObjsExt->IOStrVars[i][CurTimeInd];

      for(i=0;i < (int32)IORealVVars.size();i++)
        IORealVVars[i][CurTimeInd] = pIntObjsExt->IORealVVars[i][CurTimeInd];

      for(i=0;i < (int32)IOIntVVars.size();i++)
        IOIntVVars[i][CurTimeInd] = pIntObjsExt->IOIntVVars[i][CurTimeInd];

      for(i=0;i < (int32)IOStrVVars.size();i++)
        IOStrVVars[i][CurTimeInd] = pIntObjsExt->IOStrVVars[i][CurTimeInd];
    }

    void KIP_API_TYPE InitIntObjsZeroes()
    {
      int32 i = 0;
      int32 j = 0;
      int32 k = 0;

      for(i=0;i < (int32)IORealVars.size();i++)
        for(j=0;j < (int32)IORealVars[i].size();j++)
          IORealVars[i][j] = 0.0;

      for(i=0;i < (int32)IOIntVars.size();i++)
        for(j=0;j < (int32)IOIntVars[i].size();j++)
          IOIntVars[i][j] = 0;

      for(i=0;i < (int32)IOStrVars.size();i++)
        for(j=0;j < (int32)IOStrVars[i].size();j++)
          IOStrVars[i][j] = "";

      for(i=0;i < (int32)IORealVVars.size();i++)
        for(j=0;j < (int32)IORealVVars[i].size();j++)
          for(k=0;k < (int32)IORealVVars[i][j].size();k++)
            IORealVVars[i][j][k] = 0.0;

      for(i=0;i < (int32)IOIntVVars.size();i++)
        for(j=0;j < (int32)IOIntVVars[i].size();j++)
          for(k=0;k < (int32)IOIntVVars[i][j].size();k++)
            IOIntVVars[i][j][k] = 0;

      for(i=0;i < (int32)IOStrVVars.size();i++)
        for(j=0;j < (int32)IOStrVVars[i].size();j++)
          for(k=0;k < (int32)IOStrVVars[i][j].size();k++)
            IOStrVVars[i][j][k] = "";
    }

    void KIP_API_TYPE InitIntObjsByInd(int32 CurTimeInd)
    {
      int32 i = 0;
      int32 j = 0;

      for(i=0;i < (int32)IORealVars.size();i++)
      {
        REAL D = IORealVars[i][CurTimeInd];
        for(j=0;j < (int32)IORealVars[i].size();j++)
          IORealVars[i][j] = D;
      }

      for(i=0;i < (int32)IOIntVars.size();i++)
      {
        ptrdiff_t D = IOIntVars[i][CurTimeInd];
        for(j=0;j < (int32)IOIntVars[i].size();j++)
          IOIntVars[i][j] = D;
      }

      for(i=0;i < (int32)IOStrVars.size();i++)
      {
        const std::string &D = IOStrVars[i][CurTimeInd];
        for(j=0;j < (int32)IOStrVars[i].size();j++)
          IOStrVars[i][j] = D;
      }

      for(i=0;i < (int32)IORealVVars.size();i++)
      {
        const std::vector<REAL> &D = IORealVVars[i][CurTimeInd];
        for(j=0;j < (int32)IORealVVars[i].size();j++)
          IORealVVars[i][j] = D;
      }

      for(i=0;i < (int32)IOIntVVars.size();i++)
      {
        const std::vector<ptrdiff_t> &D = IOIntVVars[i][CurTimeInd];
        for(j=0;j < (int32)IOIntVVars[i].size();j++)
          IOIntVVars[i][j] = D;
      }

      for(i=0;i < (int32)IOStrVVars.size();i++)
      {
        const std::vector<std::string> &D = IOStrVVars[i][CurTimeInd];
        for(j=0;j < (int32)IOStrVVars[i].size();j++)
          IOStrVVars[i][j] = D;
      }
    }

////////  Массивы значений с прошлых шагов.
    std::vector<std::vector<REAL> > IORealVars;
    std::vector<std::vector<ptrdiff_t> > IOIntVars;
    std::vector<std::vector<std::string> > IOStrVars;

    std::vector<std::vector<std::vector<REAL> > > IORealVVars;
    std::vector<std::vector<std::vector<ptrdiff_t> > > IOIntVVars;
    std::vector<std::vector<std::vector<std::string> > > IOStrVVars;
};

class ModVars
{
  public:

    ModVars()
    {
      NumIntObjs = 0;
      pMapQByNode = 0;
    }

    ~ModVars()
    {
      if(pMapQByNode != 0)
      {
        delete []pMapQByNode;
        pMapQByNode = 0;
      }
    }

    int32 NumIntObjs;

////////  Для емкостей и индуктивностей.
    CIntObjs CLObjs;

////////  Для внутренних зарядов.
    CIntObjs QObjs;

////////  Для внутренних переменных.
    CIntObjsVars VarObjs;

////////  Соответствие номера узла номеру заряда.
    int32 *pMapQByNode;
};

class PerRateModKip
{
  public:

    PerRateModKip()
    {
      pModVars = 0;

      ppModPinPN = 0;
    }

    ~PerRateModKip()
    {
      if(pModVars != 0)
      {
        delete pModVars;
        pModVars = 0;
      }

      ClearPrevModPerRateInfo();

      ClearModPinPerRateInfo();
    }

    void KIP_API_TYPE InitModPinPerRateInfo(int32 sz)
    {
      ppModPinPN = new PotNet*[sz];
      memset(ppModPinPN,0,sz*sizeof(PotNet*));
    }

    void KIP_API_TYPE ClearModPinPerRateInfo()
    {
      if(ppModPinPN != 0)
      {
        delete []ppModPinPN;
        ppModPinPN = 0;
      }
    }

    void KIP_API_TYPE ClearPrevModPerRateInfo()
    {
    }

////////  Объекты модели.
    ModVars *pModVars;

////////  Инфо в ногах элементов.
    PotNet **ppModPinPN;
};

///
///  Интерфейс, обеспечивающий базовую функциональность модели.

extern void Init_YY_CC(ModKip *pMod,int32 sz);
extern void Clear_YY_CC(ModKip *pMod);
extern void Zero_YY_CC(ModKip *pMod);

class ModKip
{
  public:

    CTypeModelInfo *pTypeModInfo;

////////  Элемент модели.
    Element *pModElem;

////////  Инфо о клоне.
    CModClone *pModClone;

////////  Массив внутренних узлов.
    CIntArr *pInnerNets;

////////  Массив расширенных узлов.
    CIntArr *pExtendNets;

////////
    CIntArr *pSourPairNets;

////////  Массив всех узлов.
    CIntArr *pAllModNets;

////////  Данные, зависящие от KIPARIS.
    void *pModData;

////////
    MapPtrToPtr MapPRMtoPRD;

    PerRateModKip *pPerRateModKipCur;

////////  Инфо в ногах элемента.
    REAL **ppModPinPhi;
    int32 *pModPinNets;
    void **ppSpinLocks;

////////  Инфо о связях ног элемента.
    int32 **ppModPinLinks;

////////
    const char *MKBasePath;

////////
    CIntArr *pExtGminPairNets;

////////
    CIntArr *pFlSourDtNets;

////////
    int32 YY_CC_sz;
    REAL **YY;  // matrix of conductivities for AC
    REAL **CC;  // matrix of capacitances for AC

////////
    int32 NumModCounters;
    unsigned long long *pModCounters;

////////  std структуры должны лежать после всех полей, на случай mingw + обращение к предыдущим полям напрямую.
////////  К std структурам нельзя обращаться напрямую на случай mingw.

////////  Массив массивов указателей на инфо в матрице.
    std::vector< std::vector<REAL*> > MatrInfoArr;

////////  Массив указателей на инфо в правой части.
    std::vector< std::pair<REAL*, REAL*> > RInfoArr;
    std::vector< std::pair<REAL*, REAL*> > RQInfoArr;

////////
    std::vector< std::pair<REAL*, REAL*> > CondCurRI;
    std::vector< std::pair<REAL*, REAL*> > CondCurRAI;

    std::vector< std::pair<REAL*, REAL*> > CondCurRQI;
    std::vector< std::pair<REAL*, REAL*> > CondCurRQAI;

    struct YCI
    {
      int32 YCI_ind;
      REAL *pYCI_M;
      REAL *pYCI_Y;
      REAL *pYCI_C;
      REAL **ppYCI_Y;
      REAL **ppYCI_C;
    };
    std::vector< std::vector<YCI> > CondCurYCI;

////////
    ModKip(CTypeModelInfo *_pTypeModInfo)
    {
      pTypeModInfo = _pTypeModInfo;
      pModElem = 0;
      pModClone = 0;
      pInnerNets = 0;
      pExtendNets = 0;
      pSourPairNets = 0;
      pAllModNets = 0;

      pModData = 0;

      pPerRateModKipCur = 0;

      ppModPinPhi = 0;
      pModPinNets = 0;
      ppSpinLocks = 0;

      ppModPinLinks = 0;

      pExtGminPairNets = 0;

      pFlSourDtNets = 0;

      YY_CC_sz = 0;
      YY = 0;
      CC = 0;

      NumModCounters = 0;
      pModCounters = 0;
    }

    virtual ~ModKip()
    {
      ClearInnerNets();
      ClearExtendNets();
      ClearSourPairNets();
      ClearExtGminPairNets();
      ClearFlSourDtNets();
      ClearAllModNets();

      ClearMatrInfoArr();

      ClearPerRateModKip();

      ClearModPinInfo();

      ClearModPinLinks();

      delete []pModCounters;
      pModCounters = 0;
      NumModCounters = 0;
    }

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif

///  Возвращает общее количество ног элемента.
///  В любом массиве связанном с ногами элемента порядок ног следующий:
///  собственные(включая расширенные), внутренние.
///  Расширенные узлы - обычно это узлы заданные через параметр bulk.
///  Для правильного учета расширенных узлов список цепей перестраивается.
    virtual int32 KIP_API_TYPE GetAllNumPinNodes()
    {
      int32 sz = pModElem->numPin;

      if(pExtendNets != 0)
        sz += pExtendNets->GetSize();

      if(pInnerNets != 0)
        sz += pInnerNets->GetSize();

      return sz;
    }

    virtual void KIP_API_TYPE InitModPinInfo()
    {
      int32 sz = GetAllNumPinNodes();

      ppModPinPhi = new REAL*[sz];
      memset(ppModPinPhi,0,sz*sizeof(REAL*));

      pModPinNets = new int32[sz];
      memset(pModPinNets,0,sz*sizeof(int32));

      if(IsDirectMAndRFilled() == tRUE)
      {
        ppSpinLocks = new void*[sz];
        memset(ppSpinLocks,0,sz*sizeof(void*));
      }
    }

    virtual void KIP_API_TYPE ClearModPinInfo()
    {
      if(ppModPinPhi != 0)
      {
        delete []ppModPinPhi;
        ppModPinPhi = 0;
      }

      if(pModPinNets != 0)
      {
        delete []pModPinNets;
        pModPinNets = 0;
      }

      if(ppSpinLocks != 0)
      {
        delete []ppSpinLocks;
        ppSpinLocks = 0;
      }
    }

    virtual void KIP_API_TYPE InitModPinLinks()
    {
      int32 i = 0;

      int32 sz = GetAllNumPinNodes();

      char *pMemSA = new char[sz*sizeof(int32*) + sz*sz*sizeof(int32)];

      char *pMemSAD = pMemSA;

      ppModPinLinks = (int32**)pMemSAD;
      pMemSAD += sz*sizeof(int32*);

      for(i=0;i < sz;i++)
      {
        ppModPinLinks[i] = (int32*)pMemSAD;
        pMemSAD += sz*sizeof(int32);

        memset(ppModPinLinks[i],0,sz*sizeof(int32));
      }
    }

    virtual void KIP_API_TYPE ClearModPinLinks()
    {
      if(ppModPinLinks != 0)
      {
        delete []ppModPinLinks;
        ppModPinLinks = 0;
      }
    }

    virtual void KIP_API_TYPE InitModPinPerRateInfo(PerRateDataDoc *pPerRateDataDoc)
    {
      PerRateModKip *pPerRateModKip = GetPerRateModKip(pPerRateDataDoc);

      int32 sz = GetAllNumPinNodes();
      pPerRateModKip->InitModPinPerRateInfo(sz);
    }

    virtual void KIP_API_TYPE ClearModPinPerRateInfo(PerRateDataDoc *pPerRateDataDoc)
    {
      PerRateModKip *pPerRateModKip = GetPerRateModKip(pPerRateDataDoc);

////////  Модели могли не создаться из-за ошибок.
      if(pPerRateModKip != 0)
        pPerRateModKip->ClearModPinPerRateInfo();
    }

    virtual void KIP_API_TYPE InitMatrInfoArr(CPtrArr *pMIarrArr,CPtrArr *pRarr,int32 /*Capacity*/)
    {
      int32 i = 0;
      int32 j = 0;

      MatrInfoArr.resize(pMIarrArr->GetSize());

      for(i=0; i<pMIarrArr->GetSize(); i++)
      {
        std::vector<REAL*> &MatrInfo = MatrInfoArr[i];

        CPtrArr *pMIarr = (CPtrArr*)pMIarrArr->GetAt(i);
        if(pMIarr != 0)
        {
          for(j=0; j<pMIarr->GetSize(); j++)
            MatrInfo.push_back((REAL*)pMIarr->GetAt(j));
        }
      }

////////
      if(IsDirectMAndRFilled() == tRUE)
      {
        for(i=0; i<pRarr->GetSize(); i+=4)
        {
          RInfoArr.push_back(std::make_pair((REAL*)pRarr->GetAt(i), (REAL*)pRarr->GetAt(i+1)));
          RQInfoArr.push_back(std::make_pair((REAL*)pRarr->GetAt(i+2), (REAL*)pRarr->GetAt(i+3)));
        }
      }

      CondCurRI.resize(pAllModNets->GetSize());
      CondCurRAI.resize(pAllModNets->GetSize());
      CondCurRQI.resize(pAllModNets->GetSize());
      CondCurRQAI.resize(pAllModNets->GetSize());
      CondCurYCI.resize(pAllModNets->GetSize());
    }

    virtual void KIP_API_TYPE ClearMatrInfoArr()
    {
      MatrInfoArr.clear();
      RInfoArr.clear();
      RQInfoArr.clear();

      CondCurRI.clear();
      CondCurRAI.clear();
      CondCurRQI.clear();
      CondCurRQAI.clear();
      CondCurYCI.clear();
    }

///
///  Функция для дополнительной инициализации после создания всех моделей.
///  Сейчас не применяется, т.к. должен быть учтен случай пересоздания моделей при sweep и т.д.
    virtual void KIP_API_TYPE PostModelInit() {}

///  Возвращает кол-во емкостей и индуктивностей, к которым применяется метод интегрирования,
///  причем производные становятся известны сразу после обращения к функции интегрирования.
    virtual int32 KIP_API_TYPE GetNumCLObjs() { return 0; }

///  Возвращает кол-во зарядов, к которым применяется метод интегрирования,
///  причем производные становятся известны сразу после обращения к функции интегрирования.
    virtual int32 KIP_API_TYPE GetNumQObjs(int32 * /*pNeedMap*/) { return 0; }

///  Возвращает кол-во переменных модели.
///  Переменные в модели нужны для правильного сохранения значений переменных в прошедших временных точках.
    virtual int32 KIP_API_TYPE GetNumVarRealObjs() { return 0; }
    virtual int32 KIP_API_TYPE GetNumVarIntObjs() { return 0; }
    virtual int32 KIP_API_TYPE GetNumVarStrObjs() { return 0; }
    virtual std::vector<int32> KIP_API_TYPE GetNumVarRealVObjs() { return std::vector<int32>(); }
    virtual std::vector<int32> KIP_API_TYPE GetNumVarIntVObjs() { return std::vector<int32>(); }
    virtual std::vector<int32> KIP_API_TYPE GetNumVarStrVObjs() { return std::vector<int32>(); }

///  Функция для настройки зарядовых узлов модели.
///  pNodesIndArr - узлы модели, в которых будут интегрироваться заряды.
    virtual void KIP_API_TYPE GetNodesWithQObj(CIntArr * /*pNodesIndArr*/) {}

////////
    virtual PerRateModKip* KIP_API_TYPE CreatePerRateModKip(PerRateDataDoc *pPerRateDataDoc,int32 NumSteps,int32 NumStepsPr,bOOL *pNew);

    virtual void KIP_API_TYPE SetPerRateModKipToFree(PerRateDataDoc *pPerRateDataDoc);

    virtual void KIP_API_TYPE CreatePerRateModKipFromFree(PerRateDataDoc *pPerRateDataDoc,int32 NumSteps,int32 NumStepsPr)
    {
      bOOL NewPRMK = fALSE;
      PerRateModKip *pPerRateModKip = CreatePerRateModKip(pPerRateDataDoc,NumSteps,NumStepsPr,&NewPRMK);

      if(tRUE == NewPRMK)
      {
        int32 sz = GetAllNumPinNodes();
        pPerRateModKip->InitModPinPerRateInfo(sz);
      }
    }

    virtual void KIP_API_TYPE ClearPerRateModKip()
    {
      int32 i = 0;

      for(i=0;i < MapPRMtoPRD.GetSize();i++)
      {
        PerRateModKip *pPerRateModKip = (PerRateModKip*)MapPRMtoPRD.GetAtPtr2(i);
        ClearPerRateModKipHelper(pPerRateModKip);
      }
      MapPRMtoPRD.RemoveAll();

      pPerRateModKipCur = 0;
    }

    virtual void KIP_API_TYPE ClearPerRateModKipHelper(PerRateModKip *pPerRateModKip)
    {
      delete pPerRateModKip;
    }

    virtual void KIP_API_TYPE SetPerRateModKip(PerRateDataDoc *pPerRateDataDoc)
    {
      pPerRateModKipCur = GetPerRateModKip(pPerRateDataDoc);
    }

    virtual PerRateModKip* KIP_API_TYPE GetPerRateModKip(PerRateDataDoc *pPerRateDataDoc)
    {
      return (PerRateModKip*)MapPRMtoPRD.GetPtr2ByPtr1(pPerRateDataDoc);
    }

///
///  Функция инициализации внутренних объектов модели.
    virtual ModVars* KIP_API_TYPE InitIntObjs(int32 NumSteps,int32 NumStepsPr)
    {
      int32 i = 0;

////////
      ModVars *pMVars = new ModVars;

////////  2 настройки внутренней зарядовой модели.
////////  По номерам зарядов.
////////  По номерам ног элемента.
      int32 NeedMap = 0;
      int32 NumQObjs = GetNumQObjs(&NeedMap);

      if(NumQObjs != 0 && 1 == NeedMap)
      {
        int32 sz = GetAllNumPinNodes();

        pMVars->pMapQByNode = new int32[sz];
        for(i=0;i < sz;i++)
          pMVars->pMapQByNode[i] = -1;

        CIntArr NodesIndArr;
        NodesIndArr.SetSizeCapacity(NumQObjs);
        GetNodesWithQObj(&NodesIndArr);

        for(i=0;i < NodesIndArr.GetSize();i++)
        {
          int32 NodeInd = NodesIndArr.GetAt(i);
          pMVars->pMapQByNode[NodeInd] = i;
        }
      }

////////
      int32 NumCLObjs = GetNumCLObjs();

      int32 NumVarRealObjs = GetNumVarRealObjs();
      int32 NumVarIntObjs = GetNumVarIntObjs();
      int32 NumVarStrObjs = GetNumVarStrObjs();
      std::vector<int32> NumVarRealVObjs = GetNumVarRealVObjs();
      std::vector<int32> NumVarIntVObjs = GetNumVarIntVObjs();
      std::vector<int32> NumVarStrVObjs = GetNumVarStrVObjs();

      pMVars->NumIntObjs = (int)(NumCLObjs + NumQObjs +
                                 NumVarRealObjs         + NumVarIntObjs         + NumVarStrObjs +
                                 NumVarRealVObjs.size() + NumVarIntVObjs.size() + NumVarStrVObjs.size());

      if(0 == pMVars->NumIntObjs)
        return pMVars;

////////  Для емкостей и индуктивностей сохранять прошлые значения нужно только если это указано.
      int32 NumStepsCL = NumSteps;
      if(IsCLlastSave() == fALSE)
        NumStepsCL = 0;
      pMVars->CLObjs.InitIntObjs(NumCLObjs,NumStepsCL,NumStepsPr);

////////  Для внутренних зарядов всегда нужно сохранять заряды с прошлых шагов.
      pMVars->QObjs.InitIntObjs(NumQObjs,NumSteps,NumStepsPr);

////////  Для внутренних переменных всегда нужно сохранять значения с прошлых шагов.
      pMVars->VarObjs.InitIntObjs(NumVarRealObjs,NumVarIntObjs,NumVarStrObjs,NumVarRealVObjs,NumVarIntVObjs,NumVarStrVObjs,NumSteps);

      return pMVars;
    }

    virtual void KIP_API_TYPE ReInitIntObjs(int32 NumSteps,int32 NumStepsPr)
    {
      int32 i = 0;

      for(i=0;i < MapPRMtoPRD.GetSize();i++)
      {
        PerRateModKip *pPerRateModKip = (PerRateModKip*)MapPRMtoPRD.GetAtPtr2(i);

        delete pPerRateModKip->pModVars;
        pPerRateModKip->pModVars = InitIntObjs(NumSteps,NumStepsPr);
      }
    }

////////
    virtual void KIP_API_TYPE ClearPerRateModKipExtend()
    {
      int32 i = 0;

////////  Удалим все PerRateModKip кроме первой.
      for(i = 1;i < MapPRMtoPRD.GetSize();i++)
      {
        PerRateModKip *pPerRateModKip = (PerRateModKip*)MapPRMtoPRD.GetAtPtr2(i);
        MapPRMtoPRD.RemovePtrsBy2(pPerRateModKip);
        i--;
        delete pPerRateModKip;
      }

////////  Выставим текущим нулевой. Т.к. при выставлении текущего пробегаю только по
////////  элементам блока, то в элементах принадлежащих другому блоку текущий будет не нулевым.
      PerRateModKip *pPerRateModKip = (PerRateModKip*)MapPRMtoPRD.GetAtPtr2(0);
      pPerRateModKipCur = pPerRateModKip;
    }

////////
    virtual void KIP_API_TYPE InitPerRateModKipBySour(PerRateModKip *pPerRateModKipSour,PerRateModKip *pPerRateModKip)
    {
      pPerRateModKip->pModVars->CLObjs.SetValueByValue(&pPerRateModKipSour->pModVars->CLObjs);
      pPerRateModKip->pModVars->QObjs.SetValueByValue(&pPerRateModKipSour->pModVars->QObjs);
      pPerRateModKip->pModVars->VarObjs.SetValueByValue(&pPerRateModKipSour->pModVars->VarObjs);
    }

///
///  Функция для инициализации внутренних структур модели в начале очередного SWEEPа.
    virtual void KIP_API_TYPE InitIntObjsZeroes()
    {
      pPerRateModKipCur->pModVars->CLObjs.InitIntObjsZeroes();
      pPerRateModKipCur->pModVars->QObjs.InitIntObjsZeroes();
      pPerRateModKipCur->pModVars->VarObjs.InitIntObjsZeroes();
    }

////////
    virtual void KIP_API_TYPE UpdateIntObjs(int32 /*CurTimeInd*/)  //  obsolete.
    {
    }

    virtual void KIP_API_TYPE SetIntObjsStartValue(PerRateModKip *pPerRateModKipSourPrev,PerRateModKip *pPerRateModKipDestCurr,int32 PrevTimeInd,int32 CurTimeInd,int32 PrevTimePrInd,int32 CurTimePrInd)
    {
      pPerRateModKipDestCurr->pModVars->CLObjs.SetIntObjsStartValue(&pPerRateModKipSourPrev->pModVars->CLObjs,PrevTimeInd,CurTimeInd,PrevTimePrInd,CurTimePrInd);
      pPerRateModKipDestCurr->pModVars->QObjs.SetIntObjsStartValue(&pPerRateModKipSourPrev->pModVars->QObjs,PrevTimeInd,CurTimeInd,PrevTimePrInd,CurTimePrInd);
      pPerRateModKipDestCurr->pModVars->VarObjs.SetIntObjsStartValue(&pPerRateModKipSourPrev->pModVars->VarObjs,PrevTimeInd,CurTimeInd);
    }

    virtual void KIP_API_TYPE SetIntObjsCurrentValueByMod(ModKip *pMod,int32 CurTimeInd,int32 CurTimePrInd)
    {
      pPerRateModKipCur->pModVars->CLObjs.SetCurrentValueByValue(&pMod->pPerRateModKipCur->pModVars->CLObjs,CurTimeInd,CurTimePrInd);
      pPerRateModKipCur->pModVars->QObjs.SetCurrentValueByValue(&pMod->pPerRateModKipCur->pModVars->QObjs,CurTimeInd,CurTimePrInd);
      pPerRateModKipCur->pModVars->VarObjs.SetCurrentValueByValue(&pMod->pPerRateModKipCur->pModVars->VarObjs,CurTimeInd);
    }

    virtual void KIP_API_TYPE InitIntObjsByInd(PerRateModKip *pPerRateModKipDest,int32 CurTimeInd)
    {
      pPerRateModKipDest->pModVars->CLObjs.InitIntObjsByInd(CurTimeInd);
      pPerRateModKipDest->pModVars->QObjs.InitIntObjsByInd(CurTimeInd);
      pPerRateModKipDest->pModVars->VarObjs.InitIntObjsByInd(CurTimeInd);
    }

///
///  Если нужно сохранять емкости или индуктивности с прошлых шагов, необходимо вернуть tRUE.
    virtual bOOL KIP_API_TYPE IsCLlastSave() { return fALSE; }

////////  Для моделей, в которых используется внутренняя зарядовая модель, необходимо вернуть tRUE.
////////  Внутренняя зарядовая модель - сохранение заряда в каждой модели.
    virtual bOOL KIP_API_TYPE IsChargeBasedInt() { return fALSE; }


////////  Несмотря на наличие поддержки внешних зарядов, распределение тока с помощью UpdateModelToksByChargeExt
////////  не верно, т.к. нельзя опираться только на последний посчитаный заряд модели при распределении тока.
////////  Т.е. нельзя использовать пропорцию Icur = Isum * Qcur / Qsum, т.к. ток Isum посчитан с использованием
////////  зарядов с прошлых шагов, на которых QcurPREV мог состовлять другую часть QsumPREV.
////////  Поэтому принял решение пользоваться внутренними зарядами.


////////  Для моделей, в которых используется внешняя зарядовая модель, необходимо вернуть tRUE.
////////  Внешняя зарядовая модель - сохранение суммы зарядов в каждом узле.
    virtual bOOL KIP_API_TYPE IsChargeBasedExt() { return fALSE; }

////////  Функция для обновления токов в ногах элементов.
////////  Оба массива соответствуют ногам элемента.
    virtual void KIP_API_TYPE UpdateModelToksByChargeExt(REAL * /*pChsummArr*/,REAL * /*pToksArr*/) {}


///
///  Для моделей, в которых неизвестными являются токи.
    virtual bOOL KIP_API_TYPE IsFlowX() { return fALSE; }

///
///  Возвращает индексы внутренних узлов, которые соответствуют неизвестным токам.
    virtual int32 KIP_API_TYPE GetFlowXNodes(int32 * /*pNodes*/,int32 * /*pNodesBase*/) { return 0; }

///  Функция для обновления токов в ногах элементов.
///  Массив соответствует неизвестным токам.
    virtual void KIP_API_TYPE UpdateModelToksByTokX(REAL * /*pToksX*/) {}

///  Функция для инициализации модели по входным параметрам.
///  Применяется для клонируемых частей модели.
    virtual int32 KIP_API_TYPE InitParameters(CParError * /*pParErrRet*/) { return 0; }

///  Функция для инициализации моделей, которые участвуют в клонировании.
///  Применяется для не клонируемых частей модели.
///  В этой функции можно пользоваться информацией из клонируемой части модели,
///  т.к. она к моменту вызова переинициализирована.
    virtual int32 KIP_API_TYPE InitParametersNC() { return 0; }

////////  Функции для работы с расширенными узлами.
    virtual int32 KIP_API_TYPE InitExtendNodes() { return 0; }

    virtual void KIP_API_TYPE AddExtendNet(int32 NN)
    {
      if(0 == pExtendNets)
        pExtendNets = new CIntArr;

////////  Обычно это Bulk узел, поэтому модифицируем GrowBy.
      pExtendNets->GrowBy = 1;
      pExtendNets->Add(NN);
    }

    virtual void KIP_API_TYPE ClearExtendNets()
    {
      if(pExtendNets != 0)
      {
        delete pExtendNets;
        pExtendNets = 0;
      }
    }

    virtual int32 KIP_API_TYPE GetExtendNet(int32 Ind) { return pExtendNets->GetAt(Ind); }

    virtual CIntArr* KIP_API_TYPE GetExtendNetArr() { return pExtendNets; }

    virtual void KIP_API_TYPE FillSourPairNets();

    virtual void KIP_API_TYPE InitSourPairNets(CIntArr*) {}

    virtual CIntArr* KIP_API_TYPE GetSourPairNets() { return pSourPairNets; }

    virtual void KIP_API_TYPE ClearSourPairNets()
    {
      if(pSourPairNets != 0)
      {
        delete pSourPairNets;
        pSourPairNets = 0;
      }
    }

////////
    virtual bOOL KIP_API_TYPE IsLimitPhiExist() { return fALSE; }
    virtual void KIP_API_TYPE LimitingPhi(PotNet**,REAL**) {}

////////
    virtual bOOL KIP_API_TYPE IsRejectTimeStepExist() { return fALSE; }
    virtual bOOL KIP_API_TYPE RejectTimeStep(PotNet**) { return fALSE; }

////////
    virtual bOOL KIP_API_TYPE IsLimitTimeStepExist() { return fALSE; }
    virtual bOOL KIP_API_TYPE LimitTimeStep(PotNet**, REAL /*PS*/, REAL * /*pCS*/) { return fALSE; }

////////  Функции для работы с внутренними узлами.
    virtual int32 KIP_API_TYPE GetModelNumInnerNets() { return 0; }

    virtual void KIP_API_TYPE SetInnerNets(CIntArr *_pInnerNets)
    {
      if(0 == _pInnerNets)  //  Такое может быть для моделей, у которых нет внутренних узлов.
        return;

      pInnerNets = new CIntArr;
      pInnerNets->Copy(_pInnerNets);
    }

    virtual void KIP_API_TYPE ClearInnerNets()
    {
      if(pInnerNets != 0)
      {
        delete pInnerNets;
        pInnerNets = 0;
      }
    }

    virtual int32 KIP_API_TYPE GetInnerNet(int32 Ind) { return pInnerNets->GetAt(Ind); }

    virtual CIntArr* KIP_API_TYPE GetInnerNetArr() { return pInnerNets; }

////////
    virtual void KIP_API_TYPE FillAllModNets()
    {
      int32 i = 0;

      pAllModNets = new CIntArr;
      pAllModNets->GrowBy = 1;

      for(i=0;i < pModElem->numPin;i++)
        pAllModNets->Add(pModElem->pCondNet[i]->name);

      if(pExtendNets != 0)
        pAllModNets->Append(pExtendNets);

      if(pInnerNets != 0)
        pAllModNets->Append(pInnerNets);
    }

    virtual void KIP_API_TYPE ClearAllModNets()
    {
      if(pAllModNets != 0)
      {
        delete pAllModNets;
        pAllModNets = 0;
      }
    }

    virtual CIntArr* KIP_API_TYPE GetAllModNets() { return pAllModNets; }

////////
    virtual bOOL KIP_API_TYPE IsModPinLinksExist() { return fALSE; }
    virtual void KIP_API_TYPE FillModPinLinks() {}

    virtual void KIP_API_TYPE SetModPinLinksStructSym()
    {
      int32 sz = GetAllNumPinNodes();

      for(int32 i=0;i < sz;i++)
      {
        for(int32 j=0;j < sz;j++)
        {
          if(1 == ppModPinLinks[i][j])
            ppModPinLinks[j][i] = 1;
        }
      }
    }

////////  Функции для определения специфических узлов модели элемента.
    virtual void KIP_API_TYPE GetModIndCurStream(CIntArr *pNArr);
    virtual void KIP_API_TYPE GetModIndSpecNodes(CIntArr *pNArr);

////////
    virtual bOOL KIP_API_TYPE IsFreqNeededAC() { return fALSE; }

///
///  Возвращает количество шумовых источников.
    virtual int32 KIP_API_TYPE GetNoiseSourceNum() { return 0; }

///
///  Возвращает индексы ног, подключенных к шумовым источникам, согласно сквозной нумерации ног.
///  pOutSourceInds - индексы в tmd файле. Могут быть одинаковыми для разных источников - в этом случае произойдет суммирование.
    virtual int32 KIP_API_TYPE GetNoiseSourceInfo(int32 * /*pNodesInds*/,int32 * /*pNodesZs*/,int32 * /*pOutSourceInds*/) { return 0; }

///
///  Функция для получения Якобиана модели.
    virtual void KIP_API_TYPE PutCondCur(PotNet**,REAL**,int32*,ModControlOption*,SubArr&,bOOL) = 0;
    virtual void KIP_API_TYPE PutCondCurRHS(PotNet **pPhiNets,REAL **pDeltaPhi,int32 *pNets,ModControlOption *pModControlOption,SubArr &subArr,bOOL UsePrevMod)
    {
      PutCondCur(pPhiNets,pDeltaPhi,pNets,pModControlOption,subArr,UsePrevMod);
    }

///
///  Функция для получения Якобиана модели для AC анализа.
    virtual void KIP_API_TYPE PutCondCurAC(int32*,int32,bOOL,SubArr&){}
    virtual void KIP_API_TYPE PutCurrentsAC(PotNet*,PotNet*){}

    virtual void KIP_API_TYPE PutCurrentsPSS(PotNet**,int32*,ModControlOption*,SubArr&){}

///
///  Функция для получения токов шумовых источников.
    virtual void KIP_API_TYPE GetCurrentsNoise(PotNet*,PotNet*,REAL*){}

////////  Функции получения внутренней информации из модели.
    enum ModInfType
    {
      mit_LV = 0,
      mit_LX
    };

    virtual REAL KIP_API_TYPE GetInternalModelInfo(ModInfType /*mit*/,int32 /*mit_ind*/,PotNet ** /*pPhiNets*/) { return 0.0; }

///
///  Функция должна возвращать число, независимое от параметров текущей модели.
///  Т.к. создаваемые для мощности структуры соответствуют типу модели, а не самой модели.
    virtual int32 KIP_API_TYPE GetInternalModelInfoPowerSize() { return 0; }
///
///  Параметр pPhiNets сейчас 0 - вдруг понадобится в будущем.
///  Напряжения и токи берутся с последнего обращения к модели.
    virtual void KIP_API_TYPE GetInternalModelInfoPower(PotNet ** /*pPhiNets*/,REAL * /*pV*/,REAL * /*pI*/) {}

///
///  Должна вернуть tRUE если необходимо, чтобы ModStepSimedNotify была вызвана для модели, когда шаг защитан.
    virtual bOOL KIP_API_TYPE NeedModStepSimedNotify() { return fALSE; }

///
///  Функция вызывается когда шаг защитан для дополнительных действий в модели.
    virtual void KIP_API_TYPE ModStepSimedNotify(ModControlOption*) {};

///
///  Должна вернуть tRUE если необходимо, чтобы ModStepStartNotify была вызвана для модели, в начале рассчета точки.
    virtual bOOL KIP_API_TYPE NeedModStepStartNotify() { return fALSE; }

///
///  Функция вызывается в начале расчета текущей точки для дополнительных действий в модели.
    virtual void KIP_API_TYPE ModStepStartNotify(ModControlOption*) {};

////////  Функции для работы с разделяемыми частями модели (подмоделями) и таблицами.

///
///  Создает новую подмодель.
    virtual void* KIP_API_TYPE NewSubModel(GrpParName * /*pGrpParEl*/,IndParName * /*pIndParEl*/,int32 * /*pErr*/) { return 0; }

///
///  Возвращает текущую подмодель.
    virtual void* KIP_API_TYPE GetSubModel() { return 0; }

///
///  Переинициализирует текущую подмодель по переданной.
    virtual void KIP_API_TYPE PutSubModel(void * /*pSubMod*/) {}

///
///  Очищает переданную подмодель.
    virtual void KIP_API_TYPE ClearSubMod(void * /*pSubMod*/) {}

////////  Очищает переданное инфо о таблице.
    virtual void KIP_API_TYPE ClearTable(void * /*pTableData*/) {}

///////   Получает  некоторые данные  от модели для заполнения  ее  табличной  части
    virtual REAL KIP_API_TYPE GetDataAt3D(CTableBase * /*pTableB*/, REAL  /*x*/, REAL /*y*/, REAL /*z*/, REAL*, REAL*, REAL*){ return 0.0; }  //  трехмерный случай
    virtual REAL KIP_API_TYPE GetDataAt(CTableBase * /*pTableB*/, REAL*  /*pX*/, REAL*  /*pPr*/ , bOOL * /*pPrExist*/) { return 0.0; }  //  многомерный  случай

////////
    REAL KIP_API_TYPE _dExp_limit(REAL arg)
    {
      if(arg > EXP_THRESHOLD)
      {
////////  Линия из точки (EXP_THRESHOLD,MAXEXP).
////////  Закоментированный код - это линия параллельная оси абсцисс.
        return MAXEXP*(arg - EXP_THRESHOLD) + MAXEXP;
//        return MAXEXP;
      }
      else
      {
        if(arg < -EXP_THRESHOLD)
        {
////////  Линия из точки (-EXP_THRESHOLD,MIN_EXP)
////////  Закоментированный код - это линия параллельная оси абсцисс.
          return MIN_EXP*(arg + EXP_THRESHOLD) + MIN_EXP;
//          return MIN_EXP;
        }
        else
        {
          return KIP_EXP(arg);
        }
      }
    }

///
///  Должна вернуть tRUE если необходимо, чтобы GetNextSeparPoint была вызвана для модели.
    virtual bOOL KIP_API_TYPE NeedGetNextSeparPoint() { return fALSE; }

///
///  Функция вызывается для получения следующей точки перегиба.
    virtual bOOL KIP_API_TYPE GetNextSeparPoint(REAL,REAL*,REAL*,PotNet**) { return fALSE; }

    virtual REAL KIP_API_TYPE Slew(REAL expr, REAL maxPosSlope, REAL maxNegSlope, int32 Ind, REAL *Derivativ);

///
    virtual bOOL KIP_API_TYPE IsFinalStepExist() { return fALSE; }

///
    virtual bOOL KIP_API_TYPE IsExtendNetsChanged() { return fALSE; }

///  Внешний для модели gmin нужен, чтобы при необходимости ток в ноге генерился без учета gmin.
    virtual void KIP_API_TYPE FillExtGminPairNets();

    virtual void KIP_API_TYPE InitExtGminPairNets(CIntArr*) {}

    virtual CIntArr* KIP_API_TYPE GetExtGminPairNets() { return pExtGminPairNets; }

    virtual void KIP_API_TYPE ClearExtGminPairNets()
    {
      if(pExtGminPairNets != 0)
      {
        delete pExtGminPairNets;
        pExtGminPairNets = 0;
      }
    }

///
    virtual void KIP_API_TYPE FillFlSourDtNets()
    {
      pFlSourDtNets = new CIntArr;
      pFlSourDtNets->GrowBy = 1;
      pFlSourDtNets->SetSize(pAllModNets->GetSize());

      CIntArr SourDtNets;
      InitSourDtNets(&SourDtNets);
      CompressSourDtNets(&SourDtNets);

      for(int32 i=0; i<SourDtNets.GetSize(); i++)
      {
        int32 SDtNet = SourDtNets.GetAt(i);

        if(SDtNet < 0)  //  Если внутренний узел был "свернут".
          continue;

        pFlSourDtNets->SetAt(SDtNet, 1);
      }
    }

    virtual void KIP_API_TYPE InitSourDtNets(CIntArr *pDtNets)
    {
      for(int32 i=0; i<pAllModNets->GetSize(); i++)
        pDtNets->Add(i);
    }

    void KIP_API_TYPE CompressSourDtNets(CIntArr *pDtNets);

    virtual CIntArr* KIP_API_TYPE GetFlSourDtNets() { return pFlSourDtNets; }

    virtual void KIP_API_TYPE ClearFlSourDtNets()
    {
      if(pFlSourDtNets != 0)
      {
        delete pFlSourDtNets;
        pFlSourDtNets = 0;
      }
    }

    virtual FILE* KIP_API_TYPE FileOpen(const char *filename,const char *mode);

    virtual bOOL KIP_API_TYPE IsDirectMAndRFilled() { return fALSE; }

    void KIP_API_TYPE CompressCondCurMatrix();
    void KIP_API_TYPE InitCondCurMatrix();
    void KIP_API_TYPE InitCondCurMatrixRHS();
    void KIP_API_TYPE SetCondCurMatrix(ModControlOption *pMCO);
    void KIP_API_TYPE SetCondCurMatrixRHS(ModControlOption *pMCO);
    void KIP_API_TYPE SetCondCurMatrixIntegrate(ModControlOption *pMCO);
    void KIP_API_TYPE SetCondCurMatrixOutput(ModControlOption *pMCO);
    void KIP_API_TYPE SetCondCurMatrixRHSHelper(ModControlOption *pMCO, int32 ind);

    virtual void KIP_API_TYPE SetCondCurMatrixOutputExt(REAL *pTExt, int32 Sz);

    virtual void KIP_API_TYPE PutGAcross(ModControlOption *pMCO, REAL valI, REAL G, int32 IN1, int32 IN2);

    virtual bOOL KIP_API_TYPE IsModelSpinLock() { return fALSE; }

    int KIP_API_TYPE IsPortConnected(int node)
    {
      if(node >= 0 && node < pModElem->numPin)
        return 1;
      return 0;
    }
};

class CTypeModelInfo
{
  public:

    CKiparisMods *pKipModsServ;

    int32 TypeModelError;  //  Содержит код ошибки или 0 если нет.

    int32 TypeModelFlags;  //  Содержит флаги, определяющие работу модели.
                           //  Инициализируется в KIPARIS через функцию OnInitModelInfo.

    ModKip *pEmptyModel;      //  Указатель на фиктивную модель.
                              //  Например, для создания таблиц.

    int32 TMIind;

    int32 NumTypeModCounters;
    unsigned long long *pTypeModCounters;

    CTypeModelInfo(CKiparisMods *_pKipModsServ)
    {
      pKipModsServ = _pKipModsServ;

      TypeModelFlags = 0;

      TypeModelError = 0;

      pEmptyModel = 0;

      NumTypeModCounters = 0;
      pTypeModCounters = 0;
    }

    virtual ~CTypeModelInfo()
    {
      delete []pTypeModCounters;
      pTypeModCounters = 0;
      NumTypeModCounters = 0;
    }

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif

////////
    virtual int32 KIP_API_TYPE GetModVersion()
    {
      return MOD_VERSION;
    }

////////
    virtual int32 KIP_API_TYPE GetTypeModelError() { return TypeModelError; }
    virtual void KIP_API_TYPE SetTypeModelError(int32 _TypeModelError) { TypeModelError = _TypeModelError; }

////////
    virtual int32 KIP_API_TYPE GetTypeModelFlags() { return TypeModelFlags; }
    virtual void KIP_API_TYPE SetTypeModelFlags(int32 _TypeModelFlags) { TypeModelFlags = _TypeModelFlags; }

////////  Возвращает тип модели.
    virtual int32 KIP_API_TYPE GetAbsType()
    {
      int32 MT = GetType();

      if(MT != 0)
        return MT;

////////  Если числовой тип модели 0, то попробуем получить его по имени типа модели.
      const char *pMTN = GetTypeCh();

////////  Такое тоже может быть.
      if(0 == pMTN)
        return 0;

      return pKipModsServ->GetTypeModByCh(pMTN);
    }

    virtual int32 KIP_API_TYPE GetType() { return 0; }
    virtual const char* KIP_API_TYPE GetTypeCh() { return 0; }

////////  Создает новый экземпляр модели.
    virtual int32 KIP_API_TYPE CreateModelInst(DataDoc *pDoc,Element *pEl,CParError *pParErrRet) = 0;

////////  Удаляет экземпляр модели.
    virtual void KIP_API_TYPE DeleteModelInst(ModKip *pModel);

////////  Удаляет экземпляр таблицы.
    virtual void KIP_API_TYPE ClearTableData(void *pTableData)
    {
      pEmptyModel->ClearTable(pTableData);
    }

////////  Возвращает новый тип таблицы.
    virtual CTypeTable* KIP_API_TYPE GetNewTypeTable(char * /*pFileName*/) { return 0; }

////////  Удаляет тип таблицы.
    virtual void KIP_API_TYPE DeleteTypeTable(CTypeTable *pTT);

////////
    virtual void KIP_API_TYPE CreateTypeModCounters(ModKip *pModel)
    {
      if(0 == pModel->NumModCounters || NumTypeModCounters != 0)
        return;

      NumTypeModCounters = pModel->NumModCounters;
      pTypeModCounters = new unsigned long long[NumTypeModCounters];
      memset(pTypeModCounters,0,NumTypeModCounters*sizeof(unsigned long long));
    }
};

class TAB_DIM
{
  public:

    REAL TD_MIN_X;
    REAL TD_MAX_X;
    REAL HX;
};

class CTableBase
{
  public:

    int32 TableID;

    CTableBase(int32 _TableID)
    {
      TableID = _TableID;
    }

    virtual ~CTableBase(){}

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif
};

class CBaseClone
{
  public:

    char *pCloneName;
    int32 CloneSubNameIndex;

////////  Хранить решил в текстовом виде, чтобы не было проблем с файлами таблиц.
////////  В файле таблиц сохраняется текстовая строка.
    enum BASEClType
    {
      ClStrType,
      ClParType
    }BaseClType;

    CPtrArr ClonesInfo;

    CBaseClone(BASEClType _BaseClType);
    virtual ~CBaseClone();

    bOOL KIP_API_TYPE CheckClone(IndParName *pIndParEl,GrpParName *pGrpParEl);

    void KIP_API_TYPE SaveClone(IndParName *pIndParEl,GrpParName *pGrpParEl);
};

class CTableClone : public CBaseClone
{
  public:

    CTypeModelInfo *pTableTypeModInfo;

    void *pTableData;

    CTableClone(CTypeModelInfo *_pTableTypeModInfo);
    ~CTableClone();

    void KIP_API_TYPE SaveTableClone(IndParName *pIndParEl,GrpParName *pGrpParEl,void *_pTableData);
};

class CModClone : public CBaseClone
{
  public:

    ModKip *pClonMod;
    void *pCloneSubMod;

    int32 ModRefClone;

    void *pKipCloneData;

    CPtrArr MCSimElems;
    int32 SelectFl;

    CModClone();
    ~CModClone();

    int32 KIP_API_TYPE GetModRefClone();

    void KIP_API_TYPE SaveModClone(IndParName *pIndParEl,GrpParName *pGrpParEl,ModKip *pModel,void *pSubModel);

    void KIP_API_TYPE AddToModClone(ModKip *pModel);

    void KIP_API_TYPE RemoveFromModClone(ModKip *pModel);
};

class CTypeTable
{
  public:

////////  CTypeTable использует стандартные клоны.

    CTypeTable(CTypeModelInfo *pModInfo);
    ~CTypeTable();

    CTypeModelInfo *pTTModInfo;

    CPtrArr *pTypeTablesArr; //  Массив таблиц.

    char *pTableModelFileName;
};

template<typename ModClass> int32 CreateModelInstHelper(ModClass * /*pMDummy*/,CTypeModelInfo *pTMI,DataDoc *pDoc,Element *pEl,CParError *pParErrRet)
{
  int32 numError = 0;

  int32 ModType = pTMI->GetAbsType();

  ModClass *pModClass = new ModClass(pTMI
                                    ,pEl->pGrpParName
                                    ,pEl->pIndParName
                                    ,pEl->elName.name
                                    ,pDoc);

////////
  pEl->pModel = pModClass;
  pModClass->pModElem = pEl;

////////  Инициализируем внутренние параметры модели.
  numError = pModClass->InitParameters(pParErrRet);

////////  Если при инициализации внутренних параметров
////////  произошла ошибка, то удалим модель и выйдем.
  if(numError != 0)
  {
    pEl->pModel = 0;
    delete pModClass;
    return numError;
  }

////////  Пошлем извещение, что создана новая модель.
  pTMI->pKipModsServ->OnNewModel(ModType,pEl->pModel);

  return numError;
}

template<typename ModClass> int32 CreateModelInstHelperClone(ModClass * /*pMDummy*/,CTypeModelInfo *pTMI,DataDoc *pDoc,Element *pEl,CParError *pParErrRet)
{
  int32 numError = 0;

  int32 ModType = pTMI->GetAbsType();

  int32 TypeModFlags = pTMI->GetTypeModelFlags();

  bOOL Cloning = (TypeModFlags & TYPEMODEL_CLONE) == TYPEMODEL_CLONE ? tRUE : fALSE;
  bOOL Tabling = (TypeModFlags & TYPEMODEL_TABLE) == TYPEMODEL_TABLE ? tRUE : fALSE;

  ModClass *pModClass = 0;

////////  Получим клон - если есть. Если клонирование доступно.
  CModClone *pCl = 0;

  if(tRUE == Cloning)
    pCl = pTMI->pKipModsServ->GetCloneModel(ModType,pEl->pIndParName,pEl->pGrpParName);

  if(0 == pCl)
  {
    pModClass = new ModClass(pTMI
                            ,Cloning
                            ,Tabling
                            ,pEl->pGrpParName
                            ,pEl->pIndParName
                            ,pEl->elName.name
                            ,pDoc);

////////
    pEl->pModel = pModClass;
    pModClass->pModElem = pEl;

////////  Инициализируем внутренние параметры модели.
    numError = pModClass->InitParameters(pParErrRet);

////////  Если при инициализации внутренних параметров
////////  произошла ошибка, то удалим модель и выйдем.
    if(numError != 0)
    {
      pModClass->Cloning = fALSE;  //  Чтобы правильно очистилась память.

      pEl->pModel = 0;
      delete pModClass;
      return numError;
    }

////////  Создадим структуры для табличной модели. Если создание таблиц доступно.
    if(tRUE == Tabling)
      pModClass->PutTable();

////////  Инициализируем подмодель. Если клонирование доступно.
    if(tRUE == Cloning)
    {
      void *pClonData = pModClass->GetSubModel();

////////  Пошлем извещение, что подмодель создана и необходимо создать клон.
      pCl = pTMI->pKipModsServ->SetCloneModel(ModType
                                             ,pEl->pIndParName
                                             ,pEl->pGrpParName
                                             ,pModClass
                                             ,pClonData);

////////  Пошлем извещение, что к клону присоединена новая модель.
      pTMI->pKipModsServ->AddCloneModel(pCl,pModClass);
    }
  }
  else
  {
////////  Создаем модель по найденному клону.
    pModClass = new ModClass(pTMI
                            ,Cloning
                            ,Tabling
                            ,pCl->pCloneSubMod);

////////
    pEl->pModel = pModClass;
    pModClass->pModElem = pEl;

////////  Пошлем извещение, что к клону присоединена новая модель.
    pTMI->pKipModsServ->AddCloneModel(pCl,pModClass);
  }

////////  Пошлем извещение, что создана новая модель.
  pTMI->pKipModsServ->OnNewModel(ModType,pEl->pModel);

////////  Инициализируем не клонируемую часть модели.
  if(numError == 0)
    numError = pModClass->InitParametersNC();

  return numError;
}

struct TRANSITION_SCOPE
{
  REAL Y0;
  REAL Y1;
  REAL tbegin;
  REAL tend;
};

typedef std::pair<REAL,REAL> dpair;

class CAbsDelay
{
  public:

    std::deque<dpair> TVpoints;
    ModKip *pModel;
    CKiparisMods *pKMS;
    REAL td;

    CAbsDelay(ModKip *_pmod, REAL _delay)
    {
      pModel = _pmod;
      pKMS = pModel->pTypeModInfo->pKipModsServ;
      td = SecToNanoSec * _delay;
    }
    ~CAbsDelay(){}

    REAL KIP_API_TYPE Delay(REAL expr_value, int32 Ind);
    void KIP_API_TYPE SaveValues(int32 Ind);
    bOOL KIP_API_TYPE GetNextSeparPoint(REAL time, REAL *SepPoint);
};

class CTransition
{
  public:
    REAL scopeYexpr, time_scope_end, time_scope_begin;
    REAL delay, trise, tfall;
    std::deque<TRANSITION_SCOPE> history;
    REAL last, prelast;
    REAL tlast, tprelast;
    REAL expr;
    ModKip *pModel;
    CKiparisMods *pKMS;

    CTransition(ModKip *_pmod, REAL _delay, REAL _trise, REAL _tfall)
    {
      pModel = _pmod;
      pKMS = pModel->pTypeModInfo->pKipModsServ;
      delay = SecToNanoSec * _delay;
      trise = SecToNanoSec * _trise;
      tfall = SecToNanoSec * _tfall;
      if(tfall == 0.0) tfall = trise;
      if(trise == 0.0) trise = tfall = 0.001;  //  default 1ps
    }
    CTransition(ModKip *_pmod, REAL _delay, REAL _trise)
    {
      pModel = _pmod;
      pKMS = pModel->pTypeModInfo->pKipModsServ;
      delay = SecToNanoSec * _delay;
      trise = SecToNanoSec * _trise;
      tfall = trise;
      if(tfall == 0.0) tfall = trise;
      if(trise == 0.0) trise = tfall = 0.001;  //  default 1ps
    }
    ~CTransition(){}

    REAL KIP_API_TYPE Transition(REAL expr_value, int32 Ind);
    void KIP_API_TYPE SaveValues(int32 Ind);
    bOOL KIP_API_TYPE GetNextSeparPoint(REAL time, REAL *SepPoint);
};

class CBoundStep
{
  public:

    REAL BSStep;

    ModKip *pModel;
    CKiparisMods *pKMS;

    CBoundStep(ModKip *_pmod)
    {
      pModel = _pmod;
      pKMS = pModel->pTypeModInfo->pKipModsServ;

      BSStep = 0.0;
    }
    ~CBoundStep(){}

    void KIP_API_TYPE BoundStep(REAL expr_value);
    bOOL KIP_API_TYPE GetNextSeparPoint(REAL time, REAL *SepPoint);
};

class CTableModelFileBase
{
  public:

    CTableModelFileBase();
    virtual ~CTableModelFileBase();

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif

    virtual REAL KIP_API_TYPE GetTMFBValue(REAL *pV, REAL **ppDerV, int32 VSz) = 0;

    virtual REAL KIP_API_TYPE GetTMFBDimMax(int32 dim) = 0;
};

class CTableModel
{
  public:

    ModKip *pModel;

    CTableModelFileBase *pConstTableTMFile;

    CTableModel(ModKip *_pmod);
    ~CTableModel();

    REAL GetTMResult(const std::string &CS, const std::string &FN, bOOL ConstTable, bOOL NeedDeriv, int32 NumArgs,...);

    REAL GetDimMax(const std::string &CS, const std::string &FN, int32 dim);
};

class CTSInfoBase
{
  public:

    CTSInfoBase();
    virtual ~CTSInfoBase();

#ifdef _MSC_VER
    virtual void DummyDestructor(){}
#endif

    virtual void GetValuesYC(REAL f, REAL **ppY, REAL **ppC) = 0;

    virtual int32 GetNumPorts() = 0;
};

class CRandom
{
  public:

    ModKip *pModel;

    CRandom(ModKip *_pmod);
    ~CRandom();
};

class CCross
{
  public:

    ModKip *pModel;
    CKiparisMods *pKMS;

    int32 CrDir;
    REAL CrAbsTimeTol;
    REAL CrAbsExprTol;
    REAL CrRelExprTol;

    CCross(ModKip *_pmod);
    ~CCross();

    bOOL KIP_API_TYPE Cross(REAL  arg, int32 dir, REAL timeTol, REAL exprTol, int32 Ind);
    bOOL KIP_API_TYPE LimitTimeStep(PotNet**, REAL PS, REAL *pCS, int32 Ind);
};

class CTimer
{
  public:

    ModKip *pModel;
    CKiparisMods *pKMS;

    REAL TStartV;
    REAL TPerV;

    CTimer(ModKip *_pmod);
    ~CTimer();

    bOOL KIP_API_TYPE Timer(REAL expr_value, REAL per, REAL tol, int32 dummy);
    bOOL KIP_API_TYPE GetNextSeparPoint(REAL time, REAL *SepPoint);
};

////////  В модуле, реализующем данные интерфейсы,
////////  должны быть реализваны функции GetModelInfo, ClearModelInfo.
typedef void (*GETMODELINFO)(CKiparisMods*,CPtrArr*);
typedef void (*CLEARMODELINFO)(CKiparisMods*,CPtrArr*);

#endif // _KIPMODELS_
