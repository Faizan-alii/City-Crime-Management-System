#include <iostream>
#include <string>
#include <conio.h> // for _getch() to mask password

using namespace std;

// Function to input password with '*' masking
string inputPassword() {
    string password = "";
    char ch;

    while (true) {
        ch = _getch();

        if (ch == 13) {  // Enter key
            cout << endl;
            break;
        }
        else if (ch == 8) {  // Backspace
            if (password.length() > 0) {
                cout << "\b \b";
                password.resize(password.length() - 1);
            }
        }
        else {
            password += ch;
            cout << "*";
        }
    }

    return password;  // FIXED
}


// Admin Portal
void adminPortal() {
    const string adminPass = "admin123"; // Hardcoded admin password
    cout << "Enter Admin Password: ";
    string pass = inputPassword();

    if (pass != adminPass) {
        cout << "Incorrect Password! Access Denied.\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Admin Portal ---\n";
        cout << "1. Add New Crime\n";
        cout << "2. Update Crime\n";
        cout << "3. Search Crime by Area\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Add New Crime Function (To be implemented in Step 2)\n";
            break;
        case 2:
            cout << "Update Crime Function (To be implemented in Step 3)\n";
            break;
        case 3:
            cout << "Search Crime by Area Function (To be implemented in Step 4)\n";
            break;
        case 4:
            cout << "Exiting Admin Portal...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);
}

// Viewer Portal
void viewerPortal() {
    const string viewerPass = "123"; // Hardcoded viewer password
    cout << "Enter Viewer Password: ";
    string pass = inputPassword();

    if (pass != viewerPass) {
        cout << "Incorrect Password! Access Denied.\n";
        return;
    }

    int choice;
    do {
        cout << "\n--- Viewer Portal ---\n";
        cout << "1. View Crimes by Area\n";
        cout << "2. View Crime Statistics for Whole City\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "View Crimes by Area Function (To be implemented in Step 2)\n";
            break;
        case 2:
            cout << "View Crime Statistics for Whole City Function (To be implemented in Step 2/3)\n";
            break;
        case 3:
            cout << "Exiting Viewer Portal...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 3);
}

int main() {
    int portalChoice;
    do {
        cout << "\n=== Crime Investigator Project ===\n";
        cout << "1. Admin Portal\n";
        cout << "2. Viewer Portal\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> portalChoice;

        switch (portalChoice) {
        case 1:
            adminPortal();
            break;
        case 2:
            viewerPortal();
            break;
        case 3:
            cout << "Exiting Program...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (portalChoice != 3);

    return 0;
}
