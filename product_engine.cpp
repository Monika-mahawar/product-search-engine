#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace std;

struct Product {
    string name;
    string category;
    float price;
    float rating;
    int quantity;
};

struct Purchase {
    vector<Product> items;
    string timestamp;
    float total;
};

vector<string> searchHistory;
vector<Product> cart;
vector<Purchase> purchaseHistory;

void loadProducts(const string& filename, vector<Product>& products) {
    ifstream file(filename);
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string name, category, priceStr, ratingStr;

        getline(ss, name, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, ratingStr);

        Product product;
        product.name = name;
        product.category = category;
        product.price = stof(priceStr);
        product.rating = ratingStr.empty() ? 0.0f : stof(ratingStr);
        product.quantity = 1;
        products.push_back(product);
    }
}

void displayProduct(const Product& product) {
    cout << "- " << product.name << " | " << product.category
         << " | $" << fixed << setprecision(2) << product.price 
         << " | Rating: " << product.rating << endl;
}

void displayCart() {
    if (cart.empty()) {
        cout << "\nYour cart is empty.\n";
        return;
    }

    float total = 0.0f;
    cout << "\n🛒 Your Shopping Cart:\n";
    cout << "------------------------------------------------------------\n";
    cout << left << setw(40) << "Product" << setw(15) << "Category" 
         << setw(10) << "Price" << setw(8) << "Qty" << "Subtotal\n";
    cout << "------------------------------------------------------------\n";

    for (const auto& item : cart) {
        float subtotal = item.price * item.quantity;
        cout << left << setw(40) << item.name.substr(0, 39) 
             << setw(15) << item.category.substr(0, 14)
             << "$" << setw(9) << fixed << setprecision(2) << item.price
             << setw(8) << item.quantity
             << "$" << subtotal << endl;
        total += subtotal;
    }

    cout << "------------------------------------------------------------\n";
    cout << right << setw(73) << "Total: $" << total << "\n";
}

void displayPurchaseHistory() {
    if (purchaseHistory.empty()) {
        cout << "\nNo purchase history available.\n";
        return;
    }

    cout << "\n📜 Purchase History:\n";
    for (const auto& purchase : purchaseHistory) {
        cout << "\n🕒 " << purchase.timestamp << " | Total: $" << fixed << setprecision(2) << purchase.total << "\n";
        cout << "------------------------------------------------------------\n";
        for (const auto& item : purchase.items) {
            cout << "- " << item.name << " (" << item.quantity << "x) | $" 
                 << fixed << setprecision(2) << item.price * item.quantity << "\n";
        }
        cout << "------------------------------------------------------------\n";
    }
}

void addToCart(vector<Product>& products, const string& productName) {
    for (const auto& product : products) {
        if (product.name == productName) {
            for (auto& cartItem : cart) {
                if (cartItem.name == productName) {
                    cartItem.quantity++;
                    cout << "\nQuantity increased for " << productName << " in your cart.\n";
                    return;
                }
            }
            cart.push_back(product);
            cout << "\n" << productName << " added to your cart.\n";
            return;
        }
    }
    cout << "\nProduct not found.\n";
}

void checkout() {
    if (cart.empty()) {
        cout << "\nYour cart is empty. Nothing to checkout.\n";
        return;
    }

    // Create timestamp
    time_t now = time(0);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Calculate total
    float total = 0.0f;
    for (const auto& item : cart) {
        total += item.price * item.quantity;
    }

    // Add to purchase history
    Purchase purchase;
    purchase.items = cart;
    purchase.timestamp = timestamp;
    purchase.total = total;
    purchaseHistory.push_back(purchase);

    displayCart();
    cout << "\nThank you for your purchase!\n";
    cart.clear();
}

// Comparison functions for sorting
bool compareByPriceAscending(const Product& a, const Product& b) {
    return a.price < b.price;
}

bool compareByPriceDescending(const Product& a, const Product& b) {
    return a.price > b.price;
}

bool compareByRatingDescending(const Product& a, const Product& b) {
    return a.rating > b.rating;
}

bool compareByCategory(const Product& a, const Product& b) {
    return a.category < b.category;
}

// QuickSort implementation for sorting by rating
int partition(vector<Product>& arr, int low, int high) {
    float pivot = arr[high].rating;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].rating > pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSortByRating(vector<Product>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortByRating(arr, low, pi - 1);
        quickSortByRating(arr, pi + 1, high);
    }
}

// MergeSort algorithm for other sorting options
void merge(vector<Product>& arr, int left, int mid, int right, bool(*comp)(const Product&, const Product&)) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Product> L(n1);
    vector<Product> R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (comp(L[i], R[j])) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<Product>& arr, int left, int right, bool(*comp)(const Product&, const Product&)) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid, comp);
        mergeSort(arr, mid + 1, right, comp);
        merge(arr, left, mid, right, comp);
    }
}

void listAllProducts(const vector<Product>& products) {
    cout << "\nAll Products:\n";
    for (const auto& product : products) {
        displayProduct(product);
    }
}

void linearSearch(vector<Product>& products, const string& query) {
    cout << "\nSearch results for '" << query << "':\n";
    bool found = false;
    for (const auto& product : products) {
        if (product.name.find(query) != string::npos || product.category.find(query) != string::npos) {
            displayProduct(product);
            found = true;
        }
    }
    if (!found) cout << "No results found.\n";
}

int main() {
    vector<Product> products;
    loadProducts("products.csv", products);

    while (true) {
        system("cls");
        cout << "🛍️ Product Search Engine\n";
        cout << "\nChoose an option:\n";
        cout << "1. Search product by name or category\n";
        cout << "2. View all products\n";
        cout << "3. Sort products by price (low to high)\n";
        cout << "4. Sort products by price (high to low)\n";
        cout << "5. Sort products by rating (high to low)\n";
        cout << "6. Sort products by category\n";
        cout << "7. View shopping cart\n";
        cout << "8. Checkout\n";
        cout << "9. View purchase history\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string searchTerm;
            cout << "Enter search term: ";
            getline(cin, searchTerm);
            searchHistory.push_back(searchTerm);
            linearSearch(products, searchTerm);

            cout << "\nWould you like to add any product to cart? (yes/no): ";
            string addChoice;
            getline(cin, addChoice);
            if (addChoice == "yes" || addChoice == "y") {
                cout << "Enter the exact product name to add: ";
                string productName;
                getline(cin, productName);
                addToCart(products, productName);
            }
        } 
        else if (choice == 2) {
            listAllProducts(products);
            cout << "\nWould you like to add any product to cart? (yes/no): ";
            string addChoice;
            getline(cin, addChoice);
            if (addChoice == "yes" || addChoice == "y") {
                cout << "Enter the exact product name to add: ";
                string productName;
                getline(cin, productName);
                addToCart(products, productName);
            }
        } 
        else if (choice == 3) {
            mergeSort(products, 0, products.size() - 1, compareByPriceAscending);
            cout << "\nProducts sorted by price (Low to High):\n";
            listAllProducts(products);
        } 
        else if (choice == 4) {
            mergeSort(products, 0, products.size() - 1, compareByPriceDescending);
            cout << "\nProducts sorted by price (High to Low):\n";
            listAllProducts(products);
        } 
        else if (choice == 5) {
            quickSortByRating(products, 0, products.size() - 1);
            cout << "\nProducts sorted by rating (High to Low):\n";
            listAllProducts(products);
        } 
        else if (choice == 6) {
            mergeSort(products, 0, products.size() - 1, compareByCategory);
            cout << "\nProducts sorted by category:\n";
            listAllProducts(products);
        } 
        else if (choice == 7) {
            displayCart();
            cout << "\nPress Enter to continue...";
            cin.ignore();
        } 
        else if (choice == 8) {
            checkout();
            cout << "\nPress Enter to continue...";
            cin.ignore();
        } 
        else if (choice == 9) {
            displayPurchaseHistory();
            cout << "\nPress Enter to continue...";
            cin.ignore();
        } 
        else if (choice == 10) {
            break;
        } 
        else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}