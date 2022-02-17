#pragma once
#include "Citizen.h"
#include "DynamicArray.h"
#include <iostream>
using namespace std;

class CitizenArray
{
	private:

		DynamicArray<Citizen> citizens;
		
	public:
		
		CitizenArray(int phs = 2);
		CitizenArray(istream& in, DistrictArray* da);
		~CitizenArray();
		CitizenArray(const CitizenArray& pa);
		const CitizenArray& operator=(const CitizenArray& other);
		friend ostream& operator<<(ostream& os, const CitizenArray& ca);
		Citizen* FindCitizenById(int id);
		void addCitizen(const Citizen& c);
		int get_logs() const { return citizens.size(); }
		int get_phs() const { return  citizens.capacity(); }
		void UpdateCitizen(int size);
		Citizen& get_citizen_by_index(int index){ return citizens[index]; }
		void save(ostream& out) ;
		void load(istream& in, DistrictArray* da);
		void load(istream& in, District* d);
		int LengthIdCalc(int num);
};

