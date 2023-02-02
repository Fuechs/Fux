from os import system, listdir
from time import time
from statistics import fmean

def run_file(file: str) -> float:
    start = time()
    print("Running:", file)
    system(f"time ./fux src/examples/{file}")
    print("\033[31m"+'-'*60+"\033[39m")
    return time() - start

def run_full(files: list[str]) -> tuple[float, list[float]]:
    start = time()
    times = [run_file(file) for file in files]
    return time() - start, times

if __name__ == "__main__":
    runs = 20
    exceptions = []
    files: list[str] = [file for file in listdir("src/examples/") if file.endswith(".fux") and file not in exceptions]
    
    start = time()
    time_list = [run_full(files) for i in range(runs)]
    end = time() - start
    
    run_time = [i[0] for i in time_list]
    file_time = []
    for run in time_list:
        file_time.extend(run[1])
        
    print('='*60)
    print(f"|| Total Time for {runs} runs: {end}s")
    print(f"|| Average Time per Run: {fmean(run_time)}s")
    print(f"|| Average Time per File: {fmean(file_time)}s")
    print('='*60)