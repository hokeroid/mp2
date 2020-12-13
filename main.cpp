#include <iostream>
#include <thread>

using namespace std;

struct Table {
    int first = -1;
    int second = -1;

    Table(int first, int second) : first(first), second(second) {}
};

struct Distributor {
    Table *pTable;

    explicit Distributor(Table *pTable) {
        this->pTable = pTable;
    }

    void distribute() const {
        this_thread::sleep_for(1s);

        pTable->first = rand() % 3;
        do {
            pTable->second = rand() % 3;
        } while (pTable->first == pTable->second);
        cout << "На столе предметы " + to_string(pTable->first) + " и " + to_string(pTable->second) + "\n";
    }
};

struct Smoker {
    int component;
    Table *pTable;

    Smoker(int component, Table *pTable) {
        this->component = component;
        this->pTable = pTable;
    }

    void try_smoke() const {
        if (pTable->first == -1 || pTable->second == -1) {
            cout << "Вижу пустой стол. Курильщик номер " + to_string(component) + "\n";
        } else if (pTable->first != component && pTable->second != component) {
            pTable->first = -1;
            pTable->second = -1;
            cout << "Успешно выкурено. Курильщик номер " + to_string(component) + "\n";
        } else {
            cout << "Не хватает компонентов. Курильщик номер " + to_string(component) + "\n";
        }
    }
};

int main() {
    int n;
    cout << "Введите количество раздач: ";
    cin >> n;
    auto *pTable = new Table(-1, -1);

    Distributor distributor = Distributor(pTable);
    Smoker smoker0 = Smoker(0, pTable);
    Smoker smoker1 = Smoker(1, pTable);
    Smoker smoker2 = Smoker(2, pTable);

    for (int i = 0; i < n; i++) {
        thread distribute_thread(&Distributor::distribute, &distributor);
        distribute_thread.join();
        thread smoker0_thread(&Smoker::try_smoke, smoker0);
        thread smoker1_thread(&Smoker::try_smoke, smoker1);
        thread smoker2_thread(&Smoker::try_smoke, smoker2);
        smoker0_thread.join();
        smoker1_thread.join();
        smoker2_thread.join();
        cout << endl;
    }
}
