import streamlit as st
import requests

def get_data():
    url = 'http://150.162.235.112:5000/data'
    response = requests.get(url)
    if response.status_code == 200:
        data = response.json()
        return data
    else:
        return None

def main():
    st.title("Weight Data")

    while True:
        data = get_data()

        if data:
            st.text("Received Data:")
            st.text(f"Item: {data.get('item')}")
            st.text(f"Change: {data.get('change')}")
        else:
            st.text("Failed to fetch data")

if _name_ == '_main_':
    main()