#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <RF24/RF24.h>

RF24 radio(22, 0);
const uint8_t address[6] = "1Node";

const int buttonPin = 17;
const int ledPin    = 27;

struct Packet {
    uint8_t seq;
    char data[31];
};

std::vector<char> readFile(const std::string &filename) {
    std::ifstream f(filename, std::ios::binary);
    if (!f) return {};
    return std::vector<char>((std::istreambuf_iterator<char>(f)),
                             std::istreambuf_iterator<char>());
}

int main() {
    radio.begin();
    radio.setChannel(76);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);

    radio.openWritingPipe(address);
    radio.stopListening();

    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);

    auto fileData = readFile("prueba_texto.txt");
    if (fileData.empty()) {
        std::cerr << "Archivo vacío.\n";
        return 1;
    }

    int lastState = 1;

    while (true) {

        int s = GPIO::read(buttonPin);

        if (s == 0 && lastState == 1) {  
            std::cout << "TX: ventana de transmisión 5s...\n";

            digitalWrite(ledPin, HIGH);

            uint32_t start = millis();

            uint8_t seq = 0;
            size_t index = 0;

            while (index < fileData.size() && millis() - start < 5000) {

                Packet p{};
                p.seq = seq++;

                size_t n = std::min<size_t>(31, fileData.size() - index);
                std::copy(fileData.begin() + index,
                          fileData.begin() + index + n,
                          p.data);

                radio.write(&p, sizeof(Packet));

                index += n;
                usleep(5 * 1000);
            }

            digitalWrite(ledPin, LOW);
            std::cout << "TX: archivo enviado.\n";
        }

        lastState = s;
        usleep(10 * 1000);
    }
}