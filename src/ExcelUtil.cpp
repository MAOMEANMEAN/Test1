std::string ExcelUtils::getCurrentTimestamp() {
    auto now = time(0);
    auto tm = *localtime(&now);
    
    ostringstream oss;
    oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool ExcelUtils::fileExists(const std::string& filename) {
    ifstream file(filename);
    return file.good();
}

std::vector<std::string> ExcelUtils::getExcelHeaders() {
    vector<string> headers = {
        "Student ID", "Name", "Age", "Gender", "Date of Birth", "Email"
    };
    
    // Add subject headers
    auto subjects = GradeUtil::getSubjectNames();
    headers.insert(headers.end(), subjects.begin(), subjects.end());
    
    // Add calculated fields
    headers.insert(headers.end(), {
        "Average Score", "Letter Grade", "GPA", "Remark", "Last Updated"
    });
    
    return headers;
}

// Helper methods for Excel formatting
void ExcelUtils::formatExcelHeader(xlnt::worksheet& ws) {
    auto headers = getExcelHeaders();
    for (size_t i = 0; i < headers.size(); ++i) {
        auto cell = ws.cell(xlnt::cell_reference(i + 1, 1));
        cell.font(xlnt::font().bold(true));
    }
}

void ExcelUtils::writeStudentToExcel(xlnt::worksheet& ws, const Student& student, int row) {
    int col = 1;
    
    // Basic information
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getStudentId());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getName());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getAge());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getGender());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getDateOfBirth());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getEmail());
    
    // Subject scores
    auto scores = student.getSubjectScores();
    for (const auto& score : scores) {
        ws.cell(xlnt::cell_reference(col++, row)).value(score);
    }
    
    // Calculated fields
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getAverageScore());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getLetterGrade());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getGpa());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getRemark());
    ws.cell(xlnt::cell_reference(col++, row)).value(student.getFormattedTimestamp());
}

Student ExcelUtils::readStudentFromExcel(xlnt::worksheet& ws, int row) {
    int col = 1;
    
    try {
        // Read basic information
        string studentId = ws.cell(xlnt::cell_reference(col++, row)).to_string();
        string name = ws.cell(xlnt::cell_reference(col++, row)).to_string();
        int age = ws.cell(xlnt::cell_reference(col++, row)).value<int>();
        string gender = ws.cell(xlnt::cell_reference(col++, row)).to_string();
        string dateOfBirth = ws.cell(xlnt::cell_reference(col++, row)).to_string();
        string email = ws.cell(xlnt::cell_reference(col++, row)).to_string();
        
        // Read subject scores
        vector<double> scores;
        auto subjects = GradeUtil::getSubjectNames();
        for (size_t i = 0; i < subjects.size(); ++i) {
            double score = ws.cell(xlnt::cell_reference(col++, row)).value<double>();
            scores.push_back(score);
        }
        
        // Create student object
        Student student(studentId, name, age, gender, dateOfBirth, email, scores);
        return student;
    }
    catch (const exception& e) {
        throw runtime_error("Error reading student data from row " + to_string(row) + ": " + e.what());
    }
}