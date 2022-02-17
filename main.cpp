
#include "CitizenArray.h"
#include "PartyArray.h"
#include "DistrictArray.h"
#include "UnitedDistrict.h"
#include "DividedDistrict.h"
#include "RegularElections.h"
#include "SimpleElections.h"
#include <iostream>
#include <fstream>
using namespace std;
void main()
{
	enum pre_choice {NEW_ELECTION_ROUND=1,LOAD_ELECTIONS,EXIT};
	int pre_choice;
	int choice;
	Elections::ElectionsType election_choice;
	int election_ch_num;
	int day, month, year;
	string file_name;
	ifstream infile; // load elections
	ofstream outfile; // save elections
	Elections* elections=nullptr;
	cout << "Please choose a number between 1-3: "<<endl;
	cout << "1. new elections round " << endl;
	cout << "2. load elections from file " << endl;
	cout << "3. exit " << endl;
	cin >> pre_choice;
	try {
		if (pre_choice == NEW_ELECTION_ROUND) // pre menu
		{
			cout << "Please enter date of the elections by day, month, year: ";
			cin >> day >> month >> year;
			Elections::date_checker(day, month, year);
			cout << endl;
			cout << "Please enter the type of the elections: 1-for regular, 2-for simple: ";
			cin >> election_ch_num;
			cout << endl;
			election_choice = static_cast<Elections::ElectionsType>(election_ch_num);

			if (election_choice == Elections::ElectionsType::REGULAR)
				elections = new RegularElections(day, month, year);

			else if (election_choice == Elections::ElectionsType::SIMPLE)
			{
				int num_of_rep;
				cout << "Please enter number of representatives for the simple elections: " << endl;
				cin >> num_of_rep;
				while (num_of_rep <= 0)
				{
					cout << "ERROR: number of representatives cannot be 0 or negative!" << endl;
					cout << "please enter positive number: ";
					cin >> num_of_rep;
				}

				elections = new SimpleElections(day, month, year, num_of_rep);
				cout << endl;
			}
			else
			{
				throw invalid_argument("no option like this");
			}
		}

		else if (pre_choice == LOAD_ELECTIONS)
		{
			cout << "Please enter file name to load from: ";
			cin >> file_name;
			infile.open(file_name, ios::binary | ios::_Nocreate);
			if (!infile)
			{
				throw Openexception();
			}

			else
			{
				Elections::ElectionsType type;
				infile.read(rcastc(&type), sizeof(type));
				if (!infile.good())
				{
					infile.close();
					throw Loadexception();
				}

				if (type == Elections::ElectionsType::REGULAR)
				{

					elections = new RegularElections(1, 1, 1);
					election_choice = Elections::ElectionsType::REGULAR;
				}

				else if (type == Elections::ElectionsType::SIMPLE)
				{
					elections = new SimpleElections(1, 1, 1, 1);
					election_choice = Elections::ElectionsType::SIMPLE;
				}

				else
				{
					throw Loadexception();
				}
			    elections->load(infile);
				infile.close();
			}
		}

		else if (pre_choice == EXIT)
		{
			cout << "GOODBYE" << endl;
			exit(0);
		}

		else
		{
			throw logic_error("wrong input!");
		}
	}

	catch (std::bad_alloc& ex)
	{
		cout << "ERROR: " << ex.what() << endl;
		exit(1);
	}
	catch (Loadexception& ex)
	{
		cout << "ERROR: " << ex.what() << endl;
		delete elections;
		infile.close();
		exit(1);
	}
	catch (std::exception& ex)
	{
		cout <<"ERROR: "<<ex.what() << endl;
		exit(1);
	}
	
	int birth_year;
	string name;
	int number_of_representatives;
	Citizen* representative;
	Citizen* chief;
	Citizen* citizen_p;
	Citizen citizen;
	District* Citizen_Home;
	int party_id, district_id, citizen_id;
	enum main_choice { ADD_DISTRICT = 1, ADD_CITIZEN, ADD_PARTY, ADD_REPRESENTIVE, SHOW_DISTRICTS, SHOW_CITIZENS, SHOW_PARTIES, VOTE, SHOW_RESULTS, EXIT_PROGRAM, SAVE,LOAD };
	int district_type;

	cout << "Please choose a number between 1-10:" << endl; //// main menu
	cout << "1. Add district" << endl;
	cout << "2. Add citizen" << endl;
	cout << "3. Add party" << endl;
	cout << "4. Add representive to a party" << endl;
	cout << "5. Show all the districts at the country" << endl;
	cout << "6. Show all the citizens at the country" << endl;
	cout << "7. Show all the parties at the country " << endl;
	cout << "8. Make a vote" << endl;
	cout << "9. Show the elections results" << endl;
	cout << "10. Exit program" << endl;
	cout << "11. Save elections to file" << endl;
	cout << "12. Load elections from file" << endl;
	cin >> choice;
	while (choice != EXIT_PROGRAM)
	{
		try {
		switch (choice) {
			
		case ADD_DISTRICT:
			if (election_choice == Elections::ElectionsType::REGULAR)
			{
				cout << "Please enter district name, district type: 0-for united, 1-for divided. enter also number of representatives: ";
				cin >> name >> district_type >> number_of_representatives;
				if (district_type == 0)
				{
					UnitedDistrict district(name, number_of_representatives, &(elections->get_parties()));
					elections->get_districts().addDistrict(&district);
				}
				else if (district_type == 1)
				{
					DividedDistrict district(name, number_of_representatives, &(elections->get_parties()));
					elections->get_districts().addDistrict(&district);
				}
				else
					throw invalid_argument("invalid district type");

					elections->get_parties().add_district_to_parties();
			}

			else if (election_choice == Elections::ElectionsType::SIMPLE)
			{
				cout << "ERROR: This is simple elections. There are no districts!" << endl;
			}

			break;

		case ADD_CITIZEN:
			cout << "Please enter name, id, birth year and district num: ";
			cin >> name >> citizen_id >> birth_year >> district_id;
			if (election_choice == Elections::ElectionsType::SIMPLE)
				district_id = 0;

			if (election_choice == Elections::ElectionsType::REGULAR && elections->get_districts().get_logs() - 1 < district_id)
				throw invalid_argument("invalid district num");
			if (elections->get_Year() - birth_year <= 18)
				throw invalid_argument("Citizen age is 18 or under");
			Citizen_Home = elections->get_districts()[district_id];
			citizen = Citizen(name, citizen_id, birth_year, Citizen_Home, false);
			elections->get_citizens().addCitizen(citizen); // add to pinkas
			elections->get_districts()[citizen.getDistrict_Citizen()->get_serial_number()]->addCitizenToDistrict(citizen);// add citizen inside district
			elections->get_districts()[citizen.getDistrict_Citizen()->get_serial_number()]->set_percent_of_all();
		
			break;

		case ADD_PARTY:
			cout << "Please enter name of the party, chief party id:";
			cin >> name >> citizen_id;
			chief = elections->get_citizens().FindCitizenById(citizen_id);
			elections->get_parties().is_citizen_chief(citizen_id);
			if (chief != nullptr)
			{
				elections->get_parties().addParty(name, *chief, &(elections->get_districts()));
				elections->get_districts().add_party_to_districts();
			}
			else
				throw invalid_argument("This is not a citizen!");
			break;

		case ADD_REPRESENTIVE:
			cout << "Please enter id of representative, party id, district id:";
			cin >> citizen_id >> party_id >> district_id;

			representative = elections->get_citizens().FindCitizenById(citizen_id);
			if (representative == nullptr)
				throw invalid_argument("This is not a citizen!");

			elections->get_parties().Is_Representative(citizen_id);
			if (elections->get_parties().get_logs() < party_id + 1)
				throw invalid_argument("invalid party id");
			else if ((election_choice == Elections::ElectionsType::REGULAR) && (elections->get_districts().get_logs() < district_id + 1))
				throw invalid_argument("invalid district id");
			else
			{
				if (election_choice == Elections::ElectionsType::SIMPLE)
					district_id = 0;
				elections->get_parties().getParty_by_index(party_id).addRepresentative(*representative, district_id);

			}
			break;

		case SHOW_DISTRICTS:

			if (election_choice == Elections::ElectionsType::SIMPLE)
				cout << "There are no districts in simple elections" << endl;

			else if (election_choice == Elections::ElectionsType::REGULAR)
			{
				cout << "The districts are:" << endl;
				cout << elections->get_districts();
			}
			break;

		case SHOW_CITIZENS:
			cout << "The citizens are:" << endl;
			cout << elections->get_citizens();
			break;

		case SHOW_PARTIES:
			cout << "The parties are:" << endl;
			cout << elections->get_parties();
			break;

		case VOTE:
			cout << "Please enter your id and party id: ";
			cin >> citizen_id >> party_id;
			citizen_p = elections->get_citizens().FindCitizenById(citizen_id);
			if (citizen_p == nullptr)
				throw invalid_argument("This is not a citizen!");

			if (citizen_p->getIs_Voted())
				throw invalid_argument("citizen already voted");

			else if (elections->get_parties().get_logs() < party_id + 1)
				throw invalid_argument("invalid party id");

			else
			{
				citizen_p->set_isVoted(true);
				elections->get_districts()[citizen_p->getDistrict_Citizen()->get_serial_number()]->set_results(party_id);
				elections->get_districts()[citizen_p->getDistrict_Citizen()->get_serial_number()]->set_percent_of_all();

			}

			break;

		case SHOW_RESULTS:

			cout << "Date of the elections:  " << elections->get_Day() << "/" << elections->get_Month() << "/" << elections->get_Year() << endl;
			elections->build_representative_list();
		
				elections->show();
				if (election_choice == Elections::ElectionsType::REGULAR)
				{
					for (int i = 0; i < elections->get_districts().get_logs(); i++)
					{
						cout << endl;
						cout << "district name: ";
						cout << elections->get_districts()[i]->get_name() << endl;
						cout << "district type: ";
						elections->get_districts()[i]->show();
						cout << endl;
						cout << "number_of_representatives:";
						cout << elections->get_districts()[i]->get_number_of_representatives() << endl;
						elections->get_districts()[i]->calc_winner_party();
						if (typeid(*(elections->get_districts()[i])) == typeid(UnitedDistrict))
						{
							int winner_party = dynamic_cast<UnitedDistrict*>(elections->get_districts()[i])->get_winner_party();
							cout << "name of the chief party that won in the district: ";
							cout << elections->get_parties().getParty_by_index(winner_party).get_chief_party().getName() << endl;
						}
						else if (typeid(*(elections->get_districts()[i])) == typeid(DividedDistrict))
						{
							for (int k = 0; k < elections->get_districts()[i]->get_results_size()->get_logs(); k++)
							{
								int number_of_representatives = dynamic_cast<DividedDistrict*>(elections->get_districts()[i])->get_sorted_results()[k].party_res.elected_representatives.get_logs();
								int party_id = dynamic_cast<DividedDistrict*>(elections->get_districts()[i])->get_sorted_results()[k].party_id;
								if (number_of_representatives > 0)
								{
									cout << "chief party: " << elections->get_parties().getParty_by_index(party_id).get_chief_party().getName() << endl;
									cout << "number of representatives that party took: " << number_of_representatives << endl;
								}

							}
						}

						cout << "the precent of all the voters in the district " << i << " is : ";
						elections->get_districts()[i]->set_percent_of_all();
						cout << elections->get_districts()[i]->get_percent_of_all() << endl;
						int percent;
						for (int j = 0; j < elections->get_districts()[i]->get_parties_number(); j++)
						{
							cout << "the elected representatives of party number " << j << " are: ";
							cout << elections->get_districts()[i]->get_results()[j].elected_representatives << endl;
							cout << "number of votes for party number " << j << " is: ";
							cout << elections->get_districts()[i]->get_results()[j].votes_number << endl;
							cout << "precent of votes for party number " << j << " is: ";
							if (elections->get_districts()[i]->get_number_of_voters() == 0)
								percent = 0;
							else {
								percent = 100 * (elections->get_districts()[i]->get_results()[j].votes_number) /
									elections->get_districts()[i]->get_number_of_voters();
							}
							cout << percent << endl;
						}

					}
				}

				elections->init_results_sum();
				elections->sort_results_sum();

				if (election_choice == Elections::ElectionsType::SIMPLE)
				{
					cout << "the precent of all the voters:  ";
					elections->get_districts()[0]->set_percent_of_all();
					cout << elections->get_districts()[0]->get_percent_of_all() << endl;
				}

				int party_index;
				for (int i = 0; i < elections->get_parties().get_logs(); i++)
				{

					party_index = elections->get_results_sum()[i].party_id;
					cout << endl;
					cout << "The name of the party: " << elections->get_parties().getParty_by_index(party_index).get_name() << endl;
					cout << "The name of the chief: " << elections->get_parties().getParty_by_index(party_index).get_chief_party().getName() << endl;//
					cout << "sum of representatives: " << elections->get_results_sum()[i].sum_of_representatives << endl;
					cout << "sum of votes: " << elections->get_results_sum()[i].sum_of_votes << endl;
					cout << endl;

					if (election_choice == Elections::ElectionsType::SIMPLE)
					{
						int percent;
						cout << "precent of votes for the party : ";
						if (elections->get_districts()[0]->get_number_of_voters() != 0)
							percent = (100 * elections->get_results_sum()[i].sum_of_votes) / elections->get_districts()[0]->get_number_of_voters();
						else
							percent = 0;
						cout << percent << endl;

						cout << "The elected Representatives of the party: " << endl;
						cout << elections->get_districts()[0]->get_results()[elections->get_results_sum()[i].party_id].elected_representatives << endl;;
					}
				}
			break;
		case EXIT_PROGRAM:
		{
			delete elections;
			exit(0);
		}
			
			break;

		case SAVE:
		{
			cout << "Please enter file name: ";
			cin >> file_name;
			outfile.open(file_name, ios::binary | ios::trunc);
			if (!outfile)
			{
				throw Openexception();
			}
			else
			{
				elections->save(outfile);
				outfile.close();
			}

		}
		break;

		case LOAD:
		{
			cout << "Please enter file name: ";
			cin >> file_name;
			infile.open(file_name, ios::binary | ios::_Nocreate);
			if (!infile)
			{
				throw Openexception();
			}
			else
			{
				delete elections;

				Elections::ElectionsType type;
				infile.read(rcastc(&type), sizeof(type));
				if (!infile.good())
				{
					throw Loadexception();
				}

				if (type == Elections::ElectionsType::REGULAR)
				{
					elections = new RegularElections(1, 1, 1);
					election_choice = Elections::ElectionsType::REGULAR;
				}

				else if (type == Elections::ElectionsType::SIMPLE)
				{
					elections = new SimpleElections(1, 1, 1, 1);
					election_choice = Elections::ElectionsType::SIMPLE;
				}

				elections->load(infile);
				infile.close();
			}

		}
		break;

		default:
			cout << "There is no option like this " << endl;
			break;
			}
			}
			catch (bad_alloc& ex) {
				cout << ex.what() << endl;
				delete elections;
				exit(1);
			}
			catch (Openexception& ex) {
				cout << ex.what() << endl;
				delete elections;
				exit(1);
			}
			catch (Loadexception& ex) {
				cout << ex.what() << endl;
				infile.close();
				delete elections;
				exit(1);
			}
			catch (Saveexception& ex) {
				cout << ex.what() << endl;
				outfile.close();
				delete elections;
				exit(1);
			}
			catch (exception& ex) {
				cout << ex.what() << endl;
			}
			catch (...)//if not standard exception was occured - for potential future users.
			{
			}
		cout << endl;
		cout << "Please choose a number between 1-10:" << endl;
		cout << "1. Add district" << endl;
		cout << "2. Add citizen" << endl;
		cout << "3. Add party" << endl;
		cout << "4. Add representive to a party" << endl;
		cout << "5. Show all the districts at the country" << endl;
		cout << "6. Show all the citizens at the country" << endl;
		cout << "7. Show all the parties at the country " << endl;
		cout << "8. Make a vote" << endl;
		cout << "9. Show the elections results" << endl;
		cout << "10. Exit program" << endl;
		cout << "11. Save elections to file" << endl;
		cout << "12. Load elections from file" << endl;
		cin >> choice;

	}
	delete elections;
	cout << "goodbye" << endl;
}
