#第一题
numbers=[12,7,9,20,15,5,8]
print(list(map(lambda b:b+3,list(filter(lambda a:a>10,numbers)))))

#第二题
def filter_kwargs(**kwargs):
    a=list(filter(lambda n:n[1]>18,kwargs.items()))
    return a 
res=filter_kwargs(Alice=25,Bob=12,David=30)
print(res)

#第三题
def make_counter(start=0):
    counter=start
    def inner():
        nonlocal counter
        counter+=1
        return counter
    return inner
my_counter=make_counter()
data=[10,20,30] #没有用，就是单纯的调用

from functools import reduce
num=map(lambda x:my_counter(),data)
sum=reduce(lambda x,y:x*y,num)
print(sum)

#第四题
scores=[45,78,92,33,67,88,54]
print(list(map(lambda x:x+5,list(filter(lambda y:y>60,scores)))))

#第五题
def greet_kwargs(**kwargs):
    for name,age in kwargs.items():
        if age>20:
            print(f'Hello,{name}')
        
greet_kwargs(Alice=25,Bob=17,David=15)