#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory> // For smart pointers

using namespace std;

enum class SeatStatus { Yes, No };
enum class ClassType { FirstClass, NormalClass };
enum class TicketType { Discounted, Normal, Sleep };

class Train {
public:
    virtual void Print_Info() const = 0;
    virtual double Calc_Price() const = 0;
    virtual ~Train() = default; // Virtual destructor for proper cleanup
protected:
    ClassType ID;
};

class Fast_Train : public Train {
public:
    Fast_Train(ClassType classType, SeatStatus seatStatus, int seatNumber)
        : ID(classType), Seat_Reserved(seatStatus), Seat_Number(seatNumber), Revenue(50) {}

    void Print_Info() const override {
        cout << "Ticket type: " << GetClassTypeString(ID)
             << ", Seat reserved: " << GetSeatStatusString(Seat_Reserved)
             << ", Seat number: " << Seat_Number
             << ", Price: " << Calc_Price() << " BGN" << endl;
    }

    double Calc_Price() const override {
        double basePrice = (ID == ClassType::FirstClass) ? Revenue * 1.5 : Revenue;
        if (Seat_Reserved == SeatStatus::No) {
            basePrice += (ID == ClassType::FirstClass) ? 5 : 2;
        }
        return basePrice;
    }

private:
    ClassType ID;
    SeatStatus Seat_Reserved;
    int Seat_Number;
    double Revenue;

    string GetClassTypeString(ClassType type) const {
        switch (type) {
        case ClassType::FirstClass: return "First Class";
        case ClassType::NormalClass: return "Normal Class";
        default: return "Unknown";
        }
    }

    string GetSeatStatusString(SeatStatus status) const {
        return (status == SeatStatus::Yes) ? "Yes" : "No";
    }
};

class Passenger_Train : public Train {
public:
    Passenger_Train(TicketType ticketType)
        : ID(ticketType), Price(5) {}

    void Print_Info() const override {
        cout << "Ticket type: " << GetTicketTypeString(ID)
             << ", Price: " << Calc_Price() << " BGN" << endl;
    }

    double Calc_Price() const override {
        return (ID == TicketType::Discounted) ? Price / 2 : Price;
    }

private:
    TicketType ID;
    double Price;

    string GetTicketTypeString(TicketType type) const {
        switch (type) {
        case TicketType::Discounted: return "Discounted";
        case TicketType::Normal: return "Normal";
        default: return "Unknown";
        }
    }
};

class Night_Train : public Train {
public:
    Night_Train(int seatNumber, SeatStatus seatStatus, TicketType ticketType, bool breakfastIncluded)
        : Seat_Number(seatNumber), Seat_Reserved(seatStatus), ID(ticketType), Breakfast(breakfastIncluded), Price(15) {}

    void Print_Info() const override {
        cout << "Ticket type: " << GetTicketTypeString(ID)
             << ", Seat reserved: " << GetSeatStatusString(Seat_Reserved)
             << ", Seat number: " << Seat_Number
             << ", Breakfast: " << (Breakfast ? "Yes" : "No")
             << ", Price: " << Calc_Price() << " BGN" << endl;
    }

    double Calc_Price() const override {
        return Breakfast ? Price * 1.2 : Price;
    }

private:
    int Seat_Number;
    SeatStatus Seat_Reserved;
    TicketType ID;
    bool Breakfast;
    int Price;

    string GetTicketTypeString(TicketType type) const {
        switch (type) {
        case TicketType::Sleep: return "Sleep";
        default: return "Unknown";
        }
    }

    string GetSeatStatusString(SeatStatus status) const {
        return (status == SeatStatus::Yes) ? "Yes" : "No";
    }
};

class Passengers {
public:
    void Add(unique_ptr<Train> train) {
        Queue.push_back(move(train));
    }

    void List() const {
        if (Queue.empty()) {
            cout << "The queue is empty.\n";
            return;
        }

        cout << "Current queue:\n";
        for (const auto& train : Queue) {
            train->Print_Info();
        }
    }

    void Process() {
        if (Queue.empty()) {
            cout << "The queue is empty.\n";
            return;
        }

        cout << "Processing:\n";
        Queue.front()->Print_Info();
        cout << "Cost for ticket: " << Queue.front()->Calc_Price() << " BGN" << endl;
        Queue.erase(Queue.begin());
    }

    double Total() const {
        double sum = 0;
        for (const auto& train : Queue) {
            sum += train->Calc_Price();
        }
        return sum;
    }

private:
    vector<unique_ptr<Train>> Queue;
};

ClassType GetClassTypeFromUser() {
    int input;
    cout << "Would you like to travel in First Class?\n(1) Yes\n(2) No\n";
    cin >> input;

    while (input != 1 && input != 2) {
        cout << "Invalid input\nSelect again: ";
        cin >> input;
    }

    return (input == 1) ? ClassType::FirstClass : ClassType::NormalClass;
}

SeatStatus GetSeatStatusFromUser() {
    int input;
    cout << "Would you like to reserve a seat? \n(1) Yes\n(2) No\n";
    cin >> input;

    while (input != 1 && input != 2) {
        cout << "Invalid input\nSelect again: ";
        cin >> input;
    }

    return (input == 1) ? SeatStatus::Yes : SeatStatus::No;
}

TicketType GetTicketTypeFromUser() {
    int input;
    cout << "What kind of ticket would you like?\n(1) Discounted (For pupils, students and pensioners)\n(2) Normal\n";
    cin >> input;

    while (input != 1 && input != 2) {
        cout << "Invalid input\nSelect again: ";
        cin >> input;
    }

    return (input == 1) ? TicketType::Discounted : TicketType::Normal;
}

bool GetBreakfastOptionFromUser() {
    int input;
    cout << "Would you like breakfast? (3 BGN)\n(1) Yes\n(2) No\n";
    cin >> input;

    while (input != 1 && input != 2) {
        cout << "Invalid input\nSelect again: ";
        cin >> input;
    }

    return (input == 1);
}

int main() {
    ofstream file1("C:\\Data\\Output.txt");
    if (!file1.is_open()) {
        cout << "File not found!\n";
        return -1;
    }

    Passengers passenger;
    int choice;
    int nightSeatNumber = 1, fastSeatNumber = 1;
    double sum = 0;

    do {
        cout << "\tMenu\n=====================\n";
        cout << "(1) Fast train\n(2) Passenger train\n(3) Sleep compartment\n";
        cout << "(4) Show queue\n(5) Process payment\n(6) Expected revenue\n(0) Exit\n";
        cin >> choice;

        switch (choice) {
            case 1: {
                ClassType classType = GetClassTypeFromUser();
                SeatStatus seatStatus = GetSeatStatusFromUser();
                auto train = make_unique<Fast_Train>(classType, seatStatus, fastSeatNumber);
                passenger.Add(move(train));
                if (seatStatus == SeatStatus::Yes) fastSeatNumber++;
                break;
            }
            case 2: {
                TicketType ticketType = GetTicketTypeFromUser();
                auto train = make_unique<Passenger_Train>(ticketType);
                passenger.Add(move(train));
                break;
            }
            case 3: {
                bool breakfast = GetBreakfastOptionFromUser();
                auto train = make_unique<Night_Train>(nightSeatNumber++, SeatStatus::Yes, TicketType::Sleep, breakfast);
                passenger.Add(move(train));
                break;
            }
            case 4:
                passenger.List();
                break;
            case 5:
                passenger.Process();
                break;
            case 6:
                cout << "Total revenue: " << passenger.Total() << " BGN" << endl;
                file1 << "Total revenue: " << passenger.Total() << " BGN" << endl;
                break;
            case 0:
                break;
            default:
                cout << "Invalid input!\n";
                break;
        }
    } while (choice != 0);

    file1.close();
    return 0;
}