std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return text;
}

// Функция для записи текста в файл
void write_file(const std::string& filename, const std::string& text) {
    std::ofstream file(filename);
    
    file << text;
}


std::string bwt_encode(const std::string& text) {
    std::vector<std::string> rotations;
    std::string transformed;
    
    // Создаем все возможные циклические сдвиги и добавляем их в вектор
    std::string temp = text;
    //temp.push_back('$');
    for (size_t i = 0; i < text.length(); ++i) {
        rotations.push_back(temp);
        std::rotate(temp.begin(), temp.begin() + 1, temp.end());
    }

    // Сортируем вектор циклических сдвигов
    std::sort(rotations.begin(), rotations.end());

    // Формируем преобразованную строку, добавляя в transformed последние символы из каждого циклического сдвига
    for (const std::string& rotation : rotations) {
        transformed.push_back(rotation.back());
    }

    return transformed;
}

std::string bwt_decode(const std::string& transformed) {
    size_t length = transformed.length();

    // Создаем вектор для хранения всех возможных циклических сдвигов преобразованной строки
    std::vector<std::string> rotations(length);

    // Заполняем вектор циклическими сдвигами преобразованной строки
    for (size_t i = 0; i < length; ++i) {
        for (size_t j = 0; j < length; ++j) {
            rotations[j] = transformed[j] + rotations[j];
        }
        std::sort(rotations.begin(), rotations.end());
    }

    // Ищем строку, которая заканчивается на символ '$'
    std::string original;
    for (const std::string& rotation : rotations) {
        if (rotation[length - 1] == '$') {
            original = rotation;
            break;
        }
    }

    // Удаляем символ '$' и возвращаем исходную строку
    //original.pop_back(); // Удаляем '$'
    return original;
}
