import numpy as np
import matplotlib.pyplot as plt
from math import cos, sin
from scipy.spatial.transform import Rotation as R

def lambertian_brdf(in_direction, out_direction, normal):

	return 0.3


def blinn_phong_brdf(in_direction, out_direction, normal):

	kL = 0.9
	kg = 0.1

	h_test = np.array([in_direction[0], in_direction[1], 0.0])

	h_array = np.array(h_test + out_direction)
	h = h_array/np.linalg.norm(h_array)
	pL = 1
	pg = pL
	s = 100
	fr = np.array(kL*(pL) + kg*(pg*((8+s)/(8))*(pow(np.dot(normal, h),s))))

	return fr


normal = np.array([ 0, 0, 1 ])
out_direction = np.array([ 0, 0, 1])
in_direction = np.array([1, 0, 0])

# Rotation around the y-axis 1 degree on in_direction 
rotation_degrees = 1
rotation_radians = np.radians(rotation_degrees)
rotation_axis = np.array([0, 1, 0])
rotation_vector = rotation_radians * rotation_axis
rotation = R.from_rotvec(rotation_vector)

# initializing empty array with 180 values
temp_ypoints = [0 for i in range(180)]

for i in range(180):

	temp_ypoints[i] = ( blinn_phong_brdf(in_direction, out_direction, normal ))
	# # this used with the plot of the regular brdf
	# temp_ypoints[i] = ( lambertian_brdf(in_direction, out_direction, normal ))
	
	# updates in_direction value
	in_direction = rotation.apply(in_direction)

ypoints = temp_ypoints
xpoints = np.arange(-90, 90, 1)
plt.plot(xpoints, ypoints)
plt.show()
