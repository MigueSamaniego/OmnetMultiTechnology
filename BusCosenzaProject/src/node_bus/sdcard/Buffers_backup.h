#ifndef BUFFERS_BACKUP_H
#define BUFFERS_BACKUP_H

#include <string>
#include <vector>


// --------------------------------------------------------------]
// ----------------- memmory management--------------------------]
// --------------------------------------------------------------]
class SDCardBuffer {
private:
    static const int MAX = 10000;

    std::string buffer[MAX];
    int head = 0;
    int tail = 0;
    int count = 0;

public:
    // push batch (ej: 20 datos)
    void pushBatch(const std::vector<std::string>& dataVec) {
        for (const auto& data : dataVec) {
            if (count == MAX) {
                // overwrite (FIFO)
                tail = (tail + 1) % MAX;
                count--;
            }

            buffer[head] = data;
            head = (head + 1) % MAX;
            count++;
        }
    }

    // get batch (ej: 20 datos)
    std::vector<std::string> popBatch(int n) {
        std::vector<std::string> result;

        int toRead = std::min(n, count);

        for (int i = 0; i < toRead; i++) {
            result.push_back(buffer[tail]);
            tail = (tail + 1) % MAX;
            count--;
        }

        return result;
    }

    int size() const {
        return count;
    }

    int getCurrentBytes() const {
        int total = 0;

        int idx = tail;
        for (int i = 0; i < count; i++) {
            total += buffer[idx].size();
            idx = (idx + 1) % MAX;
        }

        return total;
    }

    // Método para precargar el 50% de la capacidad
    void preloadHalf(int percent) {

        int half = 0;

        if(percent >  100) percent = 100;

        half = (percent*MAX)/100;


        for (int i = 0; i < half; i++) {
            // Creamos un dato ficticio con formato similar al tuyo
            // Ejemplo: "PRELOADED_DATA_TIMESTAMP_XXX"
            std::string dummyData = "1970-01-01-00-00/00.000/00.000/$";

            buffer[head] = dummyData;
            head = (head + 1) % MAX;
            count++;
        }
    }
};

class InputBuffer {
private:
    static const int MAX = 20;
    std::vector<std::string> buffer;

public:
    bool add(const std::string& data) {
        buffer.push_back(data);
        return buffer.size() >= MAX;
    }

    std::vector<std::string> flush() {
        std::vector<std::string> temp = buffer;
        buffer.clear();
        return temp;
    }

    int size() const {
        return buffer.size();
    }

    int getCurrentBytes() const {
        int total = 0;
        for (const auto& s : buffer) {
            total += s.size();
        }
        return total;
    }
};

class OutputBuffer {
private:
    static const int MAX = 20;
    std::vector<std::string> buffer;

public:
    void load(const std::vector<std::string>& dataVec) {
        buffer = dataVec;
    }

    bool hasData() const {
        return !buffer.empty();
    }

    std::string getOne() {
        std::string data = buffer.front();
        buffer.erase(buffer.begin());
        return data;
    }

    bool isEmpty() const {
        return buffer.empty();
    }

    int size() const {
        return buffer.size();
    }

    int getCurrentBytes() const {
        int total = 0;
        for (const auto& s : buffer) {
            total += s.size();
        }
        return total;
    }
};

#endif
