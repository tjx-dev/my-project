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
# students=[]
# with open("scores.txt","r",encoding="utf-8") as f:
#     for x in f:
#         x=x.strip()
#         if x=="":
#             continue
#         name,score = x.split(",") #根据逗号分开
#         students.append((name,score))
# print(students)

# grades = [
#     {"name": "张三", "math": 90, "english": 85},
#     {"name": "李四", "math": 78, "english": 92},
#     {"name": "王五", "math": 88, "english": 88}
# ]
# with open("report.txt","w",encoding="utf-8") as f:
#     f.write(f"{'姓名':<4} {'数学':>4} {'英语':>4} {'总分':>4} {'平均分':>6}\n")
#     for i in grades:
#         total=i['math']+i['english']
#         arg=total/2
#         f.write(f"{i['name']:<4}  {i['math']:>4}  {i['english']:>4}    {total:>4}    {arg:>6.1f}\n")

#第八题
# def safe(a,b):
#     try:
#        num= a/b
#     except ZeroDivisionError :
#         print('除数不能为0')
#     except TypeError:
#         print('请输入数字')
#     else:
#         print(f"{num:.2f}")
#     finally:
#         print("over")

# safe(9,0)
# safe('a',2)

#第九题
# class TooLongError(Exception):
#     """字符串太长"""
#     pass
# def check(s):
#     if len(s)>10:
#         raise TooLongError('字符串太长')
#     if s!=str(s):#用isinstance(s,str) 表示是否是字符串
#         raise TypeError("必须是字符串")
#     print(s)
# for item in ['hello',123,'qwertyuoplkjhgfds']:
#     try :
#         check(item)
#     except TooLongError as e:
#         print('字符串太长',e)
#     except TypeError as e:
#         print('不是字符串',e)

# for i in range(1,10):
#     for j in range(1,i+1):
#         print(f"{j}*{i}={j*i}",end="\t")
#     print()


#第十题
# class Book:
#     def __init__(self,title,author,pages):
#         self.title=title
#         self.author=author
#         self.pages=pages
#     def info(self):
#         return f"【{self.title}】by {self.author}"
#     def __str__(self):
#         return f"【{self.title}】共{self.pages}页"
# book1=Book('三体','刘慈欣',300)
# print(book1.info())
# print(book1)
# book2=Book('活着','余华',200)
# print(book2.info())
# print(book2)

# class Student:
#     def __init__(self,name,score):
#         self.name=name
#         self.score=score
#     @property
#     def score(self): 
#         return self._score
#     @score.setter
#     def score(self,value):
#         if value<0 or value>100:
#             raise ValueError("分数必须在0~100")
#         if not isinstance(value,int):
#             raise TypeError("分数必须是整数")
#         self._score=value
#     def __str__(self):
#         return f"{self.name}:{self.score}分"
# c1=Student('小明',85)
# print(c1.score)
# print(c1)
# try:
#     c2=Student('小黑',105)
# except (ValueError,TypeError) as e:
#     print(e)









