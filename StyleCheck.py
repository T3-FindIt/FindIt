import sys

def printToStderr(string:str):
    print(string, file=sys.stderr)

def main():
    printToStderr("This test should FAIL, and this string should be easily visible in the output.")
    exit -1

if __name__ == "__main__":
    main()
