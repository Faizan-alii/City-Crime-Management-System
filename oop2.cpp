#include <iostream>
#include <fstream>
#include <string>
#include <conio.h> // for _getch()
#include <algorithm>
#include <vector>

using namespace std;

class Crime {
private:
    string area;
    string type;
    string date;
    string time;
    vector<string> weapons;
    vector<string> suspects;
    vector<string> victims;
    string description;

public:
    // ---------- Getters ----------
    string getArea() { return area; }
    string getType() { return type; }
    string getDate() { return date; }
    string getTime() { return time; }
    vector<string> getWeapons() { return weapons; }
    vector<string> getSuspects() { return suspects; }
    vector<string> getVictims() { return victims; }
    string getDescription() { return description; }

    // ---------- Setters ----------
    void setArea(string a) { area = a; }
    void setType(string t) { type = t; }
    void setDate(string d) { date = d; }
    void setTime(string t) { time = t; }
    void setWeapons(vector<string> w) { weapons = w; }
    void setSuspects(vector<string> s) { suspects = s; }
    void setVictims(vector<string> v) { victims = v; }
    void setDescription(string d) { description = d; }

    // ---------- Helper Functions ----------
    string toLowerCase(string s) {
        for (char &c : s) c = tolower(c);
        return s;
    }

    string capitalizeWords(string s) {
        bool capNext = true;
        for (char &c : s) {
            if (capNext && isalpha(c)) { c = toupper(c); capNext = false; }
            else c = tolower(c);
            if (c == ' ') capNext = true;
        }
        return s;
    }

    vector<string> inputMultiple(const string& itemName) {
        vector<string> items;
        int num;
        string input;

        while (true) {
            cout << "Enter number of " << itemName << ": ";
            getline(cin, input);
            if (toLowerCase(input) == "exit") {
                cout << "Crime entry cancelled.\n";
                return {};
            }
            try {
                num = stoi(input);
                if (num <= 0) throw invalid_argument("Invalid");
                break;
            } catch (...) {
                cout << "Please enter a valid positive number!\n";
            }
        }

        for (int i = 0; i < num; i++) {
            while (true) {
                cout << itemName << " " << (i + 1) << ": ";
                getline(cin, input);
                if (!input.empty()) {
                    items.push_back(capitalizeWords(input));
                    break;
                } else {
                    cout << itemName << " cannot be empty!\n";
                }
            }
        }

        return items;
    }

    // ---------- Main Functions ----------
    void inputCrime() {
        // cin.ignore();
        string input;
        int step = 0;

        while (step < 8) {
            switch(step) {
                case 0: cout << "Enter Area (Korangi, Landhi, Gulshan, North, Nazimabad, Clifton, Lyari, Saddar, Malir, Orangi)\n(back = previous, exit = cancel): "; break;
                case 1: cout << "Enter Crime Type (Robbery, Snatching, Murder, Theft, Assault)\n(back = previous, exit = cancel): "; break;
                case 2: cout << "Enter Date (DD/MM/YYYY)\n(back = previous, exit = cancel): "; break;
                case 3: cout << "Enter Time (HH:MM)\n(back = previous, exit = cancel): "; break;
                case 4: cout << "Enter Weapons Used\n"; break;
                case 5: cout << "Enter Suspects\n"; break;
                case 6: cout << "Enter Victims\n"; break;
                case 7: cout << "Enter Description\n(back = previous, exit = cancel): "; break;
            }

            if (step != 4 && step != 5 && step != 6) {
                getline(cin, input);
                string lower = toLowerCase(input);

                if (lower == "exit") { cout << "Crime entry cancelled.\n"; return; }
                if (lower == "back") { if(step > 0) step--; else cout << "Already at first field.\n"; continue; }

                switch(step) {
                    case 0: {
                        string a = lower;
                        if (a == "korangi" || a == "landhi" || a == "gulshan" || a == "north" || a == "nazimabad" || a == "clifton" || a == "lyari" || a == "saddar" || a == "malir" || a == "orangi") {
                            setArea(capitalizeWords(a));
                            step++;
                        } else cout << "Invalid area!\n";
                        break;
                    }
                    case 1: {
                        string t = lower;
                        if (t == "robbery" || t == "snatching" || t == "murder" || t == "theft" || t == "assault") {
                            setType(capitalizeWords(t));
                            step++;
                        } else cout << "Invalid crime type!\n";
                        break;
                    }
                    case 2: if (!input.empty()) { setDate(input); step++; } else cout << "Date cannot be empty!\n"; break;
                    case 3: if (!input.empty()) { setTime(input); step++; } else cout << "Time cannot be empty!\n"; break;
                    case 7: if (!input.empty()) { setDescription(input); step++; } else cout << "Description cannot be empty!\n"; break;
                }
            } else {
                if (step == 4) { setWeapons(inputMultiple("weapon")); if (getWeapons().empty()) return; step++; }
                if (step == 5) { setSuspects(inputMultiple("suspect")); if (getSuspects().empty()) return; step++; }
                if (step == 6) { setVictims(inputMultiple("victim")); if (getVictims().empty()) return; step++; }
            }
        }

        cout << "\nCrime entered successfully!\n";
    }

    void showCrime() {
        cout << "\n--- Crime Details ---\n";
        cout << "Area: " << getArea() << "\nType: " << getType() << "\nDate: " << getDate() << "\nTime: " << getTime() << "\n";

        cout << "Weapons Used:\n"; for (size_t i=0; i<getWeapons().size(); i++) cout << "  Weapon " << i+1 << ": " << getWeapons()[i] << "\n";
        cout << "Suspects:\n"; for (size_t i=0; i<getSuspects().size(); i++) cout << "  Suspect " << i+1 << ": " << getSuspects()[i] << "\n";
        cout << "Victims:\n"; for (size_t i=0; i<getVictims().size(); i++) cout << "  Victim " << i+1 << ": " << getVictims()[i] << "\n";

        cout << "Description: " << getDescription() << "\n";
    }

    void saveToFile() {
        ofstream file("crime_records.txt", ios::app);
        file << getArea() << ";" << getType() << ";" << getDate() << ";" << getTime() << ";";

        for (size_t i=0; i<getWeapons().size(); i++) { file << getWeapons()[i]; if(i != getWeapons().size()-1) file << ","; }
        file << ";";

        for (size_t i=0; i<getSuspects().size(); i++) { file << getSuspects()[i]; if(i != getSuspects().size()-1) file << ","; }
        file << ";";

        for (size_t i=0; i<getVictims().size(); i++) { file << getVictims()[i]; if(i != getVictims().size()-1) file << ","; }
        file << ";" << getDescription() << "\n";
        file.close();
    }
};


// -------------------------
// User Class
// -------------------------
class User {
protected:
    string password; // Encapsulated password

public:
    User(string pass) {
        password = pass; // set password
    }

    bool inputPassword() {
        int attempts = 3;
        while (attempts > 0) {
            string input = ""; char ch;
            cout << "\nEnter Password (" << attempts << " attempts left): ";
            while (true) {
                ch = _getch();
                if (ch == 13) { cout << endl; break; } // Enter
                else if (ch == 8) { // Backspace
                    if (!input.empty()) { input.resize(input.size() - 1); cout << "\b \b"; }
                }
                else { input += ch; cout << "*"; }
            }
            if (input == password) return true;
            else { cout << "Incorrect Password!\n"; attempts--; }
        }
        cout << "Wrong password 3 times. Exiting...\n";
        exit(0);
    }

void portal(){
    cout<<"User Portal(base function)\n";
}



    // -------------------------
// Search Crime By Area 
// -------------------------
protected:
    void searchCrimeByArea() {
        string searchArea;
        cout << "Enter area to search: ";
        getline(cin, searchArea);

        string searchAreaLower;
        transform(searchArea.begin(), searchArea.end(), back_inserter(searchAreaLower), ::tolower);

        ifstream file("crime_records.txt");
        if (!file) { cout << "No crime records found!\n"; return; }

        string line;
        bool found = false;

        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[8];
            size_t start = 0, end;
            int i = 0;

            while ((end = line.find(';', start)) != string::npos && i < 7) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            string areaLower = fields[0];
            transform(areaLower.begin(), areaLower.end(), areaLower.begin(), ::tolower);

            if (areaLower == searchAreaLower) {
                found = true;
                cout << "\n--- Crime ---\n";
                cout << "Area: " << fields[0]
                     << "\nType: " << fields[1]
                     << "\nDate: " << fields[2]
                     << "\nTime: " << fields[3]
                     << "\nWeapon: " << fields[4]
                     << "\nSuspect: " << fields[5]
                     << "\nVictim: " << fields[6]
                     << "\nDescription: " << fields[7] << "\n";
            }
        }

        if (!found) cout << "No crimes found in this area.\n";
        file.close();
    }




};




// -------------------------
// Admin Portal
// -------------------------
class Admin : public User {
public:
    Admin() : User("admin123") {} // inherits User constructor

    void portal() {
        if (!inputPassword()) return; // use User's inputPassword
        int choice;
        do {
            cout << "\n--- Admin Portal ---\n";
            cout << "1. Add Crime\n2. Update Crime\n3. Search Crime by Area\n4. Exit\nChoice: ";
            cin >> choice; cin.ignore();

            if (choice == 1) {
                Crime c;
                c.inputCrime();
                c.saveToFile();
            }
            else if (choice == 2) {
                updateCrime(); // existing function
            }
            else if (choice == 3) {
                searchCrimeByArea(); // existing function
            }
        } while (choice != 4);
    }

// -------------------------
// Update Crime
// -------------------------
void updateCrime() {
    // cin.ignore();
    string searchArea;
    cout << "Enter the area of crime to update: ";
    getline(cin, searchArea);
    string searchAreaLower;
    transform(searchArea.begin(), searchArea.end(), back_inserter(searchAreaLower), ::tolower);

    ifstream inFile("crime_records.txt");
    if (!inFile) {
        cout << "No records found!\n";
        return;
    }

    string line;
    string records[100];
    int count = 0;
    while (getline(inFile, line)) if (!line.empty()) records[count++] = line;
    inFile.close();

    int indices[100], idxCount = 0;
    for (int i = 0; i < count; i++) {
        string area = records[i].substr(0, records[i].find(';'));
        string areaLower = area;
        transform(areaLower.begin(), areaLower.end(), areaLower.begin(), ::tolower);
        if (areaLower == searchAreaLower) {
            indices[idxCount++] = i;
            cout << idxCount << ". " << records[i] << "\n";
        }
    }

    if (idxCount == 0) {
        cout << "No crimes found in this area.\n";
        return;
    }

    int choice;
    cout << "Enter the number of the crime to update: ";
    cin >> choice;
    if (choice < 1 || choice > idxCount) {
        cout << "Invalid choice!\n";
        return;
    }

    string fields[8];
    string selected = records[indices[choice - 1]];
    size_t start = 0, end; int i = 0;
    while ((end = selected.find(';', start)) != string::npos && i < 7) {
        fields[i++] = selected.substr(start, end - start);
        start = end + 1;
    }
    fields[i] = selected.substr(start);

    cin.ignore();
    string input;
    string stepNames[8] = { "Area","Type","Date","Time","Weapon","Suspect","Victim","Description" };
    for (int j = 0; j < 8; j++) {
        cout << "Enter new " << stepNames[j] << " (current: " << fields[j] << ", press Enter to keep): ";
        getline(cin, input);
        if (!input.empty()) fields[j] = input;
    }

    records[indices[choice - 1]] = fields[0] + ";" + fields[1] + ";" + fields[2] + ";" +
                                   fields[3] + ";" + fields[4] + ";" + fields[5] + ";" +
                                   fields[6] + ";" + fields[7];

    ofstream outFile("crime_records.txt");
    for (int j = 0; j < count; j++) outFile << records[j] << "\n";
    outFile.close();

    cout << "Crime updated successfully!\n";
}


};

// -------------------------
// Viewer Class
// -------------------------
class Viewer : public User {
public:
    Viewer() : User("123") {} // inherits User constructor

    void portal() {
        if (!inputPassword()) return; // use User's inputPassword
        int choice;
        do {
            cout << "\n--- Viewer Portal ---\n";
            cout << "1. View Crimes by Area\n2. View Crime Statistics\n3. Exit\nChoice: ";
            cin >> choice; cin.ignore();

            if (choice == 1) {
                searchCrimeByArea(); // existing function
            }
            else if (choice == 2) {
                viewStatistics(); // existing function
            }
        } while (choice != 3);
    }


// -------------------------
// View Statistics
// -------------------------
void viewStatistics() {
    ifstream file("crime_records.txt");
    if (!file) { cout << "No crime records found!\n"; return; }

    string line;
    string areasList[10] = { "Korangi","Landhi","Gulshan","Gulberg","Saddar","Clifton","Malir","F.B Area","Nazimabad","Orangi" };
    int areaCount[10] = {0};
    int robbery=0,snatching=0,murder=0,theft=0,assault=0;
    int totalCrimes = 0;

    while (getline(file,line)) {
        if(line.empty()) continue;
        totalCrimes++;
        string fields[8]; int i=0; size_t pos; string temp=line;
        while ((pos=temp.find(';'))!=string::npos && i<7) { fields[i++]=temp.substr(0,pos); temp.erase(0,pos+1); }
        fields[i]=temp;

        string areaLower=fields[0]; transform(areaLower.begin(),areaLower.end(),areaLower.begin(),::tolower);
        for(int j=0;j<10;j++){
            string aLower=areasList[j]; transform(aLower.begin(),aLower.end(),aLower.begin(),::tolower);
            if(areaLower==aLower) areaCount[j]++;
        }

        string typeLower=fields[1]; transform(typeLower.begin(),typeLower.end(),typeLower.begin(),::tolower);
        if(typeLower=="robbery") robbery++;
        else if(typeLower=="snatching") snatching++;
        else if(typeLower=="murder") murder++;
        else if(typeLower=="theft") theft++;
        else if(typeLower=="assault") assault++;
    }
    file.close();

    cout<<"\n====== CITY WIDE CRIME STATISTICS ======\n";
    cout<<"Total Crimes in City: "<<totalCrimes<<"\n";
    cout<<"\n--- Area-wise Crime Counts ---\n";
    for(int j=0;j<10;j++) cout<<areasList[j]<<": "<<areaCount[j]<<" crimes\n";

    cout<<"\n--- Crime Types Count ---\n";
    cout<<"Robbery: "<<robbery<<"\nSnatching: "<<snatching<<"\nMurder: "<<murder<<"\nTheft: "<<theft<<"\nAssault: "<<assault<<"\n";

    cout<<"\n--- Crime Percentage Area-wise (Out of 25) ---\n";
    for(int j=0;j<10;j++){ float percent=(areaCount[j]*100.0f)/25.0f; cout<<areasList[j]<<": "<<percent<<"%\n"; }
    cout<<"\n========================================\n";
}


};
// -------------------------
// Main Menu
// -------------------------
int main() {
    int portalChoice;
    do {
        cout << "\n=== Crime Investigator Project ===\n";
        cout << "1. Admin Portal\n2. Viewer Portal\n3. Exit Program\nEnter your choice: ";
        cin >> portalChoice;
        cin.ignore(); // clear input buffer

        if (portalChoice == 1) {
            Admin a;
            a.portal(); // use Admin class portal
        }
        else if (portalChoice == 2) {
            Viewer v;
            v.portal(); // use Viewer class portal
        }
        else if (portalChoice == 3) {
            cout << "Exiting Program...\n";
        }
        else {
            cout << "Invalid choice! Try again.\n";
        }
    } while (portalChoice != 3);

    return 0;
}

