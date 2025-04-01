import math

length_ab = 86.60
length_be = 73.35
length_ef = 121.20
length_ac = 95.75
length_cd = 87.70 
length_de = 168.50

deltaW = 0
deltaL = 0

Y = 120
X = 240

# THIS IS THETA
length_ap = math.sqrt(X**2 + (length_ac - Y)**2) # hypotenuse
angle_yap = math.degrees(math.atan(X / abs(length_ac - Y))) # inverse tan
Lbp = math.sqrt(deltaW**2 + (length_be + length_ef + deltaL)**2) 
angle_pab = math.degrees(math.acos((length_ap**2 + length_ab**2 - Lbp**2) / (2 * length_ap * length_ab)))
theta = 180 - angle_yap - angle_pab

# THIS IS PHI
angle_abp = math.degrees(math.asin((length_ap / Lbp) * math.sin(math.radians(angle_pab)))) # sine law
angle_pbf = math.degrees(math.atan(deltaW/(length_be + length_ef + deltaL))) # tan
angle_abe = angle_abp + angle_pbf
angle_cab = angle_yap + angle_pab

lbc = math.sqrt(length_ac**2 + length_ab**2 - (2 * length_ac * length_ab * math.cos(math.radians(angle_cab))))
phi_1 = math.degrees(math.asin((length_ab/lbc) * math.sin(math.radians(angle_cab))))

angle_abc = 180 - phi_1 - angle_cab
angle_cbe = angle_abe - angle_abc

lce = math.sqrt(lbc**2 + length_be**2 - (2 * lbc * length_be * math.cos(math.radians(angle_cbe))))

phi_2 = math.degrees(math.asin((length_be/lce) * math.sin(math.radians(angle_cbe))))
phi_3 = math.degrees(math.acos((lce**2 + length_cd**2 - length_de**2)/(2 * lce * length_cd)))

phi = phi_1 + phi_2 + phi_3

theta += 40 # account for offset of servos
phi -= 72

print("THETA ", theta)
print("PHI ", phi)