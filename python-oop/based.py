作用域
#全局变量 函数定义外的变量
# a=100
# def c():
#     print(a)
# def b():
#     print(a)
# c()
# b()
# a=120
# print(a)#离得比较近的函数

#局部函数 在函数内部 不可以调用
# def funa():
#     num=10
#     print(num)
# funa()

#global 变量名   将局部变量声明改成全局变量
# 
# def funa():
#     global funa
#     num=10
#     print(num)
# funa()
# print(funa())

#修改外层变量
# def outer():
#     count=0
#     def inner():
#         nonlocal count
#         count+=1
#         return count
#     return inner
# c=outer()
# print(c())

#匿名函数 lambda
#函数名=lambda 形参：返回值（表达式）

#普通函数
# def add(a,b):
#     return a+b
# print(add(3,6))
#匿名函数 a,b就是返回值，a+b就是返回值的表达式
# add=lambda a,b:a+b  
# print(add(3,6))

#2.lambda 参数形式
#函数名=lambda 形参：返回值（表达式）
#无参数
# funb=lambda name:name
# print(funb('tjx'))
# #俩参数
# funa=lambda name,age=18:(name,age)
# print(funa('mjc'))
# func=lambda a,b,c=12:a*b+c
# print(func(3,5))
#c已经赋值，也可以重新赋值
# fund=lambda **kwargs:kwargs
# print(fund(name='mjc',age=19))

#lambda和if嵌套
#lambda 参数：值1 if 条件 else 值2
# grade=lambda score:'优秀' if score>90 else '不行'
# print(grade(98))

#内置函数
#abs 绝对值 
# print(min(-8,5,key=abs))

#zip() 将对应元素打包成一个元组 以短的为主
#list  将内置函数表示出来 zip,filter ,map
# a=[1,2,3,4]
# b=[5,6,7,8]
# print(list(zip(a,b)))
#矩阵的转置
# c=[[1,2,3,4],[4,5,6,7]]
# print(list(zip(*c)))

#map(函数，对象) 负责处理函数
# c=[2]
# def fin(a):
#     return a**5
# print(list(map(fin,c)))

#reduce 在元素里面取俩个计算，在把结果和下一个进行计算
#先导入 functools
# from functools import reduce
# a=[1,2,3]
# def add(x,y):
#     return x*y
# print(reduce(add,a))

#filter 表示判断
# a=[1,2]
# app=lambda x:x%2==0
# print(list(filter(app,a)))

#sorted(对象，key=...)
# a=[10,50,30]
# print(sorted(a,key=lambda n:n))


#拆包
# a,b=10,20
# a,b=b,a
# print(a,b)

#元素和对象个数要一样
# tua=[1,2,3,4]
# a,b,c,d=tua
# print(a,b,c,d)
# a,*b,c=[1,2,3,45,6,78,9]
# print(a)
# print(b)#收集剩余元素
# print(c)


#异常
#创建一个exception（'xxx'）xxx-异常提示信息
#raise 异常对象
# class AgeError(Exception):
#     '''年龄异常'''
#     pass
# def aged(age):
#     if age<0  or age> 150:
#         raise AgeError('年龄不符合')
#     print(f'年龄是{age}')

# try:
#     aged(200)
# except AgeError as e:
#     print(e)
#else:
    #没有异常
    #print("结果")
#NameError,ZeroError,IndexError,


#模块 每个.py就是一个模块
#import 模块名
#print(模块.函数)
# import my_math
# print(my_math.PI)
# print(my_math.add(3,6))

#from....import...as 别名
# from functools import reduce 
# c=[1,23,4,56,5]
# print(reduce(lambda a,b:a+b,c))

#from...import 功能名
# from random import randint
# for a in range(1,100):


# if __name__=="__main_"的作用
#在if上面的为公共的，在if里面的为独有的，在外界引用函数时，里面的不会被调调用

# if __name__=="__main_":
#     print(add(2,3))

#包
#my_package
    #__init__.py
    #module_a.py
    #module_b.py(重新创建一个)
#from my_package import moudle_a.py  (首先执行init.py文件的代码)
# from mypackage.module_b import some_function


#闭包
#1.存在嵌套函数  内层引用外层，外层返回内层
# def outer(start):
#     count=start
#     def inner():
#         nonlocal count
#         count+=1
#         return count
#     return inner
# a=outer(20)
# print(a())
# print(a())

#装饰器
# def a(start):
#     def b():
#         print('要开始了')
#         start()
#         print("要结束了")
        
#     return b
# @a
# def c():
#    print('开始')
# c()
    




























