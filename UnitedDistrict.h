#pragma once
#include "District.h"
class UnitedDistrict: public District
{
	private:
		int winner_party;

	public:
		UnitedDistrict(const string name, int number_of_representatives, PartyArray* results_size);
		UnitedDistrict(istream& in, DistrictArray* da, PartyArray* pa);
		virtual ~UnitedDistrict();
		UnitedDistrict(const UnitedDistrict& ud);
		virtual const UnitedDistrict& operator=(const UnitedDistrict& other);
		void calc_winner_party() override;
		int get_winner_party() const { return winner_party; }
		virtual void show() const { cout << " United District "; }
		virtual void save(ostream& out);
		virtual void load(istream& in, DistrictArray* da, PartyArray* pa);
};

