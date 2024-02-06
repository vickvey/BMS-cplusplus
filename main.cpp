#include <iostream>
#include <set>
#include <list>
#include <string>
#include <cstdlib>
#include <limits>
#include <random>
#include <utility>
#include <algorithm>
#include <climits>


class Welcome;
class ConstantManager;
class InputManager;
class RandomNumberGenerator;
class ScreenManager;
class AccountNumberManager;
class Account;
class AccountManager;
class LoginMenuManger;
class AccountCreater;
class LoginManager;
class AccountRemover;
class Initializer;
class Main;

// Done
class ConstantManager {
    public: 
    static constexpr const char BANK_NAME[] = "RAM BHAROSE BANK";

    static constexpr int ACCOUNT_NUMBER_LENGTH = 6;
    static constexpr int PIN_LENGTH = 4;  

    static constexpr double MIN_INIT_AMOUNT_DEPOSIT = 500;
    static constexpr double MAX_BALANCE = 1e8;

    static constexpr int MIN_PIN = 1000;
    static constexpr int MAX_PIN = 9999;

    static constexpr int LOGIN_ATTEMPTS = 3;

    enum STATUS {
        SUCCESS = 0,
        FAILURE = 1
    };
};

class InputManager {
public:
    static int get_number_input(int attempts, int minm, int maxm, std::string prompt, std::string err_warning);

    static int get_number_input(int digits, std::string prompt, std::string err_warning);

    static int get_number_input(int minm, int maxm, std::string prompt, std::string err_warning);

    static double get_real_input(double low, double high, std::string prompt, std::string err_warning);

    static bool get_bool_input(std::string prompt, std::string warning);

private:
    static int count_digits(int number) {
        int count = 0;
        while (number != 0) {
            number /= 10;
            ++count;
        }
        return count;
    }
};

class ScreenManager {
public:
    static void clear_screen() {
        #ifdef _WIN32
            std::system("cls"); // For Windows
        #else
            std::system("clear"); // For Unix/Linux/Mac
        #endif
    }

    static void hold_screen() {
        std::cout << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// Done 
class RandomNumberGenerator {
public:
    static int get_random_number(int digits) {
        if (digits <= 0) {
            std::cerr << "Error: Number of digits should be greater than 0." 
                      << std::endl;

            // You can handle errors according to your application's logic
            return -1; 
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> 
            distribution(pow(10, digits - 1), pow(10, digits) - 1);

        return distribution(gen);
    }
};

// done but contains intialize() and then can be expanded
class AccountNumberManager {
    using AccountNumberSet = std::set<int>;
    static constexpr int ACC_NUM_DIGITS = ConstantManager::ACCOUNT_NUMBER_LENGTH;

    public:
    static AccountNumberSet acc_num_set;

    static bool check_existing_acc_num(int acc_num) {
        if(acc_num_set.find(acc_num) != acc_num_set.end()) 
            return true;
        return false;
    }

    static int get_fresh_acc_num() {
        int acc_num = RandomNumberGenerator::get_random_number(ACC_NUM_DIGITS);
        while(check_existing_acc_num(acc_num) == true) {
            acc_num = RandomNumberGenerator::get_random_number(ACC_NUM_DIGITS);
        }

        acc_num_set.insert(acc_num);
        return acc_num;
    }

    static void initialize() {
        // Perform any necessary initialization for AccountNumberManager here
        // For example, you might want to clear the set or load data from a file.
        acc_num_set.clear();
    }
};

// 
class Account {
    public:
    static constexpr int ACCOUNT_NUMBER_LENGTH = ConstantManager::ACCOUNT_NUMBER_LENGTH;
    static constexpr int PIN_LENGTH = ConstantManager::PIN_LENGTH;  
    static constexpr double MAX_BALANCE = ConstantManager::MAX_BALANCE;

    int acc_num;
    double balance;
    bool is_valid;
    
    enum ACCOUNT_STATUS {
        SUCCESS = ConstantManager::STATUS::SUCCESS, 
        FAILURE = ConstantManager::STATUS::FAILURE
    };

    // Constructor
    explicit Account(int acc_num, double balance, int pin):
        acc_num(acc_num), balance(balance), is_valid(true), pin(pin) {}

    // Constructor for an invalid account (null-like state)
    Account(): acc_num(-1), is_valid(false) {}

    // Member function to check if the account is valid
    bool is_valid_account() const {
        auto calculate_digits = [](int number) -> int {
            return static_cast<int>(std::floor(std::log10(std::abs(number)))) + 1;
        };

        if (calculate_digits(acc_num) != ACCOUNT_NUMBER_LENGTH 
            || calculate_digits(pin) != PIN_LENGTH) 
        {
            return false;
        }

        if (!(balance >= 0 && balance <= MAX_BALANCE)) {
            return false;
        }

        return true;
    }

    void set_pin(int pin) {
        this->pin = pin;
    }

    int get_pin() const {
        return this->pin;
    }

    void make_deposit(double amount) {
        this->balance += amount;
    }

    // returns ACCOUNT_STATUS::FAILURE if withdrawal is not possible, 
    // ACCOUNT_STATUS::SUCCESS otherwise
    int make_withdraw(double amount) {
        if(this->balance > amount) {
            this->balance -= amount;
            return ACCOUNT_STATUS::SUCCESS;
        }
        return ACCOUNT_STATUS::FAILURE;
    }

    /**
     * returns ACCOUNT_STATUS::FAILURE if you don't have that much balance
     * returns ACCOUNT_STATUS::SUCCESS if the transaction is successful
     */
    int make_transfer(Account& beneficiary, double amount) {
        if (make_withdraw(amount) == 0) {
            beneficiary.make_deposit(amount);
            return ACCOUNT_STATUS::SUCCESS;   
        }
        return ACCOUNT_STATUS::FAILURE;
    }

    void check_balance() {
        std::cout << "Curr balance: Rs " << this->balance << std::endl;
    }

    void display_account() const {
        std::cout << "Here are your recorded account details: \n";
        std::cout << "Account number: " << this->acc_num << std::endl;
        std::cout << "Balance: Rs " << this->balance << std::endl;
    }

    private:
    int pin;
};

class AccountManager {
    using AccountList = std::list<Account>;
    
    public:
    static AccountList accounts;

    static void add_account(const Account& account) {
        accounts.push_back(account);
    }

    static Account *get_account(int acc_num) {
        for(auto& it: accounts) {
            if(it.acc_num == acc_num) {
                return &it;
            }
        }
        return NULL;
    }

    static void delete_account(int acc_num) {
        for (auto it = accounts.begin(); it != accounts.end(); /* no increment here*/) {
            if (it->acc_num == acc_num) {
                // Erase the element and get the iterator to the next element
                it = accounts.erase(it); 
            } else {
                // Increment the iterator if no erase occurs
                ++it; 
            }
        }
    }

    static const Account &get_account_read_only(int acc_num) {
        for (const auto& it : accounts) {
            if (it.acc_num == acc_num) {
                return it;
            }
        }

        // Return a reference to a static null account if not found
        static Account acc_null;
        return acc_null;
    }

    static bool verify_existing_account(int acc_num, int pin) {
        for(const auto &it: accounts) {
            if(it.acc_num == acc_num && it.get_pin() == pin) {
                return true;
            }
        }
        return false;
    }

    static void initialize() {
        // Perform any necessary initialization for AccountNumberManager here
        // For example, you might want to clear the set or load data from a file.
        accounts.clear();
    }
};

class Welcome {
public:
    static constexpr const char *BANK_NAME = ConstantManager::BANK_NAME;

    static void welcome() {
        ScreenManager::clear_screen();
        std::cout << "Welcome to " << BANK_NAME << "!!" << std::endl;
        ScreenManager::hold_screen();
    }
};

class LoginMenuManger {
    public:
    static constexpr const char MENU_OPTIONS[] = 
        "Press 1: To create a new account.\n"
        "Press 2: To login into an existing account.\n"
        "Press 3: To delete an existing account.\n"
        "Press 4: To exit the portal!!\n";

    static void show_menu_options() {
        std::cout << MENU_OPTIONS << std::endl;
    }

    static int get_prompt_from_login_menu() {
        int prompt = 0;

        ScreenManager::clear_screen();
        show_menu_options();
        std::cin >> prompt;

        return prompt;
    }
};

// TODO: complete this function
class AccountCreater {
    public:
    static constexpr double MIN_BALANCE = ConstantManager::MIN_INIT_AMOUNT_DEPOSIT;
    static constexpr double MAX_BALANCE = ConstantManager::MAX_BALANCE;

    static void create_account() {
        // here you can use the InputManager class to get the input from the user
        printf("Account creation page: \n");
        int acc_num = AccountNumberManager::get_fresh_acc_num();
        
        puts("Please enter your pin carefully!!.\n");
        int pin = InputManager::get_number_input(4, "Enter your pin: ", "Invalid pin!!\n");

        puts("Please enter the intial deposit amount!!\n");
        double balance = InputManager::get_real_input
            (MIN_BALANCE, MAX_BALANCE, "Enter your initial amount to deposit: ",
                "Invalid amount deposit!!\n");

        Account new_account(acc_num, balance, pin);
        AccountManager::add_account(new_account);

        puts("Account created successfully!!\n");
    }
};

/// TODO: 
class LoginManager {
    static constexpr int ATTEMPTS = ConstantManager::LOGIN_ATTEMPTS;
    static constexpr int DIGITS_IN_PIN = ConstantManager::PIN_LENGTH;
    static constexpr int MIN_PIN = ConstantManager::MIN_PIN;
    static constexpr int MAX_PIN = ConstantManager::MAX_PIN;

    enum LOGIN_STATUS {
        SUCCESS = ConstantManager::STATUS::SUCCESS, 
        FAILURE = ConstantManager::STATUS::FAILURE
    };
    
    public:
    static bool prompt_login() {
        puts("Login page: \n");

        std::cout << "Enter your existing account number: ";
        int acc_num = InputManager::get_number_input
            (Account::ACCOUNT_NUMBER_LENGTH, "Enter your account number: ", "Invalid account number!!\n");

        std::cout << "Enter your pin: ";
        /// TODO: write the pin function and complete the function
        int pin = 
        InputManager::get_number_input (
            ATTEMPTS, 
            MIN_PIN, 
            MAX_PIN, 
            "Enter your pin: ",
            "Invalid PIN"
        );

        // calling another class AccountManager's functions
        /// TODO: Create a verify account method in AccountManager and then come back
        if(AccountManager::verify_existing_account(acc_num, pin) == false) {
            puts("Invalid account number or pin!!\n");
            return LOGIN_STATUS::FAILURE;
        }
        
        return LOGIN_STATUS::SUCCESS;
    }
};


// TODO: 
class AccountRemover {
    public: 
    static constexpr int ATTEMPTS = ConstantManager::LOGIN_ATTEMPTS;

    static void remove_account() {
        // here you can use the InputManager class to get the input from the user
        printf("Account deletion page: \n");
        int acc_num = InputManager::get_number_input
            (Account::ACCOUNT_NUMBER_LENGTH, "Enter your account number: ", "Invalid account number!!\n");

        // giving three attempts to use to enter the correct pin
        int pin = InputManager::get_number_input
            (ATTEMPTS, "Enter your pin: ", "Invalid pin!!\n");

        // calling another class AccountManager's functions
        if(AccountManager::verify_existing_account(acc_num, pin) == false) {
            puts("Invalid account number or pin!!\n");
            return;
        }

        AccountManager::delete_account(acc_num);
        puts("Account deleted successfully!!\n");
    }
};

class Initializer {
public:
    static void initialize() {
        // Initialize various resources and configurations here

        // Example: RandomNumberGenerator::initialize();
        
        // Initialize AccountNumberManager
        AccountNumberManager::initialize();
        // ...

        // Initialize AccountManager
        AccountManager::initialize();

        // Initialize 
    }
};

class Main {
    public:
    static void main() {
        // code here

        // performs any necessary initialization of all the classes above
        Initializer::initialize();
        
        Welcome::welcome();

        int t = LoginMenuManger::get_prompt_from_login_menu();
        std::cout << "you chose " << t << std::endl;
    }
};

// do not touch this
int main() {
    Main::main();
    return 0;
}

int InputManager::
get_number_input(int attempts, int minm, int maxm, std::string prompt, std::string err_warning) {
    int input = -1;
    do {
        /// TODO:
        std::cout << prompt;
        std::cin >> input;

        if (std::cin.fail() || input < minm || input > maxm) {
            std::cin.clear(); // clear error flag

            // discard invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << err_warning << std::endl;
            input = INT_MIN;
        }
        else {
            break;
        }
    } while (--attempts);

    return input;
}

int InputManager::
get_number_input(int digits, std::string prompt, std::string err_warning) {
    int input = 0;
    do {
        std::cout << prompt;
        std::cin >> input;
        if (std::cin.fail() || count_digits(input) != digits) {
            std::cin.clear(); // clear error flag

            // discard invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << err_warning << std::endl;
        } else {
            break;
        }
    } while (true);
    return input;
}

int InputManager::
get_number_input(int minm, int maxm, std::string prompt, std::string err_warning)
{
    int input;
    do
    {
        std::cout << prompt;
        std::cin >> input;
        if (std::cin.fail() || input <= minm || input >= maxm)
        {
            std::cin.clear(); // clear error flag

            // discard invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << err_warning << std::endl;
        }
        else
        {
            break;
        }
    } while (true);
    return input;
}

double InputManager::
get_real_input(double low, double high, std::string prompt, std::string err_warning) {
    double input;
    do
    {
        std::cout << prompt;
        std::cin >> input;
        if (std::cin.fail() || input < low || input > high)
        {
            std::cin.clear(); // clear error flag

            // discard invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << err_warning << std::endl;
        }
        else
        {
            break;
        }
    } while (true);
    return input;
}

bool InputManager::
get_bool_input(std::string prompt, std::string warning) {
    std::string input;
    do {
        std::cout << prompt;
        std::cin >> input;
        if (input == "true" || input == "1" || input == "yes" || input == "y") {
            return true;
        }
        else if (input == "false" || input == "0" || input == "no" || input == "n") {
            return false;
        }
        else {
            std::cout << warning << std::endl;
        }
    } while (true);
}
