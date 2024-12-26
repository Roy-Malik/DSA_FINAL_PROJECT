#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <windows.h>
#include <limits.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <ctime>
#include "data.h"
#include "data.cpp"

using namespace std;

class FlightSegment {
public:
    int src;
    int dest;
    string airline;
    string date;
    string departureTime;
    string arrivalTime;
    int cost;
    FlightSegment *next;

    FlightSegment(int s, int d, const string &a, const string &dt, const string &dep, const string &arr, int c)
            : src(s), dest(d), airline(a), date(dt), departureTime(dep), arrivalTime(arr), cost(c), next(nullptr) {}
};

const int NUM_CITIES = 13;
const string cityNames[NUM_CITIES] = {
        "ISLAMABAD", "PARIS", "SEOUL", "NEWYORK", "AMSTERDAM",
        "TOKYO", "LONDON", "SINGAPORE", "HONGKONG", "BERLIN", "SYDNEY"};

int getCityIndex(const string &city) {
    string uppercaseCity = city;
    for (char &c: uppercaseCity) {
        if (c >= 'a' && c <= 'z') {
            c = c - ('a' - 'A');
        }
    }

    for (int i = 0; i < NUM_CITIES; i++) {
        if (cityNames[i] == uppercaseCity)
            return i;
    }
    return -1;
}

void setConsoleColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void resetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
}

class node {
public:
    int src; // Source city index
    int dest; // Destination city index
    string airline, date, departureTime, arrivalTime;
    int cost;
    node *next;

    // Constructor for flight data
    node(int s, int d, string a, string dt, string dep, string arr, int c)
            : src(s), dest(d), airline(toUpperCase(a)), date(toUpperCase(dt)),
              departureTime(dep), arrivalTime(arr), cost(c), next(nullptr) {}

    // Constructor for source node
    node(int s) : src(s), dest(-1), airline(""), date(""), departureTime(""),
                  arrivalTime(""), cost(0), next(nullptr) {}
};

class AdjLst {
public:
    node *head;

    AdjLst() : head(nullptr) {}

    // Initialize the list with the source node
    void insertVertex(int src) {
        if (!head) {
            head = new node(src); // Create a node for the source city
        }
    }

    // Insert flight data into the adjacency list
    void insertEdge(int src, int dest, string airline, string date, string dep, string arr, int cost) {
        node *newNode = new node(src, dest, airline, date, dep, arr, cost);
        if (!head) {
            cerr << "Error: Source city not initialized!" << endl;
            return;
        }
        newNode->next = head->next; // Insert after the source node
        head->next = newNode;
    }

    // Display the adjacency list
    void display() {
        node *temp = head;
        temp = temp->next;
        // Print flight data
        while (temp) {
            setConsoleColor(2);
            cout << "| " << setw(12) << left << cityNames[temp->src] << " | "
                 << setw(12) << left << cityNames[temp->dest] << " | "
                 << setw(10) << left << temp->airline << " | "
                 << setw(10) << left << temp->date << " | "
                 << setw(10) << left << temp->departureTime << " | "
                 << setw(10) << left << temp->arrivalTime << " | "
                 << setw(8) << left << temp->cost << " |" << endl;
            resetConsoleColor();
            temp = temp->next;
        }
    }
};

class Graph {
public:
    AdjLst adjList[NUM_CITIES];

    // Initialize graph by adding source nodes
    Graph() {
        for (int i = 0; i < NUM_CITIES; i++) {
            adjList[i].insertVertex(i);
        }
    }

    // Insert an edge into the graph
    void insertEdge(const string &src, const string &dest, const string &airline,
                    const string &date, const string &dep, const string &arr, int cost) {
        int srcIndex = getCityIndex(src);
        int destIndex = getCityIndex(dest);

        if (srcIndex == -1 || destIndex == -1) {
            cerr << "Invalid city: " << src << " or " << dest << endl;
            return;
        }

        adjList[srcIndex].insertEdge(srcIndex, destIndex, airline, date, dep, arr, cost);
    }

    // Display the entire graph
    void displayGraph() {
        setConsoleColor(5);
        cout << "| " << setw(12) << left << "SOURCE" << " | "
             << setw(12) << left << "DESTINATION" << " | "
             << setw(10) << left << "AIRLINE" << " | "
             << setw(10) << left << "DATE" << " | "
             << setw(10) << left << "DEPARTURE" << " | "
             << setw(10) << left << "ARRIVAL" << " | "
             << setw(8) << left << "COST" << " |" << endl;
        cout << string(90, '-') << endl;
        resetConsoleColor();

        for (int i = 0; i < NUM_CITIES; i++) {
            adjList[i].display();
        }
    }

    void displayAvailableCitiesAndDates() {
        setConsoleColor(5);
        cout << "\n======================== AVAILABLE CITIES ========================\n";
        resetConsoleColor();
        for (int i = 0; i < NUM_CITIES; ++i) {
            cout << "- " << cityNames[i] << endl;
        }

        setConsoleColor(5);
        cout << "\n======================= AVAILABLE DATES =========================\n";
        resetConsoleColor();

        string uniqueDates[100];
        int uniqueDateCount = 0;

        for (int i = 0; i < NUM_CITIES; ++i) {
            node* temp = adjList[i].head->next;
            while (temp) {
                bool isDuplicate = false;
                for (int j = 0; j < uniqueDateCount; ++j) {
                    if (uniqueDates[j] == temp->date) {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate && uniqueDateCount < 100) {
                    uniqueDates[uniqueDateCount++] = temp->date;
                }
                temp = temp->next;
            }
        }

        for (int i = 0; i < uniqueDateCount - 1; ++i) {
            for (int j = i + 1; j < uniqueDateCount; ++j) {
                if (uniqueDates[i] > uniqueDates[j]) {
                    string temp = uniqueDates[i];
                    uniqueDates[i] = uniqueDates[j];
                    uniqueDates[j] = temp;
                }
            }
        }

        for (int i = 0; i < uniqueDateCount; ++i) {
            cout << "- " << uniqueDates[i] << endl;
        }

        setConsoleColor(5);
        cout << "\n======================= AVAILABLE AIRLINES =========================\n";
        resetConsoleColor();

        string uniqueAirlines[100];
        int uniqueAirlineCount = 0;

        for (int i = 0; i < NUM_CITIES; ++i) {
            node* temp = adjList[i].head->next;
            while (temp) {
                bool isDuplicate = false;
                for (int j = 0; j < uniqueAirlineCount; ++j) {
                    if (uniqueAirlines[j] == temp->airline) {
                        isDuplicate = true;
                        break;
                    }
                }
                if (!isDuplicate && uniqueAirlineCount < 100) {
                    uniqueAirlines[uniqueAirlineCount++] = temp->airline;
                }
                temp = temp->next;
            }
        }

        for (int i = 0; i < uniqueAirlineCount - 1; ++i) {
            for (int j = i + 1; j < uniqueAirlineCount; ++j) {
                if (uniqueAirlines[i] > uniqueAirlines[j]) {
                    string temp = uniqueAirlines[i];
                    uniqueAirlines[i] = uniqueAirlines[j];
                    uniqueAirlines[j] = temp;
                }
            }
        }

        for (int i = 0; i < uniqueAirlineCount; ++i) {
            cout << "- " << uniqueAirlines[i] << endl;
        }
    }



    void searchFlightsRecursive(int srcIndex, int destIndex, const string &date, const UserPreferences &preferences,
                                FlightSegment *currentPath, int currentPathSize, FlightSegment *allPaths[], int &pathCount, int maxStops,
                                const bool directVisited[]) {
        if (currentPathSize > maxStops + 1) return; // Limit the number of stops

        // If the destination is reached, store the current path
        if (srcIndex == destIndex) {
            allPaths[pathCount++] = currentPath;
            return;
        }

        // Iterate through adjacent flights
        node *temp = adjList[srcIndex].head;
        while (temp) {
            if (temp->date == date && preferences.isPreferredAirline(temp->airline)) {
                bool alreadyVisited = false;

                // Check if the destination city is already in the current path (to avoid cycles)
                FlightSegment *pathTemp = currentPath;
                while (pathTemp) {
                    if (pathTemp->src == temp->dest) {
                        alreadyVisited = true;
                        break;
                    }
                    pathTemp = pathTemp->next;
                }

                if (!alreadyVisited) {
                    // Create a new segment for the current flight
                    FlightSegment *newSegment = new FlightSegment(temp->src, temp->dest, temp->airline, temp->date, temp->departureTime, temp->arrivalTime, temp->cost);

                    // Clone the current path and append the new segment
                    FlightSegment *newPath = nullptr;
                    if (currentPath) {
                        // Clone the path
                        newPath = new FlightSegment(currentPath->src, currentPath->dest, currentPath->airline, currentPath->date, currentPath->departureTime, currentPath->arrivalTime,
                                                    currentPath->cost);
                        FlightSegment *tempClone = newPath;
                        FlightSegment *currentTemp = currentPath->next;
                        while (currentTemp) {
                            tempClone->next = new FlightSegment(currentTemp->src, currentTemp->dest, currentTemp->airline, currentTemp->date, currentTemp->departureTime, currentTemp->arrivalTime,
                                                                currentTemp->cost);
                            tempClone = tempClone->next;
                            currentTemp = currentTemp->next;
                        }
                        // Append the new segment
                        FlightSegment *tempTail = newPath;
                        while (tempTail->next) {
                            tempTail = tempTail->next;
                        }
                        tempTail->next = newSegment;
                    } else {
                        // Start a new path
                        newPath = newSegment;
                    }

                    // Recursive call for the next flight
                    searchFlightsRecursive(temp->dest, destIndex, date, preferences, newPath, currentPathSize + 1, allPaths, pathCount, maxStops, directVisited);
                }
            }
            temp = temp->next; // Move to the next flight
        }
    }


    void SearchForFlight(const string &sourceCity, const string &destinationCity, const string &date, const UserPreferences &preferences) {

        int srcIndex = getCityIndex(sourceCity);
        int destIndex = getCityIndex(destinationCity);

        if (srcIndex == -1 || destIndex == -1) {
            cerr << "Invalid city: " << sourceCity << " or " << destinationCity << endl;
            return;
        }

        const int maxStops = 4; // Allow up to 4 connecting flights
        const int maxPaths = 100; // Maximum number of paths to store
        FlightSegment *allPaths[maxPaths] = {nullptr}; // Initialize all paths with nullptr
        bool directVisited[100] = {false}; // Track visited cities from direct flights
        int pathCount = 0;
        int flightCount = 0;
        int availableSeats = 0;
        searchFlightsRecursive(srcIndex, destIndex, date, preferences, nullptr, 0, allPaths, pathCount, maxStops,
                               directVisited);

        if (pathCount > 0) {
            setConsoleColor(5);
            cout << "\n======================================  FLIGHT OPTIONS ======================================\n";
            srand(time(0));
            for (int i = 0; i < pathCount; ++i) {
                setConsoleColor(5);
                ++flightCount;
                cout << i + 1 << ".\n";
                cout << "| " << setw(12) << left << "SOURCE" << " | "
                     << setw(12) << left << "DESTINATION" << " | "
                     << setw(10) << left << "AIRLINE" << " | "
                     << setw(10) << left << "DATE" << " | "
                     << setw(10) << left << "DEPARTURE" << " | "
                     << setw(10) << left << "ARRIVAL" << " | "
                     << setw(8) << left << "COST" << " |"
                     << setw(8) << left << "AVAILABLE SEATS" << " |\n";
                cout << "--------------------------------------------------------------------------------------------------\n";
                FlightSegment *segment = allPaths[i];
                setConsoleColor(2);
                while (segment) {
                    availableSeats = rand() % 50 + 1;
                    cout << "| " << setw(12) << left << cityNames[segment->src] << " | "
                         << setw(12) << left << cityNames[segment->dest] << " | "
                         << setw(10) << left << segment->airline << " | "
                         << setw(10) << left << segment->date << " | "
                         << setw(10) << left << segment->departureTime << " | "
                         << setw(10) << left << segment->arrivalTime << " | "
                         << setw(8) << left << segment->cost << " |"
                         << setw(8) << left << availableSeats << " |\n";
                    segment = segment->next;
                }
                cout << "--------------------------------------------------------------------------------------------------\n";
            }
            resetConsoleColor();
        } else {
            setConsoleColor(4);
            cout << "No  flights found from " << sourceCity << " to " << destinationCity << " on " << date << ".\n";
            resetConsoleColor();
        }
        if (flightCount > 0) {
            dijkstra(sourceCity, destinationCity, date);
            if (getWeather(destinationCity) < 20) {
                setConsoleColor(11);
                cout << "Weather is cold at " << destinationCity << endl;
            } else if (getWeather(destinationCity) > 30) {
                setConsoleColor(12);
                cout << "Weather is hot at " << destinationCity << endl;
            } else {
                setConsoleColor(15);
                cout << "Weather is moderate at " << destinationCity << endl;
            }
            cout << "Enter the number of the flight you want to book (1-" << flightCount << "): ";
            int choice, choice_seats;
            cin >> choice;
            cout << "Enter the number of seats you want to book : ";
            cin >> choice_seats;
            if (choice_seats < 1 || choice_seats > availableSeats) {
                cout << "Invalid choice. Please run the search again and select a valid flight number.\n";
                return;
            }

            // Validate user input
            if (choice < 1 || choice > flightCount) {
                cout << "Invalid choice. Please run the search again and select a valid flight number.\n";
                return;
            }

            // Retrieve the selected flight
            FlightSegment *selectedFlight = allPaths[choice - 1];
            if (!selectedFlight) {
                cerr << "Error: Unable to retrieve selected flight details.\n";
                return;
            }

            // Display booking confirmation
            setConsoleColor(2);
            cout << "\nBooking confirmed for the following flight path:\n";
            FlightSegment *segment = selectedFlight;
            ofstream bookingFile("flightbooking.txt", ios::app);
            int totalcost = 0;
            if (bookingFile.is_open()) {
                bookingFile << "Flight Booking Details:\n";
                while (segment) {
                    setConsoleColor(6);
                    cout << "| " << setw(12) << left << cityNames[segment->src] << " | "
                         << setw(12) << left << cityNames[segment->dest] << " | "
                         << setw(10) << left << segment->airline << " | "
                         << setw(10) << left << segment->date << " | "
                         << setw(10) << left << segment->departureTime << " | "
                         << setw(10) << left << segment->arrivalTime << " | "
                         << setw(8) << left << segment->cost << " |"
                         << setw(8) << left << choice_seats << " |\n";
                    cout << "--------------------------------------------------------------------------------------------------\n";
                    totalcost += segment->cost;


                    // Save to file
                    bookingFile << "Segment: From " << cityNames[segment->src] << " to " << cityNames[segment->dest]
                                << "\n";
                    bookingFile << "  Airline: " << segment->airline << "\n";
                    bookingFile << "  Date: " << segment->date << "\n";
                    bookingFile << "  Departure: " << segment->departureTime << "\n";
                    bookingFile << "  Arrival: " << segment->arrivalTime << "\n";
                    bookingFile << "  Cost: " << segment->cost << "\n";
                    bookingFile << "  No. of Seats: " << choice_seats << " |\n";
                    bookingFile << "----------------------------------------\n";

                    // Move to the next segment in the path
                    segment = segment->next;
                }
                totalcost *= choice_seats;
                bookingFile << "Total Cost(without hotel charges): " << totalcost << "\n";
                bookingFile << "----------------------------------------\n";
                bookingFile.close();
                setConsoleColor(2);
                cout << "Total Cost of Flight Path(without hotel charges): " << totalcost << "\n";
                cout << "Booking details have been saved to 'flightbooking.txt'.\n";
            } else {
                cerr << "Unable to open booking file. Flight details were not saved.\n";
            }


            segment = selectedFlight->next;
            if (segment) {
                setConsoleColor(2);
                cout << "\nYour flight includes layovers. Here is your layover information:\n";
                segment = selectedFlight;
                while (segment->next) {
                    cout << "Layover at ";
                    setConsoleColor(6);
                    cout << cityNames[segment->dest];
                    setConsoleColor(2);
                    cout << ": Duration: ";
                    // Calculate layover duration
                    string layoverStart = segment->arrivalTime;
                    string layoverEnd = segment->next->departureTime;
                    int layoverHours = stoi(layoverEnd.substr(0, 2)) - stoi(layoverStart.substr(0, 2));
                    int layoverMinutes = stoi(layoverEnd.substr(3, 2)) - stoi(layoverStart.substr(3, 2));
                    if (layoverMinutes < 0) {
                        layoverHours -= 1;
                        layoverMinutes += 60;
                    }
                    setConsoleColor(6);
                    cout << layoverHours << " hours and " << layoverMinutes << " minutes.\n";
                    segment = segment->next;
                    if (layoverHours >= 6) {
                        setConsoleColor(2);
                        cout << "HOTEL PRICES : ";
                        setConsoleColor(6);
                        cout << getHotelPrice(cityNames[segment->src], "hotelCharges_perday.txt") << endl;
                        if (getWeather(cityNames[segment->src]) < 20) {
                            setConsoleColor(11);
                            cout << "Weather is cold at the layover city.\n";
                        } else if (getWeather(cityNames[segment->src]) > 30) {
                            setConsoleColor(12);
                            cout << "Weather is hot at the layover city.\n";
                        } else {
                            setConsoleColor(15);
                            cout << "Weather is moderate at the layover city.\n";
                        }

                    }
                }
                setConsoleColor(2);
                cout << "Please plan your layovers accordingly.\n";
                resetConsoleColor();
            }
        }
    }


    int getWeather(const string &city) {
        ifstream file("weather_data.txt");
        if (!file.is_open()) {
            cerr << "Unable to open the file!" << endl;
            return -1; // Error code for file issues
        }

        string cityName;
        int temperature;
        while (file >> cityName >> temperature) {
            if (cityName == city) {
                file.close();
                return temperature;
            }
        }
        file.close();

        cerr << "City not found in the weather data!" << endl;
        return -1; // Error code for missing city
    }


    int getHotelPrice(const string &city, const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        string City;
        int price;
        while (file >> City >> price) {
            if (City == city) {
                file.close();
                return price;
            }
        }

        file.close();
        cerr << "City not found in file: " << city << endl;
        return -1;
    }

    void displayFullGraph(sf::Font &font) {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Full Graph Visualization");

        sf::Vector2f cityPositions[NUM_CITIES] = {
                {150, 100},
                {850, 100},
                {500, 200},
                {300, 500},
                {700, 500},
                {150, 700},
                {850, 700},
                {400, 400},
                {600, 400},
                {500, 600},
                {500, 100}
        };

        sf::RectangleShape tooltipBackground;
        tooltipBackground.setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent black
        tooltipBackground.setSize(sf::Vector2f(200, 100));
        tooltipBackground.setOutlineColor(sf::Color::White);
        tooltipBackground.setOutlineThickness(1);

        sf::Text tooltipText;
        tooltipText.setFont(font);
        tooltipText.setCharacterSize(16);
        tooltipText.setFillColor(sf::Color::White);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color(240, 240, 240));

            bool tooltipShown = false;
            sf::Vector2f tooltipPosition;
            string tooltipContent;

            // Draw edges
            for (int i = 0; i < NUM_CITIES; i++) {
                node *temp = adjList[i].head->next;
                while (temp) {
                    sf::Vertex line[] = {
                            sf::Vertex(cityPositions[i], sf::Color(100, 149, 237)),
                            sf::Vertex(cityPositions[temp->dest], sf::Color(100, 149, 237))
                    };
                    window.draw(line, 2, sf::Lines);

                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                    // Check if the mouse is near this line segment
                    sf::Vector2f start = cityPositions[i];
                    sf::Vector2f end = cityPositions[temp->dest];
                    float distance = std::abs((end.y - start.y) * worldPos.x -
                                              (end.x - start.x) * worldPos.y +
                                              end.x * start.y - end.y * start.x) /
                                     std::sqrt(std::pow(end.y - start.y, 2) + std::pow(end.x - start.x, 2));

                    if (distance < 5.0f && !tooltipShown) {
                        tooltipShown = true;
                        tooltipPosition = {worldPos.x + 10, worldPos.y + 10};
                        tooltipContent = "From: " + cityNames[i] +
                                         "\nTo: " + cityNames[temp->dest] +
                                         "\nCost: $" + std::to_string(temp->cost) +
                                         "\nAirline: " + temp->airline;
                    }
                    temp = temp->next;
                }
            }

            // Draw nodes
            for (int i = 0; i < NUM_CITIES; i++) {
                sf::CircleShape cityShape(15);
                cityShape.setFillColor(sf::Color::Green);
                cityShape.setOutlineThickness(2);
                cityShape.setOutlineColor(sf::Color::White);
                cityShape.setPosition(cityPositions[i].x - 15, cityPositions[i].y - 15);
                window.draw(cityShape);

                sf::Text cityName;
                cityName.setFont(font);
                cityName.setString(cityNames[i]);
                cityName.setCharacterSize(14);
                cityName.setFillColor(sf::Color::Black);
                cityName.setPosition(cityPositions[i].x + 20, cityPositions[i].y - 10);
                window.draw(cityName);

                // Check if mouse is near a city node
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                float nodeDistance = sqrt(pow(worldPos.x - (cityPositions[i].x), 2) +
                                          pow(worldPos.y - (cityPositions[i].y), 2));
                if (nodeDistance < 20.0f && !tooltipShown) {
                    tooltipShown = true;
                    tooltipPosition = {worldPos.x + 10, worldPos.y + 10};
                    tooltipContent = "City: " + cityNames[i];
                }
            }

            // Draw tooltip if shown
            if (tooltipShown) {
                tooltipBackground.setPosition(tooltipPosition);
                tooltipText.setString(tooltipContent);
                tooltipText.setPosition(tooltipPosition.x + 5, tooltipPosition.y + 5);
                window.draw(tooltipBackground);
                window.draw(tooltipText);
            }

            window.display();
        }
    }

    void dijkstra(const string &sourceCity, const string &destinationCity, const string &selectedDate) {
        int srcIndex = getCityIndex(sourceCity);
        int destIndex = getCityIndex(destinationCity);

        if (srcIndex == -1 || destIndex == -1) {
            cerr << "Invalid city: " << sourceCity << " or " << destinationCity << endl;
            return;
        }

        int dist[NUM_CITIES];
        bool sptSet[NUM_CITIES];
        int parent[NUM_CITIES];
        string airline[NUM_CITIES];
        for (int i = 0; i < NUM_CITIES; i++) {
            dist[i] = INT_MAX;
            sptSet[i] = false;
            parent[i] = -1;
            airline[i] = "";
        }

        dist[srcIndex] = 0;
        PriorityQueue pq(NUM_CITIES);
        pq.push({0, srcIndex});

        sf::RenderWindow window(sf::VideoMode(1200, 800), "Dijkstra Visualization - Shortest Flight Path");
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            cerr << "Error: Could not load font file.\n";
            return;
        }

        sf::Vector2f cityPositions[NUM_CITIES] = {
                {150,  100},
                {1050, 100},
                {600,  200},
                {300,  500},
                {900,  500},
                {150,  700},
                {1050, 700},
                {400,  400},
                {800,  400},
                {600,  600},
                {600,  100}
        };

        sf::RectangleShape background(sf::Vector2f(1200, 800));
        background.setFillColor(sf::Color(45, 45, 48));

        while (!pq.empty() && window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            int u = pq.pop().second;
            sptSet[u] = true;

            window.clear();
            window.draw(background);

            for (int i = 0; i < NUM_CITIES; i++) {
                node *temp = adjList[i].head->next;
                while (temp) {
                    sf::Vertex line[] = {
                            sf::Vertex(cityPositions[i], sf::Color(100, 149, 237, 150)),
                            sf::Vertex(cityPositions[temp->dest], sf::Color(100, 149, 237, 150))
                    };

                    if (sptSet[temp->dest] && parent[temp->dest] == i) {
                        line[0].color = sf::Color(250, 80, 80, 200); // Glowing red for path
                        line[1].color = sf::Color(250, 80, 80, 200);
                    }

                    window.draw(line, 2, sf::Lines);
                    temp = temp->next;
                }
            }

            for (int i = 0; i < NUM_CITIES; i++) {
                sf::CircleShape cityShape(18);
                cityShape.setPosition(cityPositions[i].x - 18, cityPositions[i].y - 18);

                if (i == u) {
                    cityShape.setFillColor(sf::Color(255, 215, 0)); // Bright yellow
                } else if (sptSet[i]) {
                    cityShape.setFillColor(sf::Color(0, 128, 0)); // Bright green
                } else {
                    cityShape.setFillColor(sf::Color(0, 191, 255)); // Sky blue
                }

                cityShape.setOutlineThickness(3);
                cityShape.setOutlineColor(sf::Color::White);
                window.draw(cityShape);

                sf::Text cityName;
                cityName.setFont(font);
                cityName.setString(cityNames[i]);
                cityName.setCharacterSize(20);
                cityName.setFillColor(sf::Color::White);
                cityName.setPosition(cityPositions[i].x + 25, cityPositions[i].y - 12);
                window.draw(cityName);
            }

            window.display();
            sf::sleep(sf::milliseconds(400));

            node *temp = adjList[u].head->next;
            while (temp) {
                if (temp->date == selectedDate && !sptSet[temp->dest] && dist[u] != INT_MAX && dist[u] + temp->cost < dist[temp->dest]) {
                    dist[temp->dest] = dist[u] + temp->cost;
                    parent[temp->dest] = u;
                    airline[temp->dest] = temp->airline;
                    pq.push({dist[temp->dest], temp->dest});
                }
                temp = temp->next;
            }
        }

        // Highlight final shortest path
        int current = destIndex;
        sf::Vertex pathLines[NUM_CITIES * 2];
        int pathLineCount = 0;

        while (parent[current] != -1) {
            sf::Vector2f start = cityPositions[parent[current]];
            sf::Vector2f end = cityPositions[current];

            pathLines[pathLineCount++] = sf::Vertex(start, sf::Color::Red);
            pathLines[pathLineCount++] = sf::Vertex(end, sf::Color::Red);

            current = parent[current];
        }

        window.clear();
        window.draw(background);

        for (int i = 0; i < NUM_CITIES; i++) {
            node *temp = adjList[i].head->next;
            while (temp) {
                sf::Vertex line[] = {
                        sf::Vertex(cityPositions[i], sf::Color(100, 149, 237, 150)),
                        sf::Vertex(cityPositions[temp->dest], sf::Color(100, 149, 237, 150))
                };
                window.draw(line, 2, sf::Lines);
                temp = temp->next;
            }
        }

        window.draw(pathLines, pathLineCount, sf::Lines);

        for (int i = 0; i < NUM_CITIES; i++) {
            sf::CircleShape cityShape(18);
            cityShape.setPosition(cityPositions[i].x - 18, cityPositions[i].y - 18);

            if (i == srcIndex) {
                cityShape.setFillColor(sf::Color::Green);
            } else if (i == destIndex) {
                cityShape.setFillColor(sf::Color::Red);
            } else {
                cityShape.setFillColor(sf::Color(0, 191, 255));
            }

            cityShape.setOutlineThickness(3);
            cityShape.setOutlineColor(sf::Color::White);
            window.draw(cityShape);

            sf::Text cityName;
            cityName.setFont(font);
            cityName.setString(cityNames[i]);
            cityName.setCharacterSize(20);
            cityName.setFillColor(sf::Color::White);
            cityName.setPosition(cityPositions[i].x + 25, cityPositions[i].y - 12);
            window.draw(cityName);
        }

        window.display();
        printSolution(dist, parent, airline, srcIndex, destIndex);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
        }
    }


    void printSolution(int dist[], int parent[], string airline[], int src, int dest) {
        if (dist[dest] == INT_MAX) {
            cout << "No path exists from " << cityNames[src] << " to " << cityNames[dest] << "." << endl;
            return;
        }

        int path[NUM_CITIES];
        string airlines[NUM_CITIES];
        int pathIndex = 0;

        int current = dest;
        while (current != -1) {
            path[pathIndex] = current;
            if (current != src) {
                airlines[pathIndex] = airline[current];
            }
            current = parent[current];
            pathIndex++;
        }

        for (int i = 0, j = pathIndex - 1; i < j; ++i, --j) {
            swap(path[i], path[j]);
            if (i < j - 1) {
                swap(airlines[i], airlines[j - 1]);
            }
        }

        cout << "Shortest path from " << cityNames[src] << " to " << cityNames[dest] << " is:" << endl;
        for (int i = 0; i < pathIndex; ++i) {
            cout << cityNames[path[i]];
            if (i < pathIndex - 1) {
                cout << " --(" << airlines[i] << ", $" << dist[path[i + 1]] - dist[path[i]] << ")--> ";
            }
        }

        cout << endl << "Total cost: $" << dist[dest] << endl;
    }

    void displayFilteredAirlineGraph(const string &airline, sf::Font &font) {
        sf::RenderWindow window(sf::VideoMode(1000, 800), "Filtered Airline Graph - " + airline);

        sf::Vector2f cityPositions[NUM_CITIES] = {
                {150, 100},
                {850, 100},
                {500, 200},
                {300, 500},
                {700, 500},
                {150, 700},
                {850, 700},
                {400, 400},
                {600, 400},
                {500, 600},
                {500, 100}
        };

        sf::Text tooltip;
        tooltip.setFont(font);
        tooltip.setCharacterSize(16);
        tooltip.setFillColor(sf::Color::White);

        sf::RectangleShape tooltipBackground;
        tooltipBackground.setFillColor(sf::Color(0, 0, 0, 200));
        tooltipBackground.setSize({200, 80});

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPosition = static_cast<sf::Vector2f>(mousePosition);

            bool tooltipShown = false;
            string tooltipText;

            window.clear(sf::Color(40, 44, 52));

            for (int i = 0; i < NUM_CITIES; i++) {
                node *temp = adjList[i].head->next;
                while (temp) {
                    if (temp->airline == airline) {
                        sf::Vertex line[] = {
                                sf::Vertex(cityPositions[i], sf::Color(0, 153, 255)),
                                sf::Vertex(cityPositions[temp->dest], sf::Color(102, 204, 255))
                        };
                        window.draw(line, 2, sf::Lines);

                        sf::Vector2f start = cityPositions[i];
                        sf::Vector2f end = cityPositions[temp->dest];
                        float distance = std::abs((end.y - start.y) * mouseWorldPosition.x -
                                                  (end.x - start.x) * mouseWorldPosition.y +
                                                  end.x * start.y - end.y * start.x) /
                                         std::sqrt(std::pow(end.y - start.y, 2) + std::pow(end.x - start.x, 2));

                        if (distance < 5.0f) {
                            tooltipText = "From: " + cityNames[i] + "\nTo: " + cityNames[temp->dest] +
                                          "\nCost: $" + to_string(temp->cost);
                            tooltip.setPosition(mouseWorldPosition.x + 15, mouseWorldPosition.y + 15);
                            tooltipBackground.setPosition(mouseWorldPosition.x + 10, mouseWorldPosition.y + 10);
                            tooltip.setString(tooltipText);
                            tooltipBackground.setSize({tooltip.getGlobalBounds().width + 10, tooltip.getGlobalBounds().height + 10});
                            tooltipShown = true;
                        }
                    }
                    temp = temp->next;
                }
            }

            for (int i = 0; i < NUM_CITIES; i++) {
                sf::CircleShape cityShape(15);
                cityShape.setFillColor(sf::Color(102, 178, 255));
                cityShape.setOutlineThickness(3);
                cityShape.setOutlineColor(sf::Color::White);
                cityShape.setPosition(cityPositions[i].x - 15, cityPositions[i].y - 15);
                window.draw(cityShape);

                sf::Text cityName;
                cityName.setFont(font);
                cityName.setString(cityNames[i]);
                cityName.setCharacterSize(14);
                cityName.setFillColor(sf::Color::White);
                cityName.setPosition(cityPositions[i].x + 20, cityPositions[i].y - 10);
                window.draw(cityName);
            }

            if (tooltipShown) {
                window.draw(tooltipBackground);
                window.draw(tooltip);
            }

            window.display();
        }
    }

    void displayCityGraph(const string &cityName, sf::Font &font) {
        int cityIndex = getCityIndex(cityName);
        if (cityIndex == -1) {
            cerr << "Invalid city: " << cityName << endl;
            return;
        }

        sf::RenderWindow window(sf::VideoMode(1000, 800), "City Flight Graph - " + cityName);

        sf::Vector2f cityPositions[NUM_CITIES] = {
                {150, 100},
                {850, 100},
                {500, 200},
                {300, 500},
                {700, 500},
                {150, 700},
                {850, 700},
                {400, 400},
                {600, 400},
                {500, 600},
                {500, 100}
        };

        sf::Text tooltip;
        tooltip.setFont(font);
        tooltip.setCharacterSize(16);
        tooltip.setFillColor(sf::Color::White);

        sf::RectangleShape tooltipBackground;
        tooltipBackground.setFillColor(sf::Color(0, 0, 0, 200));
        tooltipBackground.setSize({200, 80});

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorldPosition = static_cast<sf::Vector2f>(mousePosition);

            bool tooltipShown = false;
            string tooltipText;

            window.clear(sf::Color(40, 44, 52));

            for (int i = 0; i < NUM_CITIES; i++) {
                node *temp = adjList[i].head->next;
                while (temp) {
                    if (i == cityIndex || temp->dest == cityIndex) {
                        sf::Vertex line[] = {
                                sf::Vertex(cityPositions[i], sf::Color(0, 153, 255)),
                                sf::Vertex(cityPositions[temp->dest], sf::Color(102, 204, 255))
                        };
                        window.draw(line, 2, sf::Lines);

                        sf::Vector2f start = cityPositions[i];
                        sf::Vector2f end = cityPositions[temp->dest];
                        float distance = abs((end.y - start.y) * mouseWorldPosition.x -
                                             (end.x - start.x) * mouseWorldPosition.y +
                                             end.x * start.y - end.y * start.x) /
                                         sqrt(pow(end.y - start.y, 2) + pow(end.x - start.x, 2));

                        if (distance < 5.0f) {
                            tooltipText = "From: " + cityNames[i] + "\nTo: " + cityNames[temp->dest] +
                                          "\nCost: $" + to_string(temp->cost) + "\nAirline: " + temp->airline;
                            tooltip.setPosition(mouseWorldPosition.x + 15, mouseWorldPosition.y + 15);
                            tooltipBackground.setPosition(mouseWorldPosition.x + 10, mouseWorldPosition.y + 10);
                            tooltip.setString(tooltipText);
                            tooltipBackground.setSize({tooltip.getGlobalBounds().width + 10, tooltip.getGlobalBounds().height + 10});
                            tooltipShown = true;
                        }
                    }
                    temp = temp->next;
                }
            }

            for (int i = 0; i < NUM_CITIES; i++) {
                sf::CircleShape cityShape(15);
                cityShape.setFillColor(i == cityIndex ? sf::Color::Yellow : sf::Color(102, 178, 255));
                cityShape.setOutlineThickness(3);
                cityShape.setOutlineColor(sf::Color::White);
                cityShape.setPosition(cityPositions[i].x - 15, cityPositions[i].y - 15);
                window.draw(cityShape);

                sf::Text cityNameText;
                cityNameText.setFont(font);
                cityNameText.setString(cityNames[i]);
                cityNameText.setCharacterSize(14);
                cityNameText.setFillColor(sf::Color::White);
                cityNameText.setPosition(cityPositions[i].x + 20, cityPositions[i].y - 10);
                window.draw(cityNameText);
            }

            if (tooltipShown) {
                window.draw(tooltipBackground);
                window.draw(tooltip);
            }

            window.display();
        }
    }


private:

    void printPath(int parent[], string airline[], int j) {
        if (parent[j] == -1)
            return;

        printPath(parent, airline, parent[j]);

        cout << cityNames[j] << " --";
    }
};

Graph filterByPreferredAirline(Graph &originalGraph, const string &preferredAirline) {
    Graph subGraph;

    for (int i = 0; i < NUM_CITIES; ++i) {
        node *current = originalGraph.adjList[i].head;
        while (current) {
            if (current->airline == preferredAirline) {
                subGraph.insertEdge(cityNames[i], cityNames[current->dest], current->airline,
                                    current->date, current->departureTime, current->arrivalTime, current->cost);
            }
            current = current->next;
        }
    }

    return subGraph;
}

Graph filterByPreferredCity(const string &preferredCity, const Graph &originalGraph) {
    int preferredCityIndex = getCityIndex(preferredCity);
    if (preferredCityIndex == -1) {
        cerr << "Invalid city: " << preferredCity << endl;
        return Graph();
    }

    Graph subGraph;
    for (int i = 0; i < NUM_CITIES; i++) {
        node *temp = originalGraph.adjList[i].head;
        temp = temp->next;
        while (temp) {
            if (i == preferredCityIndex || temp->dest == preferredCityIndex) {
                subGraph.insertEdge(cityNames[i], cityNames[temp->dest], temp->airline, temp->date, temp->departureTime, temp->arrivalTime, temp->cost);
            }
            temp = temp->next;
        }
    }
    return subGraph;
}

int main() {
    Graph flightGraph;
    UserPreferences preferences;
    string AirlinePref;
    ifstream file("Flights.txt");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Error: Could not load font file.\n";
        return 1;
    }
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;
        return 1;
    }
    string src, dest, date, dep, arr, airline;
    int cost;

    while (file >> src >> dest >> date >> dep >> arr >> cost >> airline) {
        flightGraph.insertEdge(toUpperCase(src), toUpperCase(dest), toUpperCase(airline), toUpperCase(date), dep, arr, cost);
    }
    file.close();


    string src_city, dest_city, date_of_flight, pref_airline, pref_city;
    int menu_choice = 1;
    do {
        setConsoleColor(5);
        cout << "<><><><><><><><><><><><><><><><><><><><><><><><> S K Y   N A V   A I <><><><><><><><><><><><><><><><><><><><><><><><><><" << endl << endl;
        setConsoleColor(2);
        cout << "1.See All Flights \n2.See Flights (Specific City) \n3.See Flights (Specific Airline) \n4.Book A Flight \n5.View Flight Map\n6.Exit\nEnter the option(1-6): ";
        resetConsoleColor();
        cin >> menu_choice;
        switch (menu_choice) {
            case 1: {
                flightGraph.displayGraph();
                break;
            }
            case 2: {
                cout << "Enter City:";
                cin >> pref_city;
                pref_city = toUpperCase(pref_city);
                cout << "\nAll the flights in and out of " << pref_city << ":\n";
                filterByPreferredCity(pref_city, flightGraph).displayGraph();
                flightGraph.displayCityGraph(pref_city, font);
                break;
            }
            case 3: {
                cout << "Enter Airline:";
                cin >> pref_airline;
                pref_airline = toUpperCase(pref_airline);
                filterByPreferredAirline(flightGraph, pref_airline).displayGraph();
                flightGraph.displayFilteredAirlineGraph(pref_airline, font);
                break;
            }
            case 4: {
                flightGraph.displayAvailableCitiesAndDates();

                do {
                    cout << "\nENTER SOURCE CITY: ";
                    cin >> src_city;
                    src_city = toUpperCase(src_city);
                    if (getCityIndex(src_city) == -1) {
                        cout << "Invalid source city. Please enter a correct city name.\n";
                    }
                } while (getCityIndex(src_city) == -1);

                do {
                    cout << "ENTER DESTINATION CITY: ";
                    cin >> dest_city;
                    dest_city = toUpperCase(dest_city);
                    if (getCityIndex(dest_city) == -1) {
                        cout << "Invalid destination city. Please enter a correct city name.\n";
                    }
                } while (getCityIndex(dest_city) == -1);

                cout << "ENTER DATE OF FLIGHT (dd/mm/yyyy): ";
                cin >> date_of_flight;
                date_of_flight = toUpperCase(date_of_flight);

                string preference;
                cout << "ENTER PREFERRED AIRLINE (or 'none' for no preference): ";
                cin >> preference;
                preferences.addPreferredAirline(preference);

                flightGraph.SearchForFlight(src_city, dest_city, date_of_flight, preferences);

                break;
            }
            case 5: {
                flightGraph.displayFullGraph(font);
                break;
            }
            case 6: {
                setConsoleColor(2);
                cout << "Thank you for using SKY NAV AI. Goodbye!\n";
                resetConsoleColor();
                return 0;
            }
            default:
                cout << "Invalid choice. Please select between 1 and 6.\n";
                break;
        }
    } while (true);
}