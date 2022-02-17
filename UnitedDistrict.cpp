#include "UnitedDistrict.h"
#include "PartyArray.h"

UnitedDistrict::UnitedDistrict(const string name, int number_of_representatives, PartyArray* results_size) : District(name, number_of_representatives, results_size) // United district constructor
{
	this->winner_party = -1;
}
UnitedDistrict::UnitedDistrict(istream& in, DistrictArray* da, PartyArray* pa) : District("", 1, pa) // load constructor
{
	this->winner_party = -1;
	load(in, da, pa);
}

void UnitedDistrict::save(ostream& out)
{
	District::save(out);
	out.write(rcastcc(&winner_party), sizeof(winner_party));
	if (!out.good())
		throw Saveexception();
}

void UnitedDistrict::load(istream& in, DistrictArray* da, PartyArray* pa)
{
	District::load(in, da, pa);
	in.read(rcastc(&winner_party), sizeof(winner_party));
	if (!in.good())
		throw Loadexception();
}

UnitedDistrict::~UnitedDistrict() // United district destructor
{

}

UnitedDistrict::UnitedDistrict(const UnitedDistrict& ud)   // United district copy constructor
{
	*this = ud;
}

const UnitedDistrict& UnitedDistrict::operator=(const UnitedDistrict& other) // United operator=
{
	District::operator=(other);
	this->winner_party = other.winner_party;
	return *this;
}

void UnitedDistrict::calc_winner_party() // set the winner party after the election round was done
{
	int max = -1;
	int winner=-1;
	for (int i = 0; i < results_size->get_logs(); i++)
		if (results[i].votes_number > max)
		{
			max = results[i].votes_number;
			winner = i;
		}
	this->winner_party = winner;
}