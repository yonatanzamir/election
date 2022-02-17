#include "Party.h"
#include "Citizen.h"
#include <string.h>
#pragma warning(disable: 4996) // disable strcpy error

Party::Party(const string _party_name, Citizen& chief_party, DistrictArray* size) : 
	chief_party(chief_party), size(nullptr) 	// Party constructor
{
	this->size = size;
	this->party_name = _party_name;
	representatives_by_district.reserve((this->size)->get_phs());
	representatives_by_district.resize((this->size)->get_logs());    
}

Party::Party(istream& in, DistrictArray* da) : size(nullptr) // load constructor
{

	load(in, da);

}
void Party::save(ostream& out)
{
	int district_phs, district_logs;
	district_phs = size->get_phs();
	district_logs = size->get_logs();
	int length =party_name.size();
	out.write(rcastcc(&length), sizeof(length));
	out.write(rcastcc(party_name.c_str()), length);
	chief_party.save(out);

	out.write(rcastcc(&district_phs), sizeof(district_phs));
	out.write(rcastcc(&district_logs), sizeof(district_logs));

	if (!out.good())
		throw Saveexception();

	for (int i = 0; i < district_logs; i++)
	{
		representatives_by_district[i].save(out);
	}
}
void Party::load(istream& in, DistrictArray* da)
{
	int length;
	int district_phs, district_logs;
	in.read(rcastc(&length), sizeof(length));

	if (!in.good())
		throw Loadexception();
	party_name.resize(length);
	in.read(rcastc(&party_name[0]), length);
	chief_party.load(in, da);
	in.read(rcastc(&district_phs), sizeof(district_phs));
	in.read(rcastc(&district_logs), sizeof(district_logs));

	if (representatives_by_district.size() != 0)
		 representatives_by_district.clear();

	if (!in.good())
		throw Loadexception();
	representatives_by_district.reserve(district_phs);
	representatives_by_district.resize(district_logs);
	for (int i = 0; i < district_logs; i++)
	{
		representatives_by_district[i].load(in, da);
	}		
	size = da;

}


Party::~Party() // Party destructor
{

}

Party::Party(const Party& p) // Party copy constructor
{
	*this = p;
}
const Party& Party::operator=(const Party& other) // Party operator=
{
	if (this != &other)
	{
		representatives_by_district.clear();
		chief_party = other.chief_party;
		size = other.size;

		party_name = other.party_name;

		representatives_by_district.reserve((other.size)->get_phs());
		representatives_by_district.resize((other.size)->get_logs());
		for (int i = 0; i < (other.size)->get_logs(); i++)
			representatives_by_district[i] = other.representatives_by_district[i]; 
	}

	return *this;
}

ostream& operator<<(ostream& os, const Party& p) // Party operator<<
{
	os << p.party_name << ", " << p.chief_party << endl;
	for (int i = 0; i < (p.size)->get_logs(); i++)
	{
		os << "participants of the party from district "<< i << ": "<< endl;
		os << p.representatives_by_district[i] << endl;
	}
	return os;
}

void Party::addRepresentative(const Citizen& representative, int district_num) // add new representative to the representatives array in the current party and according the district number
{
	representatives_by_district[district_num].addCitizen(representative);
}

void Party::resize_representatives_by_district() // resize the representatives array and double the physical size
{

	representatives_by_district.reserve(size->get_phs());
	representatives_by_district.resize(size->get_logs());


}


