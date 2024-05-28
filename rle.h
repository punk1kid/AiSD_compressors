void rle_encode(const std::string& filename, const std::string& output_filename) {
    std::ifstream f(filename, std::ios::binary);
    std::ofstream out(output_filename, std::ios::binary);
    
    std::vector<char> encoded_data;
    std::vector<char> buff_data;
    char ch;

    while (f.get(ch)) {
        char count = 1;
        while (f.peek() == ch && count < 127) {
            f.get(ch);
            count++;
        }
        if (count > 1) {
            out.put(count);
            out.put(ch);
        }
        else {
            while (f.peek() != ch && buff_data.size() < 127 ) {
                buff_data.push_back(ch);
                f.get(ch);
                if (f.eof())
                    break;
                        
            }
            out.put(static_cast<char>(buff_data.size()) + 0x80);
            for (char c : buff_data) {
                out.put(c);
            }
            buff_data.clear();
            f.putback(ch);
            
        }
    }
}

void rle_decode(const std::string& filename, const std::string& output_filename) {
    std::ifstream f(filename, std::ios::binary);
    
    std::ofstream out(output_filename, std::ios::binary);
    
    char ch;

    while (f.get(ch)) {
        if (static_cast<unsigned char>(ch) < 0x80) {
            int count = static_cast<unsigned char>(ch);
            if (!f.get(ch)) {
                std::cerr << "EOF\n";
                return;
            }
            for (int i = 0; i < count; ++i) {
                out.put(ch);
            }
        }
        else {
            int length = static_cast<unsigned char>(ch) - 0x80;
            for (int i = 0; i < length; ++i) {
                if (!f.get(ch)) {
                    std::cerr << "EOF\n";
                    return;
                }
                out.put(ch);
            }
        }
    }
}