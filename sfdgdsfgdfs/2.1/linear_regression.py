import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression

def linear_regression(window_data):
    X = np.arange(len(window_data)).reshape(-1, 1)
    y = window_data[:, 1].reshape(-1, 1)
    
    model = LinearRegression()
    model.fit(X, y)
    
    return model.coef_[0][0]

def calculate_coefficients(data):
    coefficients = []
    
    for i in range(99, len(data)):
        window_data = data[i-99:i+1].values
        coefficient = linear_regression(window_data)
        coefficients.append(coefficient)
    
    return coefficients
