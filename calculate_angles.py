import math

Lab = 86.60
Lbe = 73.35
Lef = 121.20
Lac = 95.75
Lcd = 87.70 
Lde = 168.50

deltaW = 0
deltaL = 0

Y = 250
X = 148.5

# Lab = 86.41
# Lbe = 73.47
# Lef = 121.20
# Lac = 116.97
# Lcd = 85.34
# Lde = 165.50

# deltaW = 17.77
# deltaL = 0

# Y = 220
# X = 22.75 

# t1_y = 25 cm
# t1_x = 35.27 mm
# --> 105.94, 125.26

# t2_x = 87.5 mm
# t2_y = 25 cm
# --> 102.48, 105.19

# t3_x = 148.5 mm
# t3_y = 25 cm
# --> 110.78, 99.36

# t1_y = 220 mm
# t1_x = 85.25 mm
# --> 63.58, 100.43

# t2_x = 22.75 mm
# t2_y = 220 mm
# --> 

# t3_x = -38.25
# t3_y = 220 mm
# --> 

# THIS IS THETA
lap = math.sqrt(X**2 + (Lac - Y)**2) # hypotenuse
deg_yap = math.degrees(math.atan(X / abs(Lac - Y))) # inverse tan
Lbp = math.sqrt(deltaW**2 + (Lbe + Lef + deltaL)**2) 
deg_pab = math.degrees(math.acos((lap**2 + Lab**2 - Lbp**2) / (2 * lap * Lab)))
theta = 180 - deg_yap - deg_pab

# THIS IS PHI
deg_abp = math.degrees(math.asin((lap / Lbp) * math.sin(math.radians(deg_pab)))) # sine law
deg_pbf = math.degrees(math.atan(deltaW/(Lbe + Lef + deltaL))) # tan
deg_abe = deg_abp + deg_pbf
deg_cab = deg_yap + deg_pab

lbc = math.sqrt(Lac**2 + Lab**2 - (2 * Lac * Lab * math.cos(math.radians(deg_cab))))
phi_1 = math.degrees(math.asin((Lab/lbc) * math.sin(math.radians(deg_cab))))

deg_abc = 180 - phi_1 - deg_cab
deg_cbe = deg_abe - deg_abc

lce = math.sqrt(lbc**2 + Lbe**2 - (2 * lbc * Lbe * math.cos(math.radians(deg_cbe))))

phi_2 = math.degrees(math.asin((Lbe/lce) * math.sin(math.radians(deg_cbe))))
phi_3 = math.degrees(math.acos((lce**2 + Lcd**2 - Lde**2)/(2 * lce * Lcd)))

phi = phi_1 + phi_2 + phi_3

theta += 40
phi -= 72

print("THETA ", theta)
print("PHI ", phi)