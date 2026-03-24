#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Task {
public:
    int id;
    int profit;
    int deadline;

    Task(int id, int profit, int deadline)
        : id(id), profit(profit), deadline(deadline) {}
};

class ScheduledTask {
public:
    int id;
    int assignedTime;

    ScheduledTask(int id, int assignedTime)
        : id(id), assignedTime(assignedTime) {}
};

class JobSequencer {
private:
    vector<Task> tasks;
    int maxDeadline;

public:
    JobSequencer() : maxDeadline(0) {}

    void readInput() {
        int n;
        cin >> n;
        tasks.reserve(n);

        for (int i = 1; i <= n; ++i) {
            int g, d;
            cin >> g >> d;
            tasks.emplace_back(i, g, d);
            maxDeadline = max(maxDeadline, d);
        }
    }

    pair<int, vector<ScheduledTask>> schedule() {
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            if (a.profit != b.profit) {
                return a.profit > b.profit;
            }
            if (a.deadline != b.deadline) {
                return a.deadline < b.deadline;
            }
            return a.id < b.id;
        });

        vector<bool> occupied(maxDeadline + 1, false);
        vector<int> assignedTimeById(tasks.size() + 1, 0);
        int totalProfit = 0;

        for (const Task& task : tasks) {
            for (int t = min(task.deadline, maxDeadline); t >= 1; --t) {
                if (!occupied[t]) {
                    occupied[t] = true;
                    assignedTimeById[task.id] = t;
                    totalProfit += task.profit;
                    break;
                }
            }
        }

        vector<ScheduledTask> selected;
        for (size_t id = 1; id < assignedTimeById.size(); ++id) {
            if (assignedTimeById[id] > 0) {
                selected.emplace_back(static_cast<int>(id), assignedTimeById[id]);
            }
        }

        return {totalProfit, selected};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    JobSequencer sequencer;
    sequencer.readInput();
    pair<int, vector<ScheduledTask>> result = sequencer.schedule();

    cout << result.first << '\n';

    cout << "Tareas seleccionadas:";
    for (const ScheduledTask& task : result.second) {
        cout << ' ' << task.id;
    }
    cout << '\n';

    cout << "Instantes asignados:";
    for (const ScheduledTask& task : result.second) {
        cout << ' ' << task.assignedTime;
    }
    cout << '\n';

    return 0;
}