# #第一题
# numbers=[12,7,9,20,15,5,8]
# print(list(map(lambda b:b+3,list(filter(lambda a:a>10,numbers)))))

# #第二题
# def filter_kwargs(**kwargs):
#     a=list(filter(lambda n:n[1]>18,kwargs.items()))
#     return a 
# res=filter_kwargs(Alice=25,Bob=12,David=30)
# print(res)

# #第三题
# def make_counter(start=0):
#     counter=start
#     def inner():
#         nonlocal counter
#         counter+=1
#         return counter
#     return inner
# my_counter=make_counter()
# data=[10,20,30] #没有用，就是单纯的调用

# from functools import reduce
# num=map(lambda x:my_counter(),data)
# sum=reduce(lambda x,y:x*y,num)
# print(sum)

# #第四题
# scores=[45,78,92,33,67,88,54]
# print(list(map(lambda x:x+5,list(filter(lambda y:y>60,scores)))))

# #第五题
# def greet_kwargs(**kwargs):
#     for name,age in kwargs.items():
#         if age>20:
#             print(f'Hello,{name}')
        
# greet_kwargs(Alice=25,Bob=17,David=15)

# #字符串格式化
# item='苹果'
# b=3.5
# c=2
# d=7
# print(f'商品:{item},单价:{b},数量:{c},总价:{c*b:.2f}元')

# name='tjx'
# scores=95
# print('{0:10}{1:10}'.format(name,scores))

#第六题
# price="7.5"
# quantity=3
# discount="0.9"
# a=float(price)
# b=float(discount)
# print(f"应付:{a*b*quantity}元")

# nums=[3,8,5,12,7,18,20,1]
# squared_evens=[x**2 for x in nums if x%2==0]
# for i in squared_evens:
#     if i>200:
#         break
#     if i<50:
#         continue
#     print(i)

# 基础分=58
# def grade_report(科目,*加分项,及格线=60,**附加信息):
#     global 基础分
#     加分总和=sum(加分项)
#     基础分+=加分总和
#     姓名=附加信息.get("姓名","匿名")
#     总分=基础分
#     是否及格="及格" if 总分>=及格线 else "不及格"
#     return f"姓名:{姓名},{科目},{总分},{是否及格}"
# res=grade_report("数学",5,3,及格线=60,姓名="小名",班级="1班")
# print(res)
# print(基础分)

#第七题
students=[]
with open("scores.txt","r",encoding="utf-8") as f:
    for x in f:
        x=x.strip()
        if x=="":
            continue
        name,score = x.split(",") #根据逗号分开
        students.append((name,score))
print(students)