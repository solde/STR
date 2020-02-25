# Data entry

print("Enter Ci:")
Ci = []
new_element = input()]
while(new_element != str("-1")):
    Ci.append(float(new_element))
    new_element = input()
print("Ci: " + str(Ci))

print("Enter Di:")
Ti = []
new_element = input()
while(new_element != "-1"):
    Ti.append(float(new_element))
    new_element = input()
print("Ti: " + str(Ti))

print("Enter Di:")
Di = []
new_element = input()
while(new_element != "-1"):
    Di.append(float(new_element))
    new_element = input()
print("Ti: " + str(Ti))

#--------------------------------
# Sum(Ci/Ti <= n(2^(i/n)-1)

U = 0.0
for x, j in zip(Ci, Ti):
    #print(x/j)
    U += x/j

print("Utotal = "+ str(U))

#--------------------------------
# Π((Ci/Ti)+1) <= 2

U = 1.0
for x, j in zip(Ci, Ti):
    #print(x/j)
    U *= (x/j)+1

if U <= 2:
    print(str(U) + " <= 2")
else:
    print(str(U) + " > 2")

#--------------------------------
# Wi = Ci+Sum[W/T]*Cj

