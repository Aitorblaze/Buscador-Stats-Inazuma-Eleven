#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdint>
#include <algorithm>


// Funciones auxiliares
uint16_t read16(const std::vector<uint8_t>& data, size_t pos) {
    return data[pos] | (data[pos + 1] << 8);
}
uint8_t read8(const std::vector<uint8_t>& data, size_t pos) {
    return data[pos];
}
void error_exit(const std::string& msg) {
    std::cerr << msg << "\n";
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error_exit("Uso: ./miprograma \"NombreJugador\"");
    }
    std::string targetName = argv[1];

    // Leer unitbase.dat
    std::ifstream file1("unitbase.dat", std::ios::binary);
    if (!file1) error_exit("No se pudo abrir unitbase.dat");
    std::vector<uint8_t> data1((std::istreambuf_iterator<char>(file1)), {});

    // Leer unitstat.dat
    std::ifstream file2("unitstat.dat", std::ios::binary);
    if (!file2) error_exit("No se pudo abrir unitstat.dat");
    std::vector<uint8_t> data2((std::istreambuf_iterator<char>(file2)), {});

    // Convertir nombre a bytes
    std::vector<uint8_t> pattern(targetName.begin(), targetName.end());

    // Buscar todas las ocurrencias
    bool anyFound = false;
    size_t pos_in_base = 0;
    size_t count = 0;

    while (pos_in_base <= data1.size() - pattern.size()) {
        auto it = std::search(
            data1.begin() + pos_in_base,
            data1.end(),
            pattern.begin(),
            pattern.end()
        );

        if (it == data1.end()) break;

        size_t offset_base = it - data1.begin();
        size_t unitIndex = offset_base / 0x68;
        size_t offset_stats = unitIndex * 0x48;

        if (offset_stats + 0x48 > data2.size()) {
            std::cerr << "Aviso: Offset fuera de rango, se ignora.\n";
            pos_in_base = offset_base + 1;
            continue;
        }

        std::cout << "=== Coincidencia #" << (++count) << " en offset unitbase.dat 0x"
                  << std::hex << offset_base << std::dec << " ===\n";

        // Leer datos
        size_t pos = offset_stats;
        uint32_t freedom = 0;

        uint16_t fp_min = read16(data2, pos); pos+=2;
        uint16_t fp_max = read16(data2, pos); pos+=2;
        freedom += read16(data2, pos) + read16(data2, pos+2);
        pos+=4;

        uint16_t tp_min = read16(data2, pos); pos+=2;
        uint16_t tp_max = read16(data2, pos); pos+=2;
        freedom += read16(data2, pos) + read16(data2, pos+2);
        pos+=4;

        uint8_t kick_min = read8(data2, pos); pos+=1;
        uint8_t kick_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t body_min = read8(data2, pos); pos+=1;
        uint8_t body_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t guard_min = read8(data2, pos); pos+=1;
        uint8_t guard_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t control_min = read8(data2, pos); pos+=1;
        uint8_t control_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t speed_min = read8(data2, pos); pos+=1;
        uint8_t speed_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t guts_min = read8(data2, pos); pos+=1;
        uint8_t guts_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint8_t stamina_min = read8(data2, pos); pos+=1;
        uint8_t stamina_max = read8(data2, pos); pos+=1;
        freedom += read8(data2, pos) + read8(data2, pos+1);
        pos+=2;

        uint32_t total = kick_max + body_max + guard_max + control_max + speed_max + guts_max + stamina_max;

        std::cout << "FP:       min: " << fp_min << ", max: " << fp_max << "\n";
        std::cout << "TP:       min: " << tp_min << ", max: " << tp_max << "\n";
        std::cout << "Kick:     min: " << (int)kick_min << ", max: " << (int)kick_max << "\n";
        std::cout << "Body:     min: " << (int)body_min << ", max: " << (int)body_max << "\n";
        std::cout << "Guard:    min: " << (int)guard_min << ", max: " << (int)guard_max << "\n";
        std::cout << "Control:  min: " << (int)control_min << ", max: " << (int)control_max << "\n";
        std::cout << "Speed:    min: " << (int)speed_min << ", max: " << (int)speed_max << "\n";
        std::cout << "Guts:     min: " << (int)guts_min << ", max: " << (int)guts_max << "\n";
        std::cout << "Stamina:  min: " << (int)stamina_min << ", max: " << (int)stamina_max << "\n";
        std::cout << "Freedom:  " << freedom << "\n";
        std::cout << "Total:    " << total << "\n\n";

        anyFound = true;
        // Avanzar posición
        pos_in_base = offset_base + 1;
    }

    if (!anyFound) {
        error_exit("Jugador no encontrado en unitbase.dat");
    }

    return 0;
}
