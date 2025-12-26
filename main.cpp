#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <cstdlib>
#include<stdexcept>
using namespace std;

#define FILE_NAME "crime_records.txt"
#define NUM_FIELDS 11

// --- Helper Functions ---

// Converts string to lowercase
string toLower(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
        {
            s[i] += 32;
        }
    }
    return s;
}

// Capitalizes the first letter of every word
string capitalize(string s)
{
    bool cap = true;
    for (int i = 0; i < s.length(); i++)
    {
        if (cap && isalpha(s[i]))
        {
            s[i] = toupper(s[i]);
            cap = false;
        }
        else if (s[i] == ' ')
        {
            cap = true;
        }
        else
        {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}

// Parses a semicolon-separated line into an array
void splitLine(string line, string fields[])
{
    size_t start = 0, end, i = 0;
    while ((end = line.find(';', start)) != string::npos && i < NUM_FIELDS) // hello;world;16
    {
        fields[i++] = line.substr(start, end - start);
        start = end + 1;
    }
    fields[i] = line.substr(start);
}

// Generates a new ID based on the last ID in the file
int generateID()
{
    ifstream file(FILE_NAME);
    string line, last;
    while (getline(file, line))
    {
        if (!line.empty())
        {
            last = line;
        }
    }

    if (last.empty())
    {
        return 1001;
    }
    
    // Extract ID from the last line (substring before the first ';')
    try 
    {
        // to convert the ID string to an integer
        return stoi(last.substr(0, last.find(';'))) + 1; 
    }
    catch (const exception& e) 
    {
        // If the file data is corrupted (not a number), this runs instead of crashing
        cout << "\n[Error] Last ID corrupted. Resetting sequence to 1001.\n";
        return 1001;
    }
}

// --- Crime Class ---
class Crime
{
public:
    int id = 0;
    string area, type, date, time, desc, status, handler;
    string weapons, suspects, victims;

    // --- Validations ---
    bool isDate(string s)
    {
        return s.length() == 10 && s[2] == '/' && s[5] == '/'; //12/12/2025
    }

    bool isTime(string s)
    {
        return s.length() == 5 && s[2] == ':'; //12:05
    }

    bool isArea(string a)
    {
        string s = toLower(a);
        return (s == "korangi" || s == "landhi" || s == "gulshan" || s == "north" || s == "nazimabad" ||
                s == "clifton" || s == "lyari" || s == "saddar" || s == "malir" || s == "orangi");
    }

    bool isType(string t)
    {
        string s = toLower(t);
        return (s == "robbery" || s == "snatching" || s == "murder" || s == "theft" || s == "assault");
    }

    // Handles input for lists (e.g., multiple suspects)
    string inputMulti(string name)
    {
        string res = "", temp;
        int count;
        cout << "Enter number of " << name << ": ";
        if (!(cin >> count))
        {
            cin.clear();
            cin.ignore(1000, '\n'); 
            return "";
        }
        cin.ignore();

        if (count <= 0)
        {
            return "";
        }

        for (int i = 0; i < count; i++)
        {
            cout << name << " " << i + 1 << ": ";
            getline(cin, temp);
            if (!temp.empty())
            {
                // Add comma if it's not the last item
                res += capitalize(temp) + (i < count - 1 ? "," : "");
            }
        }
        return res;
    }

    void input()
    {
        id = generateID();
        string val;
        int step = 0;

        while (step < 10)
        {
            if (step < 4 || step > 6)
            {
                if (step == 0)
                    cout << "Enter Area (Korangi, Landhi, Gulshan, North, Nazimabad, Clifton, Lyari, Saddar, Malir, Orangi)\n(back=previous, exit=cancel): ";
                else if (step == 1)
                    cout << "Enter Crime Type (Robbery, Snatching, Murder, Theft, Assault)\n(back=previous, exit=cancel): ";
                else if (step == 2)
                    cout << "Enter Date (DD/MM/YYYY)\n(back=previous, exit=cancel): ";
                else if (step == 3)
                    cout << "Enter Time (HH:MM)\n(back=previous, exit=cancel): ";
                else if (step == 7)
                    cout << "Enter Description\n(back=previous, exit=cancel): ";
                else if (step == 8)
                    cout << "Enter Status (Solved/Unsolved)\n(back=previous, exit=cancel): ";
                else if (step == 9)
                    cout << "Enter Handler\n(back=previous, exit=cancel): ";

                getline(cin, val);

                string low = toLower(val);
                if (low == "exit")
                {
                    id = 0;
                    return;
                }
                if (low == "back")
                {
                    if (step > 0)
                    {
                        step--;
                    }
                    else
                    {
                        cout << "At start.\n";
                    }
                    continue;
                }
            }

            switch (step)
            {
            case 0:
                if (isArea(val))
                {
                    area = capitalize(val);
                    step++;
                }
                else
                    cout << "Invalid Area.\n";
                break;
            case 1:
                if (isType(val))
                {
                    type = capitalize(val);
                    step++;
                }
                else
                    cout << "Invalid Type.\n";
                break;
            case 2:
                if (isDate(val))
                {
                    date = val;
                    step++;
                }
                else
                    cout << "Invalid Date.\n";
                break;
            case 3:
                if (isTime(val))
                {
                    time = val;
                    step++;
                }
                else
                    cout << "Invalid Time.\n";
                break;
            case 4:
                weapons = inputMulti("Weapons");
                if (weapons == "")
                {
                    id = 0;
                    return;
                }
                step++;
                break;
            case 5:
                suspects = inputMulti("Suspects");
                if (suspects == "")
                {
                    id = 0;
                    return;
                }
                step++;
                break;
            case 6:
                victims = inputMulti("Victims");
                if (victims == "")
                {
                    id = 0;
                    return;
                }
                step++;
                break;
            case 7:
                if (!val.empty())
                {
                    desc = val;
                    step++;
                }
                else
                    cout << "Required.\n";
                break;
            case 8:
                if (toLower(val) == "solved" || toLower(val) == "unsolved")
                {
                    status = capitalize(val);
                    step++;
                }
                else
                    cout << "Invalid.\n";
                break;
            case 9:
                if (!val.empty())
                {
                    handler = capitalize(val);
                    step++;
                }
                else
                    cout << "Required.\n";
                break;
            }
        }
    }

    void save()
    {
        ofstream f(FILE_NAME, ios::app);
        f << id << ";" << area << ";" << type << ";" << date << ";" << time << ";"
          << weapons << ";" << suspects << ";" << victims << ";" << desc << ";" << status << ";" << handler << "\n";
        f.close();
    }
};

// --- User & Derived Classes ---
class User
{
protected:
    string password;

public:
	User() {} 
	
	User(string p) 
	{ 
		password = p; 
	}

    bool login()
    {
        for (int i = 0; i < 3; i++)
{
    string in = "";
    char ch;
    cout << "\nPassword (" << 3 - i << " tries): ";

    while ((ch = _getch()) != 13) // 13 = Enter key
    {
        if (ch == 8) // 8 = Backspace key
        {
            if (!in.empty())
            {
                in.pop_back();        // remove last character from string
                cout << "\b \b";      // remove last * from console
            }
            continue; // go read next key
        }
        else
        {
            in += ch;  // store character
            cout << "*"; // print *
        }
    }

    if (in == password)
    {
        return true;
    }
    cout << "\nIncorrect!";
}
return false;

        return false;
    }

    void show(string f[], int count)
    {
        cout << "\n===================================";
        cout << "\n CRIME RECORD #" << count;
        cout << "\n===================================";
        cout << "\n ID          : " << f[0];
        cout << "\n Area        : " << f[1];
        cout << "\n Type        : " << f[2];
        cout << "\n Date        : " << f[3];
        cout << "\n Time        : " << f[4];
        cout << "\n Weapons     : " << f[5];
        cout << "\n Suspects    : " << f[6];
        cout << "\n Victims     : " << f[7];
        cout << "\n Description : " << f[8];
        cout << "\n Status      : " << f[9];
        cout << "\n Handler     : " << f[10];
        cout << "\n===================================\n";
    }

    void search(int fieldIdx, string query) 

    {
        ifstream f(FILE_NAME); // read file
        if (!f) // if not found
        {
            cout << "No records.\n";
            return;
        }

        string line;
        bool found = false;
        query = toLower(query); // convert query in lower case
        int count = 0;

        while (getline(f, line)) // read till file have lines
        {
            if (line.empty())
                continue;

            string fields[NUM_FIELDS];
            splitLine(line, fields);

            string target;

            if (fieldIdx == 3)
            {
                if (fields[fieldIdx].length() >= 10)
                    target = fields[fieldIdx].substr(6, 4); // extract year from date 01/01/2025
                else
                    target = "";
            }
            else // 0 or 1
            {
                target = toLower(fields[fieldIdx]);
            }
			
			// target can be year, id, or area based on the fieldIdx choice

            if (target == query) 
            {
                show(fields, ++count);
                found = true;
            }
        }
		

        if (!found)
            cout << "No matches found.\n";
        else
            cout << "\nTotal crimes found : " << count << "\n";

        f.close();
    }
};

class Admin : public User
{
public:
	Admin()
	{
		password = "admin123";
	}

    void menu()
    {
        if (!login())
            return;

        int ch;
        do
        {
            cout << "\n--- Admin Portal ---";
            cout << "\n1. Add Crime";
            cout << "\n2. Update Crime";
            cout << "\n3. Search Crime by Area";
            cout << "\n4. Search Crime by ID";
            cout << "\n5. Search Crime by Year";
            cout << "\n6. Exit";
            cout << "\nChoice: ";
            cin >> ch;
            cin.ignore();

            if (ch == 1)
            {
                Crime c;
                c.input();
                if (c.id != 0)
                {
                    c.save();
                    cout << "Added ID: " << c.id;
                }
            }
            if (ch == 2)
                update();
            if (ch == 3)
            {
                string s;
                cout << "Enter area to search: ";
                getline(cin, s);
                search(1, s);
            }
            if (ch == 4)
            {
                string s;
                cout << "Enter ID to search: ";
                getline(cin, s);
                search(0, s);
            }
            if (ch == 5) 
            {
                string s;
                cout << "Enter year to search: ";
                getline(cin, s);
                search(3, s);
            }
        } while (ch != 6);
    }

    void update()
    {
        string id, line, allData[100];
        cout << "Enter the Crime ID to update: ";
        getline(cin, id);

        ifstream in(FILE_NAME);
        int n = 0, target = -1;

        try
        {
            while (getline(in, line))
            {
                if (!line.empty())
                {
                    if (n >= 100)
                        throw runtime_error("Limit reached.");
                    if (line.substr(0, line.find(';')) == id)
                        target = n;
                    allData[n++] = line; // all lines of the files are loaded - id;area;
                }
            }
        }
        catch (...)
        {
            cout << "Error.\n";
            return;
        }
        in.close();

        if (target == -1)
        {
            cout << "Not found.\n";
            return;
        }

        string f[NUM_FIELDS], input;
        splitLine(allData[target], f);
        Crime c;
        string headers[] = {"ID", "Area", "Type", "Date", "Time", "Weapons", "Suspects", "Victims", "Description", "Status", "Handler"};

            //cout << "Enter new Area (current: " << f[i] << ", press Enter to keep): ";
            //getline(cin, input);			
		
            //cout << "Enter new Type (current: " << f[i] << ", press Enter to keep): ";
            //getline(cin, input);

        for (int i = 1; i < NUM_FIELDS; i++)
        {
            cout << "Enter new " << headers[i] << " (current: " << f[i] << ", press Enter to keep): ";
            getline(cin, input);

            if (input.empty())
            {
                continue;
            }

            if (i == 1)
            {
                if (c.isArea(input)) f[i] = capitalize(input);
                else cout << "Invalid Area, keeping old value.\n";
            }
            else if (i == 2)
            {
                if (c.isType(input)) f[i] = capitalize(input);
                else cout << "Invalid Type, keeping old value.\n";
            }
            else if (i == 3)
            {
                if (c.isDate(input)) f[i] = input;
                else cout << "Invalid Date, keeping old value.\n";
            }
            else if (i == 4)
            {
                if (c.isTime(input)) f[i] = input;
                else cout << "Invalid Time, keeping old value.\n";
            }
            else if (i == 9)
            {
                if (toLower(input) == "solved" || toLower(input) == "unsolved") f[i] = capitalize(input);
                else cout << "Invalid Status, keeping old value.\n";
            }
            else
            {
                if (i == 10) f[i] = capitalize(input);
                else f[i] = input;
            }
        }

        ofstream out(FILE_NAME);
        for (int i = 0; i < n; i++)
        {
            if (i == target)
            {
                for (int j = 0; j < NUM_FIELDS; j++)
                {
                    out << f[j] << (j < 10 ? ";" : "");
                }
                out << endl;
            }
            else
            {
                out << allData[i] << endl;
            }
        }
        cout << "Update Successful.\n";
    }
};

class Viewer : public User
{
public:
    Viewer() : User("123") {}

    void menu()
    {
        if (!login())
            return;

        int ch;
        do
        {
            cout << "\n--- Viewer Portal ---";
            cout << "\n1. Search Crime by Area";
            cout << "\n2. View Statistics";
            cout << "\n3. Search Crime by Type";
            cout << "\n4. Search Crime by ID";
            cout << "\n5. Search Crime by Year";
            cout << "\n6. Exit";
            cout << "\nChoice: ";
            cin >> ch;
            cin.ignore();

            if (ch == 1)
            {
                string s;
                cout << "Enter area to search: ";
                getline(cin, s);
                search(1, s);
            }
            if (ch == 2)
                stats();
            if (ch == 3)
            {
                string s;
                cout << "Enter type to search: ";
                getline(cin, s);
                search(2, s);
            }
            if (ch == 4)
            {
                string s;
                cout << "Enter ID to search: ";
                getline(cin, s);
                search(0, s);
            }
            if (ch == 5)
            {
                string s;
                cout << "Enter year to search: ";
                getline(cin, s);
                search(3, s);
            }
        } while (ch != 6);
    }

    void stats()
    {
        ifstream f(FILE_NAME);
        if (!f)
            return;

        string line, fields[NUM_FIELDS];
        string areas[] = {"Korangi", "Landhi", "Gulshan", "North", "Lyari", "Clifton", "Malir", "Saddar", "Nazimabad", "Orangi"};
        int aCount[10] = {0}, tCount[5] = {0}, total = 0;
		// Area Count = aCount[10]
		// Type Count = tCount[5]

        while (getline(f, line))
        {
            if (line.empty())
                continue;
            total++;
            splitLine(line, fields);

            for (int i = 0; i < 10; i++)
            {
                if (toLower(fields[1]) == toLower(areas[i]))
                    aCount[i]++;
            }

            string t = toLower(fields[2]);
            if (t == "robbery")
                tCount[0]++;
            else if (t == "snatching")
                tCount[1]++;
            else if (t == "murder")
                tCount[2]++;
            else if (t == "theft")
                tCount[3]++;
            else if (t == "assault")
                tCount[4]++;
        }

        // --- Simplified Table Formatting (Using Tabs) ---
        cout << "\n**";
        cout << "\n       CITY CRIME STATISTICS REPORT";
        cout << "\n\nTotal Crimes Recorded: " << total << "\n";

        cout << "\nArea-wise Crime Distribution:\n";
        cout << "------------------------------------------\n";
        cout << "Area\t\t| Crimes | % of Total\n";
        cout << "------------------------------------------\n";

        for (int i = 0; i < 10; i++)
        {
            int pct = total ? (aCount[i] * 100 / total) : 0;
            // Short names need 2 tabs, long names need 1
            if (areas[i].length() < 8)
                cout << areas[i] << "\t\t| " << aCount[i] << "\t | " << pct << "%\n";
            else
                cout << areas[i] << "\t| " << aCount[i] << "\t | " << pct << "%\n";
        }

        cout << "\nCrime Type Summary:\n";
        cout << "-----------------------------------\n";
        cout << "Robbery   : " << tCount[0] << endl;
        cout << "Snatching : " << tCount[1] << endl;
        cout << "Murder    : " << tCount[2] << endl;
        cout << "Theft     : " << tCount[3] << endl;
        cout << "Assault   : " << tCount[4] << endl;
        cout << "-----------------------------------\n";
        cout << "***** END OF REPORT *****\n";
    }
};

int main()
{
    int ch;
    do
    {
        cout << "\n--- Crime Investigator Project ---";
        cout << "\n1. Admin Portal";
        cout << "\n2. Viewer Portal";
        cout << "\n3. Exit";
        cout << "\nChoice: ";
        cin >> ch;

        if (ch == 1)
        {
            Admin a;
            a.menu();
        }
        if (ch == 2)
        {
            Viewer v;
            v.menu();
        }
    } while (ch != 3);

    return 0;
}
