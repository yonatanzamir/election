#pragma once
#include "CitizenArray.h"
#include "DistrictArray.h"

class Party
{
	private:
		string party_name;
		Citizen chief_party;
		vector<CitizenArray> representatives_by_district;
		DistrictArray* size;
	public:
		Party(const string party_name, Citizen& chief_party, DistrictArray* size);
		Party() { representatives_by_district.clear(); }
		Party(istream& in, DistrictArray* da);
		~Party();
		Party(const Party& p);
		const Party& operator=(const Party& other);
		friend ostream& operator<<(ostream& os, const Party& p);
		void addRepresentative(const Citizen& representative, int district_num);
		int get_num_of_districts() const { return size->get_logs(); }
		int get_phs() const { return size->get_phs(); }
		string get_name() const { return party_name; }
		void resize_representatives_by_district();
		vector<CitizenArray>& get_representatives_by_district()  { return representatives_by_district; }
		const Citizen& get_chief_party() const { return chief_party; }
		void save(ostream& out);
		void load(istream& in, DistrictArray* da);
};

