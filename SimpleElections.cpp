#include "SimpleElections.h"
#include "DividedDistrict.h"
#include "UnitedDistrict.h"
#include "Algorithm.h"
#include <Algorithm>
#include <math.h> 
SimpleElections::SimpleElections(int day, int month, int year, int num_of_representatives) : Elections(day, month, year),
district_hidden(1)// SimpleElections constructor
{
	DividedDistrict divided("", num_of_representatives, &(this->get_parties()));
	district_hidden.addDistrict(&divided);
}


SimpleElections::~SimpleElections() // SimpleElections destructor
{

}

SimpleElections::SimpleElections(istream& in) : Elections(in) // load constructor
{

	load(in);

}

void SimpleElections::save(ostream& out) 
{
	ElectionsType type = ElectionsType::SIMPLE;
	out.write(rcastcc(&type), sizeof(type));
	out.write(rcastcc(&day), sizeof(day));
	out.write(rcastcc(&month), sizeof(month));
	out.write(rcastcc(&year), sizeof(year));

	if (!out.good())
		throw Saveexception();

	district_hidden.save(out);
	Elections::save(out);
}

void SimpleElections::load(istream& in)
{
	in.read(rcastc(&day), sizeof(day));
	in.read(rcastc(&month), sizeof(month));
	in.read(rcastc(&year), sizeof(year));
	if (!in.good())
		throw Loadexception();
	district_hidden.load(in, &parties);
	citizens.load(in, &district_hidden);
	parties.load(in, &district_hidden);
	init_representives();
	Elections::load(in);
}



void SimpleElections::init_results_sum() // creates new array for the results and init it
{
	int size = parties.get_logs();
	if (results_sum.size() != 0)
		results_sum.clear();
	results_sum.reserve(size);
	results_sum.resize(size);
	for (int i = 0; i < size; i++)
		results_sum[i].party_id = i;
	for (int j = 0; j < size; j++)
	{
		results_sum[j].sum_of_votes = district_hidden[0]->get_results()[j].votes_number;
		results_sum[j].sum_of_representatives = district_hidden[0]->get_results()[j].elected_representatives.get_logs();
	}
}


void SimpleElections::build_representative_list() //  init the elected representatives for each party according to the results, using the base class
{
	 Elections::build_representative_list(district_hidden);
}

bool compareVotes(SimpleElections::sumOfResult a, SimpleElections::sumOfResult b) //for stl sort
{
	if (a.sum_of_votes <= b.sum_of_votes)
		return false;
	else
		return true;

}
void SimpleElections::sort_results_sum() // sort the new array of results - from the party with the most sum of votes to the least one
{
	std::sort(results_sum.begin(), results_sum.end(), compareVotes);
}

void SimpleElections::init_representives() // init_representives
{
	for (int j = 0; j < parties.get_logs(); j++)
	{
		for (int k = 0; k < district_hidden[0]->get_results()[j].elected_representatives.get_logs(); k++)
		{
			district_hidden[0]->get_results()[j].elected_representatives.get_citizen_by_index(k).set_district(district_hidden[0]);
		}
	}
}



