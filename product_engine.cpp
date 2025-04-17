#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Product {
    string name;
    string category;
    float price;
    float rating;
};

vector<string> searchHistory;

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
        products.push_back(product);
    }
}

void displayProduct(const Product& product) {
    cout << "- " << product.name << " | " << product.category
         << " | $" << product.price << " | Rating: " << product.rating << endl;
}

// Linear search by name or category
void linearSearch(vector<Product>& products, const string& query) {
    cout << "\nSearch results for '" << query << "':" << endl;
    bool found = false;
    for (const auto& product : products) {
        if (product.name.find(query) != string::npos || product.category.find(query) != string::npos) {
            displayProduct(product);
            found = true;
        }
    }
    if (!found) cout << "No results found." << endl;
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
        if (arr[j].rating > pivot) {  // Note: > for descending order
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
    cout << "\nAll Products:" << endl;
    for (const auto& product : products) displayProduct(product);
}

int main() {
    vector<Product> products;
    loadProducts("products.csv", products);

    while (true) {
        system("cls"); // Clear the console (Windows)
        cout << "Product Search Engine" << endl;
        cout << "\nChoose an option:\n";
        cout << "1. Search product by name or category\n";
        cout << "2. View all products\n";
        cout << "3. Sort products by price (low to high)\n";
        cout << "4. Sort products by price (high to low)\n";
        cout << "5. Sort products by rating (high to low)\n";
        cout << "6. Sort products by category\n";
        cout << "7. Exit\n";
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

            string viewHistory;
            cout << "Do you want to view your search history? (yes/no): ";
            cin >> viewHistory;
            cin.ignore();

            if (viewHistory == "yes" || viewHistory == "YES" || viewHistory == "Y" || viewHistory == "y") {
                cout << "\nYour Search History:" << endl;
                for (const auto& term : searchHistory) {
                    cout << "- " << term << endl;
                }
            }
        } else if (choice == 2) {
            listAllProducts(products);
        } else if (choice == 3) {
            mergeSort(products, 0, products.size() - 1, compareByPriceAscending);
            cout << "\nProducts sorted by price (Low to High):" << endl;
            listAllProducts(products);
        } else if (choice == 4) {
            mergeSort(products, 0, products.size() - 1, compareByPriceDescending);
            cout << "\nProducts sorted by price (High to Low):" << endl;
            listAllProducts(products);
        } else if (choice == 5) {
            quickSortByRating(products, 0, products.size() - 1);
            cout << "\nProducts sorted by rating (High to Low):" << endl;
            listAllProducts(products);
        } else if (choice == 6) {
            mergeSort(products, 0, products.size() - 1, compareByCategory);
            cout << "\nProducts sorted by category:" << endl;
            listAllProducts(products);
        } else if (choice == 7) {
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}