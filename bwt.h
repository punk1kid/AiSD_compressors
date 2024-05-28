std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return text;
}

// ������� ��� ������ ������ � ����
void write_file(const std::string& filename, const std::string& text) {
    std::ofstream file(filename);
    
    file << text;
}


std::string bwt_encode(const std::string& text) {
    std::vector<std::string> rotations;
    std::string transformed;
    
    // ������� ��� ��������� ����������� ������ � ��������� �� � ������
    std::string temp = text;
    //temp.push_back('$');
    for (size_t i = 0; i < text.length(); ++i) {
        rotations.push_back(temp);
        std::rotate(temp.begin(), temp.begin() + 1, temp.end());
    }

    // ��������� ������ ����������� �������
    std::sort(rotations.begin(), rotations.end());

    // ��������� ��������������� ������, �������� � transformed ��������� ������� �� ������� ������������ ������
    for (const std::string& rotation : rotations) {
        transformed.push_back(rotation.back());
    }

    return transformed;
}

std::string bwt_decode(const std::string& transformed) {
    size_t length = transformed.length();

    // ������� ������ ��� �������� ���� ��������� ����������� ������� ��������������� ������
    std::vector<std::string> rotations(length);

    // ��������� ������ ������������ �������� ��������������� ������
    for (size_t i = 0; i < length; ++i) {
        for (size_t j = 0; j < length; ++j) {
            rotations[j] = transformed[j] + rotations[j];
        }
        std::sort(rotations.begin(), rotations.end());
    }

    // ���� ������, ������� ������������� �� ������ '$'
    std::string original;
    for (const std::string& rotation : rotations) {
        if (rotation[length - 1] == '$') {
            original = rotation;
            break;
        }
    }

    // ������� ������ '$' � ���������� �������� ������
    //original.pop_back(); // ������� '$'
    return original;
}
