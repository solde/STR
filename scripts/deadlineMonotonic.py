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
print("Ti: " + str(Di))

#--------------------------------#

# Sum(Ci/Ti) <= 1
U = 0
for x, j in zip(Ci, Ti):
    U += x/j

print("Utotal = "+ str(U))
if(U>1):
    print("Pot ser programable? False")
    exit(0)

#--------------------------------#

from math import pow
idk = len(Ci)*(pow(2,1/len(Ci))-1)
if U > idk:
    print("Pot ser programable? False")
    exit(0)

#--------------------------------#

# Wi = Ci+Sum[W/T]*Cj

for i in range(0,len(Ci)):
    W=Ci[i]
    Want = 0
    while W != Want:
        Want = W
        W=Ci[i]
        if W > Di[i]:
            print("Pot ser programable? False")
            exit(0)
        for j in range(0,i):
            W += (int((Want/Ti[j]))+1)*Ci[j]
        print("W = " + str(W))
        if W > Di[i]:
            print("Pot ser programable? False")
            exit(0)
print("Pot ser programable? True")
    