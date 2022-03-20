import random, os
from dataclasses import dataclass

clear = lambda: os.system("clear")
# windows: clear = lambda: os.system("cls")

@dataclass
class version_struct:
    x: int
    y: int
    z: int
    c: str

version = version_struct(0, 0, 2, 'a')

run = None
profile = 0

def choose():
    choices = [True, False]
    
    return_value = random.choice(choices)
    return return_value

def main():
    global profile
    
    clear()
    
    run = choose()

    if not run: print("Not running")

    while run:

        match profile:

            case -1: profile = profile*profile;
            case 0: profile += 2
            case 1: profile -= 1
            case 2: profile += 1
            case _: run = False
         
        print(profile)

    print(f"v{version.x}.{version.y}.{version.z}{version.c}")


if __name__ == '__main__':
    main()