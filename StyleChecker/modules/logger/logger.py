import sys

# https://stackoverflow.com/questions/287871/how-do-i-print-colored-text-to-the-terminal
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def GenericPrint(identifier:str,
                 color:bcolors,
                 header:str,
                 body:str = '',
                 beginNewline:bool = False,
                 endNewline:bool = False):
    identifier = identifier.upper()
    
    if beginNewline:
        printToStderr('\r')
    printToStderr(color + identifier + ': ' + bcolors.HEADER + header + bcolors.ENDC)

    if body != '':
        printToStderr(body)

    if endNewline:
        printToStderr("\r")

def printToStderr(string:str):
    print(string, file=sys.stderr)

#==============================================================================#
#                                                                              #
#                                 Style Prints                                 #
#                                                                              #
#==============================================================================#

def PrintGenericStyle(index:int, lineContents:str, path:str, errorMessage:str, color:bcolors, identifier:str, header:str, optionalMessage:str = ''):
    body = 'Line ' + str(index) + ' in ' + path + ', ' + color + errorMessage + bcolors.ENDC
    body += '\n' + lineContents.strip()

    if optionalMessage != '':
        body += '\n\t' + optionalMessage

    GenericPrint(identifier, color, header, body, True, True)

def PrintStyleError(index:int, lineContents:str, path:str, errorMessage:str, optionalMessage:str = ''):
    PrintGenericStyle(index, lineContents, path, errorMessage, bcolors.FAIL, 'error', 'Style Error', optionalMessage)

def PrintStyleWarning(index:int, lineContents:str, path:str, errorMessage:str, optionalMessage:str = ''):
    PrintGenericStyle(index, lineContents, path, errorMessage, bcolors.WARNING, 'warning', 'Style Warning', optionalMessage)

def PrintStyleInfo(index:int, lineContents:str, path:str, errorMessage:str, optionalMessage:str = ''):
    PrintGenericStyle(index, lineContents, path, errorMessage, bcolors.OKGREEN, 'info', 'Style Info', optionalMessage)

#==============================================================================#
#                                                                              #
#                                 File Prints                                  #
#                                                                              #
#==============================================================================#

def PrintFileError(path:str, errorMessage:str, optionalMessage:str = ''):
    body = 'File ' + path + ', ' + bcolors.FAIL + errorMessage + bcolors.ENDC

    if optionalMessage != '':
        body += '\n\t' + optionalMessage

    GenericPrint('error', bcolors.FAIL,'File Error', body, True, True)

def PrintFileWarning(path:str, errorMessage:str, optionalMessage:str = ''):
    body = 'File ' + path + ', ' + bcolors.WARNING + errorMessage + bcolors.ENDC

    if optionalMessage != '':
        body += '\n\t' + optionalMessage

    GenericPrint('warning', bcolors.WARNING, 'File Warning', body, True, True)

def PrintFileInfo(path:str, errorMessage:str, optionalMessage:str = ''):
    body = 'File ' + path + ', ' + bcolors.OKGREEN + errorMessage + bcolors.ENDC

    if optionalMessage != '':
        body += '\n\t' + optionalMessage

    GenericPrint('info', bcolors.OKGREEN, 'File Info', body, True, True)

#==============================================================================#
#                                                                              #
#                                 Info Prints                                  #
#                                                                              #
#==============================================================================#

def Info(message:str, optionalMessage:str = '', beginNewline:bool = False, endNewline:bool = False):
    GenericPrint('info', bcolors.OKGREEN, message, optionalMessage, beginNewline, endNewline)

def __printChar(char):
    print(char, end='')

def __printColoredChar(char, index, cutoff):
    if index < cutoff:
        __printChar(bcolors.OKGREEN + char + bcolors.ENDC)
    else:
        __printChar(bcolors.FAIL + char + bcolors.ENDC)

def __printColoredCharConditionalChar(charIfTrue, charIfFalse, index, cutoff):
    if index < cutoff:
        __printChar(bcolors.OKGREEN + charIfTrue + bcolors.ENDC)
    else:
        __printChar(bcolors.FAIL + charIfFalse + bcolors.ENDC)

def __printStartingChar(char, part):
    __printColoredChar(char, 0, part)

def __printEndingChar(char, part, total):
    __printColoredChar(char, ( total - 1 ), part)

def PrintProgressBar(total, part, width=100, startChar='[', endChar=']', completeChar='▓', uncompleteChar='░'):
    progressCount = int( ( part / total ) * width)

    __printStartingChar(startChar, part)

    for i in range(0, width):
        __printColoredCharConditionalChar(completeChar, uncompleteChar, i, progressCount)

    __printEndingChar(endChar, part, total)
    print('')

def PrintProgressBarWithText(total, part, textOffset=2, width=100, startChar='[', endChar=']', completeChar='▓', uncompleteChar='░'):
    text = str(part) + '/' + str(total)
    textLength = len(text)

    progressCount = int( ( part / total ) * width)

    __printStartingChar(startChar, part)

    for i in range(0, width):
        if (i - textOffset) < textLength and i >= textOffset:
            __printColoredChar(text[i - textOffset], i, progressCount)
        else:
            __printColoredCharConditionalChar(completeChar, uncompleteChar, i, progressCount)

    __printEndingChar(endChar, part, total)
    print('')
