#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>
#include <iomanip>
using namespace std;

// Function to check if a patient number already exists in a file
bool isPatientNumberExists(const string& patientNumber, const string& filename) {
    ifstream file(filename);
    if (!file) {
        return false; // If file doesn't exist, the number doesn't exist
    }

    string line;
    while (getline(file, line)) {
        if (line.length() >= 3 && line.substr(0, 3) == patientNumber) {
            file.close();
            return true; // Patient number found
        }
    }
    file.close();
    return false; // Patient number not found
}

// Function to check if patient number exists in any of the system files
bool isPatientNumberExistsAnywhere(const string& patientNumber) {
    return isPatientNumberExists(patientNumber, "history.txt") ||
           isPatientNumberExists(patientNumber, "appointment.txt") ||
           isPatientNumberExists(patientNumber, "test.txt") ||
           isPatientNumberExists(patientNumber, "result.txt") ||
           isPatientNumberExists(patientNumber, "symptom_analysis.txt");
}

// Function to generate a unique patient number
string generateUniquePatientNumber() {
    srand(time(0)); // Initialize random seed with current time
    string patientNum;

    do {
        int num = rand() % 900 + 100; // Generate a number between 100 and 999
        patientNum = to_string(num);
    } while (isPatientNumberExistsAnywhere(patientNum));

    return patientNum;
}

class Patient {
protected:
    string patientNumber;
public:
    void inputPatientNumber() {
        do {
            cout << "Enter your patient number: ";
            cin >> patientNumber;
        } while (patientNumber.length() != 3);
    }

    string getPatientNumber() const {
        return patientNumber;
    }

    // Add setter for patient number
    void setPatientNumber(const string& number) {
        patientNumber = number;
    }
};

class MedicalHistory : public Patient {
public:
    void viewHistory() {
        ifstream inFile("history.txt");
        if (!inFile) {
            cerr << "Error opening file!" << endl;
            return;
        }

        string line;
        bool found = false;
        while (getline(inFile, line)) {
            if (line.length() >= 3 && line.substr(0, 3) == patientNumber) {
                cout << line << endl;
                found = true;
            }
        }
        if (!found) cout << "No Data Found" << endl;
        inFile.close();
    }

    // New function to save medical history automatically
    void saveMedicalHistory() {
        // Get current date and time
        time_t now = time(0);
        char* dt = ctime(&now);
        string datetime = dt;
        // Remove newline character from datetime
        datetime.erase(datetime.find_last_of('\n'));

        // Get medical information from the user
        string condition, notes;
        cin.ignore();
        cout << "Enter medical condition: ";
        getline(cin, condition);

        cout << "Enter any additional notes: ";
        getline(cin, notes);

        // Open history.txt file in append mode
        ofstream outFile("history.txt", ios::app);
        if (!outFile) {
            cerr << "Error opening history file!" << endl;
            return;
        }

        // Write patient information to file
        outFile << patientNumber << " " << datetime << " - " << condition << " - " << notes << endl;
        outFile.close();

        cout << "Medical history saved successfully!" << endl;
    }
};

class Appointment {
public:
    void bookAppointment() {
        string name, day, time;
        cin.ignore();
        cout << "Enter your name: "; getline(cin, name);
        cout << "Enter the day for your appointment: "; getline(cin, day);
        cout << "Enter the time for your appointment: "; getline(cin, time);

        // Generate a unique patient number
        string patientNum = generateUniquePatientNumber();

        ofstream outfile("appointment.txt", ios::app);
        if (!outfile) {
            cerr << "Error opening file!";
            return;
        }
        outfile << patientNum << " " << name << " " << day << " " << time << endl;
        cout << "\nAppointment booked. Your patient number is: " << patientNum << endl;
        outfile.close();
    }
};

class Test : public Patient {
public:
    void requestTest() {
        string name, testName;
        cin.ignore();
        cout << "Enter your name: "; getline(cin, name);
        char option;
        cout << "Do you want to see the test list (y/n)? "; cin >> option;
        cin.ignore();

        if (option == 'y' || option == 'Y') {
            ifstream infile("testlist.txt");
            if (!infile) {
                cerr << "Error opening file!";
                return;
            }
            string line;
            while (getline(infile, line)) {
                cout << line << endl;
            }
            infile.close();
        }

        cout << "\nEnter test name: "; getline(cin, testName);
        cout << "\nResult will be available in 2 days." << endl;

        ofstream testFile("test.txt", ios::app);
        if (!testFile) {
            cerr << "Error opening file!";
            return;
        }
        testFile << patientNumber << endl;
        testFile.close();

        ofstream resultFile("result.txt", ios::app);
        if (!resultFile) {
            cerr << "Error opening file!";
            return;
        }
        resultFile << patientNumber << " " << name << " " << testName << " Results are ready to be collected" << endl;
        resultFile.close();

        // Automatically save test request to medical history
        MedicalHistory mh;
        mh.setPatientNumber(patientNumber);

        // Open history.txt file in append mode
        ofstream historyFile("history.txt", ios::app);
        if (!historyFile) {
            cerr << "Error opening history file!" << endl;
            return;
        }

        // Get current date and time
        time_t now = time(0);
        char* dt = ctime(&now);
        string datetime = dt;
        // Remove newline character from datetime
        datetime.erase(datetime.find_last_of('\n'));

        // Write test request to history file
        historyFile << patientNumber << " " << datetime << " - Test requested: " << testName << endl;
        historyFile.close();
    }
};

class Report : public Patient {
public:
    void checkResult() {
        ifstream resultFile("result.txt");
        if (!resultFile) {
            cerr << "Error opening file!";
            return;
        }
        string line;
        bool found = false;
        while (getline(resultFile, line)) {
            if (line.length() >= 3 && line.substr(0, 3) == patientNumber) {
                cout << line << endl;
                found = true;
            }
        }
        if (!found) cout << "No test results found." << endl;
        resultFile.close();
    }
};

// New Symptom Analyzer class for online consultation
class SymptomAnalyzer : public Patient {
private:
    // Mapped symptoms with numeric keys for easy selection
    map<int, pair<string, double>> symptomSeverityMap = {
        // General Symptoms
        {1, {"fever", 0.7}},
        {2, {"fatigue", 0.6}},
        {3, {"weakness", 0.5}},
        {4, {"body_ache", 0.6}},
        {5, {"chills", 0.5}},

        // Respiratory Symptoms
        {6, {"cough", 0.6}},
        {7, {"shortness_breath", 0.8}},
        {8, {"chest_pain", 0.7}},
        {9, {"wheezing", 0.7}},
        {10, {"sore_throat", 0.4}},
        {11, {"runny_nose", 0.3}},
        {12, {"congestion", 0.5}},

        // Digestive Symptoms
        {13, {"nausea", 0.5}},
        {14, {"vomiting", 0.6}},
        {15, {"diarrhea", 0.6}},
        {16, {"abdominal_pain", 0.7}},
        {17, {"loss_appetite", 0.4}},

        // Neurological Symptoms
        {18, {"headache", 0.5}},
        {19, {"dizziness", 0.6}},
        {20, {"confusion", 0.7}},
        {21, {"seizures", 0.9}},

        // Cardiovascular Symptoms
        {22, {"chest_tightness", 0.7}},
        {23, {"irregular_heartbeat", 0.8}},
        {24, {"swelling", 0.6}},

        // Skin Symptoms
        {25, {"rash", 0.5}},
        {26, {"itching", 0.4}},
        {27, {"skin_discoloration", 0.6}},

        // Other Symptoms
        {28, {"joint_pain", 0.6}},
        {29, {"muscle_pain", 0.5}},
        {30, {"weight_loss", 0.7}},
        {31, {"night_sweats", 0.6}}
    };

    map<string, vector<int>> symptomClusterMap = {
        {"respiratory_infection", {1, 6, 7, 10, 11, 2, 4}},
        {"covid_like", {1, 6, 7, 2, 4, 17, 18}},
        {"pneumonia", {1, 5, 6, 7, 8, 9, 2}},
        {"gastroenteritis", {13, 14, 15, 16, 1, 3, 4}},
        {"cardiac_issue", {8, 22, 7, 23, 19, 24}},
        {"neurological_disorder", {18, 19, 20, 21, 3, 2}},
        {"autoimmune_condition", {2, 28, 29, 1, 25, 30, 31}}
    };

public:
    class SymptomAnalysisResult {
    public:
        double risk_score;
        vector<string> potential_conditions;
        vector<string> recommended_actions;
    };

    void displaySymptoms() {
        cout << "\n=== Symptom Selection ===\n";
        cout << "Enter the number(s) corresponding to your symptoms:\n\n";

        int columns = 3;
        int count = 0;

        for (const auto& symptom : symptomSeverityMap) {
            // Print symptom number and name, left-aligned
            cout << setw(3) << left << symptom.first << ". "
                      << setw(20) << left << symptom.second.first;

            count++;
            if (count % columns == 0) {
                cout << "\n";
            }
        }
        cout << "\n\nEnter symptom numbers (separated by spaces), or 0 to finish: ";
    }

    void getUserSymptoms() {
        vector<int> userSymptomNumbers;
        string input;
        vector<string> userSymptoms;
        string patientName;

        // Get patient's name to save with analysis results
        cin.ignore();
        cout << "Enter your name: ";
        getline(cin, patientName);

        displaySymptoms();

        // Read entire line of input
        getline(cin, input);
        istringstream iss(input);

        int symptomNum;
        while (iss >> symptomNum) {
            // Exit condition
            if (symptomNum == 0) break;

            // Validate symptom number
            if (symptomSeverityMap.find(symptomNum) != symptomSeverityMap.end()) {
                userSymptomNumbers.push_back(symptomNum);
                userSymptoms.push_back(symptomSeverityMap[symptomNum].first);
            } else {
                cout << "Invalid symptom number: " << symptomNum << endl;
            }
        }

        // Analyze symptoms if any were entered
        if (!userSymptoms.empty()) {
            SymptomAnalysisResult result = analyzeSymptoms(userSymptoms);
            displayResults(result);
            saveAnalysisResults(patientName, userSymptoms, result);

            // Also save to medical history
            saveSymptomsToMedicalHistory(patientName, userSymptoms, result);
        } else {
            cout << "No symptoms entered.\n";
        }
    }

    SymptomAnalysisResult analyzeSymptoms(const vector<string>& symptoms) {
        SymptomAnalysisResult result;
        result.risk_score = calculateRiskScore(symptoms);
        result.potential_conditions = identifyPotentialConditions(symptoms);
        result.recommended_actions = generateRecommendations(result.risk_score);

        return result;
    }

    double calculateRiskScore(const vector<string>& symptoms) {
        double totalRisk = 0.0;
        for (const auto& symptom : symptoms) {
            for (const auto& entry : symptomSeverityMap) {
                if (entry.second.first == symptom) {
                    totalRisk += entry.second.second;
                    break;
                }
            }
        }
        return min(totalRisk, 1.0);  // Cap at 1.0
    }

    vector<string> identifyPotentialConditions(const vector<string>& symptoms) {
        vector<string> potentialConditions;

        // Convert symptoms to their numeric codes
        vector<int> symptomCodes;
        for (const auto& symptom : symptoms) {
            for (const auto& entry : symptomSeverityMap) {
                if (entry.second.first == symptom) {
                    symptomCodes.push_back(entry.first);
                    break;
                }
            }
        }

        for (const auto& cluster : symptomClusterMap) {
            int matchCount = 0;
            for (int clusterSymptom : cluster.second) {
                if (find(symptomCodes.begin(), symptomCodes.end(), clusterSymptom) != symptomCodes.end()) {
                    matchCount++;
                }
            }

            // If more than 33% of the symptoms match a cluster
            if (matchCount > cluster.second.size() / 3) {
                potentialConditions.push_back(cluster.first);
            }
        }

        return potentialConditions.empty() ?
            vector<string>{"unclassified_condition"} :
            potentialConditions;
    }

    vector<string> generateRecommendations(double riskScore) {
        vector<string> recommendations;

        if (riskScore < 0.3) {
            recommendations.push_back("Monitor symptoms");
            recommendations.push_back("Rest and hydrate");
            recommendations.push_back("Over-the-counter medication if needed");
        } else if (riskScore < 0.6) {
            recommendations.push_back("Consult with general physician");
            recommendations.push_back("Get comprehensive health check");
            recommendations.push_back("Consider diagnostic tests");
        } else {
            recommendations.push_back("Urgent medical attention required");
            recommendations.push_back("Immediate hospital consultation recommended");
            recommendations.push_back("Potential emergency condition");
        }

        return recommendations;
    }

    void displayResults(const SymptomAnalysisResult& result) {
        cout << "\n=== Analysis Results ===\n";
        cout << "Risk Score: " << result.risk_score << endl;

        cout << "Potential Conditions:" << endl;
        for (const auto& condition : result.potential_conditions) {
            cout << "- " << condition << endl;
        }

        cout << "\nRecommended Actions:" << endl;
        for (const auto& action : result.recommended_actions) {
            cout << "- " << action << endl;
        }
    }

    void saveAnalysisResults(const string& patientName, const vector<string>& symptoms, const SymptomAnalysisResult& result) {
        // Save the analysis results to a file
        ofstream outFile("symptom_analysis.txt", ios::app);
        if (!outFile) {
            cerr << "Error opening symptom analysis file!" << endl;
            return;
        }

        // Record date and time of analysis
        time_t now = time(0);
        char* dt = ctime(&now);

        outFile << "Patient Number: " << patientNumber << endl;
        outFile << "Patient Name: " << patientName << endl;
        outFile << "Date/Time: " << dt;

        outFile << "Symptoms: ";
        for (const auto& s : symptoms) {
            outFile << s << ", ";
        }
        outFile << endl;

        outFile << "Risk Score: " << result.risk_score << endl;

        outFile << "Potential Conditions: ";
        for (const auto& c : result.potential_conditions) {
            outFile << c << ", ";
        }
        outFile << endl;

        outFile << "Recommended Actions: ";
        for (const auto& a : result.recommended_actions) {
            outFile << a << "; ";
        }
        outFile << "\n\n";

        outFile.close();

        cout << "\nAnalysis results have been saved to your medical record." << endl;
    }

    // New function to save symptom analysis to medical history
    void saveSymptomsToMedicalHistory(const string& patientName, const vector<string>& symptoms, const SymptomAnalysisResult& result) {
        ofstream historyFile("history.txt", ios::app);
        if (!historyFile) {
            cerr << "Error opening history file!" << endl;
            return;
        }

        // Get current date and time
        time_t now = time(0);
        char* dt = ctime(&now);
        string datetime = dt;
        // Remove newline character from datetime
        datetime.erase(datetime.find_last_of('\n'));

        // Prepare symptoms string
        string symptomsStr;
        for (size_t i = 0; i < symptoms.size(); ++i) {
            symptomsStr += symptoms[i];
            if (i < symptoms.size() - 1) {
                symptomsStr += ", ";
            }
        }

        // Prepare conditions string
        string conditionsStr;
        for (size_t i = 0; i < result.potential_conditions.size(); ++i) {
            conditionsStr += result.potential_conditions[i];
            if (i < result.potential_conditions.size() - 1) {
                conditionsStr += ", ";
            }
        }

        // Write to history file
        historyFile << patientNumber << " " << datetime << " - Online Consultation - "
                   << "Symptoms: " << symptomsStr << " - "
                   << "Potential conditions: " << conditionsStr << " - "
                   << "Risk score: " << result.risk_score << endl;

        historyFile.close();
    }

    void runSymptomAnalysis() {
        inputPatientNumber();
        getUserSymptoms();
    }
};

// Online Consultation module that uses Symptom Analyzer
class OnlineConsultation : public Patient {
public:
    void startConsultation() {
        cout << "\n=== Online Medical Consultation ===\n";
        cout << "Welcome to our AI-powered symptom analysis system.\n";
        cout << "This system will help analyze your symptoms and provide recommendations.\n\n";

        // Create a unique patient number for new patients
        string newPatientNumber = generateUniquePatientNumber();

        cout << "Do you have an existing patient number? (y/n): ";
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            inputPatientNumber();
        } else {
            patientNumber = newPatientNumber;
            cout << "Your new patient number is: " << patientNumber << endl;
        }

        SymptomAnalyzer analyzer;
        // Pass the patient number
        analyzer.setPatientNumber(patientNumber);

        // Run the symptom analysis
        analyzer.getUserSymptoms();

        // Suggest next steps
        cout << "\nBased on the analysis, you may want to:\n";
        cout << "1. Book an appointment with a specialist\n";
        cout << "2. Request specific medical tests\n";
        cout << "3. Contact emergency services if symptoms are severe\n";
    }
};

int main() {
    char option;

    cout << "\t\t\tWelcome to Hospital Management System" << endl;
    do {
        cout << "\nChoose your desired option" << endl;
        cout << "1) Medical history\n2) Book an appointment\n3) Request test\n";
        cout << "4) Check reports\n5) Online Consultation\n6) Add Medical History\n7) To Exit" << endl;

        cin >> option;

        switch (option) {
            case '1': {
                MedicalHistory mh;
                mh.inputPatientNumber();
                mh.viewHistory();
                break;
            }
            case '2': {
                Appointment app;
                app.bookAppointment();
                break;
            }
            case '3': {
                Test test;
                test.inputPatientNumber();
                test.requestTest();
                break;
            }
            case '4': {
                Report rep;
                rep.inputPatientNumber();
                rep.checkResult();
                break;
            }
            case '5': {
                OnlineConsultation consult;
                consult.startConsultation();
                break;
            }
            case '6': {
                MedicalHistory mh;
                mh.inputPatientNumber();
                mh.saveMedicalHistory();
                break;
            }
            case '7': {
                cout << "Thank you for using this program!" << endl;
                break;
            }
            default:
                cout << "Invalid option!" << endl;
        }
    } while (option != '7');
    return 0;
}