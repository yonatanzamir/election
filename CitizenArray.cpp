#include "CitizenArray.h"
#include "Algorithm.h"

CitizenArray::CitizenArray(int phs) : citizens(phs)         // CitizenArray constrctor
{
}
CitizenArray::CitizenArray(istream& in,DistrictArray* da) : citizens(0)  // load constructor
{
	load(in, da);
}

CitizenArray::~CitizenArray() // CitizenArray destructor
{
	
}
void CitizenArray::save(ostream& out) 
{
	 citizens.save(out);
}
void CitizenArray::load(istream& in,  DistrictArray* da)
{
	int logs;
	in.read(rcastc(&logs), sizeof(logs));
	if (!in.good())
		throw Loadexception();
	Citizen temp;
	citizens.clear();
	for (int i=0;i<logs;i++)
	{
		temp.load(in, da);
		citizens.push_back(temp);
	}
}

void CitizenArray::load(istream& in, District* d)
{
	int logs;
	in.read(rcastc(&logs), sizeof(logs));
	if (!in.good())
		throw Loadexception();
	Citizen temp;
	citizens.clear();
	for (int i = 0; i < logs; i++)
	{
		temp.load(in, d);
		citizens.push_back(temp);
	}
}


void CitizenArray::addCitizen(const Citizen& c) // add new citizen to the array data member and resize the array when there's no more place
{
	citizens.push_back(c);
}

ostream& operator<<(ostream& os, const CitizenArray& ca) // CitizenArray operator<<
{
	os << ca.citizens;
	return os;
}

CitizenArray::CitizenArray(const CitizenArray& s) // CitizenArray copy constructor
{
	*this = s;


}

const CitizenArray& CitizenArray::operator=(const CitizenArray& other) // CitizenArray operator=
{
	if (this != &other)
		this->citizens = other.citizens;
		return *this;

}
int CitizenArray::LengthIdCalc(int num) // calculate the number of digit in the id  
{
	int counter = 0;
	while (num != 0)
	{
		counter++;
		num /= 10;
	}
	return counter;
}
Citizen* CitizenArray::FindCitizenById(int id) // gets id and return the address of a citizen if he is exist or nullptr if he is not exist
{
	if (LengthIdCalc(id) == 9)
	{
		string name = "a";
		Citizen c(name, id, 0, nullptr, false);
		Citizen* is_found = Myfind(citizens.begin(), citizens.end(), c);
		return is_found;
	}
	return nullptr;
}

void CitizenArray::UpdateCitizen(int size) 
{
	int i = 0;
	auto itrbegin = citizens.begin();
	while (i != size)
	{
		i++;
		itrbegin++;
	}
	citizens.erase(itrbegin, citizens.end());
}
