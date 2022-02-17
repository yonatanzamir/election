#pragma once
#include "Party.h"
#include "DynamicArray.h"
class PartyArray
{
	private:
		DynamicArray<Party> parties;
	
	public:
		PartyArray(int phs=2);
		~PartyArray();
		PartyArray(const PartyArray& s);
		PartyArray(istream& in, DistrictArray* da);
		const PartyArray& operator=(const PartyArray& other);
		friend ostream& operator<<(ostream& os, const PartyArray& pa);
		void addParty(const string party_name, Citizen& chief, DistrictArray* size);
		int get_logs() const { return parties.size(); }
		int get_phs() const { return parties.capacity(); }
		void add_district_to_parties();
		int getParty_by_name(string name);
		void Is_Representative(int id);
		Party& getParty_by_index(int index)  { return parties[index]; }
		void is_citizen_chief(int id);
		void save(ostream& out);
		void load(istream& in, DistrictArray* da);
};

