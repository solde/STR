# Data entry

print("Enter Ci:")
Ci = []
new_element = input()
while(new_element != str("-1")):
    Ci.append(int(new_element))
    new_element = input()
print("Ci: " + str(Ci))

print("Enter Di:")
Ti = []
new_element = input()
while(new_element != "-1"):
    Ti.append(int(new_element))
    new_element = input()
print("Ti: " + str(Ti))

Di = Ti

#--------------------------------#

# Sum(Ci/Ti) <= 1
U = 0.0
for x, j in zip(Ci, Ti):
    U += x/j

print("Utotal = "+ str(U))
if(U>1):
    print("Pot ser programable? False")

#--------------------------------#

#H = lcm(Ti)
from math import gcd
lcm = int(Ti[0])
for i in Ti[1:]:
  lcm = int((lcm*i)/gcd(lcm, i))
print("H = " + str(lcm))

#--------------------------------#

maxC = max(Ci)
minD = min(Di)
H=[]
for i in [minD,maxC]:
    k = lcm/i
    if int(k) == k:
        H.append(i)
k=[]
sch=True
for t in Di:
    for h in H:
        newk = 2*h-gcd(h,t)
        if newk>t:
            sch=False
        else:
            if k.count(newk) == 0:
                k.append(newk)
print("Possibles periodes secundaris: " + str(k))
#--------------------------------#

print("Por ser programable? " + str(sch))