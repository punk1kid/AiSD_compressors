struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class Huffman {
public:
    std::unordered_map<char, std::vector<bool>> huffmanCode;
    std::unordered_map<char, int> freqMap;

    void buildFrequencyMap(const std::string& text) {
        for (char ch : text) {
            freqMap[ch]++;
        }
    }

    Node* buildHuffmanTree() {
        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

        for (auto pair : freqMap) {
            pq.push(new Node(pair.first, pair.second));
        }

        while (pq.size() != 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();

            int sum = left->freq + right->freq;
            Node* node = new Node('\0', sum);
            node->left = left;
            node->right = right;

            pq.push(node);
        }

        return pq.top();
    }

    void buildHuffmanCode(Node* root, std::vector<bool>& bits, int depth) {
        if (!root) {
            return;
        }

        if (!root->left && !root->right) {
            huffmanCode[root->ch] = bits;
        }

        if (root->left) {
            bits.push_back(0);
            buildHuffmanCode(root->left, bits, depth + 1);
            bits.pop_back();
        }

        if (root->right) {
            bits.push_back(1);
            buildHuffmanCode(root->right, bits, depth + 1);
            bits.pop_back();
        }
    }

    std::vector<bool> compress(const std::string& text) {
        buildFrequencyMap(text);
        Node* root = buildHuffmanTree();

        std::vector<bool> compressedText;
        std::vector<bool> bits;
        buildHuffmanCode(root, bits, 0);

        for (char ch : text) {
            compressedText.insert(compressedText.end(), huffmanCode[ch].begin(), huffmanCode[ch].end());
        }

        return compressedText;
    }

    std::string decompress(Node* root, const std::vector<bool>& compressedData) {
        std::string decompressedText;
        Node* current = root;

        for (bool bit : compressedData) {
            if (bit == 0) {
                current = current->left;
            }
            else {
                current = current->right;
            }

            if (!current->left && !current->right) {
                decompressedText += current->ch;
                current = root;
            }
        }

        return decompressedText;
    }

    void writeBitsToFile(const std::vector<bool>& bits, const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        std::vector<char> bytes((bits.size() + 7) / 8);
        for (size_t i = 0; i < bits.size(); ++i) {
            if (bits[i]) {
                bytes[i / 8] |= (1 << (7 - (i % 8)));
            }
        }

        file.write(bytes.data(), bytes.size());
        file.close();
    }

    std::vector<bool> readBitsFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);

        std::vector<bool> bits;
        char byte;
        while (file.get(byte)) {
            for (int i = 7; i >= 0; --i) {
                bits.push_back((byte >> i) & 1);
            }
        }

        file.close();
        return bits;
    }

    double calculateEntropy(const std::string& text) {
        std::unordered_map<char, int> frequency;
        for (char ch : text) {
            frequency[ch]++;
        }

        double entropy = 0.0;
        int totalChars = text.size();
        for (const auto& pair : frequency) {
            double prob = static_cast<double>(pair.second) / totalChars;
            entropy -= prob * std::log2(prob);
        }

        return entropy;
    }
};