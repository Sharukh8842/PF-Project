#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <conio.h>
#include <limits>
#include <sstream> // Required for stringstream
using namespace std;

// Structure to hold student data
struct Marks {
    float oht1;
    float oht2;
    float quizzes[4];
    float assignments[3];
    float project;
    float finalScore;
    float total;
    char grade;
};

struct Student {
    char studentID[20];
    char email[50];
    char password[20];
    Marks subjects[7]; // Marks for 7 courses
    char attendance[7][10]; // Attendance for 7 courses
};

// Function prototypes
bool loginUser(Student& loggedInStudent);
void displayMainMenu(Student& loggedInStudent);
void displaySubjectMenu(Student& loggedInStudent, int subjectIndex);
void calculateTotalAndGrade(Marks& marks);
void loadMarks(Student& student);

// File paths
const char* credentialsFile = "registered_students.txt";
const char* marksFile = "marks.txt";
const char* subjects[7] = {
    "Programming Fundamentals",
    "PF Lab",
    "ICT",
    "ICT Lab",
    "Discrete Structures",
    "Functional English",
    "Calculus"
};

int main() {
    Student loggedInStudent;
    cout<<"\t\t************************************************************\n";
    cout<<"\t\t**                                                        **\n";
    cout<<"\t\t**                                                        **\n";
    cout<<"\t\t**           Welcome to the IST Student Portal            **\n";
    cout<<"\t\t**                                                        **\n";
    cout<<"\t\t**                                                        **\n";
    cout<<"\t\t************************************************************\n\n\n";

    if (loginUser(loggedInStudent)) {
        cout << "Login successful for student ID: " << loggedInStudent.studentID << "\n";
        loadMarks(loggedInStudent);
        displayMainMenu(loggedInStudent);
    } else {
        cout << "Invalid login credentials. Please try again.\n";
    }

    return 0;
}

// Function to login user and get student ID by email
bool loginUser(Student& loggedInStudent) {
    char email[50], password[20], fileEmail[50], filePassword[50];

    cout << "Enter your email: ";
    cin >> email;
    cin.ignore();
    cout << "Enter your password: ";

    int i = 0;
    char ch;
    while (true) {
        ch = getch();
        if (ch == '\r') break;
        else if (ch == '\b' && i > 0) {
            cout << "\b \b";
            i--;
        } else if (ch != '\b' && i < sizeof(password) - 1) {
            cout << '*';
            password[i++] = ch;
        }
    }
    password[i] = '\0';
    cout << endl;

    ifstream inFile(credentialsFile);
    if (!inFile) {
        cout << "Error: Could not open credentials file.\n";
        return false;
    }

    bool found = false;

    // Loop through registered students to find matching email and password
    while (inFile >> fileEmail >> filePassword) {
        if (strcmp(email, fileEmail) == 0 && strcmp(password, filePassword) == 0) {
            // Extract StudentID from email (strip @domain)
            strncpy(loggedInStudent.studentID, email, strchr(email, '@') - email);
            loggedInStudent.studentID[strchr(email, '@') - email] = '\0';
            strcpy(loggedInStudent.email, fileEmail);
            strcpy(loggedInStudent.password, filePassword);
            found = true;
            cout << "Login successful for student ID: " << loggedInStudent.studentID << endl;
            break;
        }
    }

    if (!found) {
        cout << "Invalid email or password.\n";
    }

    inFile.close();
    return found;
}

void loadMarks(Student& student) {
     ifstream inFile(marksFile);
    if (!inFile) {
        cout << "Error: Could not open marks file." << endl;
        return;
    }

    string line; // Read each line into a string
    bool marksLoaded = false;


    while (getline(inFile, line)) {

        stringstream ss(line); // Create stringstream from line
        char fileStudentID[20];
        int subjectIndex;
        Marks marks;
        if (ss >> fileStudentID >> subjectIndex >> marks.oht1 >> marks.oht2 )
            {

           for(int i=0; i < 4 ; i++){
               ss >> marks.quizzes[i];
           }

            for (int i=0; i<3 ; i++){
                ss >> marks.assignments[i];
            }
        ss >> marks.project >> marks.finalScore;


    // Debug: Print each line being read
     //   cout << "Reading: " << fileStudentID << " SubjectIndex: " << subjectIndex << endl;


        // Check if the StudentID matches
        if (strcmp(fileStudentID, student.studentID) == 0) {
        //    cout << "Student ID match : " << student.studentID << endl;
            // Check if SubjectIndex is valid
            if (subjectIndex >= 1 && subjectIndex <= 7) {
                // Convert to 0-based index
                int index = subjectIndex - 1;
                student.subjects[index] = marks;
                marksLoaded = true;
              // cout << "Marks loaded for SubjectIndex " << subjectIndex << endl;


            } else {
              //  cout << "Invalid SubjectIndex " << subjectIndex << " for StudentID " << fileStudentID << endl;

            }

        } else{

        }
    } else {
      //   cout << "Error reading data on line: " << line << endl;
    }


}



    if (!marksLoaded) {
        cout << "No marks found for student ID " << student.studentID << ".\n";
    }

    inFile.close();
}

void displayMainMenu(Student& loggedInStudent) {
    int choice;
    do {
        cout << "\nStudent Portal Menu:\n";
        for (int i = 0; i < 7; ++i) {
            cout << i + 1 << ". " << subjects[i] << "\n";
        }
        cout << "8. Logout\nChoose a subject to view details: ";
        cin >> choice;
        cin.ignore();

        if (choice >= 1 && choice <= 7) {
            displaySubjectMenu(loggedInStudent, choice - 1);
        } else if (choice == 8) {
            cout << "Logging out...\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 8);
}

void displaySubjectMenu(Student& loggedInStudent, int subjectIndex) {
    Marks& marks = loggedInStudent.subjects[subjectIndex];
    calculateTotalAndGrade(marks);

    cout << "\n" << subjects[subjectIndex] << " Details:\n";
    cout << "OHT1: " << marks.oht1 << "\n";
    cout << "OHT2: " << marks.oht2 << "\n";
    for (int i = 0; i < 4; ++i) {
        cout << "Quiz " << i + 1 << ": " << marks.quizzes[i] << "\n";
    }
    for (int i = 0; i < 3; ++i) {
        cout << "Assignment " << i + 1 << ": " << marks.assignments[i] << "\n";
    }
    cout << "Project: " << marks.project << "\n";
    cout << "Final Score: " << marks.finalScore << "\n";
    cout << "Total Marks: " << marks.total << "\n";
    cout << "Grade: " << marks.grade << "\n";
}


void calculateTotalAndGrade(Marks& marks) {
    const float FINAL_SCORE_WEIGHTAGE = 0.4;
    const float PROJECT_WEIGHTAGE = 0.1;
    const float QUIZ_WEIGHTAGE = 0.1;
    const float OHT_WEIGHTAGE = 0.3;
    const float ASSIGNMENT_WEIGHTAGE = 0.1;

    float quizAverage = (marks.quizzes[0] + marks.quizzes[1] + marks.quizzes[2] + marks.quizzes[3]);
    float assignmentAverage = (marks.assignments[0] + marks.assignments[1] + marks.assignments[2]);

    marks.total = marks.finalScore * FINAL_SCORE_WEIGHTAGE +
                  marks.project * PROJECT_WEIGHTAGE +
                  quizAverage * QUIZ_WEIGHTAGE / 4 +
                  (marks.oht1 + marks.oht2) * OHT_WEIGHTAGE / 2 +
                  assignmentAverage * ASSIGNMENT_WEIGHTAGE/3;

     if (marks.total >= 90) marks.grade = 'A';
    else if (marks.total >= 80) marks.grade = 'B';
    else if (marks.total >= 70) marks.grade = 'C';
    else if (marks.total >= 60) marks.grade = 'D';
    else marks.grade = 'F';
}