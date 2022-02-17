#include "DividedDistrict.h"
#include "District.h"
#include "PartyArray.h"

DividedDistrict::DividedDistrict(const string name, int number_of_representatives, PartyArray* results_size) : District(name, number_of_representatives, results_size) // regular constructor 
{
	sorted_results.clear();
}


DividedDistrict::DividedDistrict(istream& in, DistrictArray* da, PartyArray* pa) : District("", 1, pa) // load constructor 
{
	sorted_results.clear();
	load(in, da, pa);
}

void DividedDistrict::save(ostream& out) 
{
	District::save(out);
	int sorted_phs = 0;
	int sorted_logs = 0;
	if (sorted_results.size() != 0)
	{
		sorted_phs = this->results_size->get_phs();
		sorted_logs = this->results_size->get_logs();

		out.write(rcastcc(&sorted_phs), sizeof(sorted_phs));
		out.write(rcastcc(&sorted_logs), sizeof(sorted_logs));

		if (!out.good())
			throw Saveexception();

		for (int i = 0; i < sorted_logs; i++)
		{
			out.write(rcastcc(&(sorted_results[i].party_id)), sizeof(int));
			out.write(rcastcc(&sorted_results[i].party_res.votes_number), sizeof(int));
			sorted_results[i].party_res.elected_representatives.save(out);
		}
	}
	else
	{
		out.write(rcastcc(&sorted_phs), sizeof(sorted_phs));
	}

	if (!out.good())
		throw Saveexception();

}

void DividedDistrict::load(istream& in, DistrictArray* da, PartyArray* pa)
{
	District::load(in, da, pa);
	int sorted_phs;
	int sorted_logs;
	if (sorted_results.size() != 0)
	{
		sorted_results.clear();
	}
	in.read(rcastc(&sorted_phs), sizeof(sorted_phs));
	if (!in.good())
		throw Loadexception();

	if (sorted_phs != 0)
	{
		sorted_results.reserve(sorted_phs);
		in.read(rcastc(&sorted_logs), sizeof(sorted_logs));
		if (!in.good())
			throw Loadexception();
		sorted_results.resize(sorted_logs);
		for (int i = 0; i < sorted_logs; i++)
		{
			in.read(rcastc(&(sorted_results[i].party_id)), sizeof(int));
			in.read(rcastc(&sorted_results[i].party_res.votes_number), sizeof(int));
			sorted_results[i].party_res.elected_representatives.load(in, da);
		}
		if (!in.good())
			throw Loadexception();
	}

	else
		sorted_results.clear();
}
DividedDistrict::~DividedDistrict() // DividedDistrict destructor 
{
}
DividedDistrict::DividedDistrict(const DividedDistrict& dd) // DividedDistrict copy constructor 
{
	*this = dd;
}

const DividedDistrict& DividedDistrict:: operator=(const DividedDistrict& other) // DividedDistrict operator= 
{

	if (this != &other)
	{
		District:: operator=(other);
		sorted_results.clear();
		sorted_results.reserve(other.results_size->get_phs());
		int size = other.results_size->get_logs();
		sorted_results.resize(size);
		if (other.sorted_results.size() != 0)
		{
			for (int i = 0; i < size; i++)
			{
				sorted_results[i].party_id = other.sorted_results[i].party_id;
				sorted_results[i].party_res.elected_representatives = other.sorted_results[i].party_res.elected_representatives;
				sorted_results[i].party_res.votes_number = other.sorted_results[i].party_res.votes_number;
			}
		}
	}
	return *this;
}
 bool compareRepresentatives(DividedDistrict::PartyResult_sorted a, DividedDistrict::PartyResult_sorted b)// for stl sort 
 {
	if (a.party_res.elected_representatives.get_logs() <= b.party_res.elected_representatives.get_logs())
		return false;
	else
		return true;
}
void DividedDistrict::calc_winner_party() // create sorted result array - to see the winner party and the rest of the results after the election round was done
{
	if (sorted_results.size() != 0)
	{
		sorted_results.clear();
	}
	int size = results_size->get_logs();
	sorted_results.reserve(size);
	sorted_results.resize(size);
	for (int i = 0; i < size; i++)
	{
		sorted_results[i].party_res.elected_representatives = results[i].elected_representatives;
		sorted_results[i].party_res.votes_number = results[i].votes_number;
		sorted_results[i].party_id = i;
	}
	std::sort(sorted_results.begin(), sorted_results.end(), compareRepresentatives);

}