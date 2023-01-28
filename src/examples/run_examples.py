if __name__ == "__main__":
    from os import system, listdir

    exceptions = ["rule110.fux"]
    files: list[str] = [file for file in listdir("src/examples/") if file.endswith(".fux") and file not in exceptions]
    for file in files:
        system(f"./fux src/examples/{file}")
        print("\033[31m"+'-'*60+"\033[39m")