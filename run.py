import os

os.system(r"g++ main.cpp ./Models/Menu/CMenu.cpp ./Models/Menu/CMenuItem.cpp ./Models/User/Client.cpp ./Models/User/User.cpp ./Models/User/Employee.cpp ./Models/User/Deposit.cpp  -o bank && ./bank")