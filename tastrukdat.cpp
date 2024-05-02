#include <iostream>
#include <vector>
#include <string>
#include <limits>
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

void addStock(vector<StockItem>& stock, CategoryNode*& categoryTree) {
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
    cout << "Stock item added successfully!" << endl;
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

void updateStock(vector<StockItem>& stock, const string& itemName, const string& itemCategory, int quantitySold) {
    bool found = false;
    for (auto& item : stock) {
        if (item.name == itemName && item.category == itemCategory) {
            if (item.quantity >= quantitySold) {
                item.quantity -= quantitySold;
                cout << "Transaction successful. Updated stock quantity for '" << itemName << "' in category '" << itemCategory << "' is " << item.quantity << endl;
            } else {
                cout << "Insufficient stock for '" << itemName << "' in category '" << itemCategory << "'. Transaction failed!" << endl;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Stock item '" << itemName << "' in category '" << itemCategory << "' not found!" << endl;
    }
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
        cout << "No stock items found with name '" << itemName << "'." << endl;
    }
}

int main() {
    vector<StockItem> stock;
    CategoryNode* categoryTree = nullptr;

    int choice;
    string itemName, itemCategory;
    do {
        cout << "\nStock Management System\n";
        cout << "1. Add new stock\n";
        cout << "2. Display all stock\n";
        cout << "3. Update stock after transaction\n";
        cout << "4. Display categories\n";
        cout << "5. Find stock by category\n";
        cout << "6. Find stock by name\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }    
        
        switch(choice) {
            case 1:
                addStock(stock, categoryTree);
                break;
            case 2:
                displayStock(stock);
                break;
            case 3: {
                int quantitySold;
                cout << "Enter the name of the stock item: ";
                cin >> itemName;
                cout << "Enter the category of the stock item: ";
                cin >> itemCategory;
                cout << "Enter the quantity sold: ";
                cin >> quantitySold;
                updateStock(stock, itemName, itemCategory, quantitySold);
                break;
            }
            case 4: {
                cout << "Categories:\n";
                string prevCategory = "";
                printCategories(categoryTree, prevCategory);
                break;
            }
            case 5: {
                string category;
                cout << "Enter the category: ";
                cin >> category;
                findStockByCategory(stock, category);
                break;
            }
            case 6: {
                cout << "Enter the name of the stock item: ";
                cin >> itemName;
                findStockByName(stock, itemName);
                break;
            }
            case 7:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number from 1 to 7." << endl;
        }        
    } while (choice != 7);

    return 0;
}