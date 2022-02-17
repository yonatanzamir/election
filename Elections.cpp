#include "Elections.h"
#include "UnitedDistrict.h"
#include <math.h> 
int DaysInMonths[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
Elections::Elections(int day, int month, int year) // Elections constructor
{
	set_Year(year);
	set_Month(month);
	set_Day(day);
	results_sum.clear();

}

Elections::Elections(istream& in) : day(1), month(1), year(1) // load constructor
{

	results_sum.clear();

}

void Elections::save(ostream& out) 
{
	citizens.save(out);
	parties.save(out);

	int results_phs = 0;
	int results_logs = 0;

	if (results_sum.size()!=0)
	{
		int results_phs = parties.get_phs();
		int results_logs = parties.get_logs();
		out.write(rcastcc(&(results_phs)), sizeof(results_phs));
		out.write(rcastcc(&(results_logs)), sizeof(results_logs));

		if (!out.good())
			throw Saveexception();

		for (int i = 0; i < results_logs; i++)
		{
			out.write(rcastcc(&(results_sum[i])), sizeof(results_sum[i]));
		}
	}

	else
		out.write(rcastcc(&(results_phs)), sizeof(results_phs));

	if (!out.good())
		throw Saveexception();
}
 void Elections::load(istream& in)
{

	 if (results_sum.size() != 0)
	 {
		 results_sum.clear();
	 }

	 int results_phs;
	 int results_logs;
	 in.read(rcastc(&(results_phs)), sizeof(results_phs));

	 if (!in.good())
		 throw Loadexception();

	 if (results_phs != 0)
	 {
		 results_sum.reserve(results_phs);
		 in.read(rcastc(&(results_logs)), sizeof(results_logs));

		 if (!in.good())
			 throw Loadexception();
		 results_sum.resize(results_logs);
		 for (int i = 0; i < results_logs; i++)
		 {
			 in.read(rcastc(&(results_sum[i])), sizeof(results_sum[i]));
		 }

	 }

	 else
		 results_sum.clear();
	 if (!in.good())
		 throw Loadexception();
}

Elections::~Elections() // Elections destructor
{

}


void Elections::set_Year(int year) // set the year that elections occur. 
{
	if (year <= 0)
		throw invalid_argument("Not positive year");
		this->year = year;
}

void Elections::set_Day(int day) // set the day that elections occur. 
{
	if (DaysInMonths[this->month - 1] < day || 0 >= day)
		throw invalid_argument("invalid day");

		this->day = day;
}

void Elections::set_Month(int month) // set the month that elections occur.
{
	if (month <= 0 || month > 12)
		throw invalid_argument("invalid month");
		this->month = month;
}

void Elections::date_checker(int day, int month, int year)//check if the date is valid
{
	if (year <= 0)
		throw invalid_argument("Not positive year");
	if (month <= 0 || month > 12)
		throw invalid_argument("invalid month");
	if (DaysInMonths[month - 1] < day || 0 >= day)
		throw invalid_argument("invalid day");
	
}

void Elections::build_representative_list(DistrictArray& d_array) // init and update the results array when the elections is over. It creates the representatives list for each party according to the results
{														
	float ratio;
	int number_of_representatives;
	int count = 0;


	if (d_array.get_logs() == 0)
	{
		throw logic_error("cant calculate results,there are no districts");
	}

	if (this->parties.get_logs() == 0)
	{
		throw logic_error("cant calculate results,there are no parties");
	}
		

	for (int i = 0; i < d_array.get_logs(); i++)
	{
		count = 0;
		for (int j = 0; j < d_array[i]->get_parties_number() - 1; j++)
		{
			if (d_array[i]->get_number_of_voters() == 0)								
				ratio = 0;																
			else
			{
				ratio = static_cast<float>
					(d_array[i]->get_results()[j].votes_number) /
					d_array[i]->get_number_of_voters();
			}

			number_of_representatives = ceil(ratio * static_cast<float>(d_array[i]->get_number_of_representatives())); // up round for number of representatives
			count += number_of_representatives;
			if (count > d_array[i]->get_number_of_representatives())
			{
				number_of_representatives = 0;
			}
			vector<CitizenArray> ca = parties.getParty_by_index(j).get_representatives_by_district();
			if (number_of_representatives > ca[i].get_logs())// there is an error when the party have less representatives than the district gave her
			{
				throw logic_error("cant calculate results,party have less representatives than the district gave her");
			} 	
			int cur = d_array[i]->get_results()[j].elected_representatives.get_logs();
			if (number_of_representatives < cur)
			{
				d_array[i]->get_results()[j].elected_representatives.UpdateCitizen(number_of_representatives);
			}
			else
			{
				for (int k = 0; k < number_of_representatives; k++)
					if (ca[i].get_logs() - 1 >= k)
					{
						if (d_array[i]->get_results()[j].elected_representatives.FindCitizenById(ca[i].get_citizen_by_index(k).getId()) == nullptr)
							d_array[i]->get_results()[j].elected_representatives.addCitizen(ca[i].get_citizen_by_index(k));
					}
			}	
		}
		// the last party gets the remain representatives to avoid losing representatives
		int j = d_array[i]->get_parties_number() - 1;
		number_of_representatives = d_array[i]->get_number_of_representatives() - count;
		if (number_of_representatives < 0)
			number_of_representatives = 0;
		vector<CitizenArray> ca = parties.getParty_by_index(j).get_representatives_by_district();
		if (number_of_representatives > ca[i].get_logs())
		{
			throw logic_error("cant calculate results,party have less representatives than the district gave her");
		}
		int cur = d_array[i]->get_results()[j].elected_representatives.get_logs();
		if (number_of_representatives < cur)
		{
			d_array[i]->get_results()[j].elected_representatives.UpdateCitizen(number_of_representatives);
		}
		else
		{
			for (int k = 0; k < number_of_representatives; k++)
				if (ca[i].get_logs() - 1 >= k)
				{
					if (d_array[i]->get_results()[j].elected_representatives.FindCitizenById(ca[i].get_citizen_by_index(k).getId()) == nullptr)
						d_array[i]->get_results()[j].elected_representatives.addCitizen(ca[i].get_citizen_by_index(k));
				}
		}
	}
}




