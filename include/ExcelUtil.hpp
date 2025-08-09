// Add report title
        ws.cell("A1").value("GRADE REPORT - " + getCurrentTimestamp());
        ws.merge_cells("A1:H1");
        
        // Add summary statistics
        int totalStudents = students.size();
        int passingStudents = 0;
        double totalAverage = 0.0;
        
        for (const auto& student : students) {
            if (student.hasPassingGrade()) {
                passingStudents++;
            }
            totalAverage += student.getAverageScore();
        }
        
        double classAverage = totalStudents > 0 ? totalAverage / totalStudents : 0.0;
        double passRate = totalStudents > 0 ? (static_cast<double>(passingStudents) / totalStudents) * 100.0 : 0.0;
        
        ws.cell("A3").value("Total Students: " + to_string(totalStudents));
        ws.cell("A4").value("Passing Students: " + to_string(passingStudents));
        ws.cell("A5").value("Pass Rate: " + to_string(static_cast<int>(passRate * 100) / 100.0) + "%");
        ws.cell("A6").value("Class Average: " + to_string(static_cast<int>(classAverage * 100) / 100.0));

        // Write headers starting from row 8
        auto headers = getExcelHeaders();
        for (size_t i = 0; i < headers.size(); ++i) {
            ws.cell(xlnt::cell_reference(i + 1, 8)).value(headers[i]);
        }

        // Write student data
        for (size_t i = 0; i < students.size(); ++i) {
            writeStudentToExcel(ws, students[i], i + 9);
        }

        wb.save(filename);
        cout << "Grade report exported to: " << filename << endl;
    }
    catch (const exception& e) {
        cerr << "Error creating grade report: " << e.what() << endl;
    }
}

// Import operations
bool ExcelUtils::importStudentData(const std::string& filename, std::vector<Student>& students) {
    try {
        if (!fileExists(filename)) {
            cerr << "File '" << filename << "' does not exist!" << endl;
            return false;
        }

        auto importedStudents = readExcelToVector(filename);
        if (importedStudents.empty()) {
            cerr << "No valid student data found in the file." << endl;
            return false;
        }

        // Add imported students to existing vector
        students.insert(students.end(), importedStudents.begin(), importedStudents.end());
        
        cout << "Successfully imported " << importedStudents.size() << " students." << endl;
        return true;
    }
    catch (const exception& e) {
        cerr << "Error importing student data: " << e.what() << endl;
        return false;
    }
}

bool ExcelUtils::validateExcelFormat(const std::string& filename) {
    try {
        if (!fileExists(filename)) {
            return false;
        }

        xlnt::workbook wb;
        wb.load(filename);
        xlnt::worksheet ws = wb.active_sheet();

        // Check if file has expected headers
        auto expectedHeaders = getExcelHeaders();
        for (size_t i = 0; i < expectedHeaders.size(); ++i) {
            try {
                string cellValue = ws.cell(xlnt::cell_reference(i + 1, 1)).to_string();
                if (cellValue != expectedHeaders[i]) {
                    return false;
                }
            }
            catch (...) {
                return false;
            }
        }

        return true;
    }
    catch (...) {
        return false;
    }
}

// Utility methods
std::string ExcelUtils::generateTimestampFilename(const std::string& baseFilename) {
    string timestamp = getCurrentTimestamp();
    
    // Replace spaces and colons with underscores for filename safety
    for (char& c : timestamp) {
        if (c == ' ' || c == ':') {
            c = '_';
        }
    }
    
    size_t dotPos = baseFilename.find_last_of('.');
    if (dotPos != string::npos) {
        return baseFilename.substr(0, dotPos) + "_" + timestamp + baseFilename.substr(dotPos);
    }
    return baseFilename + "_" + timestamp;
}