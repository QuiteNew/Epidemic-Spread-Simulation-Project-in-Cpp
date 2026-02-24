#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <random>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <optional>
#include <cmath>

using namespace std;
using namespace sf;

mt19937 rng(static_cast<unsigned>(time(nullptr)));
uniform_real_distribution<double> dist(0.0, 1.0);

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInt(const string& prompt, const string& description, int minVal, int maxVal) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "?") {
            cout << description << endl;
            continue;
        }
        try {
            size_t pos;
            int value = stoi(input, &pos);
            if (pos == input.length() && value >= minVal && value <= maxVal) {
                return value;
            }
        }
        catch (...) {
        }
        cout << "Invalid input. Must be a number between " << minVal << " and " << maxVal << ". Enter ? for help.\n";
    }
}

double getValidDouble(const string& prompt, const string& description, double minVal, double maxVal) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "?") {
            cout << description << endl;
            continue;
        }
        try {
            size_t pos;
            double value = stod(input, &pos);
            if (pos == input.length() && value >= minVal && value <= maxVal) {
                return value;
            }
        }
        catch (...) {
        }
        cout << "Invalid input. Must be a number between " << minVal << " and " << maxVal << ". Enter ? for help.\n";
    }
}

char getValidYN(const string& prompt, const string& description) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "?") {
            cout << description << endl;
            continue;
        }
        if (input.length() == 1) {
            char value = toupper(input[0]);
            if (value == 'Y' || value == 'N') {
                return value;
            }
        }
        cout << "Invalid input. Must be 'y' or 'n'. Enter ? for help.\n";
    }
}

string getValidVirusName(const string& prompt, const string& description) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "?") {
            cout << description << endl;
            continue;
        }
        if (!input.empty()) {
            return input;
        }
        cout << "Invalid input. Virus name cannot be empty. Enter ? for help.\n";
    }
}

string getValidString(const string& prompt, const string& description) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (input == "?") {
            cout << description << endl;
            continue;
        }
        if (!input.empty()) {
            input[0] = toupper(input[0]);
            for (size_t i = 1; i < input.length(); ++i) {
                input[i] = tolower(input[i]);
            }
        }
        if (input == "Susceptible" || input == "Infected" || input == "Recovered" || input == "Dead") {
            return input;
        }
        cout << "Invalid input. Must be Susceptible, Infected, Recovered, or Dead. Enter ? for help.\n";
    }
}

class Person {
private:
    string state;
    int age;
    bool vaccinated;
    bool revaccinated;
    bool reinfected;
    Vector2f position;
    Vector2f velocity;
    CircleShape shape;

public:
    Person(int a, bool vax, bool infected, float x = 0.0f, float y = 0.0f)
        : age(a), vaccinated(vax), revaccinated(false), reinfected(false), position(x, y) {
        state = infected ? "Infected" : "Susceptible";
        shape.setRadius(5.0f);
        shape.setPosition(position);
        updateColor();
        uniform_real_distribution<float> distVel(-1.0f, 1.0f);
        velocity = Vector2f(distVel(rng), distVel(rng));
    }

    void update(float dt, Vector2f windowSize) {
        position += velocity * 50.0f * dt;
        if (position.x < 0 || position.x > windowSize.x - shape.getRadius() * 2) {
            velocity.x = -velocity.x;
            position.x = clamp(position.x, 0.0f, windowSize.x - shape.getRadius() * 2);
        }
        if (position.y < 0 || position.y > windowSize.y - shape.getRadius() * 2) {
            velocity.y = -velocity.y;
            position.y = clamp(position.y, 0.0f, windowSize.y - shape.getRadius() * 2);
        }
        shape.setPosition(position);
    }

    string getState() const { return state; }
    int getAge() const { return age; }
    bool isVaccinated() const { return vaccinated; }
    bool isRevaccinated() const { return revaccinated; }
    bool isReinfected() const { return reinfected; }
    Vector2f getPosition() const { return position; }
    CircleShape getShape() const { return shape; }

    void setState(const string& s) {
        if (s == "Susceptible" || s == "Infected" || s == "Recovered" || s == "Dead") {
            state = s;
            updateColor();
        }
    }
    void setVaccinated(bool v) { vaccinated = v; updateColor(); }
    void setRevaccinated(bool r) { revaccinated = r; updateColor(); }
    void setReinfected(bool r) { reinfected = r; updateColor(); }

    void infect(bool isReinfection) {
        if (state == "Susceptible" || (state == "Recovered" && isReinfection)) {
            state = "Infected";
            updateColor();
            if (isReinfection && !reinfected) {
                reinfected = true;
            }
        }
    }
    void recover() {
        if (state == "Infected") {
            state = "Recovered";
            updateColor();
        }
    }
    void die() {
        if (state == "Infected") {
            state = "Dead";
            updateColor();
        }
    }

private:
    void updateColor() {
        if (vaccinated) {
            shape.setFillColor(sf::Color::Yellow);
        }
        else if (revaccinated) {
            shape.setFillColor(sf::Color::Cyan);
        }
        else {
            if (state == "Susceptible") {
                shape.setFillColor(sf::Color::Blue);
            }
            else if (state == "Infected") {
                shape.setFillColor(reinfected ? sf::Color::Magenta : sf::Color::Red);
            }
            else if (state == "Recovered") {
                shape.setFillColor(sf::Color::Green);
            }
            else if (state == "Dead") {
                shape.setFillColor(sf::Color(128, 128, 128));
            }
        }
        shape.setOutlineThickness(0.0f);
    }

    float clamp(float value, float minVal, float maxVal) {
        return max(minVal, min(maxVal, value));
    }
};

class Virus {
protected:
    string name;
    double infectionRate;
    double recoveryRate;
    double pathogenStrength;
    double mutationChance;
    double mutationStrength;

public:
    Virus(const string& n, double ir, double rr, double ps, double mc, double ms)
        : name(n), infectionRate(ir), recoveryRate(rr), pathogenStrength(ps),
        mutationChance(mc), mutationStrength(ms) {}
    virtual ~Virus() = default;

    virtual void mutate(int day, ofstream& logFile) = 0;

    string getName() const { return name; }
    double getInfectionRate() const { return infectionRate; }
    double getRecoveryRate() const { return recoveryRate; }
    double getPathogenStrength() const { return pathogenStrength; }
    double getMutationChance() const { return mutationChance; }
    double getMutationStrength() const { return mutationStrength; }

    void setInfectionRate(double ir) { if (ir >= 0.0 && ir <= 1.0) infectionRate = ir; }
    void setRecoveryRate(double rr) { if (rr >= 0.0 && rr <= 1.0) recoveryRate = rr; }
    void setPathogenStrength(double ps) { if (ps >= 0.0 && ps <= 1.0) pathogenStrength = ps; }
    void setMutationChance(double mc) { if (mc >= 0.0 && mc <= 1.0) mutationChance = mc; }
    void setMutationStrength(double ms) { if (ms >= 1.0 && ms <= 2.0) mutationStrength = ms; }
};

class AlphaX : public Virus {
public:
    AlphaX() : Virus("Alpha-X", 0.25 + (rng() % 10 - 5) / 100.0, 0.65 + (rng() % 10 - 5) / 100.0,
        0.015 + (rng() % 10 - 5) / 1000.0, 0.07, 1.5) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 3;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: trait = "Infection Rate"; target = &infectionRate; break;
            case 1: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 2: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class Beta9 : public Virus {
public:
    Beta9() : Virus("Beta-9", 0.40 + (rng() % 10 - 5) / 100.0, 0.80 + (rng() % 10 - 5) / 100.0,
        0.005 + (rng() % 4 - 2) / 1000.0, 0.08, 1.4) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 4;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: case 1: trait = "Infection Rate"; target = &infectionRate; break;
            case 2: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 3: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class GammaV : public Virus {
public:
    GammaV() : Virus("Gamma-V", 0.15 + (rng() % 10 - 5) / 100.0, 0.45 + (rng() % 10 - 5) / 100.0,
        0.030 + (rng() % 10 - 5) / 1000.0, 0.06, 1.6) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 4;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: trait = "Infection Rate"; target = &infectionRate; break;
            case 1: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 2: case 3: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class DeltaK : public Virus {
public:
    DeltaK() : Virus("Delta-K", 0.30 + (rng() % 10 - 5) / 100.0, 0.60 + (rng() % 10 - 5) / 100.0,
        0.025 + (rng() % 10 - 5) / 1000.0, 0.07, 1.5) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 4;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: trait = "Infection Rate"; target = &infectionRate; break;
            case 1: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 2: case 3: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class OmegaR : public Virus {
public:
    OmegaR() : Virus("Omega-R", 0.10 + (rng() % 10 - 5) / 100.0, 0.90 + (rng() % 10 - 5) / 100.0,
        0.003 + (rng() % 2 - 1) / 1000.0, 0.05, 1.3) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 3;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: trait = "Infection Rate"; target = &infectionRate; break;
            case 1: case 2: trait = "Recovery Rate"; target = &recoveryRate; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class NovaT : public Virus {
public:
    NovaT() : Virus("Nova-T", 0.45 + (rng() % 10 - 5) / 100.0, 0.55 + (rng() % 10 - 5) / 100.0,
        0.015 + (rng() % 10 - 5) / 1000.0, 0.09, 1.6) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 4;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: case 1: trait = "Infection Rate"; target = &infectionRate; break;
            case 2: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 3: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class CustomVirus : public Virus {
public:
    CustomVirus(const string& n, double ir, double rr, double ps, double mc, double ms)
        : Virus(n, ir, rr, ps, mc, ms) {}
    void mutate(int day, ofstream& logFile) override {
        double mutationRoll = dist(rng);
        if (mutationRoll < mutationChance) {
            double mutationEffect = (rng() % 10 + 5) / 100.0;
            int mutationType = rng() % 3;
            string trait, directionText;
            double* target = nullptr;
            double before = 0.0;

            switch (mutationType) {
            case 0: trait = "Infection Rate"; target = &infectionRate; break;
            case 1: trait = "Recovery Rate"; target = &recoveryRate; break;
            case 2: trait = "Pathogen Strength"; target = &pathogenStrength; break;
            }
            directionText = (rng() % 2) ? "increased" : "decreased";
            before = *target;
            double changeAmount = mutationEffect * (*target) * (mutationStrength - 1.0);
            if (directionText == "increased") *target += changeAmount;
            else *target -= changeAmount;
            *target = max(0.0, min(1.0, *target));

            string mutationOutput = "Mutation occurred on Day " + to_string(day) + "!\n" +
                " " + trait + " has " + directionText + " by " +
                to_string(changeAmount * 100.0) + "%\n" +
                "   Before: " + to_string(before) + " After: " + to_string(*target) + "\n";
            cout << fixed << setprecision(4) << mutationOutput;
            logFile << mutationOutput;
        }
    }
};

class Simulation {
protected:
    vector<Person> population;
    Virus* virus;
    int totalPopulation;
    int initiallyInfected;
    int maxDays;
    int deadCount;
    int reinfectionCount;
    int uniqueReinfectionCount;
    int vaccinationCount;
    int revaccinationCount;

    int getSusceptibleCount() const {
        int count = 0;
        for (const Person& p : population) {
            if (p.getState() == "Susceptible") count++;
        }
        return count;
    }

    int getInfectedCount() const {
        int count = 0;
        for (const Person& p : population) {
            if (p.getState() == "Infected") count++;
        }
        return count;
    }

    void generatePopulation() {
        population.clear();
        uniform_real_distribution<float> distX(0.0f, 800.0f);
        uniform_real_distribution<float> distY(0.0f, 600.0f);
        for (int i = 0; i < totalPopulation; ++i) {
            int age = (rng() % 100) + 1;
            bool vax = false;
            bool infected = i < initiallyInfected;
            float x = distX(rng);
            float y = distY(rng);
            population.emplace_back(age, vax, infected, x, y);
        }
    }

public:
    Simulation(Virus* v, int pop, int infected, int days)
        : virus(v), totalPopulation(pop), initiallyInfected(infected), maxDays(days),
        deadCount(0), reinfectionCount(0), uniqueReinfectionCount(0),
        vaccinationCount(0), revaccinationCount(0) {}
    virtual ~Simulation() { delete virus; }
    virtual void simulate(RenderWindow& window) = 0;
};

class EpidemicSimulation : public Simulation {
private:
    double vaccinationRate;
    double vaccinationStrength;
    double reinfectionRate;
    double reinfectionStrength;
    double revaccinationRate;
    double revaccinationStrength;
    double recoveredRevaccinationRate;
    int fullRecoveryDay;
    int totalBaseRateCount;
    int totalVaccinatedRateCount;
    int totalRevaccinatedRateCount;
    bool endedEarly;
    int mutationCount;
    int mutationIncreaseCount;
    int mutationDecreaseCount;
    vector<int> vaxMilestones;
    int daysWithoutReinfection;
    int noReinfectionDays;
    Font font;
    bool fontLoaded;
    vector<Text> legendTexts;
    vector<RectangleShape> legendColors;

    void initLegend() {
        vector<pair<string, sf::Color>> legendItems = {
            {"Susceptible", sf::Color::Blue},
            {"Infected", sf::Color::Red},
            {"Reinfected", sf::Color::Magenta},
            {"Recovered", sf::Color::Green},
            {"Dead", sf::Color(128, 128, 128)},
            {"Vaccinated", sf::Color::Yellow},
            {"Revaccinated", sf::Color::Cyan}
        };

        float yPos = 10.0f;
        const float spacing = 20.0f;
        const float boxSize = 10.0f;

        for (const auto& item : legendItems) {
            RectangleShape box(sf::Vector2f(boxSize, boxSize));
            box.setPosition(sf::Vector2f(10.0f, yPos + 2.0f));
            box.setFillColor(item.second);
            legendColors.push_back(box);
            yPos += spacing;
        }
    }

    void checkVaxMilestones(int day) {
        int vaccinatedCount = 0;
        for (const Person& p : population) {
            if (p.isVaccinated()) vaccinatedCount++;
        }
        double percent = (double(vaccinatedCount) / totalPopulation) * 100.0;

        if (vaxMilestones[0] == -1 && percent >= 10.0) vaxMilestones[0] = day;
        if (vaxMilestones[1] == -1 && percent >= 20.0) vaxMilestones[1] = day;
        if (vaxMilestones[2] == -1 && percent >= 50.0) vaxMilestones[2] = day;
        if (vaxMilestones[3] == -1 && percent >= 80.0) vaxMilestones[3] = day;
        if (vaxMilestones[4] == -1 && percent >= 100.0) vaxMilestones[4] = day;
    }

    void runDay(ofstream& logFile, int day, bool& reinfectionOccurred, int& dailyReinfectionCount) {
        int susceptibleCount = 0, infectedCount = 0, recoveredCount = 0;

        if (vaccinationRate > 0.0) {
            vector<int> susceptibleIndices;
            for (int i = 0; i < population.size(); ++i) {
                if (population[i].getState() == "Susceptible" && !population[i].isVaccinated()) {
                    susceptibleIndices.push_back(i);
                }
            }
            shuffle(susceptibleIndices.begin(), susceptibleIndices.end(), rng);

            int toVaccinate = static_cast<int>(susceptibleIndices.size() * vaccinationRate);
            for (int i = 0; i < toVaccinate && i < susceptibleIndices.size(); ++i) {
                population[susceptibleIndices[i]].setVaccinated(true);
                vaccinationCount++;
            }
        }

        if (revaccinationRate > 0.0) {
            vector<int> reinfectedIndices;
            for (int i = 0; i < population.size(); ++i) {
                if (population[i].getState() == "Infected" && population[i].isReinfected() && !population[i].isRevaccinated()) {
                    reinfectedIndices.push_back(i);
                }
            }
            shuffle(reinfectedIndices.begin(), reinfectedIndices.end(), rng);

            int toRevaccinate = static_cast<int>(reinfectedIndices.size() * revaccinationRate);
            for (int i = 0; i < toRevaccinate && i < reinfectedIndices.size(); ++i) {
                if (!population[reinfectedIndices[i]].isVaccinated()) {
                    population[reinfectedIndices[i]].setVaccinated(true);
                }
                population[reinfectedIndices[i]].setRevaccinated(true);
                revaccinationCount++;
            }

            vector<int> infectedIndices;
            for (int i = 0; i < population.size(); ++i) {
                if (population[i].getState() == "Infected" && !population[i].isRevaccinated()) {
                    infectedIndices.push_back(i);
                }
            }
            shuffle(infectedIndices.begin(), infectedIndices.end(), rng);

            toRevaccinate = static_cast<int>(infectedIndices.size() * revaccinationRate);
            for (int i = 0; i < toRevaccinate && i < infectedIndices.size(); ++i) {
                if (!population[infectedIndices[i]].isVaccinated()) {
                    population[infectedIndices[i]].setVaccinated(true);
                }
                population[infectedIndices[i]].setRevaccinated(true);
                revaccinationCount++;
            }
        }

        int revaccinatedRecoveredCount = 0;
        if (recoveredRevaccinationRate > 0.0) {
            vector<int> recoveredIndices;
            for (int i = 0; i < population.size(); ++i) {
                if (population[i].getState() == "Recovered" && !population[i].isRevaccinated()) {
                    recoveredIndices.push_back(i);
                }
            }
            shuffle(recoveredIndices.begin(), recoveredIndices.end(), rng);

            int toRevaccinate = static_cast<int>(recoveredIndices.size() * recoveredRevaccinationRate);
            for (int i = 0; i < toRevaccinate && i < recoveredIndices.size(); ++i) {
                if (!population[recoveredIndices[i]].isVaccinated()) {
                    population[recoveredIndices[i]].setVaccinated(true);
                }
                population[recoveredIndices[i]].setRevaccinated(true);
                revaccinationCount++;
                revaccinatedRecoveredCount++;
            }
        }

        int totalRevaccinatedRecovered = 0;
        for (const Person& p : population) {
            if (p.getState() == "Recovered" && p.isRevaccinated()) {
                totalRevaccinatedRecovered++;
            }
        }
        cout << "Day " << day << ": " << totalRevaccinatedRecovered << " recovered individuals are revaccinated.\n";

        double baseReinfectionRate = virus->getInfectionRate() * reinfectionRate * 0.01;
        double vaccinatedReinfectionRate = baseReinfectionRate * (1.0 - vaccinationStrength);
        double revaccinatedReinfectionRate = vaccinatedReinfectionRate * (1.0 - revaccinationStrength);
        int baseRateCount = 0, vaccinatedRateCount = 0, revaccinatedRateCount = 0;
        if (reinfectionRate > 0.0 && day == 1) {
            cout << "Day " << day << ": Base reinfection rate = " << fixed << setprecision(6) << baseReinfectionRate
                << ", Vaccinated = " << vaccinatedReinfectionRate
                << ", Revaccinated = " << revaccinatedReinfectionRate << endl;
        }
        if (reinfectionRate > 0.0) {
            for (Person& p : population) {
                if (p.getState() == "Recovered") {
                    double adjustedReinfectionRate = baseReinfectionRate;
                    string rateUsed = "base";
                    if (p.isRevaccinated()) {
                        adjustedReinfectionRate = revaccinatedReinfectionRate;
                        rateUsed = "revaccinated";
                        revaccinatedRateCount++;
                        totalRevaccinatedRateCount++;
                    }
                    else if (p.isVaccinated()) {
                        adjustedReinfectionRate = vaccinatedReinfectionRate;
                        rateUsed = "vaccinated";
                        vaccinatedRateCount++;
                        totalVaccinatedRateCount++;
                    }
                    else {
                        baseRateCount++;
                        totalBaseRateCount++;
                    }
                    double reinfectionChance = dist(rng);
                    if (reinfectionChance < adjustedReinfectionRate) {
                        p.infect(true);
                        if (!p.isReinfected()) {
                            uniqueReinfectionCount++;
                        }
                        reinfectionCount++;
                        dailyReinfectionCount++;
                        reinfectionOccurred = true;
                        logFile << "Day " << day << ": Reinfection occurred with " << rateUsed << " rate ("
                            << fixed << setprecision(6) << adjustedReinfectionRate
                            << "), chance = " << reinfectionChance << ".\n";
                    }
                }
            }
        }

        for (Person& p : population) {
            if (p.getState() == "Infected") {
                double deathChance = dist(rng);
                double adjustedMortality = virus->getPathogenStrength();
                if (p.getAge() <= 18) {
                    adjustedMortality *= 0.5;
                }
                else if (p.getAge() <= 40) {
                    adjustedMortality *= 0.75;
                }
                else if (p.getAge() <= 65) {
                    adjustedMortality *= 1.0;
                }
                else {
                    adjustedMortality *= 1.5;
                }
                if (p.isVaccinated()) {
                    adjustedMortality *= (1.0 - vaccinationStrength);
                }
                if (reinfectionRate > 0.0 && reinfectionStrength > 0.0 && p.isReinfected()) {
                    adjustedMortality *= (1.0 - reinfectionStrength);
                }
                if (p.isRevaccinated()) {
                    adjustedMortality *= (1.0 - revaccinationStrength);
                }
                if (deathChance < adjustedMortality) {
                    p.die();
                    deadCount++;
                    continue;
                }
                double recoveryChance = dist(rng);
                if (recoveryChance < virus->getRecoveryRate()) {
                    p.recover();
                }
            }
        }

        for (size_t i = 0; i < population.size(); ++i) {
            if (population[i].getState() == "Infected") {
                for (size_t j = 0; j < population.size(); ++j) {
                    if (population[j].getState() == "Susceptible") {
                        Vector2f diff = population[i].getPosition() - population[j].getPosition();
                        float distance = sqrt(diff.x * diff.x + diff.y * diff.y);
                        if (distance < 20.0f) {
                            double adjustedInfectionRate = virus->getInfectionRate();
                            if (population[j].getAge() <= 18) {
                                adjustedInfectionRate *= 0.5;
                            }
                            else if (population[j].getAge() <= 40) {
                                adjustedInfectionRate *= 0.75;
                            }
                            else if (population[j].getAge() <= 65) {
                                adjustedInfectionRate *= 1.0;
                            }
                            else {
                                adjustedInfectionRate *= 1.25;
                            }
                            if (population[j].isVaccinated()) {
                                adjustedInfectionRate *= (1.0 - vaccinationStrength);
                            }
                            if (population[j].isRevaccinated()) {
                                adjustedInfectionRate *= (1.0 - revaccinationStrength);
                            }
                            if (dist(rng) < adjustedInfectionRate) {
                                population[j].infect(false);
                            }
                        }
                    }
                }
            }
        }

        for (const Person& p : population) {
            if (p.getState() == "Susceptible") susceptibleCount++;
            else if (p.getState() == "Infected") infectedCount++;
            else if (p.getState() == "Recovered") recoveredCount++;
        }

        string output = "Day " + to_string(day) +
            ": Susceptible=" + to_string(susceptibleCount) +
            ", Infected=" + to_string(infectedCount) +
            ", Recovered=" + to_string(recoveredCount) +
            ", Dead=" + to_string(deadCount);

        cout << output << endl;
        logFile << output << endl;

        cout << "Day " << day << ": [";
        int barWidth = 30;
        int progress = (day * barWidth) / maxDays;
        for (int i = 0; i < barWidth; ++i)
            cout << (i < progress ? '=' : ' ');
        cout << "] " << day << "/" << maxDays << endl;
    }

    void searchAgeGroups() {
        int ageGroup18 = 0, ageGroup40 = 0, ageGroup65 = 0, ageGroupAbove65 = 0;
        for (const Person& p : population) {
            if (p.getAge() <= 18) ageGroup18++;
            else if (p.getAge() <= 40) ageGroup40++;
            else if (p.getAge() <= 65) ageGroup65++;
            else ageGroupAbove65++;
        }
        cout << "\n--- Age Group Summary ---\n";
        cout << "Age 1-18: " << ageGroup18 << endl;
        cout << "Age 18-40: " << ageGroup40 << endl;
        cout << "Age 40-65: " << ageGroup65 << endl;
        cout << "Age 65+: " << ageGroupAbove65 << endl;
    }

    void searchPopulation(string targetState) {
        int minAge = getValidInt(
            "Enter minimum age to filter: ",
            "The minimum age for filtering individuals. Must be a number between 0 and 100.",
            0, 100
        );
        int maxAge = getValidInt(
            "Enter maximum age to filter: ",
            "The maximum age for filtering individuals. Must be a number greater than the minimum age (" + to_string(minAge) + ") and up to 100.",
            minAge + 1, 100
        );

        int totalCount = 0;
        for (const Person& p : population) {
            if (p.getState() == targetState && p.getAge() >= minAge && p.getAge() <= maxAge) {
                totalCount++;
            }
        }

        cout << "\nPeople with state " << targetState
            << " and age in range [" << minAge << "-" << maxAge << "]\n";
        if (totalCount > 0) {
            cout << "Total individuals: " << totalCount << endl;
        }
        else {
            cout << "No individuals found matching criteria.\n";
        }
    }

    void reportSummary() {
        int susceptible = 0, infected = 0, recovered = 0;
        int vaccinatedIndividuals = 0, revaccinatedIndividuals = 0;
        for (const Person& p : population) {
            if (p.getState() == "Susceptible") susceptible++;
            else if (p.getState() == "Infected") infected++;
            else if (p.getState() == "Recovered") recovered++;
            if (p.isVaccinated()) vaccinatedIndividuals++;
            if (p.isRevaccinated()) revaccinatedIndividuals++;
        }

        cout << "\n--- Vaccination Report ---\n";
        if (vaccinationRate == 0.0 && vaccinationStrength == 0.0) {
            cout << "Vaccination mechanic not selected.\n";
        }
        else {
            cout << "Total Vaccinations: " << vaccinationCount << endl;
            cout << "Number of Vaccinated Individuals: " << vaccinatedIndividuals << endl;
            const char* milestones[] = { "10%", "20%", "50%", "80%", "100%" };
            for (int i = 0; i < 5; ++i) {
                if (vaxMilestones[i] != -1) {
                    cout << "Vaccination reached " << milestones[i] << " on Day " << vaxMilestones[i] << ".\n";
                }
                else {
                    cout << "Vaccination never reached " << milestones[i] << ".\n";
                }
            }
            if (revaccinationRate > 0.0 && revaccinationStrength > 0.0) {
                cout << "Total Revaccinations: " << revaccinationCount << endl;
                cout << "Number of Revaccinated Individuals: " << revaccinatedIndividuals << endl;
            }
        }

        cout << "\n--- Virus Mutation Summary ---\n";
        if (virus->getMutationChance() > 0.0 && virus->getMutationStrength() > 1.0) {
            cout << "Total Mutations: " << mutationCount << endl;
            cout << "Value Increases: " << mutationIncreaseCount << endl;
            cout << "Value Decreases: " << mutationDecreaseCount << endl;
        }
        else {
            cout << "Virus mutation mechanic not selected.\n";
        }

        cout << "\n--- Reinfection Report ---\n";
        if (reinfectionRate == 0.0 && reinfectionStrength == 0.0) {
            cout << "Reinfection mechanic not selected.\n";
        }
        else {
            cout << "Total Reinfections: " << reinfectionCount << endl;
            cout << "Total Reinfected Individuals: " << uniqueReinfectionCount << endl;
        }

        cout << "\n--- Summary Report ---\n";
        cout << "Final Susceptible: " << susceptible << endl;
        cout << "Final Infected: " << infected << endl;
        cout << "Final Recovered: " << recovered << endl;
        cout << "Total Dead: " << deadCount << endl;
        if (vaccinationRate > 0.0 && vaccinationStrength > 0.0) {
            cout << "Total Vaccinations: " << vaccinationCount << endl;
        }

        if (reinfectionRate > 0.0 && reinfectionStrength > 0.0 && revaccinationRate > 0.0 && revaccinationStrength > 0.0) {
            cout << "Total Revaccinations: " << revaccinationCount << endl;
        }

        double mortalityPercentage = (double(deadCount) / totalPopulation) * 100.0;
        cout << "Mortality Rate: " << fixed << setprecision(4) << mortalityPercentage << "%\n";

        if (fullRecoveryDay != -1) {
            if (daysWithoutReinfection >= noReinfectionDays && reinfectionRate > 0.0) {
                cout << "Simulation ended early due to no significant reinfections and low infected count for " << noReinfectionDays << " days on Day " << fullRecoveryDay << ".\n";
            }
            else {
                cout << "All individuals recovered or died by Day " << fullRecoveryDay << ".\n";
            }
        }
        else {
            cout << "Not all individuals recovered within the simulation period.\n";
        }

        searchAgeGroups();

        string searchState = getValidString(
            "Enter state to search for (Susceptible, Infected, Recovered, Dead): ",
            "The health state to filter individuals by in the population search. Valid states are Susceptible, Infected, Recovered, or Dead."
        );
        searchPopulation(searchState);
    }

public:
    EpidemicSimulation(Virus* v, int pop, int infected, int days, double vaxRate, double vaxStrength,
        double reinfRate, double reinfStrength, double revaxRate, double revaxStrength,
        double recRevaxRate, int noReinfDays)
        : Simulation(v, pop, infected, days),
        vaccinationRate(vaxRate), vaccinationStrength(vaxStrength),
        reinfectionRate(reinfRate), reinfectionStrength(reinfStrength),
        revaccinationRate(revaxRate), revaccinationStrength(revaxStrength),
        recoveredRevaccinationRate(recRevaxRate),
        fullRecoveryDay(-1), totalBaseRateCount(0), totalVaccinatedRateCount(0),
        totalRevaccinatedRateCount(0), endedEarly(false), mutationCount(0),
        mutationIncreaseCount(0), mutationDecreaseCount(0),
        vaxMilestones(vector<int>(5, -1)), daysWithoutReinfection(0),
        noReinfectionDays(noReinfDays), fontLoaded(false) {
        generatePopulation();
        initLegend();
    }

    void simulate(RenderWindow& window) override {
        ofstream logFile("simulation_output.txt");
        int mutationCheckInterval = max(1, maxDays / 10);
        int day = 1;
        bool paused = false;
        float simulationSpeed = 1.0f;
        bool stepDay = false;
        float timeSinceLastDay = 0.0f;
        Clock clock;

        while (window.isOpen() && day <= maxDays) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<Event::Closed>()) {
                    window.close();
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
                window.close();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Space)) {
                paused = !paused;
                while (Keyboard::isKeyPressed(Keyboard::Key::Space)) {}
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Equal)) {
                simulationSpeed += 0.1f;
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Hyphen)) {
                simulationSpeed = max(0.1f, simulationSpeed - 0.1f);
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::S)) {
                stepDay = true;
                while (Keyboard::isKeyPressed(Keyboard::Key::S)) {}
            }

            float dt = clock.restart().asSeconds() * simulationSpeed;
            if (!paused || stepDay) {
                for (auto& person : population) {
                    person.update(dt, Vector2f(800.0f, 600.0f));
                }

                timeSinceLastDay += dt;
                if (timeSinceLastDay >= 1.0f || stepDay) {
                    bool reinfectionOccurred = false;
                    int dailyReinfectionCount = 0;
                    if (virus->getMutationChance() > 0.0 && virus->getMutationStrength() > 1.0 && day % mutationCheckInterval == 0) {
                        virus->mutate(day, logFile);
                        mutationCount++;
                        mutationIncreaseCount += (rng() % 2) ? 1 : 0;
                        mutationDecreaseCount += (rng() % 2) ? 0 : 1;
                    }
                    runDay(logFile, day, reinfectionOccurred, dailyReinfectionCount);
                    checkVaxMilestones(day);
                    int recoveredCount = 0;
                    for (const Person& p : population) {
                        if (p.getState() == "Recovered") recoveredCount++;
                    }
                    cout << "Day " << day << ": " << dailyReinfectionCount << " reinfections occurred.\n";
                    if (reinfectionRate > 0.0) {
                        if (reinfectionOccurred && dailyReinfectionCount > 0.0001 * totalPopulation) {
                            cout << "Day " << day << ": " << dailyReinfectionCount << " reinfections occurred, resetting days without reinfection.\n";
                            daysWithoutReinfection = 0;
                        }
                        else if (recoveredCount > totalPopulation * 0.01 && getInfectedCount() < totalPopulation * 0.01) {
                            daysWithoutReinfection++;
                            cout << "Day " << day << ": " << dailyReinfectionCount << " reinfections, days without reinfection = " << daysWithoutReinfection << ".\n";
                            if (daysWithoutReinfection >= noReinfectionDays) {
                                cout << "No significant reinfections and low infected count for " << noReinfectionDays << " days, ending on Day " << day << ".\n";
                                fullRecoveryDay = day;
                                endedEarly = true;
                                break;
                            }
                        }
                        else {
                            daysWithoutReinfection = 0;
                            cout << "Day " << day << ": " << dailyReinfectionCount << " reinfections, infected count too high to increment days without reinfection.\n";
                        }
                    }
                    if (fullRecoveryDay != -1 || (getInfectedCount() == 0 && getSusceptibleCount() == 0)) {
                        endedEarly = true;
                        break;
                    }

                    day++;
                    timeSinceLastDay = 0.0f;
                    stepDay = false;
                }
            }

            window.clear(Color::Black);
            for (const auto& person : population) {
                window.draw(person.getShape());
            }
            if (fontLoaded) {
                for (const auto& box : legendColors) {
                    window.draw(box);
                }
                for (const auto& text : legendTexts) {
                    window.draw(text);
                }
            }
            window.display();
        }
        logFile.close();
        reportSummary();
    }
};

vector<Virus*> generatePredefinedViruses() {
    return {
        new AlphaX(),
        new Beta9(),
        new GammaV(),
        new DeltaK(),
        new OmegaR(),
        new NovaT()
    };
}

int main() {
    vector<Virus*> viruses = generatePredefinedViruses();
    cout << "Select a virus:\n";
    for (int i = 0; i < viruses.size(); ++i) {
        cout << i + 1 << ". " << viruses[i]->getName() << endl;
    }
    cout << "7. Custom virus\n";
    int virusChoice = getValidInt(
        "Enter your choice of virus (1-7): ",
        "Select a virus to simulate. Choose 1-6 for predefined viruses with set characteristics, or 7 to create a custom virus with user-defined properties.",
        1, 7
    );

    Virus* selectedVirus = nullptr;

    if (virusChoice == 7) {
        string name = getValidVirusName(
            "Enter custom virus name: ",
            "The name of your custom virus. Can be any non-empty string."
        );
        double infectionRate = getValidDouble(
            "Enter the infection rate (0.0 - 1.0): ",
            "The probability (0.0 to 1.0) that a susceptible person becomes infected each day.",
            0.0, 1.0
        );
        double recoveryRate = getValidDouble(
            "Enter the recovery rate (0.0 - 1.0): ",
            "The probability (0.0 to 1.0) that an infected person recovers each day.",
            0.0, 1.0
        );
        double pathogenStrength = getValidDouble(
            "Enter the Pathogen Strength (0.0 - 1.0): ",
            "The probability (0.0 to 1.0) that an infected person dies each day.",
            0.0, 1.0
        );
        selectedVirus = new CustomVirus(name, infectionRate, recoveryRate, pathogenStrength, 0.0, 1.0);
    }
    else {
        selectedVirus = viruses[virusChoice - 1];
    }

    char enableMutation = getValidYN(
        "Do you want to enable virus mutation? (y/n): ",
        "Choose whether to enable virus mutation (y/n)."
    );

    if (enableMutation == 'Y') {
        selectedVirus->setMutationChance(getValidDouble(
            "Enter the Virus Mutation Chance (0.0 - 1.0): ",
            "The probability (0.0 to 1.0) that the virus mutates every few days.",
            0.0, 1.0
        ));
        selectedVirus->setMutationStrength(getValidDouble(
            "Enter the Virus Mutation Strength (1.0 - 2.0): ",
            "The factor (1.0 to 2.0) by which a mutation changes the virus's properties.",
            1.0, 2.0
        ));
    }

    char enableVax = getValidYN(
        "Do you want to enable vaccination? (y/n): ",
        "Choose whether to enable vaccination (y/n)."
    );

    double vaccinationRate = 0.0, vaccinationStrength = 0.0;
    double reinfectionRate = 0.0, reinfectionStrength = 0.0;
    double revaccinationRate = 0.0, revaccinationStrength = 0.0;
    double recoveredRevaccinationRate = 0.0;
    int noReinfectionDays = 0;

    if (enableVax == 'Y') {
        vaccinationRate = getValidDouble(
            "Enter vaccination rate (0.01 - 0.10): ",
            "The fraction (0.01 to 0.10) of susceptible individuals vaccinated each day.",
            0.01, 0.10
        );
        vaccinationStrength = getValidDouble(
            "Enter vaccination strength (0.01 - 0.50): ",
            "The effectiveness (0.01 to 0.50) of the vaccine.",
            0.01, 0.50
        );
    }

    char enableReinfection = getValidYN(
        "Do you want to enable reinfection? (y/n): ",
        "Choose whether to enable reinfection (y/n)."
    );

    if (enableReinfection == 'Y') {
        reinfectionRate = getValidDouble(
            "Enter reinfection rate (0.01 - 0.10): ",
            "The fraction (0.01 to 0.10) of the virus's infection rate for reinfection.",
            0.01, 0.10
        );
        reinfectionStrength = getValidDouble(
            "Enter reinfection strength (0.01 - 0.10): ",
            "The reduction (0.01 to 0.10) in mortality rate for reinfected individuals.",
            0.01, 0.10
        );

        noReinfectionDays = getValidInt(
            "Enter days with no reinfections to end (3 - 10): ",
            "The number of consecutive days with no significant reinfections to end the simulation.",
            3, 10
        );

        if (enableVax == 'Y') {
            char enableRevax = getValidYN(
                "Do you want to enable revaccination for reinfected individuals? (y/n): ",
                "Choose whether to enable revaccination (y/n)."
            );
            if (enableRevax == 'Y') {
                revaccinationRate = getValidDouble(
                    "Enter revaccination rate (0.01 - 0.20): ",
                    "The fraction (0.01 to 0.20) of infected individuals revaccinated each day.",
                    0.01, 0.20
                );
                revaccinationStrength = getValidDouble(
                    "Enter revaccination strength (0.01 - 0.75): ",
                    "The effectiveness (0.01 to 0.75) of revaccination.",
                    0.01, 0.75
                );
                recoveredRevaccinationRate = getValidDouble(
                    "Enter recovered revaccination rate (0.01 - 0.20): ",
                    "The fraction (0.01 to 0.20) of recovered individuals revaccinated each day.",
                    0.01, 0.20
                );
            }
        }
    }

    int populationSize = getValidInt(
        "Enter population size (1-1000): ",
        "The total number of individuals in the simulation (max 1000 for visualization).",
        1, 1000
    );
    if (populationSize < 100) {
        cout << "Warning: Small population (<100) may lead to early termination.\n";
    }
    int initiallyInfected = getValidInt(
        "Enter initially infected population (1 - " + to_string(populationSize) + "): ",
        "The number of individuals infected at the start.",
        1, populationSize
    );
    int maxDays = getValidInt(
        "Enter number of days to simulate (positive integer): ",
        "The duration of the simulation in days.",
        1, numeric_limits<int>::max()
    );

    cout << "\n--- Color Legend ---\n";
    cout << "Susceptible: Blue\n";
    cout << "Infected: Red\n";
    cout << "Reinfected: Magenta\n";
    cout << "Recovered: Green\n";
    cout << "Dead: Gray\n";
    cout << "Vaccinated: Yellow\n";
    cout << "Revaccinated: Cyan\n";
    cout << "-------------------\n\n";

    RenderWindow window(VideoMode({ 800, 600 }), "Epidemic Simulation", Style::Default);
    Simulation* simulation = new EpidemicSimulation(selectedVirus, populationSize, initiallyInfected, maxDays,
        vaccinationRate, vaccinationStrength, reinfectionRate, reinfectionStrength,
        revaccinationRate, revaccinationStrength, recoveredRevaccinationRate, noReinfectionDays);
    simulation->simulate(window);

    delete simulation;
    for (Virus* v : viruses) {
        if (v != selectedVirus) delete v;
    }
    if (virusChoice == 7) delete selectedVirus;

    return 0;
}