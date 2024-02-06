#include <iostream>

#define EXIT_PROMPT 0

class Welcome;
class MenuManager;
class Main;

class Welcome {
    public: 
    static void welcome() {
        std::cout << "Welcome to RAM BHAROSE BANK\n";
    }
};

class MenuManager {
    public:
    static void displayMenu();
};

class Main {
    public:
    static void main() {
        // helper variable declarations
        int prompt = 0;

        // render welcome message
        Welcome::welcome();

        do {
            // display menu options
            MenuManager::displayMenu();
            std::cout << "Enter your option: ";
        } while (prompt != EXIT_PROMPT);
    }
};

int main() {
    // code here

    Main::main();

    return 0;
}





