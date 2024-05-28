class LZ77 {
public:
    struct Match {
        uint8_t offset;
        uint8_t length;
        char nextChar;
    };

    static std::vector<Match> encode(const std::string& input, int windowSize, int lookAheadBufferSize) {
        std::vector<Match> encodedData;
        int inputSize = input.size();

        for (int i = 0; i < inputSize;) {
            uint8_t matchLength = 0;
            uint8_t matchOffset = 0;
            char nextChar = '\0';

            int searchBufferStart = std::max(0, i - windowSize);
            int lookAheadBufferEnd = std::min(inputSize, i + lookAheadBufferSize);

            for (int j = searchBufferStart; j < i; ++j) {
                int k = j;
                int l = i;
                while (l < lookAheadBufferEnd && input[k] == input[l]) {
                    ++k;
                    ++l;
                }

                int currentMatchLength = l - i;
                if (currentMatchLength > matchLength) {
                    matchLength = currentMatchLength;
                    matchOffset = i - j;
                    if (l < inputSize) {
                        nextChar = input[l];
                    }
                    else {
                        nextChar = '\0';
                    }
                }
            }

            if (matchLength == 0) {
                matchOffset = 0;
                matchLength = 0;
                nextChar = input[i];
            }

            encodedData.push_back({ matchOffset, matchLength, nextChar });
            i += (matchLength == 0) ? 1 : matchLength + 1;
        }

        return encodedData;
    }

    static std::string decode(const std::vector<Match>& encodedData) {
        std::string decodedData;

        for (const auto& match : encodedData) {
            int start = decodedData.size() - match.offset;
            for (int i = 0; i < match.length; ++i) {
                decodedData += decodedData[start + i];
            }
            decodedData += match.nextChar;
        }

        return decodedData;
    }
};

void encodeFile(const std::string& inputFileName, const std::string& encodedFileName, int windowSize, int lookAheadBufferSize) {
    std::ifstream inputFile(inputFileName);
    

    std::string input((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    std::vector<LZ77::Match> encodedData = LZ77::encode(input, windowSize, lookAheadBufferSize);

    std::ofstream encodedFile(encodedFileName, std::ios::binary);
    

    for (const auto& match : encodedData) {
        encodedFile.write(reinterpret_cast<const char*>(&match.offset), sizeof(match.offset));
        encodedFile.write(reinterpret_cast<const char*>(&match.length), sizeof(match.length));
        encodedFile.write(&match.nextChar, sizeof(match.nextChar));
    }

    encodedFile.close();
}

void decodeFile(const std::string& encodedFileName, const std::string& decodedFileName) {
    std::ifstream encodedFile(encodedFileName, std::ios::binary);
    

    std::vector<LZ77::Match> encodedData;
    LZ77::Match match;
    while (encodedFile.read(reinterpret_cast<char*>(&match.offset), sizeof(match.offset))) {
        encodedFile.read(reinterpret_cast<char*>(&match.length), sizeof(match.length));
        encodedFile.read(&match.nextChar, sizeof(match.nextChar));
        encodedData.push_back(match);
    }

    encodedFile.close();

    std::string decodedData = LZ77::decode(encodedData);

    std::ofstream decodedFile(decodedFileName);
    

    decodedFile << decodedData;
    decodedFile.close();
}