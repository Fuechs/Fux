import os, sys, time
# from progress.bar import ChargingBar

platforms = ["linux", "linux2", "darwin"]

if sys.platform in platforms:
    clear = lambda: os.system("clear")
elif sys.platform == "win32":
    clear = lambda: os.system("cls")

clear()



def guide(part: str = '0'):

    if part == '0':
        print("Introduction...")
    
    elif part.startswith('I'):
        print("Part 1...")

    time.sleep(0.5)



def menu():

    run = True

    title = "Full Guide for Fux"
    head = len(title)
    subtitle = "Flawless unambigous programming"
    bottom = len(subtitle)

    while run:

        clear()

        print('-'*head)
        print(title)

        print("1 - Start guide")

        print("2 - Contents")

        print("3 - Exit")

        print(subtitle)
        print('-'*bottom)

        try: 
            option = int(input(">> "))

        except:
            print("Input has to be an integer.")
            time.sleep(0.65)
            continue

        match option:

            case 1: 
                guide()
            case 2: 

                title = "Contents"
                head = len(title)
                _run = True

                while _run:

                    clear()
                    print('-'*head)
                    print(title)

                    print("0 - Getting started")
                    print("I - Input and Output")

                    print(subtitle)
                    print('-'*bottom)

                    option = input(">> ")

                    match option:

                        case '0': guide()
                        case 'I': guide('I')

                        case _: print("Unknown part"); time.sleep(0.65); _run = False

                    _run = False

            case 3: 
                run = False
            case _: 
                print("Unknown option")
                time.sleep(0.65)
                continue

    clear()
    print("Thanks for using this guide.")
    time.sleep(0.65)
    exit()


menu()