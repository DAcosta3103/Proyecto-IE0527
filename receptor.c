#include <iostream>
#include <fstream>
#include <vector>
#include <map>
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

void save(const std::vector<char> &v) {
    std::ofstream f("archivo_recibido.txt", std::ios::binary);
    if (!v.empty()) f.write(&v[0], v.size());
}

int main() {

    radio.begin();
    radio.setChannel(76);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_1MBPS);

    radio.openReadingPipe(1, address);
    radio.startListening();

    pinMode(buttonPin, INPUT);
    pinMode(ledPin, OUTPUT);

    int lastState = 1;

    while (true) {

        int s = GPIO::read(buttonPin);

        if (s == 0 && lastState == 1) {

            std::cout << "RX: ventana de recepción 5s...\n";

            digitalWrite(ledPin, HIGH);

            uint32_t start = millis();

            std::map<uint8_t, std::vector<char>> received;
            Packet p;

            while (millis() - start < 5000) {

                if (radio.available()) {

                    radio.read(&p, sizeof(Packet));

                    received[p.seq] = std::vector<char>(p.data, p.data + 31);
                }

                usleep(5 * 1000);
            }

            digitalWrite(ledPin, LOW);

            if (received.empty()) {
                std::cout << "RX: no se recibió nada.\n";
            } else {

                std::vector<char> full;

                for (auto &kv : received) {
                    full.insert(full.end(), kv.second.begin(), kv.second.end());
                }

                while (!full.empty() && full.back() == '\0')
                    full.pop_back();

                save(full);

                std::cout << "RX: archivo reconstruido y guardado.\n";
            }

        }

        lastState = s;
        usleep(10 * 1000);
    }
}