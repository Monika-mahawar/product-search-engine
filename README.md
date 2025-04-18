# 🛍️ Product Search Engine

A robust and smart product catalog application that allows users to browse, search, sort, and filter products from a CSV-based product catalog. The backend is built with **C++** for high performance, while the frontend is powered by **Python** and **Streamlit** for a dynamic, web-based user interface. The application uses efficient algorithms like **Linear Search** for searching, **Quick Sort** for sorting by rating, and **Merge Sort** for sorting by price and category.

---

## ✨ Features

- **🔍 Search**: Search products by name or category using **Linear Search** for fast, accurate results.
- **📊 Sort**: Sort products dynamically by:
  - **Price (Low to High / High to Low)** – powered by **Merge Sort**
  - **Rating (High to Low)** – powered by **Quick Sort**
  - **Category** – also using **Merge Sort** for stability
- **⚙️ Filter**:
  - Filter by one or multiple **categories**
  - Set **price range** and **minimum rating** to narrow down your results
- **🧾 Purchase History**:
  - View a list of all previous checkouts including timestamp, items purchased, quantity, and total amount
  - Automatically tracks each purchase when the user checks out their cart
- **📋 Display**:
  - Browse **all products**, **search results**, or **filtered results**
  - View them in an interactive, sortable table
- **🛒 Shopping Cart**:
  - Add products from any tab (Search, All, or Filter)
  - View itemized list with quantities and subtotals
  - Remove items or adjust cart in real time
  - Checkout with one click and save the session purchase

---

## 🛠️ Technologies Used

### Backend (C++)
- **File Handling**: Reads product data from a CSV file.
- **Sorting Algorithms**:
  - **Quick Sort**: Optimized for sorting products by rating.
  - **Merge Sort**: Used for sorting by price and category.
- **Search**: Implements **Linear Search** to locate products efficiently.
- **STL Containers**:
  - **`vector`**: To dynamically store product data.
- **Time & Date**: Tracks checkout timestamps using `<ctime>`.

### Frontend (Python + Streamlit)
- **Streamlit**: Builds a clean, interactive web UI.
- **Pandas**: Efficiently handles CSV data operations.
- **Session State**: Maintains cart contents, search history, and purchase history during a session.

---

## 📦 Data Storage

- **CSV File**: 
  - Stores product catalog including: `name`, `category`, `price`, and `rating`
  - Make sure `products.csv` is available in the project directory

---

## ✅ Prerequisites

### For the Frontend

Ensure you have Python installed along with the required packages:
```bash
pip install streamlit pandas
```

Start the application by the below command
```bash
streamlit run app.py
```
