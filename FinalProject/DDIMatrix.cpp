#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "DDIMatrix.h"

using namespace std;



/*******************************************************************************
 Just assign value, no parsing here.
*******************************************************************************/
DDIMatrixEntry::DDIMatrixEntry(string market, string demo, float data)
{
  // Using -1 to indicate the data is void
  for(int i = INTERIM; i <= FINAL; i++){
    this->mDataList[i] = -1;
  }
  this->mMarketID = market;
  this->mDemoID = demo;
  this->mDataList[INTERIM] = data;
}

DDIMatrixEntry::~DDIMatrixEntry()
{
  cout << "Entry destroyed" << endl;
}

string DDIMatrixEntry::dump()
{
  stringstream s;
  s << mMarketID << ", " << mDemoID << ", "
    << mDataList[INTERIM] << ":"
    << mDataList[PLANNING] << ":"
    << mDataList[FINAL] << ":"
    << endl;

  return s.str();
}

void DDIMatrixEntry::addData(eDataType type, float data)
{
  this->mDataList[type] = data;
}

string DDIMatrixEntry::getMarketID()
{
  return this->mMarketID;
}

string DDIMatrixEntry::getDemoID()
{
  return this->mDemoID;
}

float DDIMatrixEntry::getData(eDataType type)
{
  return this->mDataList[type];
}

bool DDIMatrixEntry::match(std::string marketID, std::string demoID)
{
  if( (0 == marketID.compare(this->mMarketID)) &&
      (0 == demoID.compare(this->mDemoID))){
    return true;
  }

  return false;
}

/*******************************************************************************
 The implementation of DDIMatrix.
*******************************************************************************/

DDIMatrix::DDIMatrix(string interim, string planning, string final)
{
  parseFile(interim, INTERIM);
  parseFile(planning, PLANNING);
  parseFile(final, FINAL);
}

void DDIMatrix::parseFile(string filename, eDataType type)
{
  string demo, market, ddis;
  float data;
  ifstream file(filename);
  string dummyLine;
  getline(file, dummyLine);
  if(file.good()){
    while(getline(file, demo, ',')){
      getline(file, market, ',') ;
      getline(file, ddis); 
      data = atof(ddis.c_str());
      //      cout << "Read data: " << data << endl;
      updateData(market, demo, type, data);
    }
  }
  else{
    cout << "file not good: " << filename << endl;
  }

}

void DDIMatrix::updateData(std::string marketID, std::string demoID, eDataType type, float data)
{
  // Check if there is already an entry with the marketID and demoID
  list<DDIMatrixEntry*>::iterator it;
  for(it = mDataList.begin(); it != mDataList.end(); it++){
    //    cout << "Matching " << marketID << ", " << demoID
    //	 << " with " << (*it)->getMarketID() << ", " << (*it)->getDemoID() << endl;
    if((*it)->match(marketID, demoID)){
      (*it)->addData(type, data);
      //      cout << "Already exist: " << (*it)->dump() << ". Adding: " << data << endl;
      return;
    }
  }

  // If read for the first time, it must be interim data.
  DDIMatrixEntry* pTemp = new DDIMatrixEntry(marketID, demoID, data);
  this->mDataList.push_back(pTemp);

  if(type == INTERIM){
    updateMarketMap(pTemp);
    updateDemoMap(pTemp);
  }
}

void DDIMatrix::updateMarketMap(DDIMatrixEntry* pEntry)
{
  map<string, list<DDIMatrixEntry*> >::iterator it = mMarketMap.find(pEntry->getMarketID());

  if(it == mMarketMap.end()){
    string market = pEntry->getMarketID();
    list<DDIMatrixEntry*> l;
    l.push_back(pEntry);
    pair<string, list<DDIMatrixEntry*> > p(market, l);
    mMarketMap.insert(p);
  }
  else{
    it->second.push_back(pEntry);
  }
}

void DDIMatrix::updateDemoMap(DDIMatrixEntry* pEntry)
{
  map<string, list<DDIMatrixEntry*> >::iterator it = mDemoMap.find(pEntry->getDemoID());

  if(it == mDemoMap.end()){
    string demo = pEntry->getDemoID();
    list<DDIMatrixEntry*> l;
    l.push_back(pEntry);
    pair<string, list<DDIMatrixEntry*> > p(demo, l);
    mDemoMap.insert(p);
  }
  else{
    it->second.push_back(pEntry);
  }
}

DDIMatrix::~DDIMatrix()
{
}

void DDIMatrix::dump()
{
  list<DDIMatrixEntry*>::iterator it;
  for(it = mDataList.begin(); it != mDataList.end(); it++){
    cout << (*it)->dump();
  }
}

void DDIMatrix::dumpMarket()
{
  map<string, list<DDIMatrixEntry*> >::iterator itMap;
  for(itMap = mMarketMap.begin(); itMap != mMarketMap.end(); itMap++){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      cout << (*it)->dump();
    }
  }
}

void DDIMatrix::dumpDemo()
{
  map<string, list<DDIMatrixEntry*> >::iterator itMap;
  for(itMap = mDemoMap.begin(); itMap != mDemoMap.end(); itMap++){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      cout << (*it)->dump();
    }
  }
}

void DDIMatrix::dumpMarket(string marketID)
{
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      cout << (*it)->dump();
    }
  }
}

void DDIMatrix::dumpDemo(string demoID)
{
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      cout << (*it)->dump();
    }
  }
}

ListS DDIMatrix::getDemos()
{
    ListS demos;
    map<string, list<DDIMatrixEntry*> >::iterator iterator;

    for (iterator = mDemoMap.begin(); iterator != mDemoMap.end(); iterator++)
    {
        demos.push_back(iterator->first);
    }

    return demos;
}

ListS DDIMatrix::getMarkets()
{
    ListS markets;
    map<string, list<DDIMatrixEntry*> >::iterator iterator;

    for (iterator = mMarketMap.begin(); iterator != mMarketMap.end(); iterator++)
    {
        markets.push_back(iterator->first);
    }

    return markets;
}

ListS DDIMatrix::getMarket(string demoID){
  ListS result;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      string temp = (*it)->getMarketID();
      result.push_back(temp);
    }
  }
  return result;
}

ListS DDIMatrix::getDemo(string marketID){
  ListS result;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      string temp = (*it)->getDemoID();
      result.push_back(temp);
    }
  }
  return result;
}

int DDIMatrix::getMCount(string demoID){
  int result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      result++;
    }
  }
  return result;
}

int DDIMatrix::getDCount(string marketID){
  int result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      result++;
    }
  }
  return result;
}

float DDIMatrix::getMMin(string demoID, eDataType type){
  float result = 1000;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      if(result > temp){
        result = temp;
      }
    }
  }
  return result;
}

float DDIMatrix::getDMin(string marketID, eDataType type){
  float result = 1000;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      if(result > temp){
        result = temp;
      }
    }
  }
  return result;
}


float DDIMatrix::getMMax(string demoID, eDataType type){
  float result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      if(result < temp){
        result = temp;
      }
    }
  }
  return result;
}

float DDIMatrix::getDMax(string marketID, eDataType type){
  float result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      if(result < temp){
        result = temp;
      }
    }
  }
  return result;
}

float DDIMatrix::getMAvg(string demoID, eDataType type){
  float result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      result += temp;
    }
  }
  int count = getMCount(demoID);
  return result/count;
}

float DDIMatrix::getDAvg(string marketID, eDataType type){
  float result = 0;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      result += temp;
    }
  }
  int count = getDCount(marketID);
  return result/count;
}

float DDIMatrix::getMStd(string demoID, eDataType type){
  float result = 0;
  float avg = getMAvg(demoID, type);
  int count = getMCount(demoID);
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mDemoMap.find(demoID);
  if(itMap != mDemoMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      result += temp * temp;
    }
  }
  result = result/count;
  result -= avg * avg;
  
  return sqrt(result);
}

float DDIMatrix::getDStd(string marketID, eDataType type){
  float result = 0;
  float avg = getDAvg(marketID, type);
  int count = getDCount(marketID);
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      float temp = (*it)->getData(type);
      result += temp * temp;
    }
  }
  result = result/count;
  result -= avg * avg;
  
  return sqrt(result);
}

float DDIMatrix::getDDI(std::string marketID, std::string demoID, eDataType type){
  float result;
  map<string, list<DDIMatrixEntry*> >::iterator itMap = mMarketMap.find(marketID);
  if(itMap != mMarketMap.end()){
    list<DDIMatrixEntry*>::iterator it;
    for(it = itMap->second.begin(); it != itMap->second.end(); it++){
      string temp = (*it)->getDemoID();
      if(temp== demoID){
        result = (*it)->getData(type);
      }
    }
  }
  return result;
}


