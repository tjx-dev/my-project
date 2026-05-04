#  if 嵌套
# a=input('输入体温')
# if a>='37':
#     print('温度不正常')
#     if '37'<a<'39':
#         print('温度过高')
#     elif a>='39':
#         print('隔离')
# else:
#     print('体温正常')

# for a in range (1,101):
#     a='好好学习，天天向上'
#     print(a)

# i=1
# s=0
# while i<=100:
#     s+=i
#     i+=1
# print(s)

# # while#  嵌套循环
# i=1
# while i<=3:
#     print(f'这是第{i}次')
#     k=1
#     while k<=3:
#         print(f'里面第{k}次')
#         k+=1
#     i+=1

# s=0
# for i in range (1,101):
#     s+=i
# print(s)    

# break(停止)/continue(跳过某个)
# i=1
# while i<=5:
#     print(f'在吃第{i}个')
#     if i==3:
#         break
#     i+=1

# i=1
# while i<=5:
#     print(f'第{i}个')
#     if i==3:
#         print('不吃了')
#         i+=1
#         continue
#     i+=1

# a='123'
# b=a.encode() 
# # c=b.decode()
# print(type(b))

# name='mjc520'
#  左边是0开始索引，右边是-1开始
# print(name[1])
# print(name[-1])

# 切片，包前不包后
# a='mjc520'
# print(a[3:6:2])
# print(a[0:5])
# print(a[-1:-5:-2])

# 字符串
# a='taijunxiang'
# print(a.find('i'))
# print(a.find('u',5))#-1 超出范围，不包括返回-1
# print(a.find('a',1,6))

# index找不到就会报异常
# a='天天开心，心想事成'
# print(a.index('心'))
# print(a.index('心',6))

# count(),出现的次数，没有就返回
# a='taijunxiang'
# print(a.count('a'))
# print(a.count('i',1,6))

# startswith是就是true，不是就是false
# a='taijunxiang'
# print(a.startswith("t"))
# print(a.startswith("t",2,6))

# capitalize(第一个字符大写)，startswith（是以某字符开头）
# lower变小写，upper变大写
# a='ABC'
# print(a.lower())
# c='Tx'
# print(c.upper())
# print(a.capitalize())

# replace(旧内容，新内容，次数)
# a='taijunxiang'
# print(a.replace('tai','ma'))
# print(a.replace('tai','ma',2))

# split分割
# a='taijunxiang'
# print(a.split('j'))
# 从j开始分割

# a='taijunxiang'
# # capitalize第一个变大写，其它的小写
# print(a.capitalize())

# a=['1','2']
# # 添加元素append，extend，insert
# # append整体添加
# # extend分散添加，insert指定位置添加
# a.append('6')
# a.extend('5231')
# a.insert(0,'9')
# print(a)
#  
# 修改
# a=[1,2,3,4,5,6]
# a[2]=0
# print(a)

# 查找
# a=[1,2,3,4,5,6]
# print(4 in a)

# a=['xx','hh','jj']
# while True:
#     b=input('输入你的名字')
#     if b in a:
#         print(f'名字{b}已经使用过')
#     else:
#         print(f'名字{b}成功注册')
#         a.append(b)
#         print(a)  
#         break

# del删除，pop默认删除最后一个元素,不能超出范围
# a=[1,2,3,4,5,6]
# del a[3]
# print(a)

# a.pop()
# print(a)

# a.remove(4)
# print(a)
# remove,不存在就会报错，默认删除开始的指定元素

# mjc=[]
# [mjc.append(i) for i in range(1,6)]
# print(mjc)

# 列表嵌套
# a=[1,2,3,[4,5,6]]
# print(a[3])
# print(a[3][2])

# 元组tuple，# 小括号，逗号隔开,元组不支持修改，只能查询,切片
# a=[1,2,6,1]
# print(a.index(2))
# print(a.count(1))
# print(a[2:])

# name='mjc'
# age=19
# print('%s的年龄是：%d'%(name,age))
# # 数据不可以修改
# %s可以占位字符串，%d占位整数，%f占位浮点型

# 字典，用冒号隔开
# dic={键1：值1，键2：值2}键具有唯一性，值可以重复
# dic={'name':'mjc','age':'19'}
# print(type(dic))
# 不可以根据下标查找元素
# print(dic[2])会报错
# print(dic['age'])

# 变量名.get（键名）
# print(dic.get('terl')),会给none

# 修改
# dic['age']=80
# print(dic)

# 添加元素
# dic['tel']=123456
# print(dic)

# 删除
# del 字典名（删除整个）
# del dic['age']
# print(dic)

# clear(删掉内容，保留字典)
# dic.clear()
# print(dic)

# pop删除指定键和值，不存在就报错
# dic.pop('name')
# print(dic)

#  len()求长度
# dic={'name':'mjc','age':'19'}
# print(len(dic))
# a=[1,2,3,4]
# print(len(a))

# 返回所有键名
# dic=#{'name':'mjc','age':'19'}
# print(dic.keys())
# 用for循环取键名
# for i in dic:
#     print(i)

# 返回所有值
# dic={'name':'mjc','age':'19'}
# print(dic.values())
# for i in dic.values():          
#     print(i)

# items()返回键和值
# dic={'name':'mjc','age':'19'}
# for i in dic.items():
#     print(i)

# 集合
# s1={1,2,3,4}
# print(s1,type(s1)) set集合
 
# 集合无序性
# a1={'1','2','5'}
# print(a1) 结果不一样
# a2={1,2,3,4,5}
# print(a2) 结果一样

# 集合的添加元素 add
# s1={1,2,3,4}
# s1.add((6))
# print(s1)
# s1.add(5,8) 报错，一次只能加一个
# print(s1)

# update 把元素分开，一个个加入到集合里面
# s1={1,23,4}
# s1.update((18,9))
# print(s1) 

# 删除元素，remove()
# s1={1,2,3,4}
# s1.remove((3))
# print(s1)
# s1.remove((5)) 没有就会报错

# pop() 删除第一个
# s1={1,2,3,4}
# s1.pop()
# print(s1)

# discard 删除指定的元素
# s1={1,2,3,4}
# s1.discard((2))
# print(s1)

# 交集和并集 交集& 没有就会返回set()
# s1={1,2,3,4}
# s2={3,4,5,6,7}
# print(s1 & s2)
# 并集 |
# print(s1 |s2)

# 类型转换
# int 整数 (只保留整数部分)
# print(int('-10'))
# float 浮点型
# str 字符串
# eval 计算py中的表达式
 
# age=int(input('输入年龄'))
# if age>=18:
#     print('成年人')

# print(10+10)
# print('10'+'10')
# print(eval("10+10")) 

# eval 可以实现list,dic,tuple和str之间的转换
# str-list
# a="[1]"
# b=eval(a)
# print(type(b)) eval,去引号

#list转换 可迭代对象 str,tuple,dict,set

# 深浅拷贝
# 赋值
# a=[1,2,3,4,5]
# b=a
# print(b)
# a.append(6)
# print(b)

# 浅拷贝 （里动外不动） ab都变
# import copy
# a=[1,2,3,4,5,[7,8]]
# b=copy.copy(a)
# 查看地址
# b[5].append(7)
# print(a)
# print(b)
# print(id(b)) 
#   id不同，拷贝速度块，原函数外层不变，内层改变

# 深拷贝（都不变）a变b不变
# import copy
# a=[1,2,3,4,5,[7,8]]
# b=copy.deepcopy(a)
# a[5].append(9)
# print(a)
# print(b)
#  只影响自身，不改变a

# 函数
# def 函数名():   定义函数
#     函数体
# 调用函数
# 函数名()
# def login():
#     print('不分手')
# login()  
# #   调用函数

 

# 返回值
# 执行结束后，给一个结果
# 遇到return，表示函数结束，不能执行
# def b():
#     return 'mjc','tjx'  
# print(b())
# #     以元组的形式返回
 

# 参数
# def 函数名（形参a，形参b）：
#     函数体
# 调用格式：
# 函数名（实参1，实参2）
# def app(a,b):
#     return a+b
# print(app(3,5))
# def add(x,y,z):
#     return x+y*z
# print(add(6,2,2))


# 函数参数
# 必备参数(位置参数）
# 传递和定义函数顺序和个数要一致
# def funa(n1,n2,n3):
#     return n1,n2,n3
# print(funa('mjc','tjx','aa'))