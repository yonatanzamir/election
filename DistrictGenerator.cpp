#include "DistrictGenerator.h"
#include "DividedDistrict.h"
#include "UnitedDistrict.h"

DistrictGenerator::DistrictType DistrictGenerator::district_generator(District* const d) // gets a district and return its type
{
	if (typeid(*d) == typeid(DividedDistrict))
		return DistrictType::DIVIDED;

	else if (typeid(*d) == typeid(UnitedDistrict))
		return DistrictType::UNITED;
}

District* DistrictGenerator::load(istream& in, DistrictArray* da, PartyArray* pa) // gets a file and load a new divided/united district from the file
{
	DistrictType type;
	in.read(rcastc(&type), sizeof(type));
	District* d=nullptr;
	if (!in.good())
	{
		throw Loadexception();
	}
	if (type == DistrictType::DIVIDED)
	{
		d=new DividedDistrict("",1,pa);
		d->load(in, da, pa);
			
	}	
	else if (type == DistrictType::UNITED)
	{
		d = new UnitedDistrict("",1, pa);
		d->load(in, da, pa);
	}
	return d;
}

