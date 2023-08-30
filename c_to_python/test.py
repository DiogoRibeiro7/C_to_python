import numpy as np
import pelt


#test

data = np.array([1.0, 2.0, 3.0, 4.0, 5.0])

start = 1
end = 4
result = pelt.cost(data, start, end)
print("Cost:", result)
