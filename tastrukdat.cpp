#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <queue>
using namespace std;

struct StockItem {
    string name;
    int quantity;
    double price;
    string category;
};

struct CategoryNode {
    string name;
    CategoryNode* left;
    CategoryNode* right;

    CategoryNode(string categoryName) : name(categoryName), left(nullptr), right(nullptr) {}
};

struct Notification {
    string message;
    int priority;

    Notification(string msg, int prio) : message(msg), priority(prio) {}
    
    bool operator<(const Notification& other) const {
        return priority < other.priority;
    }
};

CategoryNode* insertCategory(CategoryNode* root, string categoryName) {
    if (root == nullptr) {
        return new CategoryNode(categoryName);
    }

    if (categoryName < root->name) {
        root->left = insertCategory(root->left, categoryName);
    } else {
        root->right = insertCategory(root->right, categoryName);
    }

    return root;
}

void addStock(vector<StockItem>& stock, CategoryNode*& categoryTree, priority_queue<Notification>& notifications) {
    StockItem newItem;
    cout << "Enter the name of the stock item: ";
    cin >> newItem.name;
    cout << "Enter the quantity: ";
    cin >> newItem.quantity;
    cout << "Enter the price per unit: ";
    cin >> newItem.price;
    cout << "Enter the category: ";
    cin >> newItem.category;

    categoryTree = insertCategory(categoryTree, newItem.category);

    stock.push_back(newItem);
    notifications.push(Notification("Stock item added: " + newItem.name, 1));
    cout << "Stock item added successfully!" << endl;
}

void updateStock(vector<StockItem>& stock, const string& itemName, const string& itemCategory, int quantityChanged, priority_queue<Notification>& notifications) {
    bool found = false;
    for (auto& item : stock) {
        if (item.name == itemName && item.category == itemCategory) {
            if (quantityChanged > 0) {
                item.quantity += quantityChanged;
                cout << "Stock quantity for '" << itemName << "' in category '" << itemCategory << "' increased to " << item.quantity << endl;
            } else if (quantityChanged < 0 && item.quantity >= abs(quantityChanged)) {
                item.quantity -= abs(quantityChanged);
                cout << "Stock quantity for '" << itemName << "' in category '" << itemCategory << "' decreased to " << item.quantity << endl;
            } else {
                cout << "Invalid quantity change! Transaction failed." << endl;
                return;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Stock item '" << itemName << "' in category '" << itemCategory << "' not found!" << endl;
    } else {
        notifications.push(Notification("Stock item updated: " + itemName + " in category " + itemCategory, 2));
    }
}

void deleteStock(vector<StockItem>& stock, const string& itemName, const string& itemCategory, priority_queue<Notification>& notifications) {
    auto it = remove_if(stock.begin(), stock.end(), [&](const StockItem& item) {
        return item.name == itemName && item.category == itemCategory;
    });

    if (it != stock.end()) {
        stock.erase(it, stock.end());
        cout << "Stock item '" << itemName << "' in category '" << itemCategory << "' deleted successfully!" << endl;
        notifications.push(Notification("Stock item deleted: " + itemName + " in category " + itemCategory, 3));
    } else {
        cout << "Stock item '" << itemName << "' in category '" << itemCategory << "' not found!" << endl;
    }
}

void displayStock(const vector<StockItem>& stock) {
    if (stock.empty()) {
        cout << "No stock items available!" << endl;
        return;
    }
    cout << "Stock items:\n";
    for (const auto& item : stock) {
        cout << "Name: " << item.name << ", Quantity: " << item.quantity << ", Price per unit: $" << item.price << ", Category: " << item.category << endl;
    }
}

void printCategories(CategoryNode* root, string& prevCategory) {
    if (root == nullptr) {
        return;
    }

    printCategories(root->left, prevCategory);
    if (root->name != prevCategory) {
        cout << root->name << endl;
        prevCategory = root->name;
    }
    printCategories(root->right, prevCategory);
}

void findStockByCategory(const vector<StockItem>& stock, const string& category) {
    bool found = false;
    cout << "Stock items in category '" << category << "':\n";
    for (const auto& item : stock) {
        if (item.category == category) {
            cout << "Name: " << item.name << ", Quantity: " << item.quantity << ", Price per unit: $" << item.price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No stock items found in category '" << category << "'." << endl;
    }
}

void findStockByName(const vector<StockItem>& stock, const string& itemName) {
    bool found = false;
    cout << "Search results for stock item '" << itemName << "':\n";
    for (const auto& item : stock) {
        if (item.name == itemName) {
            cout << "Category: " << item.category << ", Quantity: " << item.quantity << ", Price per unit: $" << item.price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No stock items found with the name '" << itemName << "'." << endl;
    }
}

void addNotification(priority_queue<Notification>& notifications, const string& message, int priority) {
    notifications.push(Notification(message, priority));
}

void showNotifications(priority_queue<Notification>& notifications) {
    while (!notifications.empty()) {
        Notification notification = notifications.top();
        cout << "Notification: " << notification.message << endl;
        notifications.pop();
    }
}

int main() {
    vector<StockItem> stock;
    CategoryNode* categoryTree = nullptr;
    priority_queue<Notification> notifications;

    int choice;
    string itemName, itemCategory;
    do {
        cout << "\n=== Stock Management System ===\n";
        cout << "1. Add new stock\n";
        cout << "2. Update stock\n";
        cout << "3. Delete stock\n";
        cout << "4. Display all stock\n";
        cout << "5. Display categories\n";
        cout << "6. Find stock by category\n";
        cout << "7. Find stock by name\n";
        cout << "8. Show Notifications\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }    
        
        switch(choice) {
            case 1:
                addStock(stock, categoryTree, notifications);
                break;
            case 2: {
                int quantityChanged;
                cout << "Enter the name of the stock item: ";
                cin >> itemName;
                cout << "Enter the category of the stock item: ";
                cin >> itemCategory;
                cout << "Enter the quantity changed: ";
                cin >> quantityChanged;
                updateStock(stock, itemName, itemCategory, quantityChanged, notifications);
                break;
            }
            case 3: {
                cout << "Enter the name of the stock item: ";
                cin >> itemName;
                cout << "Enter the category of the stock item: ";
                cin >> itemCategory;
                deleteStock(stock, itemName, itemCategory, notifications);
                break;
            }
            case 4:
                displayStock(stock);
                break;
            case 5: {
                cout << "Categories:\n";
                string prevCategory = "";
                printCategories(categoryTree, prevCategory);
                break;
            }
            case 6: {
                string category;
                cout << "Enter the category: ";
                cin >> category;
                findStockByCategory(stock, category);
                break;
            }
            case 7: {
                cout << "Enter the name of the stock item: ";
                cin >> itemName;
                findStockByName(stock, itemName);
                break;
            }
            case 8:
                showNotifications(notifications);
                break;
            case 9:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number from 1 to 9." << endl;
        }        
    } while (choice != 9);

    return 0;
}