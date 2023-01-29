if __name__ == "__main__":
    from os import system, listdir
    from time import time

    exceptions = []
    files: list[str] = [file for file in listdir("src/examples/") if file.endswith(".fux") and file not in exceptions]
    start = time()
    for file in files:
        print("Running:", file)
        system(f"time ./fux src/examples/{file}")
        print("\033[31m"+'-'*60+"\033[39m")
    print(f"Time: {time() - start}s")