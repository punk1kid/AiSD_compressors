#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
#include <queue>
#include <cmath>
#include <list>
#include <numeric>
#include <unordered_map>
#include <bitset>
#include <SFML/Graphics.hpp>
#include "rle.h"
#include "bwt.h"
#include "mtf.h"
#include "lz77.h"
#include "ha.h"
using namespace sf;

void saveImageToBin(const Image& image, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    // Получение размеров изображения
    unsigned int width = image.getSize().x;
    unsigned int height = image.getSize().y;

    // Запись размеров изображения в файл
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Запись цветов каждого пикселя в файл
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            Color pixel = image.getPixel(x, y);
            file.write(reinterpret_cast<const char*>(&pixel), sizeof(pixel));
        }
    }

    file.close();
}

void loadImageFromBin(Image& image, const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    // Чтение размеров изображения из файла
    unsigned int width, height;
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Создание изображения нужного размера
    image.create(width, height);

    // Чтение цветов каждого пикселя из файла и установка их в изображении
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            Color pixel;
            file.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
            image.setPixel(x, y, pixel);
        }
    }

    file.close();
}



void main() {

    std::string input_file;
    std::string decoded_file;

    std::cout << "1 - txt\n2 - img\n select >";
    int choose1;
    std::cin >> choose1;
    if (choose1 == 2)
    {
        Image originalImage;
        originalImage.loadFromFile("test.png");
        // Сохранение изображения в бинарный файл
        saveImageToBin(originalImage, "input.bin");
        input_file = "input.bin";
        decoded_file = "decoded.bin";
    }
    else
    {
        input_file = "input.txt";
        decoded_file = "decoded.txt";
    }
    

    Image restoredImage;
    loadImageFromBin(restoredImage, "input.bin");
    restoredImage.saveToFile("restored1.png");


    int choose2;
    std::cout << "\n\n1 - rle \n2 - bwt \n3 - mtf \n4 - lz77 \n5 - ha \n\nselect >";
    
    std::cin >> choose2;


    switch (choose2)
    {
    case 1:
        //rle
    {
        std::string encoded_file = "encoded_rle.bin";
        rle_encode(input_file, encoded_file);
        rle_decode(encoded_file, decoded_file);
    }
    break;

    case 2:
        //bwt
    {
        std::string encoded_file = "encoded_bwt.bin";

        // Чтение исходного текста из файла
        std::string text = read_file(input_file);

        // Кодирование текста и запись в файл
        std::string transformed_text = bwt_encode(text);
        write_file(encoded_file, transformed_text);

        // Чтение закодированного текста из файла
        std::string read_transformed_text = read_file(encoded_file);

        // Декодирование текста и запись в файл
        std::string original_text = bwt_decode(read_transformed_text);
        write_file(decoded_file, original_text);
    }
    break;

    case 3:
        //mtf
    {
        std::string encoded_file = "encoded_mtf.bin";

        std::string input = readFile(input_file);

        // Кодирование MTF
        std::string encoded = encodeMTF(input);
        writeFile(encoded_file, encoded);

        // Декодирование MTF
        std::string decoded = decodeMTF(encoded);
        writeFile(decoded_file, decoded);

    }
    break;

    case 4:
        //lz77
    {
        std::string encoded_file = "encoded_lz77.bin";
        int windowSize = 255;
        int lookAheadBufferSize = 255;

        encodeFile(input_file, encoded_file, windowSize, lookAheadBufferSize);
        decodeFile(encoded_file, decoded_file);
    }
    break;

    case 5:
        //ha
    {
        input_file = "encoded_lz77.bin";
        std::string encoded_file = "encoded_ha.bin";
        std::ifstream input(input_file);
        std::string text((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
        input.close();

        Huffman huffman;
        std::vector<bool> compressedData = huffman.compress(text);

        huffman.writeBitsToFile(compressedData, encoded_file);

        std::vector<bool> binaryData = huffman.readBitsFromFile(encoded_file);

        Node* root = huffman.buildHuffmanTree();
        std::string decompressedData = huffman.decompress(root, binaryData);

        std::ofstream decompressed(decoded_file);
        decompressed << decompressedData;
        decompressed.close();

        //Энтропия
        double entropy = huffman.calculateEntropy(text);
        std::cout << "Entropy: " << entropy << "bits/character" << std::endl;
    }
    break;
    default:
        break;
    }

    if (choose1 == 2)
    {
        Image restoredImage;
        loadImageFromBin(restoredImage, decoded_file);
        restoredImage.saveToFile("restored2.png");
    }

}

