import os
import sys
import re

#--------------------------------------------------------------------------------------------------#
#                                                                                                  #
#                                              Prints                                              #
#                                                                                                  #
#--------------------------------------------------------------------------------------------------#

def printFileTypeError(path:str, errorMessage:str):
    printToStderr("\r")
    printToStderr("Forbidden File Found")
    printToStderr("File " + path + ", " + errorMessage)
    printToStderr("\r")

def printError(index:int, lineContents:str, path:str, errorMessage:str, optionalMessage:str = ""):
    printToStderr("\r")
    printToStderr("Forbidden Style Found")
    printToStderr("Line " + str(index) + " in " + path + ", " + errorMessage)
    printToStderr("\tLine: " + lineContents.strip())

    if optionalMessage != "":
        printToStderr("\t" + optionalMessage)

    printToStderr("\r")

def printToStderr(string:str):
    print(string, file=sys.stderr)

#--------------------------------------------------------------------------------------------------#
#                                                                                                  #
#                                          Style Checkers                                          #
#                                                                                                  #
#--------------------------------------------------------------------------------------------------#

#------------------------------------------------#
#                                                #
#             C / C++ Common Checks              #
#                                                #
#------------------------------------------------#

def checkCCPPCommonStyle(path:str):
    failed = False

    with open(path, "r") as file:

        for index, line in enumerate(file, 1):

            def checkBeforeStatement(lineBeforeStatement:str):
                if lineBeforeStatement.count(' ') != lineBeforeStatement.__len__():
                    printError(index, line, path, "has code before if statement")
                    failed = True

            def checkAfterStatement(lineAfterStatement:str):
                if lineAfterStatement.count('{') > 0:
                    printError(index, line, path, "has '{' on same line as statement")
                    failed = True

            def checkForOpeningBracket(lineAfterStatement:str, statement:str):
                indexStatement = line.find(statement) + statement.__len__()
                indexBracket = line.find("(")

                if indexBracket == -1:
                    printError(index, line, path, "has no opening bracket on the same line as statement")
                    failed = True
                elif indexBracket - indexStatement != 1:
                    printError(index, line, path, "has incorrect spacing between statement and opening bracket", ("Spaces: " + str(indexBracket - indexStatement)))
                    failed = True

            if line.find("if") != -1:
                if line.find("else") != -1:
                    checkBeforeStatement(line[:line.find("else")])
                    indexElse = line.find("else") + 4
                    indexIf = line.find("if")

                    if indexIf - indexElse != 1:
                        printError(index, line, path, "has incorrect spacing between else and if", ("Spaces: " + str(indexIf - indexElse)))
                        failed = True
                else:
                    checkBeforeStatement(line[:line.find("if")])

                checkForOpeningBracket(line[line.find("if") + 2:], "if")
                checkAfterStatement(line[line.find("if") + 2:])

            elif line.find("else") != -1:
                checkBeforeStatement(line[:line.find("else")])
                checkAfterStatement(line[line.find("else") + 4:])

            elif line.find("for") != -1:
                checkBeforeStatement(line[:line.find("for")])
                checkForOpeningBracket(line[line.find("for") + 3:], "for")
                checkAfterStatement(line[line.find("for") + 3:])
            
            elif line.find("while") != -1:
                checkBeforeStatement(line[:line.find("while")])
                checkForOpeningBracket(line[line.find("while") + 5:], "while")
                checkAfterStatement(line[line.find("while") + 5:])

            elif line.find("using namespace") != -1:
                printError(index, line, path, "has 'using namespace' statement")

            if line[:line.find(line.lstrip())].__len__() % 4 != 0:
                printError(index, line, path, "has incorrect spacing", ("Spaces: " + str(line[:line.find(line.lstrip())].__len__())))
                failed = True

            if line != "" and line.__len__() > 1:
                if line[-2] == " ":
                    printError(index, line, path, "whitespace at end of line")
                    failed = True

    return failed

def CheckHHPPCommonStyle(path:str):
    failed = False

    with open(path, "r") as file:
        includeGuardIfndefPresent = False
        includeGuardDefinePresent = False
        includeGuardEndifPresent = False
        fileName = path[path.rfind("\\") + 1:]
        fileExtension = fileName[fileName.rfind(".") + 1:]
        fileName = fileName[:fileName.rfind(".")]
        words = re.sub( r"([A-Z])", r" \1", fileName).split()
        includeGuard = ""
        nrOfWords = sum(1 for word in words)

        for index, word in enumerate(words, 1):
            includeGuard += word.upper()

            if index != nrOfWords:
                includeGuard += "_"
            else:
                includeGuard += "_" + fileExtension.upper()

        for index, line in enumerate(file, 1):

            def checkIncludeGuard(guard:str, isClosingGuard:bool = False):
                nrOfSpaces = guard[:guard.find(guard.lstrip())].__len__()
                if nrOfSpaces > 1:
                    printError(index, line, path, "has incorrect spacing", ("Spaces: " + str(nrOfSpaces)))
                    failed = True

                if not isClosingGuard:
                    for wordIndex, char in enumerate(includeGuard, 1):
                        if char != guard[wordIndex]:
                            printError(index, line, path, "has incorrect include guard", ("Expected: " + includeGuard + ", Actual: " + guard))
                            failed = True
                            break
                else:
                    if guard[1] != '/' or guard[2] != '/':
                        printError(index, line, path, "has incorrect include guard", ("Expected: // " + includeGuard + ", Actual: " + guard))
                        failed = True
                    elif len(guard[3:]) - len(guard[3:].lstrip()) != 1:
                        printError(index, line, path, "has incorrect include guard spacing between // and name", ("Expected: // " + includeGuard + ", Actual: " + guard))
                        failed = True
                    else:
                        for wordIndex, char in enumerate(includeGuard, 4):
                            if char != guard[wordIndex]:
                                printError(index, line, path, "has incorrect include guard", ("Expected: " + includeGuard + ", Actual: " + guard))
                                failed = True
                                break

            if line.find("#ifndef") != -1:
                checkIncludeGuard(line[line.find("#ifndef") + 7:])
                includeGuardIfndefPresent = True

            elif line.find("#define") != -1:
                checkIncludeGuard(line[line.find("#define") + 7:])
                includeGuardDefinePresent = True

            elif line.find("#endif") != -1:
                checkIncludeGuard(line[line.find("#endif") + 6:], True)
                includeGuardEndifPresent = True

            elif line.find("using namespace") != -1:
                printError(index, line, path, "has 'using namespace' statement")
                failed = True

            if line[:line.find(line.lstrip())].__len__() % 4 != 0:
                printError(index, line, path, "has incorrect spacing", ("Spaces: " + str(line[:line.find(line.lstrip())].__len__())))
                failed = True

            if line != "" and line.__len__() > 1:
                if line[-2] == " ":
                    printError(index, line, path, "whitespace at end of line")
                    failed = True

        if not includeGuardIfndefPresent and not includeGuardDefinePresent and not includeGuardEndifPresent:
            printError(0, "", path, "missing include guard")
            failed = True

        elif not includeGuardIfndefPresent or not includeGuardDefinePresent or not includeGuardEndifPresent:
            printError(0, "", path, "partially missing include guard")
            failed = True

    return failed

#------------------------------------------------#
#                                                #
#                      C++                       #
#                                                #
#------------------------------------------------#

def checkHPPStyle(path:str):
    failed = False
    failed = CheckHHPPCommonStyle(path)
    return failed

def checkCPPStyle(path:str):
    failed = False
    failed = checkCCPPCommonStyle(path)
    return failed

#------------------------------------------------#
#                                                #
#                       C                        #
#                                                #
#------------------------------------------------#

def checkHStyle(path:str):
    failed = False
    failed = CheckHHPPCommonStyle(path)
    return failed

def checkCStyle(path:str):
    failed = False
    failed = checkCCPPCommonStyle(path)
    return failed

#--------------------------------------------------------------------------------------------------#
#                                                                                                  #
#                                      Global Checking Logic                                       #
#                                                                                                  #
#--------------------------------------------------------------------------------------------------#

def checkFolder(folderPath:str):
    failed = False
    files = os.listdir(folderPath)

    for file in files:
        absPath = os.path.join(folderPath, file)

        if os.path.isdir(absPath):
            folderName = os.path.basename(absPath)

            if (folderName != ".git"
                and folderName != ".github"
                and folderName != "build"
                and folderName != "lib"):
                failed = checkFolder(absPath)

            continue

        if not os.path.isfile(absPath):
            continue

        name, extension = os.path.splitext(file)
        file_failed_check = False

        if extension == ".hpp":
            file_failed_check = checkHPPStyle(absPath)
        elif extension == ".cpp":
            file_failed_check = checkCPPStyle(absPath)
        elif extension == ".h":
            file_failed_check = checkHStyle(absPath)
        elif extension == ".c":
            file_failed_check = checkCStyle(absPath)

        if file_failed_check == True:
            failed = True

    return failed

def main():
    failed = False
    dir = os.path.abspath(os.path.dirname(__file__))
    failed = checkFolder(dir)

    if (failed == True):
        exit(-1)
    else:
        exit(0)

if __name__ == "__main__":
    main()
