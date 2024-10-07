import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
import java.io.*;
import java.util.regex.Pattern;

public class BankManagementSystem {

    // Account Class
    static class Account {
        private int accountNumber;
        private double balance;
        private String accountType;
        private ArrayList<String> transactionHistory;

        public Account(String accountType) {
            this.accountNumber = new Random().nextInt(100000);
            this.balance = 0;
            this.accountType = accountType;
            this.transactionHistory = new ArrayList<>();
        }

        public int getAccountNumber() {
            return accountNumber;
        }

        public double getBalance() {
            return balance;
        }

        public String getAccountType() {
            return accountType;
        }

        public void deposit(double amount) {
            if (amount > 0) {
                balance += amount;
                transactionHistory.add("Deposited: " + amount);
                System.out.println("Deposited " + amount + ". New Balance: " + balance);
            } else {
                System.out.println("Deposit amount should be positive.");
            }
        }

        public void withdraw(double amount) {
            if (amount > 0 && amount <= balance) {
                balance -= amount;
                transactionHistory.add("Withdrew: " + amount);
                System.out.println("Withdrew " + amount + ". New Balance: " + balance);
            } else {
                System.out.println("Insufficient funds or invalid amount.");
            }
        }

        public void transfer(Account targetAccount, double amount) {
            if (amount > 0 && amount <= balance) {
                this.withdraw(amount);
                targetAccount.deposit(amount);
                transactionHistory.add("Transferred: " + amount + " to Account: " + targetAccount.getAccountNumber());
                System.out.println("Transferred " + amount + " to account: " + targetAccount.getAccountNumber());
            } else {
                System.out.println("Transfer failed. Insufficient funds or invalid amount.");
            }
        }

        public void displayAccount() {
            System.out.println("Account Number: " + accountNumber);
            System.out.println("Account Type: " + accountType);
            System.out.println("Balance: " + balance);
            System.out.println("Transaction History: ");
            for (String transaction : transactionHistory) {
                System.out.println("- " + transaction);
            }
        }

        public void closeAccount() {
            System.out.println("Account " + accountNumber + " has been closed.");
        }

        public void displayTransactionHistory() {
            System.out.println("Transaction History for Account Number " + accountNumber + ": ");
            for (String transaction : transactionHistory) {
                System.out.println("- " + transaction);
            }
        }
    }

    // FixedDepositAccount Class (Inheritance)
    static class FixedDepositAccount extends Account {
        private double interestRate;
        private int maturityMonths;

        public FixedDepositAccount(double interestRate, int maturityMonths) {
            super("Fixed Deposit");
            this.interestRate = interestRate;
            this.maturityMonths = maturityMonths;
        }

        public double calculateInterest() {
            return getBalance() * (interestRate / 100) * (maturityMonths / 12.0);
        }

        public void displayAccount() {
            super.displayAccount();
            System.out.println("Interest Rate: " + interestRate + "%");
            System.out.println("Maturity Duration: " + maturityMonths + " months");
        }
    }

    // SavingsAccount Class (Inheritance)
    static class SavingsAccount extends Account {
        private double interestRate;

        public SavingsAccount(double interestRate) {
            super("Savings");
            this.interestRate = interestRate;
        }

        public double calculateInterest() {
            return getBalance() * (interestRate / 100);
        }

        public void displayAccount() {
            super.displayAccount();
            System.out.println("Interest Rate: " + interestRate + "%");
        }
    }

    // Customer Class
    static class Customer {
        private String name;
        private String address;
        private String phoneNumber;
        private ArrayList<Account> accounts;

        public Customer(String name, String address, String phoneNumber) {
            this.name = name;
            this.address = address;
            this.phoneNumber = phoneNumber;
            this.accounts = new ArrayList<>();
        }

        public String getName() {
            return name;
        }

        public String getAddress() {
            return address;
        }

        public String getPhoneNumber() {
            return phoneNumber;
        }

        public ArrayList<Account> getAccounts() {
            return accounts;
        }

        public void addAccount(Account account) {
            accounts.add(account);
        }

        public void displayCustomerDetails() {
            System.out.println("\n=== Customer Details ===");
            System.out.println("Name: " + name);
            System.out.println("Address: " + address);
            System.out.println("Phone: " + phoneNumber);
            System.out.println("Accounts: ");
            for (Account account : accounts) {
                account.displayAccount();
            }
        }

        public void closeAccount(int accountNumber) {
            for (Account account : accounts) {
                if (account.getAccountNumber() == accountNumber) {
                    account.closeAccount();
                    accounts.remove(account);
                    break;
                }
            }
        }
    }

    // Bank Class
    static class Bank {
        private ArrayList<Customer> customers;
        private Scanner scanner;

        public Bank() {
            this.customers = new ArrayList<>();
            this.scanner = new Scanner(System.in);
        }

        public void createCustomer() {
            System.out.println("\n=== Create New Customer ===");
            System.out.print("Enter Name: ");
            String name = scanner.next();
            System.out.print("Enter Address: ");
            String address = scanner.next();
            System.out.print("Enter Phone Number: ");
            String phoneNumber = scanner.next();

            if (isValidPhoneNumber(phoneNumber)) {
                Customer customer = new Customer(name, address, phoneNumber);
                customers.add(customer);
                System.out.println("Customer created successfully.");
            } else {
                System.out.println("Invalid phone number. Customer not created.");
            }
        }

        public void createAccount() {
            System.out.println("\n=== Create New Account ===");
            System.out.print("Enter Customer Name: ");
            String customerName = scanner.next();

            Customer customer = findCustomerByName(customerName);
            if (customer != null) {
                System.out.print("Enter Account Type (Checking/Savings/Fixed Deposit): ");
                String accountType = scanner.next();
                Account newAccount;

                if (accountType.equalsIgnoreCase("Fixed Deposit")) {
                    System.out.print("Enter Interest Rate: ");
                    double interestRate = scanner.nextDouble();
                    System.out.print("Enter Maturity Duration in Months: ");
                    int maturityMonths = scanner.nextInt();
                    newAccount = new FixedDepositAccount(interestRate, maturityMonths);
                } else if (accountType.equalsIgnoreCase("Savings")) {
                    System.out.print("Enter Interest Rate: ");
                    double interestRate = scanner.nextDouble();
                    newAccount = new SavingsAccount(interestRate);
                } else {
                    newAccount = new Account(accountType);
                }

                customer.addAccount(newAccount);
                System.out.println("Account created successfully for " + customerName);
            } else {
                System.out.println("Customer not found.");
            }
        }

        public void displayAllAccounts() {
            System.out.println("\n=== Display All Accounts ===");
            for (Customer customer : customers) {
                System.out.println("Customer: " + customer.getName());
                for (Account account : customer.getAccounts()) {
                    account.displayAccount();
                }
            }
        }

        public void depositMoney() {
            System.out.print("\nEnter Account Number: ");
            int accountNumber = scanner.nextInt();

            Account account = findAccountByNumber(accountNumber);
            if (account != null) {
                System.out.print("Enter Amount to Deposit: ");
                double amount = scanner.nextDouble();
                account.deposit(amount);
            } else {
                System.out.println("Account not found.");
            }
        }

        public void withdrawMoney() {
            System.out.print("\nEnter Account Number: ");
            int accountNumber = scanner.nextInt();

            Account account = findAccountByNumber(accountNumber);
            if (account != null) {
                System.out.print("Enter Amount to Withdraw: ");
                double amount = scanner.nextDouble();
                account.withdraw(amount);
            } else {
                System.out.println("Account not found.");
            }
        }

        public void transferMoney() {
            System.out.print("\nEnter From Account Number: ");
            int fromAccountNumber = scanner.nextInt();
            Account fromAccount = findAccountByNumber(fromAccountNumber);

            System.out.print("Enter To Account Number: ");
            int toAccountNumber = scanner.nextInt();
            Account toAccount = findAccountByNumber(toAccountNumber);

            if (fromAccount != null && toAccount != null) {
                System.out.print("Enter Amount to Transfer: ");
                double amount = scanner.nextDouble();
                fromAccount.transfer(toAccount, amount);
            } else {
                System.out.println("Account not found for transfer.");
            }
        }

        public void displayCustomerDetails() {
            System.out.print("\nEnter Customer Name: ");
            String name = scanner.next();

            Customer customer = findCustomerByName(name);
            if (customer != null) {
                customer.displayCustomerDetails();
            } else {
                System.out.println("Customer not found.");
            }
        }

        public void closeAccount() {
            System.out.print("\nEnter Customer Name: ");
            String name = scanner.next();

            Customer customer = findCustomerByName(name);
            if (customer != null) {
                System.out.print("Enter Account Number to close: ");
                int accountNumber = scanner.nextInt();
                customer.closeAccount(accountNumber);
            } else {
                System.out.println("Customer not found.");
            }
        }

        private boolean isValidPhoneNumber(String phoneNumber) {
            String regex = "\\d{10}"; // Basic validation for 10-digit phone numbers
            return Pattern.matches(regex, phoneNumber);
        }

        private Customer findCustomerByName(String name) {
            for (Customer customer : customers) {
                if (customer.getName().equalsIgnoreCase(name)) {
                    return customer;
                }
            }
            return null;
        }

        private Account findAccountByNumber(int accountNumber) {
            for (Customer customer : customers) {
                for (Account account : customer.getAccounts()) {
                    if (account.getAccountNumber() == accountNumber) {
                        return account;
                    }
                }
            }
            return null;
        }
        
        public void saveDataToFile() {
            try (BufferedWriter writer = new BufferedWriter(new FileWriter("bank_data.txt"))) {
                for (Customer customer : customers) {
                    writer.write(customer.getName() + "," + customer.getAddress() + "," + customer.getPhoneNumber());
                    writer.newLine();
                    for (Account account : customer.getAccounts()) {
                        writer.write(account.getAccountNumber() + "," + account.getAccountType() + "," + account.getBalance());
                        writer.newLine();
                    }
                }
                System.out.println("Data saved successfully.");
            } catch (IOException e) {
                System.out.println("Error saving data: " + e.getMessage());
            }
        }

        public void loadDataFromFile() {
            File file = new File("bank_data.txt");
            if (!file.exists()) {
                System.out.println("No data file found.");
                return;
            }
            try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
                String line;
                while ((line = reader.readLine()) != null) {
                    String[] customerData = line.split(",");
                    String name = customerData[0];
                    String address = customerData[1];
                    String phoneNumber = customerData[2];
                    Customer customer = new Customer(name, address, phoneNumber);
                    customers.add(customer);
                    while ((line = reader.readLine()) != null && !line.isEmpty()) {
                        String[] accountData = line.split(",");
                        int accountNumber = Integer.parseInt(accountData[0]);
                        String accountType = accountData[1];
                        double balance = Double.parseDouble(accountData[2]);
                        Account account = new Account(accountType);
                        account.deposit(balance);
                        customer.addAccount(account);
                    }
                }
                System.out.println("Data loaded successfully.");
            } catch (IOException e) {
                System.out.println("Error loading data: " + e.getMessage());
            }
        }
    }

    // Main method to run the bank management system
    public static void main(String[] args) {
        Bank bank = new Bank();
        bank.loadDataFromFile(); // Load existing data at startup
        Scanner scanner = new Scanner(System.in);
        int option;

        do {
            System.out.println("\n=== BANK MANAGEMENT SYSTEM ===");
            System.out.println("1. Create Customer");
            System.out.println("2. Create Account");
            System.out.println("3. Display All Accounts");
            System.out.println("4. Deposit Money");
            System.out.println("5. Withdraw Money");
            System.out.println("6. Transfer Money");
            System.out.println("7. Display Customer Details");
            System.out.println("8. Close Account");
            System.out.println("9. Save Data and Exit");
            System.out.print("Select an option: ");
            option = scanner.nextInt();

            switch (option) {
                case 1:
                    bank.createCustomer();
                    break;
                case 2:
                    bank.createAccount();
                    break;
                case 3:
                    bank.displayAllAccounts();
                    break;
                case 4:
                    bank.depositMoney();
                    break;
                case 5:
                    bank.withdrawMoney();
                    break;
                case 6:
                    bank.transferMoney();
                    break;
                case 7:
                    bank.displayCustomerDetails();
                    break;
                case 8:
                    bank.closeAccount();
                    break;
                case 9:
                    bank.saveDataToFile(); // Save data before exit
                    System.out.println("Exiting the system...");
                    break;
                default:
                    System.out.println("Invalid option. Please try again.");
                    break;
            }
        } while (option != 9);

        scanner.close();
    }
}
