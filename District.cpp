#include "District.h"
#include "PartyArray.h"
#include <string.h>
#pragma warning(disable: 4996) // disable strcpy error

int District::serial_number_generator=0; // automatic counter 

District::District(const string _name, int number_of_representatives, PartyArray* results_size) :  // District constructor
	serial_number(serial_number_generator++), number_of_representatives(number_of_representatives), 
	percent_of_all(0), results_size(nullptr) ,number_of_voters(0)
{
	if (number_of_representatives <= 0)
	{
		throw logic_error("number of representatives must be positive");
	}
	this->results_size = results_size; // initizlize data member results_size
	if (_name.size() != 0)
	{
		this->name = _name;
	}
	else // for simple elections 
	{
		this->name = "country";
	}
	results.reserve(this->results_size->get_phs());
	results.resize(this->results_size->get_logs()); 
} 

District::District():percent_of_all(0), results_size(nullptr), number_of_voters(0), number_of_representatives(0)
{ 
	results.clear(); 
}
District::District(istream& in,DistrictArray* da, PartyArray* pa) :serial_number(0), percent_of_all(0)
, results_size(nullptr), number_of_voters(0), number_of_representatives(0) // load constructor
								
{
	load(in, da, pa);
	serial_number_generator=serial_number;
}


void District::save(ostream& out)
{
	int length = name.size();
	out.write(rcastcc(&length), sizeof(length));
	out.write(rcastcc(this->name.c_str()), length);
	out.write(rcastcc(&serial_number), sizeof(serial_number));
	out.write(rcastcc(&serial_number_generator), sizeof(serial_number_generator));

	citizens_of_district.save(out);
	
	out.write(rcastcc(&percent_of_all), sizeof(percent_of_all));

	int results_phs = results.capacity();
	int results_logs = results.size();
	out.write(rcastcc(&results_phs), sizeof(results_phs));
	out.write(rcastcc(&results_logs), sizeof(results_logs));

	if (!out.good())
		throw Saveexception();

	for (int i = 0; i < results_logs; i++)
	{
		out.write(rcastcc(&(results[i].votes_number)), sizeof(results[i].votes_number));
		results[i].elected_representatives.save(out);
	}

	out.write(rcastcc(&number_of_representatives), sizeof(number_of_representatives));
	out.write(rcastcc(&number_of_voters), sizeof(number_of_voters));
	if (!out.good())
	{
		throw Saveexception();
	}
}

void District::load(istream& in, DistrictArray* da, PartyArray* pa)
  {
	int length;
	in.read(rcastc(&length), sizeof(length));
	if (!in.good())
		throw Loadexception();
	name.resize(length);
	in.read(rcastc(&name[0]), length);
	in.read(rcastc(&serial_number), sizeof(serial_number));
	in.read(rcastc(&serial_number_generator), sizeof(serial_number_generator));

	citizens_of_district.load(in, this);
		

	in.read(rcastc(&percent_of_all), sizeof(percent_of_all));

	int results_phs;
	int results_logs;
	in.read(rcastc(&results_phs), sizeof(results_phs));
	in.read(rcastc(&results_logs), sizeof(results_logs));

	if (!in.good())
		throw Loadexception();

	results.reserve(results_phs);
	results.resize(results_logs);
	for (int i = 0; i < results_logs; i++)
	{
		in.read(rcastc(&(results[i].votes_number)), sizeof(results[i].votes_number));
		results[i].elected_representatives.load(in, da);
	}
	results_size = pa;
	in.read(rcastc(&number_of_representatives), sizeof(number_of_representatives));
	in.read(rcastc(&number_of_voters), sizeof(number_of_voters));

	if (!in.good())
	{
		throw Loadexception();
	}
}


District::~District() // District destructor
{	
}


void District::addCitizenToDistrict(const Citizen& c) // gets a citizen and add him to the citizen array inside the district
{
	citizens_of_district.addCitizen(c);
}

ostream& operator<<(ostream& os, const District& d) // District operator<<
{
	os << d.serial_number << ". " << "name of district: " << d.name << " district type: "; 
	d.show();
	os<<" , number of representatives: " << d.number_of_representatives << endl;
	return os;
	
}


District::District(const District& other) // District copy constructor
{
	*this = other;
	
}

const District& District::operator=(const District& other)// District operator=
{
	if (this != &other)
	{

		number_of_representatives = other.number_of_representatives;
		serial_number = other.serial_number;
		percent_of_all = other.percent_of_all;
		results_size = other.results_size;
		number_of_voters = other.number_of_voters;
		citizens_of_district = other.citizens_of_district;
		name = other.name;
        results.reserve(other.results_size->get_phs());
		results.resize(other.results_size->get_logs());
		for (int i = 0; i < results_size->get_logs(); i++)
		{
			results[i].votes_number = other.results[i].votes_number;
			results[i].elected_representatives = other.results[i].elected_representatives;
		}
	}
	return *this;
}

void District::set_results(int party_id) // gets party id and update the vote of the citizen in results array
{
	results[party_id].votes_number++;
	number_of_voters++;

}

void District::set_percent_of_all() // set the percent of the citizens that voted from the district
{
	percent_of_all = ((number_of_voters * 100) / citizens_of_district.get_logs()) ;
}



void District::resize_results() // resize the results array when place is over
{
	results.reserve(results_size->get_phs());
	results.resize(results_size->get_logs()-1);
}

void District::update_num_of_parties() // update the logical size of results array - when adding new party
{
	results.resize(results.size() + 1);
	results[results_size->get_logs() - 1].votes_number = 0;
}

int District::get_parties_number() const
{ 
	return results.size(); 
}

int District::get_results_phs() const
{
	return results.capacity();
}


