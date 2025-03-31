import math

length_CD = 86.60 # Length CD
length_DE = 73.35 # Length DE
length_E_to_Claw = 121.20 # 121.20 , E to Claw
length_BC = 95.75 # BC
length_AB = 87.70 # Length AB
length_AE = 168.50 # Length AE

Y = 113.857
X = 249.843

# THIS IS THETA
length_AP = math.sqrt(X**2 + (length_BC - Y)**2)
angle_yap = math.degrees(math.atan(X / abs(length_BC - Y)))
length_BP = math.sqrt((length_DE + length_E_to_Claw)**2)
angle_pab = math.degrees(math.acos((length_AP**2 + length_CD**2 - length_BP**2) / (2 * length_AP * length_CD)))
theta = 180 - angle_yap - angle_pab

# THIS IS PHI
angle_abp = math.degrees(math.asin((length_AP / length_BP) * math.sin(math.radians(angle_pab))))
angle_cab = angle_yap + angle_pab

length_BC = math.sqrt(length_BC**2 + length_CD**2 - (2 * length_BC * length_CD * math.cos(math.radians(angle_cab))))
print("lengthBC", length_BC)
phi_1 = math.degrees(math.asin((length_CD/length_BC) * math.sin(math.radians(angle_cab))))

angle_abc = 180 - phi_1 - angle_cab
angle_cbe = angle_abp - angle_abc

length_CE = math.sqrt(length_BC**2 + length_DE**2 - (2 * length_BC * length_DE * math.cos(math.radians(angle_cbe))))

phi_2 = math.degrees(math.asin((length_DE/length_CE) * math.sin(math.radians(angle_cbe))))
phi_3 = math.degrees(math.acos((length_CE**2 + length_AB**2 - length_AE**2)/(2 * length_CE * length_AB)))

phi = phi_1 + phi_2 + phi_3

print("PHI1, ", phi_1)
print("PHI2, ", phi_2)
print("PHI3, ", phi_3)

print("THETA ", theta)
print("PHI ", phi)
