# Hospital-management-system-with-consultation
This is a console-based Hospital Management System developed in C++, implementing key features of patient management, appointment scheduling, test handling, symptom analysis, and medical history tracking. The project follows Object-Oriented Programming (OOP) principles with a focus on inheritance, encapsulation, and file handling.

🚀 Features
🔢 Unique Patient Identification

Automatically generates unique 3-digit patient numbers.

Prevents duplication by checking across all records.

📅 Appointment Booking

Allows patients to schedule appointments by name, day, and time.

Stores data persistently in appointment.txt.

📋 Medical History Management

View and maintain patient medical histories.

Appends consultation outcomes and test results to history.

🧪 Medical Test Requests

Lists available tests (read from test_list.txt).

Records test selections and results to test.txt and result.txt.

🤒 Symptom Analyzer & Risk Assessment

Patients input symptoms based on mapped values.

Analyzes symptoms using a risk score system.

Suggests possible conditions and recommends actions.

📞 Online Consultation

End-to-end consultation experience.

Includes symptom analysis and next-step recommendations.

Stores analysis to both detailed and summary records.

🧰 Technologies Used
C++ (OOP & File Handling)

Plain Text Files for Storage (.txt files for appointments, tests, history, etc.)

🏗️ Object-Oriented Structure
Base Class:

Patient: Stores and manages patient number.

Derived Classes:

MedicalHistory: Manages historical data.

Test: Handles test selection and storage.

Report: Reads and displays test results.

SymptomAnalyzer: Analyzes symptoms and recommends actions.

📂 File Structure
appointment.txt – Stores appointment data.

history.txt – Stores summary of patient histories.

symptom_analysis.txt – Detailed consultation outcomes.

test.txt – Test requests per patient.

result.txt – Corresponding test results.

test_list.txt – List of available tests.

📌 How to Use
Compile the code using a C++ compiler (e.g., g++).

Run the executable to access the menu and start managing patient data.

Follow on-screen instructions to book appointments, consult online, request tests, and more.
