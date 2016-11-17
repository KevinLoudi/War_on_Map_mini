/*
 Mostly for STL
*/
#ifndef BASIC_MODEL_H
#define BASIC_MODEL_H

#include <list>
#include <vector>
#include <set>
#include <map>

#include <fstream>
#include <sstream>
#include <string>

#include <algorithm>
#include <iostream>
#include "basicPlace.h"
#define CITYLIST "CityList.txt"
using namespace std;

typedef unsigned long UINT;

//reserved is-a relationship for potential usage;
class Method
{
  private:
  	list<int> _num;
  	list<string> _name;
  protected:

  public:
  	explicit Method(){}
  	virtual ~Method(){}
};

//inital class object through I/O
class FileIO: public Method
{
  private:
  	string _Filename; 
  protected:
  	static UINT _IO_Num;
  public:
  	explicit FileIO(const string & fn):_Filename(fn){_IO_Num++;}

  	//reset the number of objects need operation
  	void resetFilename(const string & newname);

  	//I/O operations for an array of city objects
  	bool readBinaryFile(string* filename,City* c, UINT len);
  	//save city object in binary file
  	bool saveBinaryFile(City* c,UINT len) const;
  	//read text file
    bool readTextFile(City* c,UINT);
  	//bool saveBinaryFile(City* c,UINT len) const;

  	~FileIO(){_IO_Num--;}
};


//varous functor
//applied to cout all elements with for_each
template<typename eleType>
struct displayElement
{
  int m_count; //hold the count
  displayElement() {m_count=0;}//constructor
  void operator()(const eleType & element)
  {
    ++m_count; //record the elements being output
    cout<<element<<' ';
  }
};

//for map or set
template<typename assContainer>
struct displayAssElement
{
  int m_count; //hold the count
  displayElement() {m_count=0;}//constructor
  //no longer a const function for it modify a data field inside
  void operator()(const assContainer & element)
  {
    ++m_count; //record the elements being output
    cout<<element.first<<" : "<<element.second<<endl;
  }
};

//make non-sensitive comparsion 
struct caseInsensitive
{
  bool operator()(const string & str1,const string & str2) const
  {
    string str1_tmp(str1); string str2_tmp(str2);
    //transform to remove the case-sensitivity of city name
    //wrote - the "::" selects the tolower from the global namespace, or it will crash
    transform(str1.begin(),str1.end(),str1_tmp.begin(),::tolower);
    transform(str2.begin(),str2.end(),str2_tmp.begin(),::tolower);

    return(str1_tmp<str2_tmp);
  };
};

//make comparsion on the length of key-string
struct comparelength 
{
  bool operator() (const string& str1, const string& str2)
  {
    return str1.length() < str2.length(); 
  }
};

//rank by value
typedef pair<string,UINT> PAIR_STRING_UINT;
struct compareValue
{
	bool operator()(const PAIR_STRING_UINT& P1, const PAIR_STRING_UINT& P2)
  {
    return P1.second<P2.second;
  }
};

//logical "larger than" with two-parmeters-functor
template <typename numType>
struct isLarger
{
  numType _Thershold;
  //initial a user-defined thershold
  isLarger(const numType& thershold=0):_Thershold(thershold){}

  bool operator()(const numType& element) const
  {
    return element>_Thershold;
  }
};

//multiply two containers' items, used in operation- "transform"
template <typename eleType>
class Multiplyer
{
  public:
    eleType operator()(const eleType& elem1, const eleType& elem2)
    {
      return elem1*elem2;
    }
};
 
class Rank: public Method
{
  private:
	//order in decreasing order
	typedef map<string,UINT,comparelength> MAP_STRING_UINT;
	typedef set<string> SET_STRING;
	typedef vector<City> VECTOR_CITY;
    MAP_STRING_UINT _rank;
    VECTOR_CITY _cities;
    SET_STRING _citynames; //city name is unqiue
    
    //print contents of sequence container
    template<typename Container>
    void printContents(const Container & stlcontainer);

    //print contents of associative container
    template<typename assConatiner>
    void printAssContainer(const assConatiner & asscontainer);
	protected:
		
	public:
    explicit Rank(VECTOR_CITY & incity):_cities(incity)
    {
     //initialize city rank and name dir
     VECTOR_CITY::iterator it = incity.begin();
     VECTOR_CITY::iterator it_end = incity.end();

     for (;it != it_end;it++)
     {
        _citynames.insert((*it)._cityInfo->_CityName);

        //_rank.insert(make_pair((*it)._cityInfo->_CityName,(*it)._cityInfo->_PopulationDensity)); 
        _rank.insert(pair<string,UINT>((*it)._cityInfo->_CityName,(*it)._cityInfo->_PopulationDensity));
        //_rank[(*it)._cityInfo->_CityName]=(*it)._cityInfo->_PopulationDensity);
     } 

     //display elements using std::for_each
     displayElement<string> mResult;
     mResult=for_each(_citynames.begin(),_citynames.end(),displayElement<string>());
     cout<<"\n Totally "<<mResult.m_count<<" have been print."<<endl;
     cout<<"\n********************************Initialized****************\n";
    }

    //add city to rank
    bool addCityname(const string & cityname);
	//remove city from rank
    bool removeCityname(const string & cityname);
    //print cityname and rank by order
	void printCityname();
    void printRank();
	//look up a city with name
	bool isexist(const string & cityname);
	//customerize functor for ranking
	void rankbyValue();
	//find element larger than stand
	void findLargerItems(const UINT & stand, vector<string>& v_find_result) const;
    //multiply and return are*population
    void transformtoPopluation(const UINT & citynum,vector<UINT>& pop) const;
    ~Rank(){}
};


#endif
