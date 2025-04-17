import streamlit as st
import pandas as pd
from datetime import datetime

# Load product data from CSV
@st.cache_data
def load_data():
    df = pd.read_csv("products.csv")
    df["rating"] = pd.to_numeric(df["rating"], errors="coerce").fillna(0.0)
    df["price"] = pd.to_numeric(df["price"], errors="coerce").fillna(0.0)
    return df

# Initialize session state
if 'search_history' not in st.session_state:
    st.session_state.search_history = []
if 'cart' not in st.session_state:
    st.session_state.cart = []
if 'filtered_products' not in st.session_state:
    st.session_state.filtered_products = pd.DataFrame()
if 'purchase_history' not in st.session_state:
    st.session_state.purchase_history = []

products = load_data()
st.title("🛍️ Product Search Engine")

# Sidebar for cart
with st.sidebar:
    st.header("🛒 Your Shopping Cart")

    if st.session_state.cart:
        cart_df = pd.DataFrame(st.session_state.cart)
        cart_df['subtotal'] = cart_df['price'] * cart_df['quantity']
        st.dataframe(cart_df[['name', 'price', 'quantity', 'subtotal']])

        total = cart_df['subtotal'].sum()
        st.write(f"### Total: ${total:.2f}")

        remove_options = [f"{item['name']} (Qty: {item['quantity']})" 
                         for item in st.session_state.cart]
        remove_choice = st.selectbox("Select item to remove:", 
                                   ["--Select--"] + remove_options)

        if remove_choice != "--Select--":
            item_index = remove_options.index(remove_choice)
            st.session_state.cart.pop(item_index)
            st.rerun()

        if st.button("💳 Checkout"):
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            purchase = {
                'timestamp': timestamp,
                'items': [item.copy() for item in st.session_state.cart],
                'total': total
            }
            st.session_state.purchase_history.append(purchase)
            st.success("Thank you for your purchase!")
            st.session_state.cart = []
            st.rerun()
    else:
        st.write("Your cart is empty.")

# Main content
tab1, tab2, tab3 = st.tabs(["🔍 Search Products", "📋 All Products", "⚙️ Filters"])

with tab1:
    search_query = st.text_input("Search for a product or category:")
    if search_query:
        st.session_state.search_history.append(search_query)
        st.session_state.filtered_products = products[
            products['name'].str.contains(search_query, case=False) |
            products['category'].str.contains(search_query, case=False)
        ]
        st.write(f"### Results for: `{search_query}`")
        st.dataframe(st.session_state.filtered_products)

        if not st.session_state.filtered_products.empty:
            product_names = st.session_state.filtered_products['name'].tolist()
            selected_product = st.selectbox("Select a product to add to cart:", 
                                         ["--Select--"] + product_names)

            if selected_product != "--Select--":
                quantity = st.number_input("Quantity:", min_value=1, value=1)

                if st.button("Add to Cart"):
                    product = st.session_state.filtered_products[
                        st.session_state.filtered_products['name'] == selected_product
                    ].iloc[0].to_dict()

                    found = False
                    for item in st.session_state.cart:
                        if item['name'] == product['name']:
                            item['quantity'] += quantity
                            found = True
                            break

                    if not found:
                        product['quantity'] = quantity
                        st.session_state.cart.append(product)

                    st.success(f"{selected_product} added to cart!")
                    st.rerun()

with tab2:
    st.write("### All Products")
    st.dataframe(products)

    product_names = products['name'].tolist()
    selected_product = st.selectbox("Select a product to add to cart:", 
                                   ["--Select--"] + product_names, key="all_products")

    if selected_product != "--Select--":
        quantity = st.number_input("Quantity:", min_value=1, value=1, key="qty_all")

        if st.button("Add to Cart", key="add_all"):
            product = products[products['name'] == selected_product].iloc[0].to_dict()

            found = False
            for item in st.session_state.cart:
                if item['name'] == product['name']:
                    item['quantity'] += quantity
                    found = True
                    break

            if not found:
                product['quantity'] = quantity
                st.session_state.cart.append(product)

            st.success(f"{selected_product} added to cart!")
            st.rerun()

with tab3:
    category_options = products["category"].unique().tolist()
    selected_categories = st.multiselect("Choose category/categories:", category_options)

    col1, col2 = st.columns(2)
    with col1:
        price_range = st.slider("Select price range:",
                              float(products['price'].min()), 
                              float(products['price'].max()),
                              (float(products['price'].min()), 
                               float(products['price'].max())))
    with col2:
        rating_threshold = st.slider("Minimum rating:", 0.0, 5.0, 0.0)

    filtered = products.copy()
    if selected_categories:
        filtered = filtered[filtered["category"].isin(selected_categories)]
    filtered = filtered[
        (filtered['price'] >= price_range[0]) &
        (filtered['price'] <= price_range[1]) &
        (filtered['rating'] >= rating_threshold)
    ]

    st.write("### Filtered Results")
    st.dataframe(filtered)

    if not filtered.empty:
        product_names = filtered['name'].tolist()
        selected_product = st.selectbox("Select a product to add to cart:", 
                                       ["--Select--"] + product_names, key="filtered")

        if selected_product != "--Select--":
            quantity = st.number_input("Quantity:", min_value=1, value=1, key="qty_filtered")

            if st.button("Add to Cart", key="add_filtered"):
                product = filtered[filtered['name'] == selected_product].iloc[0].to_dict()

                found = False
                for item in st.session_state.cart:
                    if item['name'] == product['name']:
                        item['quantity'] += quantity
                        found = True
                        break

                if not found:
                    product['quantity'] = quantity
                    st.session_state.cart.append(product)

                st.success(f"{selected_product} added to cart!")
                st.rerun()

# Sort options
st.sidebar.write("## Sort Options")
sort_option = st.sidebar.selectbox("Sort by:", 
                                  ["Price (Low to High)", 
                                   "Price (High to Low)", 
                                   "Rating", 
                                   "Category"])

if sort_option == "Price (Low to High)":
    sorted_products = products.sort_values(by="price")
elif sort_option == "Price (High to Low)":
    sorted_products = products.sort_values(by="price", ascending=False)
elif sort_option == "Rating":
    sorted_products = products.sort_values(by="rating", ascending=False)
else:
    sorted_products = products.sort_values(by="category")

st.sidebar.write("### Sorted Products Preview")
st.sidebar.dataframe(sorted_products, use_container_width=True)

# Search history
if st.sidebar.checkbox("📜 Show Search History"):
    st.sidebar.write("### Search History")
    st.sidebar.write(st.session_state.search_history)

# Purchase History Button at the bottom
st.write("---")
if st.button("📜 View Purchase History"):
    if not st.session_state.purchase_history:
        st.info("No purchase history available.")
    else:
        st.write("## 🧾 Purchase History")
        for purchase in reversed(st.session_state.purchase_history):
            st.write(f"**🕒 {purchase['timestamp']} | Total: ${purchase['total']:.2f}**")
            st.table(pd.DataFrame(purchase['items'])[['name', 'price', 'quantity']])
            st.write("---")
