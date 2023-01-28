if __name__ == "__main__":
    from os import system, listdir

    exceptions = []
    files: list[str] = [file for file in listdir("src/examples/") if file.endswith(".fux") and file not in exceptions]
    for file in files:
        print("Running:", file)
        system(f"time ./fux src/examples/{file}")
        print("\033[31m"+'-'*60+"\033[39m")