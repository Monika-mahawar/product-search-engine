import streamlit as st
import pandas as pd

# Load product data from CSV
@st.cache_data
def load_data():
    df = pd.read_csv("products.csv")
    df["rating"] = pd.to_numeric(df["rating"], errors="coerce").fillna(0.0)
    df["price"] = pd.to_numeric(df["price"], errors="coerce").fillna(0.0)
    return df

# Store search history
if 'search_history' not in st.session_state:
    st.session_state.search_history = []

products = load_data()
st.title("🛍️ Product Search Engine")

# Show all products in an expander
with st.expander("📋 Show All Products"):
    st.write("### All Products")
    st.dataframe(products)

# Search bar
search_query = st.text_input("Search for a product or category:")
if search_query:
    st.session_state.search_history.append(search_query)
    filtered = products[products['name'].str.contains(search_query, case=False) |
                        products['category'].str.contains(search_query, case=False)]
    st.write(f"### Results for: `{search_query}`")
    st.dataframe(filtered)

# Category filter
with st.expander("📦 Filter by Category"):
    category_options = products["category"].unique().tolist()
    selected_categories = st.multiselect("Choose category/categories:", category_options)
    if selected_categories:
        filtered_by_category = products[products["category"].isin(selected_categories)]
        st.write("### Filtered by Category")
        st.dataframe(filtered_by_category)

# Show filters
with st.expander("🔍 Advanced Filters"):
    price_range = st.slider("Select price range:",
                            float(products['price'].min()), float(products['price'].max()),
                            (float(products['price'].min()), float(products['price'].max())))
    rating_threshold = st.slider("Minimum rating:", 0.0, 5.0, 0.0)

    filtered = products[(products['price'] >= price_range[0]) &
                        (products['price'] <= price_range[1]) &
                        (products['rating'] >= rating_threshold)]
    st.write("### Filtered Results")
    st.dataframe(filtered)

# Sort options
sort_option = st.selectbox("Sort by:", ["Price (Low to High)", "Price (High to Low)", "Rating", "Category"])
if sort_option == "Price (Low to High)":
    sorted_products = products.sort_values(by="price")
elif sort_option == "Price (High to Low)":
    sorted_products = products.sort_values(by="price", ascending=False)
elif sort_option == "Rating":
    sorted_products = products.sort_values(by="rating", ascending=False)
else:
    sorted_products = products.sort_values(by="category")

st.write("### Sorted Products")
st.dataframe(sorted_products)

# Search history
if st.checkbox("📜 Show Search History"):
    st.write("### Search History")
    st.write(st.session_state.search_history)
