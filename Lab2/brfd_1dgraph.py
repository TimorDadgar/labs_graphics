import numpy as np
import matplotlib.pyplot as plt
from math import cos, sin

def lambertian_brdf(in_direction, out_direction, normal):

	return 0.3
	# for i in range(-90, 90, 1):
	# 	temp_arr = np.array()

normal = np.array([ 0, 0, 1 ])
out_direction = np.array([ 0, 0, 1 ])

# rotation in z doesnt matter that is why we are using 2 variables
in_direction = np.array([ 1, 0])
# 1 degree rotation
theta = np.deg2rad(1)
# rotation matrix
rot = np.array([[cos(theta), -sin(theta)], [sin(theta), cos(theta)]])

# initializing empty array with 180 values
temp_ypoints = [0 for i in range(180)]

for i in range(180):

	# np.dot rot, rotates the in_direction by 1 degree
	temp_ypoints[i] = ( lambertian_brdf(np.dot(rot, in_direction), out_direction, normal ))
	# updates in_direction value
	in_direction = np.dot(rot, in_direction)
	print(in_direction)

ypoints = temp_ypoints
xpoints = np.arange(-90, 90, 1)
# xpoints = np.array([-90, -45, 0, 45, 90])
plt.plot(xpoints, ypoints)
plt.show()
