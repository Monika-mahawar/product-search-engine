# Product Search Engine

A robust product catalog application that allows users to browse, search, sort, and filter products from a CSV-based product catalog. The backend is built with **C++** for high performance, while the frontend is powered by **Python** and **Streamlit** for a dynamic, web-based user interface. The application uses efficient algorithms like **Linear Search** for searching, **Quick Sort** for sorting by rating, and **Merge Sort** for sorting by price and category.

## Features

- **Search**: Search products by name or category using **Linear Search** for reliable results.
- **Sort**: Sort products by price, rating, or category:
  - **Quick Sort** for fast sorting by rating.
  - **Merge Sort** for stable sorting by price and category.
- **Filter**: Filter products by price range or rating range.
- **Display**: View all products or filtered results in an interactive, tabular interface.

## Technologies Used

### Backend (C++)
- **File Handling**: Reads product data from a CSV file.
- **Sorting Algorithms**:
  - **Quick Sort**: Optimizes sorting by rating.
  - **Merge Sort**: Ensures stable sorting for price and category.
- **Linear Search**: Implements search for product name and category.
- **STL Containers**:
  - **Vector**: Stores product data dynamically.
  - **Unordered Map**: Enables fast category-based lookups.
- **String Manipulation**: Handles robust search queries.

### Frontend (Python)
- **Streamlit**: Renders an interactive web-based interface.
- **Pandas**: Manages CSV data for integration with the frontend.

### Data Storage
- **CSV File**: Stores product details (e.g., name, category, price, rating) in a structured format.

## Prerequisites

### For the Frontend
Ensure you have Python installed, along with the following libraries:
- `streamlit`
- `pandas`

Install them using pip:
```bash
pip install streamlit pandas
```

Start the application by the below command
```
streamlit run app.py
```
