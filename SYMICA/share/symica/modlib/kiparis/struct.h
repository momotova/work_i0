#pragma once
#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <vector>

#include "kiparis/paramt.h"

#define MOD_MASK(t) (static_cast<int>((t)&0xFF000000))

class CDFModelView;

enum KipRetCode
{
  KRC_NORMEND = 0,
  KRC_NORMCON = 2,

  KRC_ERROR1 = 1,
  KRC_ERROR2 = -1,

  KRC_ERROR3 = -2
};

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wuseless-cast"
#endif // __GNUC__ version

typedef int elem_t;
typedef std::vector<elem_t> elem_vector;
const elem_t NO_ELEMENT = static_cast<elem_t>( -1 );

typedef int node_t;
typedef std::vector<node_t> node_vector;
const node_t NO_NODE = static_cast<node_t>( -1 );

typedef int pin_t;
const pin_t NO_PIN = static_cast<pin_t>( -1 );

const int NO_INDEX = -1;
const size_t NO_INDEX_U = static_cast<size_t>( -1 );

#if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6))
#  pragma GCC diagnostic pop
#endif // __GNUC__ version


struct CirSize
{
  CirSize() : numInerNet(0), numNet(0), numElement(0) {}

  node_t    numInerNet;  // количество внутренних цепей.
  node_t    numNet;      // общее количество цепей.
  elem_t    numElement;  // количество эл-тов.
};

struct ElemName
{
  ElemName() : name(0), pchName(NULL) {}

  elem_t      name;      // номер - идентификатор.
  char*       pchName;   // может быть 0.
};

struct Net;
class ModKip;

typedef std::vector<GrpParName*> GrpParNameVec;

struct Element
{
  Element() : elName(), flag(0), pModel(NULL),
              pIndParName(NULL), pElCdfMV(NULL), pGrpParName(NULL),
              pElemModels(NULL), pCondNet(NULL), numPin(0), model_override(-1) {}

  ElemName elName;

  int flag;

  ModKip *pModel;

  IndParName *pIndParName;
  const CDFModelView *pElCdfMV;

  GrpParName *pGrpParName; // NULL, selected in KIPARIS during sweep adjustment
  GrpParNameVec *pElemModels;

  Net **pCondNet;
  pin_t numPin;
  
  int model_override;
};

struct Net  //  —начала идут внутренние узлы, потом земл€, потом внешние узлы. ѕример: 1, 0, -1.
{
  Net() : name(0), numElemList(0), NetNeighbor(0), analog(0), pElConNet(NULL) {}

  node_t    name;        // номер - идентификатор (1, 2, ... 0(gnd), -1, -2, ...).
  elem_t    numElemList; // количество подсоединенных элементов.
  node_t    NetNeighbor;
  int       analog;      // используетс€ дл€ указани€ цепи расчЄт дл€ которой будет производитс€ всегда
                         // 0 - DIGITAL, 1 - ANALOG
  Element **pElConNet;
};

typedef char*      pChar;
typedef Net*       pNet;
typedef const Net* pcNet;
typedef Element*       pElement;
typedef const Element* pcElement;

#endif  // STRUCTS_H
