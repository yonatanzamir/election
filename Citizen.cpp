#include "Citizen.h"
#include <string>
#include "District.h"
#include "exception.h"
#include "DistrictArray.h"
#pragma warning(disable: 4996) // disable strcpy error

Citizen::Citizen(const string name, int id, int birth_year,  District* d, bool isVoted) :
	 isVoted(isVoted),d(nullptr) // citizen constructor
{
		if (9 != LengthIdCalc(id))
			throw length_error("Citizen id has not 9 digits");
	this->birth_year = birth_year;//check in the main 
	this->id = id;
	this->name = name;
	this->d = d;
}
 int Citizen::LengthIdCalc(int num) // calculate the number of digit in the id  
{
	int counter = 0;
	while (num != 0)
	{
		counter++;
		num /=  10;
	}
	return counter;
}
Citizen::Citizen(istream& in, DistrictArray* da): id(0), d(nullptr), birth_year(0), isVoted(false) // load constructor
{
	load(in,da);

}


Citizen::~Citizen() // citizen destructor
{
}

void Citizen::save(ostream& out) 
{
	int dis_num = d->get_serial_number();
	int length = name.size();
	out.write(rcastcc(&length), sizeof(length));
	out.write(rcastcc(this->name.c_str()), length);
	out.write(rcastcc(&id), sizeof(id));
	out.write(rcastcc(&dis_num), sizeof(int)); // write dis_num 
	out.write(rcastcc(&birth_year), sizeof(birth_year));
	out.write(rcastcc(&isVoted), sizeof(isVoted));
	if (!out.good())
		throw Saveexception();
}
void Citizen::load(istream& in, DistrictArray* da)
{
	int dis_num=setlength(in);
	this->d = (*da)[dis_num];
}
void Citizen::load(istream& in, District* d)
{
	setlength(in);
	this->d = d;	
}
bool Citizen::operator==(const Citizen& other) const // citizen operator== 
{
	return (id==other.id);
}
int Citizen::setlength(istream& in) // set new length and return district of a citizen
{
	int dis_num;
	int length;
	in.read(rcastc(&length), sizeof(length));
	if (!in.good())
		throw Loadexception(); // error reading length

	name.resize(length);
	in.read(rcastc(&name[0]), length);
	if (!in.good())
		throw Loadexception(); // error reading name
	in.read(rcastc(&id), sizeof(id));
	in.read(rcastc(&dis_num), sizeof(int)); // write dis_num 
	in.read(rcastc(&birth_year), sizeof(birth_year));
	in.read(rcastc(&isVoted), sizeof(isVoted));
	if (!in.good())
		throw Loadexception();
	return dis_num;
}


Citizen::Citizen(const Citizen& c) // citizen copy constructor
{
	//Notice:the data member "d" is not dynamically allocated in this class and therefore we are not initialize him here. 
	*this = c;
}

const Citizen& Citizen::operator=(const Citizen& other) // citizen operator= 
{
	if (this != &other)
	{
		id = other.id;
		birth_year = other.birth_year;
		d = other.d;
		isVoted = other.isVoted;
		this->name = other.name;
	}
	return *this;
}

ostream& operator<<(ostream& os, const Citizen& c) // citizen operator<< 
{
	os << "name: "<<c.name << " , id: " << c.id << " , birth year: " << c.birth_year << " , district num: " << (c.d)->get_serial_number() << endl;
	return os;
}


void Citizen::set_isVoted(bool isvoted) // setter of isVoted data member
{
	this->isVoted = isvoted;
}

void Citizen::set_district(District* district) //setter for district of citizen
{
	this->d = district;
}