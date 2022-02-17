#pragma once
#include "District.h"
#include <algorithm>
class DividedDistrict: public District
{
	private:
		struct PartyResult_sorted
		{
			int party_id;
			PartyResult party_res;
		};

		vector<PartyResult_sorted> sorted_results ;

	public:
		DividedDistrict(const string name, int number_of_representatives, PartyArray* results_size);
		DividedDistrict(istream& in, DistrictArray* da, PartyArray* pa);
		virtual ~DividedDistrict();
		DividedDistrict(const DividedDistrict& ud);
		virtual const DividedDistrict& operator=(const DividedDistrict& other);
		void calc_winner_party() override;
		virtual void show() const { cout << " Divided District "; }
		vector<PartyResult_sorted>& get_sorted_results()  { return sorted_results; }
		virtual void save(ostream& out);
		virtual void load(istream& in, DistrictArray* da, PartyArray* pa);
		friend bool compareRepresentatives(PartyResult_sorted a, PartyResult_sorted b);

};

