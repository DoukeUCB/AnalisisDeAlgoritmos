#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <algorithm>

using namespace std;

struct Process {
    string id;
    int remaining_time;
};

void show_status(const deque<Process>& dq) {
    if (dq.empty()) {
        cout << ">> Estado: Ningún proceso en fila." << endl;
        return;
    }
    
    cout << ">> Procesos en cola: ";
    bool first = true;
    for (const Process& p : dq) {
        if (!first) cout << ", ";
        cout << p.id << "(" << p.remaining_time << "ms)";
        first = false;
    }
    cout << endl;
}

int main() {
    ifstream inputFile("input.txt");

    if (!inputFile.is_open()) {
        cerr << "Error: opening the file input.txt" << endl;
        return 1;
    }

    deque<Process> process_queue;
    string command;

    while (inputFile >> command) {
        if (command == "NUEVO") {
            string id;
            int total_time;
            inputFile >> id >> total_time;
            process_queue.push_back({id, total_time});
            cout << "Nuevo proceso agregado: " << id << endl;
        } 
        else if (command == "EJECUTAR") {
            int quantum;
            inputFile >> quantum;
            
            if (!process_queue.empty()) {
                Process p = process_queue.front();
                process_queue.pop_front();
                
                int time_spent = min(quantum, p.remaining_time);
                p.remaining_time -= time_spent;
                
                cout << "Ejecutando " << p.id;
                
                if (p.remaining_time > 0) {
                    cout << " (quedan: " << p.remaining_time << "ms)" << endl;
                    process_queue.push_back(p);
                } else {
                    cout << " (COMPLETADO)" << endl;
                }
            } else {
                cout << "Todos los procesos fueron completados" << endl;
            }
        } 
        else if (command == "ESTADO") {
            show_status(process_queue);
        }
    }

    inputFile.close();
    return 0;
}
