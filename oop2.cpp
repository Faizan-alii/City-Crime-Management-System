#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <limits>

using namespace std;

#define MAX_ITEMS 20
#define MAX_CRIMES 100

// ======================
// CLEAR SCREEN FUNCTION
// ======================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// -------------------------
// Date utilities
// -------------------------
bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool isValidDateParts(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) return false;
    int mdays[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
    int maxd = mdays[m];
    if (m == 2 && isLeapYear(y)) maxd = 29;
    return d <= maxd;
}

bool parseDateString(const string &s, int &d, int &m, int &y) {
    size_t p1 = s.find('/');
    if (p1 == string::npos) return false;
    size_t p2 = s.find('/', p1 + 1);
    if (p2 == string::npos) return false;
    try {
        string sd = s.substr(0, p1);
        string sm = s.substr(p1 + 1, p2 - (p1 + 1));
        string sy = s.substr(p2 + 1);
        auto trim = [](string str) {
            size_t a = 0;
            while (a < str.size() && isspace((unsigned char)str[a])) a++;
            size_t b = str.size();
            while (b > a && isspace((unsigned char)str[b - 1])) b--;
            return str.substr(a, b - a);
        };
        sd = trim(sd); sm = trim(sm); sy = trim(sy);
        d = stoi(sd); m = stoi(sm); y = stoi(sy);
    } catch (...) { return false; }
    return isValidDateParts(d, m, y);
}

int dateToInt(int d, int m, int y) {
    return y * 10000 + m * 100 + d;
}

int parseDateToInt(const string &s) {
    int d, m, y;
    if (!parseDateString(s, d, m, y)) return -1;
    return dateToInt(d, m, y);
}

// -------------------------
// Time validation
// -------------------------
bool isValidTime(const string& t) {
    if (t.length() != 5 || t[2] != ':') return false;
    int hh, mm;
    try {
        hh = stoi(t.substr(0, 2));
        mm = stoi(t.substr(3, 2));
    } catch (...) { return false; }
    return (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59);
}

// -------------------------
// Crime Class
// -------------------------
class Crime {
private:
    int crimeID; // New field for Crime ID
    string area, type, date, time, description;
    string weapons[MAX_ITEMS], suspects[MAX_ITEMS], victims[MAX_ITEMS];
    int weaponCount, suspectCount, victimCount;
    string status;
    string handler;

public:
    Crime() {
        weaponCount = suspectCount = victimCount = 0;
        status = "Unsolved";
        handler = "Unknown";
        crimeID = 0;
    }

    int getCrimeID() { return crimeID; }
    string getArea() { return area; }
    string getType() { return type; }
    string getDate() { return date; }
    string getTime() { return time; }
    string getStatus() { return status; }
    string getHandler() { return handler; }
    string getDescription() { return description; }

    void setArea(string a) { area = a; }
    void setType(string t) { type = t; }
    void setDate(string d) { date = d; }
    void setTime(string t) { time = t; }
    void setDescription(string d) { description = d; }
    void setStatus(string s) { status = s; }
    void setHandler(string h) { handler = h; }

    string toLowerCase(string s) {
        for (int i = 0; i < (int)s.length(); i++)
            if (s[i] >= 'A' && s[i] <= 'Z') s[i] += 32;
        return s;
    }

    string capitalizeWords(string s) {
        bool capNext = true;
        for (int i = 0; i < (int)s.length(); i++) {
            if (capNext && s[i] >= 'a' && s[i] <= 'z') s[i] -= 32;
            else if (!capNext && s[i] >= 'A' && s[i] <= 'Z') s[i] += 32;
            capNext = (s[i] == ' ');
        }
        return s;
    }

    int inputNumber() {
        string buf; int num;
        while (true) {
            getline(cin, buf);
            if (buf == "exit") return -1;
            try {
                num = stoi(buf);
                if (num > 0) return num;
            }
            catch (...) {}
            cout << "Please enter a valid positive number!\n";
        }
    }

    int inputMultiple(string itemName, string items[]) {
        cout << "Enter number of " << itemName << ": ";
        int num = inputNumber();
        if (num == -1) return 0;

        for (int i = 0; i < num; i++) {
            string input;
            do {
                cout << itemName << " " << i + 1 << ": ";
                getline(cin, input);
            } while (input.empty());
            items[i] = capitalizeWords(input);
        }
        return num;
    }

    void generateCrimeID() {
        ifstream idFile("crime_id_counter.txt");
        int lastID = 1000; // start before 1001
        if (idFile) {
            idFile >> lastID;
            idFile.close();
        }
        crimeID = lastID + 1;

        ofstream outFile("crime_id_counter.txt");
        outFile << crimeID;
        outFile.close();
    }

    void inputCrime() {
        clearScreen();
        generateCrimeID(); // Generate ID when creating a new crime

        string input;
        int step = 0;

        while (step < 10) {
            switch (step) {
            case 0: cout << "Enter Area (Korangi, Landhi, Gulshan, North, Nazimabad, Clifton, Lyari, Saddar, Malir, Orangi ): "; break;
            case 1: cout << "Enter Crime Type (Robbery, Snatching, Murder, Theft, Assault): "; break;
            case 2: cout << "Enter Date (DD/MM/YYYY): "; break;
            case 3: cout << "Enter Time (HH:MM): "; break;
            case 4: cout << "Enter Weapons:\n"; break;
            case 5: cout << "Enter Suspects:\n"; break;
            case 6: cout << "Enter Victims:\n"; break;
            case 7: cout << "Enter Description: "; break;
            case 8: cout << "Enter Case Status (Solved/Unsolved): "; break;
            case 9: cout << "Enter Case Handler: "; break;
            }

            if (step >= 4 && step <= 6) {
                if (step == 4) weaponCount = inputMultiple("Weapon", weapons);
                if (step == 5) suspectCount = inputMultiple("Suspect", suspects);
                if (step == 6) victimCount = inputMultiple("Victim", victims);
                step++;
                continue;
            }

            getline(cin, input);
            string lower = toLowerCase(input);

            switch (step) {
            case 0:
                if (lower == "korangi" || lower == "landhi" || lower == "gulshan" ||
                    lower == "north" || lower == "nazimabad" || lower == "clifton" ||
                    lower == "lyari" || lower == "saddar" || lower == "malir" || lower == "orangi")
                {
                    area = capitalizeWords(lower);
                    step++;
                } else cout << "Invalid area!\n";
                break;

            case 1:
                if (lower == "robbery" || lower == "snatching" || lower == "murder" ||
                    lower == "theft" || lower == "assault")
                {
                    type = capitalizeWords(lower);
                    step++;
                } else cout << "Invalid crime type!\n";
                break;

            case 2:
                if (!input.empty()) {
                    int tmp = parseDateToInt(input);
                    if (tmp == -1) cout << "Invalid date format.\n";
                    else { date = input; step++; }
                }
                break;

            case 3:
                if (!input.empty()) {
                    if (isValidTime(input)) { time = input; step++; }
                    else cout << "Invalid time format!\n";
                }
                break;

            case 7:
                if (!input.empty()) { description = input; step++; }
                break;

            case 8:
                if (lower == "solved" || lower == "unsolved") {
                    status = capitalizeWords(lower);
                    step++;
                } else cout << "Invalid status!\n";
                break;

            case 9:
                if (!input.empty()) { handler = capitalizeWords(input); step++; }
                else cout << "Handler cannot be empty!\n";
                break;
            }
        }

        cout << "\nCrime entered successfully with ID: " << crimeID << "\n";
    }

    void saveToFile() {
        try {
            ofstream file("crime_records.txt", ios::app);
            if (!file) throw runtime_error("Error opening file!");

            file << crimeID << ";" << area << ";" << type << ";" << date << ";" << time << ";";

            for (int i = 0; i < weaponCount; i++) {
                file << weapons[i];
                if (i != weaponCount - 1) file << ",";
            }
            file << ";";

            for (int i = 0; i < suspectCount; i++) {
                file << suspects[i];
                if (i != suspectCount - 1) file << ",";
            }
            file << ";";

            for (int i = 0; i < victimCount; i++) {
                file << victims[i];
                if (i != victimCount - 1) file << ",";
            }
            file << ";";

            file << description << ";" << status << ";" << handler << "\n";
            file.close();
        }
        catch (const exception& e) {
            cout << "Write Error: " << e.what() << "\n";
        }
    }
};

// -------------------------
// USER CLASS
// -------------------------
class User {
protected:
    string password;

public:
    User(string pass) { password = pass; }

    bool inputPassword() {
        int attempts = 3;
        while (attempts > 0) {
            string input = "";
            char ch;

            cout << "\nEnter Password (" << attempts << " attempts left): ";

            while (true) {
                ch = _getch();
                if (ch == 13) break;

                if (ch == 8 && !input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
                else if (ch != 8) {
                    input += ch;
                    cout << "*";
                }
            }

            cout << endl;

            if (input == password)
                return true;

            attempts--;
            cout << "Incorrect Password!\n";
        }

        cout << "Too many wrong attempts. Exiting...\n";
        exit(0);
    }



protected:

    // ----------- SEARCH BY ID -----------
    void searchCrimeByID() {
        clearScreen();
        string searchID;
        cout << "Enter Crime ID to search: ";
        getline(cin, searchID);

        ifstream file("crime_records.txt");
        if (!file) {
            cout << "No crime records found!\n";
            return;
        }

        string line;
        bool found = false;

        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[11]; // Crime ID included
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 10) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            if (fields[0] == searchID) { // Match ID
                found = true;
                cout << "\n=== Crime Found ===\n";
                cout << "Crime ID: " << fields[0] << "\n";
                cout << "Area: " << fields[1] << "\n";
                cout << "Type: " << fields[2] << "\n";
                cout << "Date: " << fields[3] << "\n";
                cout << "Time: " << fields[4] << "\n";
                cout << "Weapons: " << fields[5] << "\n";
                cout << "Suspects: " << fields[6] << "\n";
                cout << "Victims: " << fields[7] << "\n";
                cout << "Description: " << fields[8] << "\n";
                cout << "Status: " << fields[9] << "\n";
                cout << "Handler: " << fields[10] << "\n";
                break;
            }
        }

        if (!found) cout << "No crime found with ID " << searchID << ".\n";
        file.close();
    }

    // ----------- SEARCH BY AREA -----------
    void searchCrimeByArea() {
        clearScreen();
        string searchArea;
        cout << "Enter Area to search: ";
        getline(cin, searchArea);

        string searchLower = "";
        for (char c : searchArea) {
            if (c >= 'A' && c <= 'Z') c += 32;
            searchLower += c;
        }

        ifstream file("crime_records.txt");
        if (!file) {
            cout << "No crime records found!\n";
            return;
        }

        string line;
        int countFound = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[11];
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 10) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            string areaLower = fields[1];
            for (char &c : areaLower) if (c >= 'A' && c <= 'Z') c += 32;

            if (areaLower == searchLower) {
                countFound++;
                cout << "\n=== Crime " << countFound << " ===\n";
                cout << "Crime ID: " << fields[0] << "\n";
                cout << "Area: " << fields[1] << "\n";
                cout << "Type: " << fields[2] << "\n";
                cout << "Date: " << fields[3] << "\n";
                cout << "Time: " << fields[4] << "\n";
                cout << "Weapons: " << fields[5] << "\n";
                cout << "Suspects: " << fields[6] << "\n";
                cout << "Victims: " << fields[7] << "\n";
                cout << "Description: " << fields[8] << "\n";
                cout << "Status: " << fields[9] << "\n";
                cout << "Handler: " << fields[10] << "\n";
            }
        }

        if (countFound == 0) cout << "No crimes found for this area.\n";
        else cout << "\nTotal crimes found: " << countFound << "\n";

        file.close();
    }

    // ----------- SEARCH BY TYPE -----------
    void searchCrimeByType() {
        clearScreen();
        string typeInput;
        cout << "Enter Crime Type (Robbery, Snatching, Murder, Theft, Assault): ";
        getline(cin, typeInput);

        string searchLower = "";
        for (char c : typeInput) {
            if (c >= 'A' && c <= 'Z') c += 32;
            searchLower += c;
        }

        ifstream file("crime_records.txt");
        if (!file) {
            cout << "No crime records found!\n";
            return;
        }

        string line;
        int countFound = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[11];
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 10) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            string typeLower = fields[2];
            for (char &c : typeLower) if (c >= 'A' && c <= 'Z') c += 32;

            if (typeLower == searchLower) {
                countFound++;
                cout << "\n=== Crime " << countFound << " ===\n";
                cout << "Crime ID: " << fields[0] << "\n";
                cout << "Area: " << fields[1] << "\n";
                cout << "Type: " << fields[2] << "\n";
                cout << "Date: " << fields[3] << "\n";
                cout << "Time: " << fields[4] << "\n";
                cout << "Weapons: " << fields[5] << "\n";
                cout << "Suspects: " << fields[6] << "\n";
                cout << "Victims: " << fields[7] << "\n";
                cout << "Description: " << fields[8] << "\n";
                cout << "Status: " << fields[9] << "\n";
                cout << "Handler: " << fields[10] << "\n";
            }
        }

        if (countFound == 0) cout << "No crimes found for this type.\n";
        else cout << "\nTotal crimes found: " << countFound << "\n";

        file.close();
    }

    // ----------- SEARCH BY DATE RANGE -----------
    void searchCrimeByDateRange() {
        clearScreen();

        string startDateStr, endDateStr;
        cout << "Enter START Date (DD/MM/YYYY): ";
        getline(cin, startDateStr);
        cout << "Enter END Date (DD/MM/YYYY): ";
        getline(cin, endDateStr);

        int startInt = parseDateToInt(startDateStr);
        int endInt = parseDateToInt(endDateStr);

        if (startInt == -1 || endInt == -1) {
            cout << "Invalid date entered!\n";
            return;
        }

        if (startInt > endInt) {
            cout << "Start date must be earlier than end date.\n";
            return;
        }

        ifstream file("crime_records.txt");
        if (!file) {
            cout << "No crime records found!\n";
            return;
        }

        string line;
        int countFound = 0;

        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[11];
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 10) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            int crimeDate = parseDateToInt(fields[3]);
            if (crimeDate == -1) continue;

            if (crimeDate >= startInt && crimeDate <= endInt) {
                countFound++;
                cout << "\n=== Crime " << countFound << " ===\n";
                cout << "Crime ID: " << fields[0] << "\n";
                cout << "Area: " << fields[1] << "\n";
                cout << "Type: " << fields[2] << "\n";
                cout << "Date: " << fields[3] << "\n";
                cout << "Time: " << fields[4] << "\n";
                cout << "Weapons: " << fields[5] << "\n";
                cout << "Suspects: " << fields[6] << "\n";
                cout << "Victims: " << fields[7] << "\n";
                cout << "Description: " << fields[8] << "\n";
                cout << "Status: " << fields[9] << "\n";
                cout << "Handler: " << fields[10] << "\n";
            }
        }

        if (countFound == 0) cout << "No crimes found in this date range.\n";
        else cout << "\nTotal crimes found: " << countFound << "\n";

        file.close();
    }

    // ----------- SEARCH MENU -----------
    void searchCrimeMenu() {
        int choice;
        do {
            clearScreen();
            cout << "\n--- Search Crime ---\n"
                 << "1. Search by ID\n"
                 << "2. Search by Area\n"
                 << "3. Search by Type\n"
                 << "4. Search by Date Range\n"
                 << "5. Back\n"
                 << "Choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore();

            if (choice == 1) searchCrimeByID();
            else if (choice == 2) searchCrimeByArea();
            else if (choice == 3) searchCrimeByType();
            else if (choice == 4) searchCrimeByDateRange();

            if (choice != 5) {
                cout << "\nPress Enter to continue...";
                cin.get();
            }

        } while (choice != 5);
    }
};


// -------------------------
// ADMIN CLASS
// -------------------------
class Admin : public User {
public:
    Admin() : User("admin123") {}

    void portal();
    void updateCrime();
};

// -------------------------
// ADMIN PORTAL
// -------------------------
void Admin::portal() {
    clearScreen();
    if (!inputPassword()) return;

    int choice;
    do {
        clearScreen();

        cout << "\n--- Admin Portal ---\n"
             << "1. Add Crime\n"
             << "2. Update Crime\n"
             << "3. Search Crime\n"
             << "4. Back\n"
             << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            clearScreen();
            Crime c;
            c.inputCrime();
            c.saveToFile();
        }
        else if (choice == 2) {
            clearScreen();
            updateCrime();
        }
        else if (choice == 3) {
            searchCrimeMenu(); // Includes search by ID
        }

        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 4);
}

// -------------------------
// UPDATE CRIME
// -------------------------
void Admin::updateCrime() {
    clearScreen();

    string searchID;
    cout << "Enter Crime ID to update: ";
    getline(cin, searchID);

    ifstream inFile("crime_records.txt");
    if (!inFile) {
        cout << "No records found!\n";
        return;
    }

    string records[MAX_CRIMES];
    int count = 0;
    string line;

    while (getline(inFile, line))
        if (!line.empty()) records[count++] = line;

    inFile.close();

    int index = -1;
    for (int i = 0; i < count; i++) {
        size_t pos = records[i].find(';');
        string id = (pos == string::npos) ? records[i] : records[i].substr(0, pos);
        if (id == searchID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "\nNo crime found with ID " << searchID << ".\n";
        return;
    }

    string fields[11];
    int i = 0;
    size_t start = 0, end;
    string selected = records[index];
    while ((end = selected.find(';', start)) != string::npos && i < 10) {
        fields[i++] = selected.substr(start, end - start);
        start = end + 1;
    }
    fields[i] = selected.substr(start);

    string stepNames[11] = {
        "Crime ID", "Area", "Type", "Date", "Time",
        "Weapons", "Suspects", "Victims",
        "Description", "Status", "Handler"
    };

    string input;
    for (int j = 1; j < 11; j++) { // Skip Crime ID
        cout << "Enter new " << stepNames[j]
             << " (current: " << fields[j]
             << ", press Enter to keep): ";
        getline(cin, input);

        if (!input.empty()) {
            if (j == 3) { // Date field
                if (parseDateToInt(input) != -1)
                    fields[j] = input;
                else
                    cout << "Invalid date! Keeping old.\n";
            }
            else fields[j] = input;
        }
    }

    records[index] =
        fields[0] + ";" + fields[1] + ";" + fields[2] + ";" +
        fields[3] + ";" + fields[4] + ";" + fields[5] + ";" +
        fields[6] + ";" + fields[7] + ";" + fields[8] + ";" +
        fields[9] + ";" + fields[10];

    ofstream outFile("crime_records.txt");
    for (int j = 0; j < count; j++)
        outFile << records[j] << "\n";

    outFile.close();
    cout << "\nCrime updated successfully!\n";
}

// -------------------------
// VIEWER CLASS
// -------------------------
class Viewer : public User {
public:
    Viewer() : User("123") {}
    void portal();
};

// -------------------------
// VIEWER PORTAL
// -------------------------
void Viewer::portal() {
    clearScreen();
    if (!inputPassword()) return;

    int choice;
    do {
        clearScreen();
        cout << "\n--- Viewer Portal ---\n"
             << "1. Search Crime\n"
             << "2. Crime Statistics\n"
             << "3. Back\n"
             << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            User::searchCrimeMenu(); // Includes search by ID
        }
        else if (choice == 2) {
            clearScreen();
            int total = 0, unsolved = 0, solved = 0;
            ifstream file("crime_records.txt");
            if (!file) {
                cout << "No data available!\n";
            }
            else {
                string line;
                while (getline(file, line)) {
                    if (line.empty()) continue;
                    total++;

                    size_t pos1 = 0, pos2;
                    string fields[11];
                    int i = 0;
                    while ((pos2 = line.find(';', pos1)) != string::npos && i < 10) {
                        fields[i++] = line.substr(pos1, pos2 - pos1);
                        pos1 = pos2 + 1;
                    }
                    fields[i] = line.substr(pos1);

                    string status = fields[9]; // Status
                    for (char& c : status) if (c >= 'A' && c <= 'Z') c += 32;

                    if (status == "solved") solved++;
                    else unsolved++;
                }

                cout << "\n--- Crime Statistics ---\n";
                cout << "Total Crimes: " << total << "\n";
                cout << "Solved: " << solved << "\n";
                cout << "Unsolved: " << unsolved << "\n";
            }

            cout << "\nPress Enter to continue...";
            cin.get();
        }

    } while (choice != 3);
}

// -------------------------
// MAIN PROGRAM
// -------------------------
int main() {
    Admin admin;
    Viewer viewer;

    int choice;
    do {
        clearScreen();
        cout << "\n=== Crime Investigation System ===\n"
             << "1. Admin Portal\n"
             << "2. Viewer Portal\n"
             << "3. Exit Program\n"
             << "Enter your choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) admin.portal();
        else if (choice == 2) viewer.portal();

    } while (choice != 3);

    cout << "\nExiting Program...\n";
    return 0;
}

