#include "RegularElections.h"
#include "DividedDistrict.h"
#include "UnitedDistrict.h"
#include <math.h> 

RegularElections::RegularElections(int day, int month, int year): Elections(day,month,year) // RegularElections constructor
{

	
}


RegularElections::~RegularElections() // RegularElections destructor
{
	
}

RegularElections::RegularElections(istream& in) : Elections(in) // load constructor
{

	load(in);

}

void RegularElections::save(ostream& out) 
{
	ElectionsType type = ElectionsType::REGULAR;
	out.write(rcastcc(&type), sizeof(type));
	out.write(rcastcc(&day), sizeof(day));
	out.write(rcastcc(&month), sizeof(month));
	out.write(rcastcc(&year), sizeof(year));
	if (!out.good())
		throw Saveexception();
    districts.save(out);
	Elections::save(out);

}

void RegularElections::load(istream& in)
{
	in.read(rcastc(&day), sizeof(day));
	in.read(rcastc(&month), sizeof(month));
	in.read(rcastc(&year), sizeof(year));

	if (!in.good())
		throw Loadexception();

	districts.load(in, &parties);
	citizens.load(in, &districts);
	parties.load(in, &districts);
	init_representives();
    Elections::load(in);

}



void RegularElections::init_results_sum() // creates new array for the results. Here we pad zeros to reset it for later use
{
	int size = parties.get_logs();
	if (results_sum.size() != 0)
		results_sum.clear();
	results_sum.reserve(size);  
	results_sum.resize(size);
	for (int i = 0; i < size; i++)
	{
		results_sum[i].party_id = i;
		results_sum[i].sum_of_representatives = 0;
		results_sum[i].sum_of_votes = 0;
	}

	for (int i = 0; i < districts.get_logs(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			results_sum[j].sum_of_votes += districts[i]->get_results()[j].votes_number;
		}

		if (dynamic_cast<UnitedDistrict*>(districts[i]))
		{
			UnitedDistrict* ud = dynamic_cast<UnitedDistrict*>(districts[i]);
			int winner_of_district = ud->get_winner_party();
			results_sum[winner_of_district].sum_of_representatives += districts[i]->get_number_of_representatives();
		}

		else if (dynamic_cast<DividedDistrict*>(districts[i]))
		{
			DividedDistrict* dd = dynamic_cast<DividedDistrict*>(districts[i]);
			for (int k = 0; k < size; k++)
				results_sum[k].sum_of_representatives += dd->get_results()[k].elected_representatives.get_logs();
		}
		
	}


}

void RegularElections::build_representative_list() //  init the elected representatives for each party according to the results, using the base class
{

	 Elections::build_representative_list(this->districts);
}



bool compareSumOfRepresentative(RegularElections::sumOfResult a, RegularElections::sumOfResult b) //for stl sort
{
	if (a.sum_of_representatives <= b.sum_of_representatives)
		return false;
	else
		return true;
}

void RegularElections::sort_results_sum() // sort the new array of results - from the party with the most sum of representatives to the least one
{
	std::sort(results_sum.begin(), results_sum.end(), compareSumOfRepresentative);
}

void RegularElections::init_representives() // init representives 
{
	for (int i = 0; i < districts.get_logs(); i++)
	{
		for (int j = 0; j < parties.get_logs();j++)
		{
			for (int k = 0; k < districts[i]->get_results()[j].elected_representatives.get_logs(); k++)
			{
				int id = districts[i]->get_results()[j].elected_representatives.get_citizen_by_index(k).getId();
				District* d=citizens.FindCitizenById(id)->getDistrict_Citizen();
				districts[i]->get_results()[j].elected_representatives.get_citizen_by_index(k).set_district(d);
				
			}
		}
	}
}