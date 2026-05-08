#面向对象

#类(模板)与对象(实例)
#class 类名:
#    pass

#对象名=类名() 对象名。属性名1=属性值1
#__dict__特殊属性，表示全部属性

# class Car:
#     pass
# c1=Car()
# c1.name="bwm"
# c1.color="black"
# c1.pice=500000
# print(c1.__dict__)
# print(c1.color)

#类的外面叫做函数，在类的里面叫做方法
# class 类名:
#     def __init__(self,参数列表):
#         self.属性名=参数值
#         
#对象名=类名(参数列表)

# class Car:
#     def __init__(self,c_name,c_color,c_price):
#         #self 创建实例对象
#         self.name=c_name
#         self.color=c_color
#         self.price=c_price
# c1=Car("BWM","red",500000)
# c2=Car("奔驰","pink",80000)
# print(c1.__dict__)
# print(c1.price)
# print(c2.__dict__)

#实例方法
# class 类名:
#     def __init__(self,参数列表):
#         self.属性名=参数值
# def 方法名(self,形参列表):

#对象名=类名(参数列表) 对象名.方法名(实参)
# class Car:
#     def __init__(self,c_name,c_color,c_price):
#         self.name=c_name
#         self.color=c_color
#         self.price=c_price

# #定义实例方法
#     def running(self):
#         print(f"{self.name},正在驾驶")
#     def total(self,discount,rate): 
#         total = self.price*(discount+rate)
#         return total
# c1=Car("BWM","red",500000)
# c1.running()
# total=c1.total(0.8,0.1)
# print(total)

#魔法方法
#以双下划线定义的特殊方法，__init__
#__init__  初始化  __str__  表示字符串
#__eq__ 俩个对象是否相等   le<=,lt<,gt>,ge>=
#删除对象 __del__   手动删或者执行结束，都会调用这个函数

# class Car:
#     def __init__(self,c_name,c_color,c_price):
#         self.name=c_name
#         self.color=c_color
#         self.price=c_price
#     def running(self):
#         print(f"{self.name},正在驾驶")
#         #魔法方法
#     def __str__(self):
#         return f'{self.color}' f"{self.name}" f"{self.price}"
#     def __eq__(self, other):
#         return self.name == other.name and self.color == other.color
#     def __lt__(self, other):
#         return self.price < other.price
#     def total(self,discount,rate):
#         total = self.price*(discount+rate)
#         return total
# c1=Car("BWM","red",500000)
# c1.running()
# total=c1.total(0.8,0.1)
# print(total)

# c2=Car("BWM","red",500000)
# print(c1)
# print(c1==c2)
# print(c1 < c2)

#实例属性(对象的属性)和类属性(类本身的属性)
# class Car:
# #类属性
#     wheel=4
#     rate=0.1
#     def __init__(self,c_name,c_color,c_price):
#        #实例属性
#         self.name=c_name
#         self.color=c_color
#         self.price=c_price


#     def running(self):
#         print(f"{self.name},正在驾驶")
#     def total(self,discount,rate):
#         total = self.price*(discount+rate)
#         return total
# c1=Car("BWM","red",500000)
# c1.running()
# total=c1.total(0.8,0.1)
# print(total)

# class Student :
#     def __init__(self,name,score):
#         self.name=name
#         self.score=score
#     def is_pass(self):
#         return self.score>=60
# zs=Student("张三",85)
# ls=Student("李四",55)
# print(zs.is_pass())


#封装  把属性和方法封装一起，仅给别人去访问 
#继承  孩子给老爹的东西
#多态  同样一个函数在不同场景表现不同形态

#继承(类的继承)
#class  子类（父类）
    #pass
#单继承
# class Master:
#     def __init__(self):
#         self.jb='[煎饼方法]'
#     def make(self):
#         print(f"用{self.jb}制作")
    
# class Student(Master):
#     pass
# c=Student()
# c.make()
   
#多继承
#class 父类1：
    #pass
#class 父类2：
#   pass
#class  子类（父类1，父类2）：（就近原则）
    #pass

#子类调用父类的方法
#1.父类名.父类方法名（self）（父类全部）
#2.super().父类函数名（）（只能是最近的父类） 单继承













