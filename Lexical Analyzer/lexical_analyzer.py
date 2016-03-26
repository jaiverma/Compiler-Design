operators = ['+', '-', '*', '/', '=', '<', '>', '%', '==', '!=']
delimiters = ['{', '}', ';', '(', ')', '[', ']', ',']
keywords = ['int', 'main', 'void', 'char', 'float', 'double', 'for', 'while', 'to', 'if', 'else']

alphabets = ''
numbers = ''
others = ''

f = open('program.txt', 'r')

for line in f:
    for i in line:
        if i == ' ':
            numbers += ','
        if i.isalpha():
            alphabets += i
            if numbers != '' and numbers[-1] != ',':
                numbers += ','
        elif i.isdigit():
            numbers += i
            if alphabets != '' and alphabets[-1] != ',':
                alphabets += ','
        else:
            others += i
            others += ','
            if numbers != '' and numbers[-1] != ',':
                numbers += ','
            if alphabets != '' and alphabets[-1] != ',':
                alphabets += ','

alphabets = alphabets.split(',')
alphabets = filter(lambda a: a != '' and a != '\n' and a != ' ', alphabets)

numbers = numbers.split(',')
numbers = filter(lambda a: a != '' and a != '\n' and a != ' ', numbers)

others = others.split(',')
others = filter(lambda a: a != '' and a != '\n' and a != ' ', others)

Keywords = []
Delimiters = []
Operators = []
Constants = numbers
Identifiers = []
Others = []

for i in alphabets:
    if i in keywords:
        Keywords.append(i)
    else:
        Identifiers.append(i)

for i in others:
    if i in delimiters:
        Delimiters.append(i)
    elif i in operators:
        Operators.append(i)
    else:
        Others.append(i)

print "----------Keywords----------"
for i in set(Keywords):
    print i
print "----------------------------"

print "----------Delimiters----------"
for i in set(Delimiters):
    print i
print "------------------------------"

print "----------Operators----------"
for i in set(Operators):
    print i
print "-----------------------------"

print "----------Constants----------"
for i in set(Constants):
    print i
print "-----------------------------"

print "----------Identifiers----------"
for i in set(Identifiers):
    print i
print "-------------------------------"

print "----------Others----------"
for i in set(Others):
    print i
print "--------------------------"
