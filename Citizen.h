#pragma once
#include <iostream>
#include <string>
#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>
class District;
class DistrictArray;
using namespace std;
class Citizen
{
private:

	string name;
	int id;
	District* d;
	int birth_year;
	bool isVoted;
public:
	Citizen(const string name, int id, int birth_year, District* d, bool isVoted = false);
	Citizen(istream& in, DistrictArray* da);
	~Citizen();
	Citizen() {  d = nullptr; }
	Citizen(const Citizen& c);
	const Citizen& operator=(const Citizen& other);
	bool operator==(const Citizen& other) const;
	friend ostream& operator<<(ostream& os, const Citizen& c);
	const string& getName() const { return name; }
	int getId()  const { return id; }
	int getBirth_Year() const { return birth_year; }
	District* getDistrict_Citizen() const { return d; }
	bool getIs_Voted() const { return isVoted; }
	void set_isVoted(bool isvoted);
	void set_district(District* district);
	void save(ostream& out);
	void load(istream& in, DistrictArray* da);
	void load(istream& in, District* d);
	int setlength(istream& in);
    int LengthIdCalc(int num);
};