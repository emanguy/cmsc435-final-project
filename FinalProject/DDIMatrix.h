#ifndef __DDIMATRIX_H__
#define __DDIMATRIX_H__

#include <list>
#include <map>
#include <string.h>

enum eDataType{
    INTERIM = 0,
    PLANNING,
    FINAL,
};


typedef std::list<std::string> ListS;
typedef ListS::iterator ItS;

/*******************************************************************************
 Matrix entry, contains market ID, demo ID and a list of data
*******************************************************************************/
class DDIMatrixEntry{
 public:
  DDIMatrixEntry(std::string market, std::string demo, float data);
  ~DDIMatrixEntry();
  void addData(eDataType type, float data);
  std::string getMarketID();
  std::string getDemoID();
  float getData(eDataType type);
  bool match(std::string marketID, std::string demoID);
  std::string dump();
 private:
  float mDataList[FINAL + 1];
  std::string mMarketID;
  std::string mDemoID;
};

/*******************************************************************************
 DDI Matrix entry, contains a list of DDIMatrixEntry, and two maps that get the
 colunms matching "market" or "demo"
*******************************************************************************/
class DDIMatrix{
 public:
  DDIMatrix(std::string interim, std::string planning, std::string final);
  ~DDIMatrix();

//print out the information
  void dump();
  void dumpMarket();
  void dumpDemo();
  void dumpMarket(std::string marketID);
  void dumpDemo(std::string demoID);

// Get names of all demos/markets
  ListS getDemos();
  ListS getMarkets();

//return information for a specific demo
  ListS getMarket(std::string demoID);// return a list of markets in demoID
  int getMCount(std::string demoID);//return the number of markets in demoID
//return the distribution of a data type in demoID
  float getMMin(std::string demoID, eDataType type);
  float getMMax(std::string demoID, eDataType type);
  float getMAvg(std::string demoID, eDataType type);
  float getMStd(std::string demoID, eDataType type);

//return information for a specific market
  ListS getDemo(std::string marketID);// return a list of demos in marketID
  int getDCount(std::string marketID);//return the number of demos in marketID
//return the distribution of a data type in marketID
  float getDMin(std::string marketID, eDataType type);
  float getDMax(std::string marketID, eDataType type);
  float getDAvg(std::string marketID, eDataType type);
  float getDStd(std::string marketID, eDataType type);

//return the ddi of a type for a specific marketID and demoID
  float getDDI(std::string marketID, std::string demoID, eDataType type);

 protected:
  void parseFile(std::string filename, eDataType type);
  void updateData(std::string marketID, std::string demoID, eDataType type, float data);
  void updateMarketMap(DDIMatrixEntry* pEntry);
  void updateDemoMap(DDIMatrixEntry* pEntry);

 private:
  std::list<DDIMatrixEntry*> mDataList;
  std::map<std::string, std::list<DDIMatrixEntry*> > mMarketMap;
  std::map<std::string, std::list<DDIMatrixEntry*> > mDemoMap;
};

#endif
