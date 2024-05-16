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
    cout << "Masukkan nama barang stok: ";
    cin >> newItem.name;
    cout << "Masukkan kuantitas barang: ";
    cin >> newItem.quantity;
    cout << "Masukkan harga per barang: ";
    cin >> newItem.price;
    cout << "Masukkan nama kategori: ";
    cin >> newItem.category;

    categoryTree = insertCategory(categoryTree, newItem.category);

    stock.push_back(newItem);
    notifications.push(Notification("Barang stok dimasukkan: " + newItem.name, 1));
    cout << "Stok berhasil dimasukkan!" << endl;
}

void updateStock(vector<StockItem>& stock, const string& itemName, const string& itemCategory, int quantityChanged, priority_queue<Notification>& notifications) {
    bool found = false;
    for (auto& item : stock) {
        if (item.name == itemName && item.category == itemCategory) {
            if (quantityChanged > 0) {
                item.quantity += quantityChanged;
                cout << "Kuantitas stok untuk '" << itemName << "' di kategori '" << itemCategory << "' ditambah menjadi " << item.quantity << endl;
            } else if (quantityChanged < 0 && item.quantity >= abs(quantityChanged)) {
                item.quantity -= abs(quantityChanged);
                cout << "Kuantitas stok untuk '" << itemName << "' di kategori '" << itemCategory << "' dikurangi menjadi " << item.quantity << endl;
            } else {
                cout << "Pengubahan kuantitas invalid! Update gagal." << endl;
                return;
            }
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Barang stok '" << itemName << "' di kategori '" << itemCategory << "' tidak ditemukan!" << endl;
    } else {
        notifications.push(Notification("Barang stok diupdate: " + itemName + " di kategori " + itemCategory, 2));
    }
}

void deleteStock(vector<StockItem>& stock, const string& itemName, const string& itemCategory, priority_queue<Notification>& notifications) {
    auto it = remove_if(stock.begin(), stock.end(), [&](const StockItem& item) {
        return item.name == itemName && item.category == itemCategory;
    });

    if (it != stock.end()) {
        stock.erase(it, stock.end());
        cout << "Barang stok '" << itemName << "' di kategori '" << itemCategory << "' berhasil dihapus!" << endl;
        notifications.push(Notification("Barang stok dihapus: " + itemName + " di kategori " + itemCategory, 3));
    } else {
        cout << "Barang stok '" << itemName << "' di kategori '" << itemCategory << "' tidak ditemukan!" << endl;
    }
}

void displayStock(const vector<StockItem>& stock) {
    if (stock.empty()) {
        cout << "Tidak ada barang yang tersedia!" << endl;
        return;
    }
    cout << "Barang stok:\n";
    for (const auto& item : stock) {
        cout << "Nama: " << item.name << ", Kuantitas: " << item.quantity << ", Harga per barang: Rp" << item.price << ", Kategori: " << item.category << endl;
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
    cout << "Barang stok di kategori '" << category << "':\n";
    for (const auto& item : stock) {
        if (item.category == category) {
            cout << "Nama: " << item.name << ", Kuantitas: " << item.quantity << ", Harga per barang: Rp" << item.price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Tidak ada barang stok ditemukan di kategori '" << category << "'." << endl;
    }
}

void findStockByName(const vector<StockItem>& stock, const string& itemName) {
    bool found = false;
    cout << "Hasil pencarian untuk barang stok '" << itemName << "':\n";
    for (const auto& item : stock) {
        if (item.name == itemName) {
            cout << "Kategori: " << item.category << ", Kuantitas: " << item.quantity << ", Harga per barang: Rp" << item.price << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Tidak ada barang stok di temukan dengan nama '" << itemName << "'." << endl;
    }
}

void addNotification(priority_queue<Notification>& notifications, const string& message, int priority) {
    notifications.push(Notification(message, priority));
}

void showNotifications(priority_queue<Notification>& notifications) {
    while (!notifications.empty()) {
        Notification notification = notifications.top();
        cout << "Notifikasi: " << notification.message << endl;
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
        cout << "\n=== Manajemen Stok Barang ===\n";
        cout << "1. Tambahkan barang stok baru\n";
        cout << "2. Update barang stok\n";
        cout << "3. Hapus barang stok\n";
        cout << "4. Tampilkan seluruh barang stok\n";
        cout << "5. Tampilkan kategori barang\n";
        cout << "6. Cari barang stok berdasarkan kategori\n";
        cout << "7. Cari barang stok berdasarkan nama\n";
        cout << "8. Tampilkan notifikasi\n";
        cout << "9. Keluar\n";
        cout << "Masukkan pilihanmu: ";
        
        if (!(cin >> choice)) {
            cout << "Input invalid! Tolong masukkan nomor.\n";
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
                cout << "Masukkan nama barang stok: ";
                cin >> itemName;
                cout << "Masukkan kategori dari barang stok: ";
                cin >> itemCategory;
                cout << "Masukkan kuantitas yang diubah: ";
                cin >> quantityChanged;
                updateStock(stock, itemName, itemCategory, quantityChanged, notifications);
                break;
            }
            case 3: {
                cout << "Masukkan nama barang stok: ";
                cin >> itemName;
                cout << "Masukkan kategori barang stok: ";
                cin >> itemCategory;
                deleteStock(stock, itemName, itemCategory, notifications);
                break;
            }
            case 4:
                displayStock(stock);
                break;
            case 5: {
                cout << "Kategori:\n";
                string prevCategory = "";
                printCategories(categoryTree, prevCategory);
                break;
            }
            case 6: {
                string category;
                cout << "Masukkan kategori: ";
                cin >> category;
                findStockByCategory(stock, category);
                break;
            }
            case 7: {
                cout << "Masukkan nama dari barang stok: ";
                cin >> itemName;
                findStockByName(stock, itemName);
                break;
            }
            case 8:
                showNotifications(notifications);
                break;
            case 9:
                cout << "Keluar dari program...\n";
                break;
            default:
                cout << "Input Invalid! Tolong masukkan nomor dari 1 sampai 9." << endl;
        }        
    } while (choice != 9);

    return 0;
}