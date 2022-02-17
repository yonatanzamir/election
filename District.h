﻿#pragma once
class PartyArray;
#include "CitizenArray.h"
#include <vector>
class District
{
	protected:
		string name;
		int serial_number;
		static int serial_number_generator;
		CitizenArray citizens_of_district;
		int percent_of_all;
		struct PartyResult
		{
			int votes_number=0;
			CitizenArray elected_representatives;
		};
		vector <PartyResult> results;
		PartyArray* results_size;
		int number_of_representatives;
		int number_of_voters; 

	public:
		District(const string name, int number_of_representatives,PartyArray* results_size);
		District(istream& in, DistrictArray* da, PartyArray* pa);
		virtual ~District();
		void addCitizenToDistrict(const Citizen& c);
		District();
		friend ostream& operator<<(ostream& os, const District& d);
		District(const District& d);
		virtual const District& operator=(const District& other);
		void set_results(int party_id);
		void set_percent_of_all();
		int get_percent_of_all() const { return percent_of_all; }
		const string get_name() const { return name; }
		int get_number_of_representatives() const { return number_of_representatives; }
		virtual void calc_winner_party()=0;
		int get_parties_number() const;
		int get_results_phs() const;
		void resize_results();
		void update_num_of_parties();
		vector<PartyResult>& get_results() { return results; }
		int get_number_of_voters() const { return number_of_voters; }
		int get_serial_number() const { return serial_number; }
		CitizenArray& get_citizens_of_district () { return citizens_of_district; }
		PartyArray* get_results_size() const { return results_size; }
		virtual void show() const = 0 ;
		void init_results();
		virtual void save(ostream& out);
		virtual void load(istream& in, DistrictArray* da, PartyArray* pa);


};

