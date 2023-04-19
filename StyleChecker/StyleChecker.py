import os

# Custom modules
import modules.logger.logger as logger
# Supported languages
import modules.C.Checker as CChecker
import modules.CPP.Checker as CPPChecker
import modules.PlantUml.Checker as PlantUmlChecker

def CheckFolder(folderPath:str, supported_extensions:list, ignored_folders:list):
    failed = False
    files = os.listdir(folderPath)
    fileCount = 0
    failedFiles = 0

    for file in files:
        absPath = os.path.join(folderPath, file)

        if os.path.isdir(absPath):
            folderName = os.path.basename(absPath)

            if (ignored_folders.count(folderName) == 0):
                checkFolderResult = CheckFolder(absPath, supported_extensions, ignored_folders)
                fileCount += checkFolderResult[1]
                failedFiles += checkFolderResult[2]
                if checkFolderResult[0] is True:
                    failed = True

            continue

        if not os.path.isfile(absPath):
            continue

        name, extension = os.path.splitext(file)

        if supported_extensions.count(extension) == 0:
            continue

        if extension == CChecker.supported_extensions[0]:
            fileCount += 1
            if files.count(name + CChecker.supported_extensions[1]) == 0:
                if 'TEST' in name.upper():
                    logger.PrintFileInfo(absPath, 'C source file found without a corresponding header file. This is expected for test files.', 'If this is not a test file, please rename it so it does not contain the word \'test\'.')
                elif 'MAIN' in name.upper():
                    logger.PrintFileInfo(absPath, 'C source file found without a corresponding header file. This is expected for main files.', 'If this is not a main file, please rename it so it does not contain the word \'main\'.')
                else:
                    logger.PrintFileWarning(absPath, 'C source file found without a corresponding header file.')
            
            if CChecker.CheckSource(absPath) is True:
                failed = True
                failedFiles += 1

        elif extension == CChecker.supported_extensions[1]:
            fileCount += 1
            if files.count(name + CChecker.supported_extensions[0]) == 0:
                logger.PrintFileWarning(absPath, 'C header file found without a corresponding source file.')

            if CChecker.CheckHeader(absPath) is True:
                failed = True
                failedFiles += 1

        elif extension == CPPChecker.supported_extensions[0]:
            fileCount += 1
            if files.count(name + CPPChecker.supported_extensions[1]) == 0:
                if 'TEST' in name.upper():
                    logger.PrintFileInfo(absPath, 'C++ source file found without a corresponding header file. This is expected for test files.', 'If this is not a test file, please rename it so it does not contain the word \'test\'.')
                elif 'MAIN' in name.upper():
                    logger.PrintFileInfo(absPath, 'C++ source file found without a corresponding header file. This is expected for main files.', 'If this is not a main file, please rename it so it does not contain the word \'main\'.')
                else:
                    logger.PrintFileWarning(absPath, 'C++ source file found without a corresponding header file.')

            if CPPChecker.CheckSource(absPath) is True:
                failed = True
                failedFiles += 1

        elif extension == CPPChecker.supported_extensions[1]:
            fileCount += 1
            if files.count(name + CPPChecker.supported_extensions[0]) == 0:
                logger.PrintFileWarning(absPath, 'C++ header file found without a corresponding source file.')

            if CPPChecker.CheckHeader(absPath) is True:
                failed = True
                failedFiles += 1

        elif PlantUmlChecker.supported_extensions.count(extension) > 0:
            fileCount += 1
            if PlantUmlChecker.CheckFile(absPath) is True:
                failed = True
                failedFiles += 1

    return [failed, fileCount, failedFiles]

def main():
    '''StyleChecker: A tool to check the style of certain source files.\n
    Currently supports:
    - C++
    - C
    
    If all checks pass, the program will exit with a return code of 0.\n
    If any checks fail, the program will exit with a return code of -1.'''

#==============================================================================#
#                                                                              #
#                                   Startup                                    #
#                                                                              #
#==============================================================================#

    failed = False

    clearConsole = lambda: os.system('cls')
    clearConsole()

    supported_extensions = []
    supported_extensions.extend(CChecker.supported_extensions)
    supported_extensions.extend(CPPChecker.supported_extensions)
    supported_extensions.extend(PlantUmlChecker.supported_extensions)

    ignored_folders = [".git", ".github", "build", "lib"]

    projectDir = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

#==============================================================================#
#                                                                              #
#                                   Checking                                   #
#                                                                              #
#==============================================================================#

    logger.Info('StyleChecker: Starting checks...', beginNewline = True, endNewline = False)
    logger.Info('Supported extensions: ',str(supported_extensions), beginNewline = False, endNewline = False)
    logger.Info('Ignored folders: ', str(ignored_folders), beginNewline = False, endNewline = True)

    results = CheckFolder(projectDir, supported_extensions, ignored_folders)

    passedFiles = ( results[1] - results[2] )

    try:
        terminalWidth = int(os.get_terminal_size().columns * 0.9)
    except:
        terminalWidth = 140

    if results[0]:
        logger.Info('StyleChecker: Failed one or more checks.', str(results[2]) + ' file(s) failed check(s).')
        print('')
        logger.PrintProgressBarWithText(results[1], passedFiles, width=terminalWidth)
        print('')
        exit(-1)
    else:
        logger.Info('StyleChecker: Passed all checks.')
        print('')
        logger.PrintProgressBarWithText(results[1], passedFiles, width=terminalWidth)
        print('')
        exit(0)

if __name__ == '__main__':
    main()
