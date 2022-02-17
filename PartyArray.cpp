#include "PartyArray.h"

PartyArray::PartyArray(int phs) : parties(phs)// PartyArray constructor
{

}

PartyArray::PartyArray(istream& in, DistrictArray* da) :parties(0) // load constructor
{

	load(in, da);

}

void PartyArray::save(ostream& out) 
{
	parties.save(out);
}

void PartyArray::load(istream& in, DistrictArray* da)
{
	int logs;
	in.read(rcastc(&logs), sizeof(logs));
	if (!in.good())
		throw Loadexception();
	Party temp;
	parties.clear();
	for (int i = 0; i < logs; i++)
	{
		temp.load(in, da);
		parties.push_back(temp);
	}
}

PartyArray::~PartyArray() // PartyArray destructor
{
}

PartyArray::PartyArray(const PartyArray& pa) // PartyArray copy constructor
{
	*this = pa;
}

const PartyArray& PartyArray::operator=(const PartyArray& other) // PartyArray operator=
{
	if (this != &other)
	{
		parties = other.parties;
	}

	return *this;
}

ostream& operator<<(ostream& os, const PartyArray& pa) // PartyArray operator<<
{
	os << pa.parties;
	return os;
}


void PartyArray::addParty(const string party_name, Citizen& chief, DistrictArray* size) //  gets party details and add the party to the array and resize if the place is over
{
	Party p(party_name, chief, size);
	parties.push_back(p);
}


void PartyArray::add_district_to_parties() // add the new district to all the parties
{
	auto itr = parties.begin();
	for (; itr != parties.end(); itr++)
	{
		if ((*itr).get_num_of_districts() + 1 >= (*itr).get_phs())
		{
			(*itr).resize_representatives_by_district();
		}
	}
}

int PartyArray::getParty_by_name(string name) // gets name and return the index of the party id it is exist
{
	int i = 0;
	bool flag = false;
	int res = -1;
	auto itr = parties.begin();
	while (itr != parties.end() && !flag)
	{
		if (name == (*itr).get_name())
		{
			res = i;
			flag = true;
		}
		i++;
		itr++;
	}
	return res;
}
void PartyArray::Is_Representative(int id) // gets id and return true if this is a representative of the party or false if it's not
{
	int j;
	auto itri = parties.begin();
	for (; itri != parties.end(); itri++)
	{
		if (id == (*itri).get_chief_party().getId())
			throw logic_error("Citizen is already a chief, can't be representative");
		for (j = 0; j < (*itri).get_num_of_districts(); j++)
		{
			if ((*itri).get_representatives_by_district()[j].FindCitizenById(id) != nullptr)
				throw logic_error("Citizen is already a representative");
		}
	}
}

void PartyArray::is_citizen_chief(int id) // gets id and return true if this citizen is a chief in one of the parties
{
	auto itr = parties.begin();
	for (; itr != parties.end(); itr++)
	{
		if (id == (*itr).get_chief_party().getId())
		throw logic_error("Citizen is already chief");
	}
}