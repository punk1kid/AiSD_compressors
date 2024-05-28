std::string readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

// Функция для записи содержимого в файл
void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename, std::ios::binary);

    file.write(content.c_str(), content.size());
    file.close();
}

// Функция для кодирования MTF
std::string encodeMTF(const std::string& input) {
    std::list<char> alphabet;
    for (int i = 0; i < 256; ++i) {
        alphabet.push_back(static_cast<char>(i));
    }

    std::string output;
    for (char c : input) {
        auto it = std::find(alphabet.begin(), alphabet.end(), c);
        int index = std::distance(alphabet.begin(), it);
        output.push_back(static_cast<char>(index));
        alphabet.erase(it);
        alphabet.push_front(c);
    }
    return output;
}

// Функция для декодирования MTF
std::string decodeMTF(const std::string& input) {
    std::list<char> alphabet;
    for (int i = 0; i < 256; ++i) {
        alphabet.push_back(static_cast<char>(i));
    }

    std::string output;
    for (char c : input) {
        int index = static_cast<unsigned char>(c);
        auto it = alphabet.begin();
        std::advance(it, index);
        char decodedChar = *it;
        output.push_back(decodedChar);
        alphabet.erase(it);
        alphabet.push_front(decodedChar);
    }
    return output;
}