#pragma once
#include <string>
#include <vector>
#include "rapidcsv.h"
using namespace std;

class CustomersListCSV
{
public:
	static void Load();
	static void CreateNewFile();
	static void Interface();
	static bool IsIDNumberAvailable(string CCCD);
	static bool IsIDNumberAvailable(string CCCD, string& customerName);
	static bool IsIDNumberAvailable(string CCCD, string& customerName, int& index);

private:
	static vector<string> vNames;
	static vector<string> vBirthdates;
	static vector<string> vPhoneNumbers;
	static vector<string> vAddresses;
	static vector<string> vCustomerIDs;
	static vector<string> vNotes;

	static int customersCount;

	static rapidcsv::Document CSVFile;

	static void EditCustomerInfo(int index);

	static void Save();
	static bool ValidateCCCD(string CCCD);
	static void AddCustomer();
	static void FindByIDNumber();
	static void RemoveCustomer();
	static void RemoveCustomer(int index);
};

