#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <limits>

using namespace std;

const int NUM_WATER = 4;
const int NUM_SUN = 3;
const int MIN_NAME = 3;
const string FILE_OPEN_ERR = "Error opening file 'name.txt'. Ending program.";
const string UNKNOWN_ERR = "Unknown Error";
const string NAME_ERR = "Plant Name is less than the name length requirement characters.";
const string BRANCHES_ERR = "Number of Branches is below the default.";
const string ROOTS_ERR = "Number of Roots is below the default.";

enum Menu { PrintMenu = 1, Update, Sort, Stats, Num_Obj, Exit };
enum SubMenu { Name = 1, Branches, Roots, BACK_TO_MAIN };

class Plant
{
private:
	int branches = DEFAULT_NUM_BRANCHES;
	int roots = DEFAULT_NUM_ROOTS;
	string name = DEFAULT_NAME;
	static int numPlantObj;
	static const int DEFAULT_NUM_BRANCHES = 2;
	static const int DEFAULT_NUM_ROOTS = 2;
	static const string DEFAULT_NAME;

public:
	Plant();

	explicit Plant(string newName);

	Plant(string newName, int newBranches, int newRoots);

	~Plant();



	void SetName(const string& newName);

	void Water();

	void Sun();

	void SetPlant(string newName, int newBranches, int newRoots);



	string GetName() const;

	int GetBranch() const;

	int GetRoot() const;

	string to_String(int numWords) const;

	static int GetNumPlantObj();
};
const string Plant::DEFAULT_NAME = "N/A";
int Plant::numPlantObj = 0;



class PlantException
{
private:
	string msg;
public:
	explicit PlantException(const string& n_msg = UNKNOWN_ERR) : msg(n_msg) {}
	string what() const { return msg; }
};



class Garden
{
private:
	int numPlants = 0;
	Plant** ppPlants = nullptr;

public:
	Garden();

	explicit Garden(int newNumPlants);

	~Garden();



	void clearGarden();

	void SortGarden(int option);

	void UpdateGarden();

	void SetPlantName();

	void PrintSorted();

	void PrintStats();

	void PrintLongestNamePlant(int _nWidth, int _Width);

	void PrintShortestNamePlant(int _nWidth, int _Width);

	void PrintHighBranchPlant(int _nWidth, int _Width);

	void AllocatePlants(int newNumPlants);

	void ReadFile(const string& fileName);

	void SetGarden(int newNumPlants);

	void SetGardenSize(int userInput);


	int GetNumPlants() const;

	int LongestName() const;

	int ShortestName() const;

	int HighestBranchNum() const;
};



string Lower(string name);
void mainMenu(Garden& userGarden, string errMess);
void printMainMenu(int& userInput);
void printOutputMenu(Garden& userGarden);
void updateMenu(Garden& userGarden);
void subSortMenu(Garden& userGarden, string errMess);
void statsMenu(Garden& userGarden);
void printObjMenu();
void printSubSortMenu(int& userInput);
void sort(Garden& userGarden, int userInput);
void invalidInput(string errMess);
int gardenPop(string errMess);



int main()
{
	srand(time(0));
	int menuOption = 0;
	string errMsg = "You entered an invalid number. Please try again.\n";


	Garden myGarden(gardenPop(errMsg));
	mainMenu(myGarden, errMsg);

	return 0;
}



Plant::Plant()
{
	SetPlant(DEFAULT_NAME, DEFAULT_NUM_BRANCHES, DEFAULT_NUM_ROOTS);
	numPlantObj++;
}

Plant::Plant(string newName)
{
	SetPlant(newName, DEFAULT_NUM_BRANCHES, DEFAULT_NUM_ROOTS);
	numPlantObj++;
}

Plant::Plant(string newName, int newBranches, int newRoots)
{
	SetPlant(newName, newBranches, newRoots);
	numPlantObj++;
}

Plant::~Plant()
{
	cout << GetName() << " destructor" << endl;

	SetPlant(DEFAULT_NAME, DEFAULT_NUM_BRANCHES, DEFAULT_NUM_ROOTS);
	numPlantObj--;
}

void Plant::SetName(const string& newName)
{
	SetPlant(newName, DEFAULT_NUM_BRANCHES, DEFAULT_NUM_ROOTS);
}

void Plant::Water()
{
	int water = rand() % NUM_WATER;

	switch (water)
	{
	case 1: branches += 1; break;
	case 2: branches += 2; break;
	case 3: branches += 2; roots += 1; break;
	}
}

void Plant::Sun()
{
	int sun = rand() % NUM_SUN;

	switch (sun)
	{
	case 1: roots += 1; break;
	case 2: roots += 2; break;
	}
}

void Plant::SetPlant(string newName, int newBranches, int newRoots)
{
	try
	{
		if (newName.length() < MIN_NAME)
			throw PlantException(NAME_ERR);

		if (newBranches < DEFAULT_NUM_BRANCHES)
			throw PlantException(BRANCHES_ERR);

		if (newRoots < DEFAULT_NUM_ROOTS)
			throw PlantException(ROOTS_ERR);

		name = newName;
		branches = newBranches;
		roots = newRoots;
	}
	catch (PlantException& ex)
	{
		cout << "Error: " << ex.what() << "Plant was not updated." << endl;
	}
	catch (...)
	{
		cout << "Error: Unknown error." << endl;
	}

}

string Plant::GetName() const
{
	return name;
}

int Plant::GetBranch() const
{
	return branches;
}

int Plant::GetRoot() const
{
	return roots;
}

string Plant::to_String(int numWords) const
{
	int width = max(numWords, 10) + 7;
	stringstream ss;
	ss << left << setw(width) << name << right << setw(width - 9) << branches << setw(width - 3) << roots;
	return ss.str();
}

int Plant::GetNumPlantObj()
{
	return numPlantObj;
}

Garden::Garden()
{
	numPlants = 0;
}

Garden::Garden(int newNumPlants)
{
	SetGarden(newNumPlants);
}

Garden::~Garden()
{
	clearGarden();

	cout << "\nGarden object destructor in process" << endl;
}

void Garden::clearGarden()
{
	if (ppPlants != nullptr)
	{
		for (int i = 0; i < numPlants; i++)
		{
			if (ppPlants[i] != nullptr)
				delete ppPlants[i];
		}
		delete ppPlants;
		ppPlants = nullptr;
		numPlants = 0;
	}
}

void Garden::SortGarden(int option)
{
	bool swap = false;

	for (int i = 0; i < numPlants - 1; i++)
	{
		for (int j = 0; j < numPlants - i - 1; j++)
		{
			switch (option)
			{
			case Name:
				swap = ppPlants[j]->GetName() > ppPlants[j + 1]->GetName();
				break;

			case Branches:
				swap = ppPlants[j]->GetBranch() > ppPlants[j + 1]->GetBranch();
				break;

			case Roots:
				swap = ppPlants[j]->GetRoot() > ppPlants[j + 1]->GetRoot();
				break;
			}

			if (swap)
			{
				Plant* temp = ppPlants[j];
				ppPlants[j] = ppPlants[j + 1];
				ppPlants[j + 1] = temp;
			}
		}
	}
}

void Garden::UpdateGarden()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < numPlants; j++)
		{
			ppPlants[j]->Water();
			ppPlants[j]->Sun();
		}
}

void Garden::SetPlantName()
{
	ifstream ifile("name.txt");

	if (!ifile.is_open())
	{
		cout << FILE_OPEN_ERR << endl;
		exit(-1);
	}

	int i = 0;
	string tempName;

	while (getline(ifile, tempName) && i < numPlants)
	{
		ppPlants[i]->SetName(Lower(tempName));
		i++;
	}

	ifile.close();
}

void Garden::PrintSorted()
{
	int noWidth = 6;
	int width = max(LongestName(), 10) + 7;

	cout << "\n" << left << setw(noWidth) << "NO"
		<< setw(width) << "Name"
		<< setw(width) << "Branches"
		<< setw(width) << "Roots" << endl;


	cout << setfill('-') << setw(noWidth + 3 * width) << "" << endl
		<< setfill(' ');

	for (int i = 0; i < GetNumPlants(); i++)
	{
		cout << setw(2) << i + 1 << "    " << ppPlants[i]->to_String(LongestName()) << endl;
	}

	PrintLongestNamePlant(noWidth, width);
	PrintShortestNamePlant(noWidth, width);
	PrintHighBranchPlant(noWidth, width);
}

void Garden::PrintStats()
{
	int noWidth = 6;
	int width = max(LongestName(), 10) + 7;

	PrintLongestNamePlant(noWidth, width);
	PrintShortestNamePlant(noWidth, width);
	PrintHighBranchPlant(noWidth, width);
}

void Garden::PrintLongestNamePlant(int _nWidth, int _Width)
{
	cout << "\nLongest Name\n"
		<< left << setw(_nWidth) << "NO"
		<< setw(_Width) << "Name"
		<< setw(_Width) << "Branches"
		<< setw(_Width) << "Roots" << endl;


	cout << setfill('-') << setw(_nWidth + 3 * _Width) << "" << endl
		<< setfill(' ');

	for (int i = 0; i < GetNumPlants(); i++)
	{
		if (ppPlants[i]->GetName().length() == LongestName())
			cout << setw(2) << i + 1 << "    " << ppPlants[i]->to_String(LongestName()) << endl;
	}
}

void Garden::PrintShortestNamePlant(int _nWidth, int _Width)
{
	cout << "\nShortest Name\n"
		<< left << setw(_nWidth) << "NO"
		<< setw(_Width) << "Name"
		<< setw(_Width) << "Branches"
		<< setw(_Width) << "Roots" << endl;


	cout << setfill('-') << setw(_nWidth + 3 * _Width) << "" << endl
		<< setfill(' ');

	for (int i = 0; i < GetNumPlants(); i++)
	{
		if (ppPlants[i]->GetName().length() == ShortestName())
			cout << setw(2) << i + 1 << "    " << ppPlants[i]->to_String(ShortestName()) << endl;
	}
}

void Garden::PrintHighBranchPlant(int _nWidth, int _Width)
{
	cout << "\nHighest Branch Name\n"
		<< left << setw(_nWidth) << "NO"
		<< setw(_Width) << "Name"
		<< setw(_Width) << "Branches"
		<< setw(_Width) << "Roots" << endl;


	cout << setfill('-') << setw(_nWidth + 3 * _Width) << "" << endl
		<< setfill(' ');

	int highNum = 0;
	for (int i = 0; i < GetNumPlants(); i++)
	{
		if (ppPlants[i]->GetBranch() == HighestBranchNum())
		{
			cout << setw(2) << i + 1 << "    " << ppPlants[i]->to_String(LongestName()) << endl;
			highNum++;
		}
	}

	cout << "\nHighest branches count: " << highNum << endl;
}

void Garden::AllocatePlants(int newNumPlants)
{
	try
	{
		ppPlants = new Plant * [newNumPlants] {nullptr};
		for (int i = 0; i < newNumPlants; i++)
		{
			ppPlants[i] = new Plant();
		}
		numPlants = newNumPlants;
	}
	catch (bad_alloc)
	{
		clearGarden();
		cout << "Failed to create the garden. Garden was not created.";
	}
}

void Garden::ReadFile(const string& fileName)
{
	ifstream infile(fileName);
	if (!infile.is_open())
	{
		cout << FILE_OPEN_ERR << endl;
		exit(-1);
	}

	string tempName;
	int i = 0, j = 0;;
	while (getline(infile, tempName) && i < numPlants)
	{
		ppPlants[i]->SetName(Lower(tempName));
		i++;
		j++;
	}

	while (i < numPlants)
	{
		string plantName = "Plant" + to_string(i + 1 - j);
		ppPlants[i]->SetName(plantName);
		i++;
	}

	infile.close();
}

void Garden::SetGarden(int newNumPlants)
{
	AllocatePlants(newNumPlants);
	ReadFile("name.txt");
}

int Garden::GetNumPlants() const
{
	return numPlants;
}

int Garden::LongestName() const
{
	int longest = ppPlants[0]->GetName().length();

	for (int i = 1; i < numPlants; i++)
	{
		int nameLength = ppPlants[i]->GetName().length();
		if (nameLength > longest)
			longest = nameLength;
	}

	return longest;
}

int Garden::ShortestName() const
{
	int shortest = ppPlants[0]->GetName().length();

	for (int i = 1; i < numPlants; i++)
	{
		int nameLength = ppPlants[i]->GetName().length();
		if (nameLength < shortest)
			shortest = nameLength;
	}

	return shortest;
}

int Garden::HighestBranchNum() const
{
	int highestBranch = ppPlants[0]->GetBranch();

	for (int i = 1; i < numPlants; i++)
	{
		if (ppPlants[i]->GetBranch() > highestBranch)
			highestBranch = ppPlants[i]->GetBranch();
	}

	return highestBranch;
}

string Lower(string name)
{
	string temp = name;
	int length = temp.length();

	for (int j = 0; j < length; j++)
		if (temp.at(j) >= 'A' && temp.at(j) <= 'Z')
			temp.at(j) = temp.at(j) + 32;

	return temp;
}

void mainMenu(Garden& userGarden, string errMess)
{
	int menuOption = 0;

	while (menuOption != Exit)
	{
		printMainMenu(menuOption);

		switch (menuOption)
		{
		case PrintMenu:
			printOutputMenu(userGarden);
			break;
		case Update:
			updateMenu(userGarden);
			break;
		case Sort:
			subSortMenu(userGarden, errMess);
			break;
		case Stats:
			statsMenu(userGarden);
			break;
		case Num_Obj:
			printObjMenu();
			break;
		case Exit:
			cout << "Programm is ending." << endl;
			break;
		default:
			invalidInput(errMess);
			break;
		}
	}
}

void printMainMenu(int& userInput)
{
	cout << "\n\n********** Main Menu **********\n"
		"[" << PrintMenu << "] Print\n"
		"[" << Update << "] Update\n"
		"[" << Sort << "] Sort\n"
		"[" << Stats << "] Stats\n"
		"[" << Num_Obj << "] Number of plant objects\n"
		"[" << Exit << "] Exit\n"
		"Select from the menu above: ";

	cin >> userInput;
}

void printOutputMenu(Garden& userGarden)
{
	userGarden.PrintSorted();
}

void updateMenu(Garden& userGarden)
{
	userGarden.UpdateGarden();
	cout << "\nPlants Updated." << endl;
	userGarden.PrintSorted();
}

void subSortMenu(Garden& userGarden, string errMess)
{
	int sortkey = 0;

	while (sortkey != BACK_TO_MAIN)
	{
		printSubSortMenu(sortkey);

		switch (sortkey)
		{
		case Name:
		case Branches:
		case Roots:
			sort(userGarden, sortkey);
			break;
		case BACK_TO_MAIN:
			break;
		default:
			invalidInput(errMess);
			break;
		}
	}
}

void statsMenu(Garden& userGarden)
{
	userGarden.PrintStats();
}

void printObjMenu()
{
	cout << "The number of plant objects currently in memory is " << Plant::GetNumPlantObj() << endl;
}

void printSubSortMenu(int& userInput)
{
	cout << "\n\n########## Sort Menu ##########\n"
		"[" << Name << "] Name\n"
		"[" << Branches << "] Branches\n"
		"[" << Roots << "] Roots\n"
		"[" << BACK_TO_MAIN << "] Back to Main Menu\n"
		"Select from the menu above: ";

	cin >> userInput;
}

void sort(Garden& userGarden, int userInput)
{
	userGarden.SortGarden(userInput);
	userGarden.PrintSorted();
}

void invalidInput(string errMess)
{
	cout << errMess << endl;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int gardenPop(string errMess)
{
	int userNumPlants;
	bool validNum = false;

	while (!validNum)
	{
		cout << "Enter the number of plants you want: ";
		cin >> userNumPlants;

		if (userNumPlants <= 0)
		{
			invalidInput(errMess);
		}
		else
		{
			validNum = true;
		}
	}

	return userNumPlants;
}