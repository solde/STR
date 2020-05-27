print("Enter Ci:")
Ci = []
new_element = input()
while(new_element != str("-1")):
    Ci.append(int(new_element))
    new_element = input()
print("Ci: " + str(Ci))

print("Enter Ti:")
Ti = []
new_element = input()
while(new_element != "-1"):
    Ti.append(int(new_element))
    new_element = input()
print("Ti: " + str(Ti))

print("Enter Di:")
Di = []
new_element = input()
while(new_element != "-1"):
    Di.append(int(new_element))
    new_element = input()
print("Di: " + str(Di))

#--------------------------------#

# Sum(Ci/Ti) <= 1
U = 0
Ui = []
for x, j in zip(Ci, Ti):
    Ui.append(x/j)
    U += x/j

if(U>1):
    print("Utotal = "+ str(U) + " > 1")
    print("Pot ser programable? False")
    exit(0)
else:
    print("Utotal = "+ str(U) + " <= 1")

#--------------------------------#

#H = lcm(Ti)
from math import gcd
lcm = int(Ti[0])
for i in Ti[1:]:
  lcm = int((lcm*i)/gcd(lcm, i))
print("H = " + str(lcm))

#--------------------------------#

L = 0
for t, d, u in zip(Ti, Di, Ui):
    L += (t-d)*u
L /= 1-U
print("L = " + str(L))

#--------------------------------#

D = []
for i in range(0, len(Ci)):
    D0 = Di[i]
    while D0 <= min(lcm, L):
        if D.count(D0) == 0:
            D.append(D0)
        D0 += Ti[i]
D.sort()
print("D = " + str(D))

#--------------------------------#

for l in D:
    g = 0
    for t, d, c in zip(Ti, Di, Ci):
        g += int((l+t-d)/t)*c
    if g > l:
        print("g = " + str(g) > "l = " + l)
        print("Pot ser programable? False")
        exit(0)
    else:
        print("g(0," + str(l) + ") = " + str(g) + " <= l = " + str(l))
print("Pot ser programable? True")