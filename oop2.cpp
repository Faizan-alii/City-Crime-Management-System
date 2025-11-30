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

// -------------------------
// Crime Class
// -------------------------
class Crime {
private:
    string area, type, date, time, description;

    string weapons[MAX_ITEMS], suspects[MAX_ITEMS], victims[MAX_ITEMS];
    int weaponCount, suspectCount, victimCount;

    string status;      // NEW FIELD
    string handler;     // NEW FIELD

public:
    Crime() {
        weaponCount = suspectCount = victimCount = 0;
        status = "Unsolved";  // default
        handler = "Unknown";
    }

    // ---------- Getters ----------
    string getArea() { return area; }
    string getType() { return type; }
    string getDate() { return date; }
    string getTime() { return time; }
    string getStatus() { return status; }
    string getHandler() { return handler; }
    string getDescription() { return description; }

    int getWeaponCount() { return weaponCount; }
    int getSuspectCount() { return suspectCount; }
    int getVictimCount() { return victimCount; }

    string getWeapon(int i) { return weapons[i]; }
    string getSuspect(int i) { return suspects[i]; }
    string getVictim(int i) { return victims[i]; }

    // ---------- Setters ----------
    void setArea(string a) { area = a; }
    void setType(string t) { type = t; }
    void setDate(string d) { date = d; }
    void setTime(string t) { time = t; }
    void setDescription(string d) { description = d; }

    void setStatus(string s) { status = s; }
    void setHandler(string h) { handler = h; }

    void setWeapons(string w[], int count) {
        weaponCount = count;
        for (int i = 0; i < count; i++) weapons[i] = w[i];
    }

    void setSuspects(string s[], int count) {
        suspectCount = count;
        for (int i = 0; i < count; i++) suspects[i] = s[i];
    }

    void setVictims(string v[], int count) {
        victimCount = count;
        for (int i = 0; i < count; i++) victims[i] = v[i];
    }

    // ---------- Helper Functions ----------
    string toLowerCase(string s) {
        for (int i = 0; i < (int)s.length(); i++)
            if (s[i] >= 'A' && s[i] <= 'Z') s[i] += 32;
        return s;
    }

    string capitalizeWords(string s) {
        bool capNext = true;
        for (int i = 0; i < (int)s.length(); i++) {
            if (capNext && s[i] >= 'a' && s[i] <= 'z')
                s[i] -= 32;
            else if (!capNext && s[i] >= 'A' && s[i] <= 'Z')
                s[i] += 32;

            capNext = (s[i] == ' ');
        }
        return s;
    }

    int inputNumber() {
        string buf;
        int num;
        while (true) {
            getline(cin, buf);
            if (buf == "exit") return -1;
            try {
                // stoi will throw if not valid integer
                num = stoi(buf);
                if (num > 0) return num;
                cout << "Please enter a valid positive number!\n";
            }
            catch (const exception&) {
                cout << "Please enter a valid positive number!\n";
            }
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

    // ---------- Main input function ----------
    void inputCrime() {
        string input;
        int step = 0;

        while (step < 10) {
            switch (step) {
            case 0: cout << "Enter Area (Korangi, Landhi, Gulshan, North, Nazimabad, Clifton, Lyari, Saddar, Malir, Orangi): "; break;
            case 1: cout << "Enter Crime Type (Robbery, Snatching, Murder, Theft, Assault): "; break;
            case 2: cout << "Enter Date (DD/MM/YYYY): "; break;
            case 3: cout << "Enter Time (HH:MM): "; break;
            case 4: cout << "Enter Weapons Used:\n"; break;
            case 5: cout << "Enter Suspects:\n"; break;
            case 6: cout << "Enter Victims:\n"; break;
            case 7: cout << "Enter Description: "; break;
            case 8: cout << "Enter Case Status (Solved / Unsolved): "; break;
            case 9: cout << "Enter Case Handler (Officer Name): "; break;
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
                    lower == "lyari" || lower == "saddar" || lower == "malir" ||
                    lower == "orangi") {
                    area = capitalizeWords(lower);
                    step++;
                } else cout << "Invalid area!\n";
                break;

            case 1:
                if (lower == "robbery" || lower == "snatching" || lower == "murder" ||
                    lower == "theft" || lower == "assault") {
                    type = capitalizeWords(lower);
                    step++;
                } else cout << "Invalid crime type!\n";
                break;

            case 2: if (!input.empty()) { date = input; step++; } break;
            case 3: if (!input.empty()) { time = input; step++; } break;
            case 7: if (!input.empty()) { description = input; step++; } break;

            case 8:
                if (lower == "solved" || lower == "unsolved") {
                    status = capitalizeWords(input);
                    step++;
                } else cout << "Invalid status! Use Solved/Unsolved.\n";
                break;

            case 9:
                if (!input.empty()) {
                    handler = capitalizeWords(input);
                    step++;
                } else cout << "Handler name cannot be empty!\n";
                break;
            }
        }
        cout << "\nCrime entered successfully!\n";
    }

    // ---------- Display Crime ----------
    void showCrime() {
        cout << "\n--- Crime Details ---\n";
        cout << "Area: " << area << "\n";
        cout << "Type: " << type << "\n";
        cout << "Date: " << date << "\n";
        cout << "Time: " << time << "\n";

        cout << "Weapons:\n";
        for (int i = 0; i < weaponCount; i++)
            cout << "  " << weapons[i] << "\n";

        cout << "Suspects:\n";
        for (int i = 0; i < suspectCount; i++)
            cout << "  " << suspects[i] << "\n";

        cout << "Victims:\n";
        for (int i = 0; i < victimCount; i++)
            cout << "  " << victims[i] << "\n";

        cout << "Description: " << description << "\n";
        cout << "Status: " << status << "\n";
        cout << "Handler: " << handler << "\n";
    }

    // ---------- Save to File ----------
    void saveToFile() {
        try {
            ofstream file("crime_records.txt", ios::app);
            if (!file) throw runtime_error("Error opening crime_records.txt for writing!");

            file << area << ";" << type << ";" << date << ";" << time << ";";

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

            file << ";" << description << ";" << status << ";" << handler << "\n";

            file.close();
        }
        catch (const exception& e) {
            cout << "File Write Error: " << e.what() << "\n";
        }
    }
};


// --------------------------------
// USER CLASS
// --------------------------------
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
                } else if (ch != 8) {
                    input += ch;
                    cout << "*";
                }
            }

            cout << endl;

            if (input == password) return true;

            attempts--;
            cout << "Incorrect Password!\n";
        }

        cout << "Too many wrong attempts. Exiting...\n";
        exit(0);
    }

protected:
    // ----------- SEARCH BY AREA -----------
    void searchCrimeByArea() {
        string searchArea;
        cout << "Enter  area to search: ";
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

        try {
            while (getline(file, line)) {
                if (line.empty()) continue;

                string fields[10];
                int i = 0;
                size_t start = 0, end;

                while ((end = line.find(';', start)) != string::npos && i < 9) {
                    fields[i++] = line.substr(start, end - start);
                    start = end + 1;
                }
                fields[i] = line.substr(start);

                string areaLower = fields[0];
                for (char& c : areaLower)
                    if (c >= 'A' && c <= 'Z') c += 32;

                if (areaLower == searchLower) {
                    countFound++;

                    cout << "\n=== Crime " << countFound << " ===\n";
                    cout << "Area: " << fields[0] << "\n";
                    cout << "Type: " << fields[1] << "\n";
                    cout << "Date: " << fields[2] << "\n";
                    cout << "Time: " << fields[3] << "\n";
                    cout << "Weapons: " << fields[4] << "\n";
                    cout << "Suspects: " << fields[5] << "\n";
                    cout << "Victims: " << fields[6] << "\n";
                    cout << "Description: " << fields[7] << "\n";
                    cout << "Status: " << fields[8] << "\n";
                    cout << "Handler: " << fields[9] << "\n";
                }
            }
        }
        catch (const exception& e) {
            cout << "Error reading records: " << e.what() << "\n";
        }

        if (countFound == 0)
            cout << "No crimes found for this area.\n";
        else
            cout << "\nTotal crimes found: " << countFound << "\n";

        file.close();
    }
};


// -------------------------
// Admin Class
// -------------------------
class Admin : public User {
public:
    Admin() : User("admin123") {}

    void portal();
    void updateCrime();
};

void Admin::portal() {
    if (!inputPassword()) return;

    int choice;
    do {
        cout << "\n--- Admin Portal ---\n1. Add Crime\n2. Update Crime\n3. Search Crime by Area\n4. Exit\nChoice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            Crime c;
            c.inputCrime();
            c.saveToFile();
        }
        else if (choice == 2)
            updateCrime();
        else if (choice == 3)
            searchCrimeByArea();

    } while (choice != 4);
}

void Admin::updateCrime() {
    string searchArea;
    cout << "Enter area of crime to update: ";
    getline(cin, searchArea);

    string searchLower = "";
    for (char c : searchArea) {
        if (c >= 'A' && c <= 'Z') c += 32;
        searchLower += c;
    }

    ifstream inFile("crime_records.txt");
    if (!inFile) {
        cout << "No records found!\n";
        return;
    }

    string records[MAX_CRIMES];
    int count = 0;
    string line;

    try {
        while (getline(inFile, line))
            if (!line.empty()) records[count++] = line;
    }
    catch (const exception& e) {
        cout << "Error reading file: " << e.what() << "\n";
        inFile.close();
        return;
    }

    inFile.close();

    int indices[MAX_CRIMES];
    int idxCount = 0;

    for (int i = 0; i < count; i++) {
        size_t posSep = records[i].find(';');
        string area = (posSep == string::npos) ? records[i] : records[i].substr(0, posSep);

        string areaLower = area;
        for (char& c : areaLower)
            if (c >= 'A' && c <= 'Z') c += 32;

        if (areaLower == searchLower) {
            indices[idxCount] = i;
            cout << idxCount + 1 << ". " << records[i] << "\n";
            idxCount++;
        }
    }

    if (idxCount == 0) {
        cout << "No crime found in this area.\n";
        return;
    }

    int choice;
    cout << "Enter number of crime to update: ";
    if (!(cin >> choice)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    cin.ignore();

    if (choice < 1 || choice > idxCount) {
        cout << "Invalid choice!\n";
        return;
    }

    string selected = records[indices[choice - 1]];

    string fields[10];
    int i = 0;
    size_t start = 0, end;

    while ((end = selected.find(';', start)) != string::npos && i < 9) {
        fields[i++] = selected.substr(start, end - start);
        start = end + 1;
    }
    fields[i] = selected.substr(start);

    string stepNames[10] = {
        "Area", "Type", "Date", "Time",
        "Weapons", "Suspects", "Victims",
        "Description", "Status", "Handler"
    };

    string input;

    for (int j = 0; j < 10; j++) {
        cout << "Enter new " << stepNames[j] << " (current: " << fields[j] << ", press Enter to keep): ";
        getline(cin, input);
        if (!input.empty()) fields[j] = input;
    }

    records[indices[choice - 1]] =
        fields[0] + ";" + fields[1] + ";" + fields[2] + ";" +
        fields[3] + ";" + fields[4] + ";" + fields[5] + ";" +
        fields[6] + ";" + fields[7] + ";" + fields[8] + ";" +
        fields[9];

    try {
        ofstream outFile("crime_records.txt");
        if (!outFile) throw runtime_error("Could not open crime_records.txt for writing!");

        for (int j = 0; j < count; j++)
            outFile << records[j] << "\n";

        outFile.close();

        cout << "Crime updated successfully!\n";
    }
    catch (const exception& e) {
        cout << "File Write Error: " << e.what() << "\n";
    }
}



// -------------------------
// Viewer Class
// -------------------------
class Viewer : public User {
public:
    Viewer() : User("123") {}

    void portal();
    void viewStatistics();
    void searchCrimeByCrimeType();
};

void Viewer::portal() {
    if (!inputPassword()) return;

    int choice;
    do {
        cout << "\n--- Viewer Portal ---\n1. View Crimes by Area\n2. View Crime Statistics\n3. Search Crime by Type\n4. Exit\nChoice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) searchCrimeByArea();
        else if (choice == 2) viewStatistics();
        else if (choice == 3) searchCrimeByCrimeType();

    } while (choice != 4);
}

void Viewer::viewStatistics()
{
    ifstream file("crime_records.txt");
    if (!file)
    {
        cout << "No crime records found!\n";
        return;
    }

    string line;
    string areasList[10] = {"Korangi", "Landhi", "Gulshan", "Gulberg", "Saddar", "Clifton", "Malir", "F.B Area", "Nazimabad", "Orangi"};
    int areaCount[10] = {0};
    int robbery = 0, snatching = 0, murder = 0, theft = 0, assault = 0;
    int totalCrimes = 0;

    try {
        while (getline(file, line))
        {
            if (line.empty()) continue;
            totalCrimes++;

            string fields[10];
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 9) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            // Ensure we have at least two fields
            if (fields[0].empty() || fields[1].empty()) continue;

            // Convert to lowercase for comparison
            for (int k = 0; k < (int)fields[0].length(); k++)
                if (fields[0][k] >= 'A' && fields[0][k] <= 'Z') fields[0][k] += 32;
            for (int k = 0; k < (int)fields[1].length(); k++)
                if (fields[1][k] >= 'A' && fields[1][k] <= 'Z') fields[1][k] += 32;

            // Count area occurrences
            for (int j = 0; j < 10; j++)
            {
                string aLower = areasList[j];
                for (int k = 0; k < (int)aLower.length(); k++)
                    if (aLower[k] >= 'A' && aLower[k] <= 'Z') aLower[k] += 32;

                if (fields[0] == aLower) areaCount[j]++;
            }

            // Count crime types
            if (fields[1] == "robbery") robbery++;
            else if (fields[1] == "snatching") snatching++;
            else if (fields[1] == "murder") murder++;
            else if (fields[1] == "theft") theft++;
            else if (fields[1] == "assault") assault++;
        }
    }
    catch (const exception& e) {
        cout << "Error processing statistics: " << e.what() << "\n";
        file.close();
        return;
    }

    file.close();

    // ======================= AUTHENTIC REPORT =======================
    cout << "\n**\n";
    cout << "          CITY CRIME STATISTICS REPORT\n";
    cout << "\n";
    cout << "Total Crimes Recorded: " << totalCrimes << "\n\n";

    cout << "Area-wise Crime Distribution:\n";
    cout << "----------------------------------------------\n";
    cout << "Area          | Crimes | % of Total (25)\n";
    cout << "----------------------------------------------\n";
    for (int j = 0; j < 10; j++)
    {
        float percent = (areaCount[j] * 100.0f) / 25.0f;
        printf("%-13s | %-6d | %-10.2f%%\n", areasList[j].c_str(), areaCount[j], percent);
    }
    cout << "----------------------------------------------\n\n";

    cout << "Crime Type Summary:\n";
    cout << "---------------------------\n";
    printf("Robbery   : %d\n", robbery);
    printf("Snatching : %d\n", snatching);
    printf("Murder    : %d\n", murder);
    printf("Theft     : %d\n", theft);
    printf("Assault   : %d\n", assault);
    cout << "---------------------------\n";

    cout << "*************** END OF REPORT ***************\n\n";
}
  

void Viewer::searchCrimeByCrimeType() {
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

    try {
        while (getline(file, line)) {
            if (line.empty()) continue;

            string fields[10];
            int i = 0;
            size_t start = 0, end;

            while ((end = line.find(';', start)) != string::npos && i < 9) {
                fields[i++] = line.substr(start, end - start);
                start = end + 1;
            }
            fields[i] = line.substr(start);

            string typeLower = fields[1];
            for (char& c : typeLower)
                if (c >= 'A' && c <= 'Z') c += 32;

            if (typeLower == searchLower) {
                countFound++;

                cout << "\n=== Crime " << countFound << " ===\n";
                cout << "Area: " << fields[0] << "\n";
                cout << "Type: " << fields[1] << "\n";
                cout << "Date: " << fields[2] << "\n";
                cout << "Time: " << fields[3] << "\n";
                cout << "Weapons: " << fields[4] << "\n";
                cout << "Suspects: " << fields[5] << "\n";
                cout << "Victims: " << fields[6] << "\n";
                cout << "Description: " << fields[7] << "\n";
                cout << "Status: " << fields[8] << "\n";
                cout << "Handler: " << fields[9] << "\n";
            }
        }
    }
    catch (const exception& e) {
        cout << "Error reading records: " << e.what() << "\n";
    }

    if (countFound == 0)
        cout << "No crimes found for this crime type.\n";
    else
        cout << "\nTotal crimes found: " << countFound << "\n";

    file.close();
}



// -------------------------
// Main
// -------------------------
int main() {
    try {
        int portalChoice;

        do {
            cout << "\n=== Crime Investigator Project ===\n1. Admin Portal\n2. Viewer Portal\n3. Exit Program\nEnter your choice: ";
            if (!(cin >> portalChoice)) {
                cout << "Invalid input. Please enter a numeric choice.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore();

            if (portalChoice == 1) {
                Admin a;
                a.portal();
            }
            else if (portalChoice == 2) {
                Viewer v;
                v.portal();
            }
            else if (portalChoice == 3)
                cout << "Exiting Program...\n";
            else
                cout << "Invalid choice! Try again.\n";

        } while (portalChoice != 3);
    }
    catch (const exception& e) {
        cout << "\nA system error occurred: " << e.what() << "\n";
    }

    return 0;
}

