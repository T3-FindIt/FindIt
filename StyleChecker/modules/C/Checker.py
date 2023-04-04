import os
import re
from ..logger.logger import *

#==============================================================================#
#                                                                              #
#                                     Main                                     #
#                                                                              #
#==============================================================================#

supported_extensions = ['.c', '.h']

def CheckPair(sourcePath, headerPath):
    '''Check a pair of C source and header files.'''
    failed = False
    failed = CheckSource(sourcePath)
    failed = CheckHeader(headerPath)
    return failed

def CheckSource(sourcePath):
    '''Check a C source file.'''
    failed = False

    if CheckFileName(sourcePath) is True:
        failed = True

    with open(sourcePath, 'r') as sourceFile:
        lines = sourceFile.readlines()
        for index, line in enumerate(lines, 1):
            if (CheckForSkip(line, sourceFile = True)):
                continue

            if CheckForForbiddenToken(line, index, sourcePath) is True:
                failed = True

            if CheckForOperator(line, index, sourcePath) is True:
                failed = True

            if CheckCurlyBrackets(line, index, sourcePath) is True:
                failed = True

            if CheckWhileStatementSimple(line, index, sourcePath) is True:
                failed = True

            if CheckForStatementSimple(line, index, sourcePath) is True:
                failed = True

            if CheckIfStatementSimple(line, index, sourcePath) is True:
                failed = True

            if CheckIndentation(line, index, sourcePath) is True:
                failed = True

            if CheckTrailingWhiteSpace(line, index, sourcePath) is True:
                failed = True

    return failed

def CheckHeader(headerPath):
    '''Check a C header file.'''
    failed = False
    includeGuardPresent = [False, False, False]

    if CheckFileName(headerPath) is True:
        failed = True
    
    with open(headerPath, 'r') as headerFile:
        lines = headerFile.readlines()
        for index, line in enumerate(lines, 1):
            if (CheckForSkip(line, sourceFile = False)):
                continue

            if CheckForForbiddenToken(line, index, headerPath) is True:
                failed = True

            if CheckForOperator(line, index, headerPath) is True:
                failed = True

            includeGuardCheck =CheckIncludeGuard(line, headerPath)
            if includeGuardCheck[0] is False:
                includeGuardPresent[0] = True
            if includeGuardCheck[1] is False:
                includeGuardPresent[1] = True
            if includeGuardCheck[2] is False:
                includeGuardPresent[2] = True
            
            if CheckCurlyBrackets(line, index, headerPath) is True:
                failed = True

            if CheckWhileStatementSimple(line, index, headerPath) is True:
                failed = True

            if CheckForStatementSimple(line, index, headerPath) is True:
                failed = True
                
            if CheckIfStatementSimple(line, index, headerPath) is True:
                failed = True

            if CheckIndentation(line, index, headerPath) is True:
                failed = True

            if CheckTrailingWhiteSpace(line, index, headerPath) is True:
                failed = True

    if CheckIncludeGuardAtEndOfFileCheck(headerPath, includeGuardPresent) is True:
        failed = True

    return failed

#==============================================================================#
#                                                                              #
#                                    Checks                                    #
#                                                                              #
#==============================================================================#

def CheckForSkip(line:str, sourceFile:bool):
    '''Checks if a line should be skipped.\n
    Returns if the line should be skipped or not.'''

    # //, /*, */, *, #
    # Exceptions for #:
    # - #define
    # - #ifndef
    # - #endif
    skippedPrefixes = ['^[\s]*\/\/', '^[\s]*\/\*', '^[\s]*\*\/', '^[\s]*\*', '^[\s]*\#(?!define|ifndef|endif\\b)']

    for Prefix in skippedPrefixes:
        if re.search(Prefix, line) is not None:
            return True

    return False

def CheckFileName(currentFilePath:str):
    '''Checks if the file name is correct.\n
    Returns if the file name is correct or not.'''

    failed = False

    fileName = os.path.basename(currentFilePath)
    fileName = os.path.splitext(fileName)[0]

    # print(fileName)

    if fileName.count(' ') > 0:
        PrintFileError(currentFilePath, 'File name cannot contain whitespace.', 'File name: ' + fileName)
        failed = True

    return failed

def CheckForOperator(line:str, index:int, currentFilePath:str):
    '''Checks for correct operators on a single line.\n
    Returns if the check failed or not.'''

    failed = False

    # Regex to check for correct operators.
    # singleRegex = '(\+|-|=|\/|\*|%|==|!=|>|<|>=|<=|&&|\|\||&|\||\^|~|\+=|-=|\*=|\/=|%=|<<=|>>=|&=|\^=|\|=|<<|>>)'
    singleRegex = '(?(?!=(->))(\+|-|=|\/|%|==|!=|>|<|>=|<=|&&|\|\||\||\^|\+=|-=|\*=|\/=|%=|<<=|>>=|&=|\^=|\|=|<<|>>))'
    completeRegex = '(\\b' + singleRegex + '\\b)|(' + singleRegex + '\\b)|(\\b' + singleRegex + ')'
    if re.search(completeRegex, line) is not None:
        PrintStyleError(index, line, currentFilePath, 'Incorrect operator.', 'Operators must be preceded and followed by a whitespace.')
        failed = True

    return failed

def CheckForForbiddenToken(line:str, index:int, currentFilePath:str):
    '''Checks for forbidden tokens on a single line.\n'''
    failed = False
    
    # Regex to check for goto.
    if re.search('\\bgoto\\b[^;]*\;$', line) is not None:
        PrintStyleError(index, line, currentFilePath, 'Forbidden token.', 'The \'goto\' keyword is not allowed.')
        failed = True

    return failed

def CheckCurlyBrackets(line:str, index:int, currentFilePath:str):
    '''Checks for correct curly brackets on a single line.\n'''
    failed = False
    
    if line.find('{') == -1 and line.find('}') == -1:
        return failed

    if line.find('{') != -1:
        # Regex to check for correct opening curly brackets.
        if re.search('^(?![^\s{]+).*\{(?![^{\\n]+)$', line) is None:
            PrintStyleError(index, line, currentFilePath, 'Incorrect curly brackets.', 'Opening curly brackets may not be preceded or followed by any text.')
            failed = True

    if line.find('}') != -1:
        # Regex to check for correct closing curly brackets.
        if re.search('^(?![^\s}]+).*\}([^;]*\;)?$', line) is None:
            PrintStyleError(index, line, currentFilePath, 'Incorrect curly brackets.', 'Closing curly brackets may not be preceded or followed by any text except when the text is between the bracket and a \';\'.')
            failed = True
        
    return failed

def CheckIndentation(line:str, index:int, currentFilePath:str):
    '''Checks for correct indentation on a single line.\n
    Returns a tuple of (failed, indent).
    - failed: True if the check failed, False if check passed.
    - indent: The number of spaces the line is indented with.'''

    failed = False
    
    if line[:line.find(line.lstrip())].__len__() % 4 != 0:
                PrintStyleError(index, line, currentFilePath, "has incorrect spacing", ("Spaces: " + str(line[:line.find(line.lstrip())].__len__())))
                failed = True

    return (failed)

def CheckTrailingWhiteSpace(line:str, index:int, currentFilePath:str):
    '''Checks for trailing whitespace on a single line.\n
    Returns if the check failed or not.'''

    failed = False

    if line != "" and line.__len__() > 1:
        if line[-2] == " ":
            PrintStyleError(index, line, currentFilePath, 'Trailing whitespace.', 'Trailing whitespace is not allowed.')
            failed = True

    return failed

def CheckIfStatementSimple(line:str, index:int, currentFilePath:str):
    '''Checks if an if statement.\n
    Returns if the check failed or not.'''

    failed = False

    # Regex to check for 'if' with optional 'else' as whole words.
    # Any amount of whitespace BEFORE and BETWEEN these words is allowed.
    # (excludes sole else)
    if re.search('^\s*(\\belse\\b)?\s*\\bif\\b', line) is None:
        return failed

    # Regex to check for a valid if statement.
    # (excludes sole else)
    if re.search('^(?!.*\{).*(\\belse\\b)?\\bif\\b\s\((.*)(\)?)$', line) is not None:
        return failed
    
    ifIndex = line.find('if')
    firstParenIndex = line.find('(')
    if firstParenIndex == -1:
        PrintStyleError(index, line, currentFilePath, 'Incorrect if statement.', 'Missing opening parenthesis on same line.')
        failed = True
    else:
        if firstParenIndex - (ifIndex + 2) != 1:
            PrintStyleError(index, line, currentFilePath, 'Incorrect if statement.', 'There must be exactly one space between \'if\' and \'(\'.')
            failed = True

    elseIndex = line.find('else')
    if elseIndex != -1:
        if ifIndex - (elseIndex + 4) != 1:
            PrintStyleError(index, line, currentFilePath, 'Incorrect else statement.', 'There must be exactly one space between \'else\' and \'if\'.')
            failed = True

    return failed

def CheckWhileStatementSimple(line:str, index:int, currentFilePath:str):
    '''Checks if a while statement.\n
    Returns if the check failed or not.'''

    failed = False

    # Regex to check for 'while' as whole word.
    if re.search('^\s*\\bwhile\\b', line) is None:
        return failed
    
    whileIndex = line.find('while')
    firstParenIndex = line.find('(')
    if firstParenIndex == -1:
        PrintStyleError(index, line, currentFilePath, 'Incorrect while statement.', 'Missing opening parenthesis on same line.')
        failed = True
    else:
        if firstParenIndex - (whileIndex + 5) != 1:
            PrintStyleError(index, line, currentFilePath, 'Incorrect while statement.', 'There must be exactly one space between \'while\' and \'(\'.')
            failed = True

    return failed

def CheckForStatementSimple(line:str, index:int, currentFilePath:str):
    '''Checks if a for statement.\n
    Returns if the check failed or not.'''

    failed = False

    # Regex to check for 'for' as whole word.
    if re.search('^\s*\\bfor\\b', line) is None:
        return failed
    
    forIndex = line.find('for')
    firstParenIndex = line.find('(')
    if firstParenIndex == -1:
        PrintStyleError(index, line, currentFilePath, 'Incorrect for statement.', 'Missing opening parenthesis on same line.')
        failed = True
    else:
        if firstParenIndex - (forIndex + 3) != 1:
            PrintStyleError(index, line, currentFilePath, 'Incorrect for statement.', 'There must be exactly one space between \'for\' and \'(\'.')
            failed = True

    return failed

def CheckIncludeGuardAtEndOfFileCheck(currentFilePath:str, results):
    failed = False
    fileName = os.path.basename(currentFilePath)
    fileName = os.path.splitext(fileName)[0]

    words = re.sub('([A-Z])', r' \1', fileName).split()

    guardName = ''

    for word in words:
        guardName += word.upper() + '_'

    guardName = guardName.upper() + 'H'

    if results[0] is False and results[1] is False and results [2] is False:
        PrintFileError(currentFilePath, '(Correct) Include guard not found.', 'Include guard must be \'#ifndef/#define/#endif ' + guardName + '\'.')
        failed = True
        return failed

    if results[0] is False:
        PrintFileError(currentFilePath, '(Correct) #ifndef Include guard not found.', 'Include guard must be \'#ifndef ' + guardName + '\'.')
        failed = True
    if results[1] is False:
        PrintFileError(currentFilePath, '(Correct) #define Include guard not found.', 'Include guard must be \'#define ' + guardName + '\'.')
        failed = True
    if results[2] is False:
        PrintFileError(currentFilePath, '(Correct) #endif Include guard not found.', 'Include guard must be \'#endif ' + guardName + '\'.')
        failed = True

    return failed

def CheckIncludeGuard(line:str, currentFilePath:str):
    '''Checks if a header file has an include guard.\n
    Returns if the check failed or not.'''

    failed = [True, True, True]

    fileName = os.path.basename(currentFilePath)
    fileName = os.path.splitext(fileName)[0]

    words = re.sub('([A-Z])', r' \1', fileName).split()

    guardName = ''

    for word in words:
        guardName += word.upper() + '_'

    guardName = guardName.upper() + 'H'

    ifndefIndex = line.find('#ifndef')
    defineIndex = line.find('#define')
    endifIndex = line.find('#endif')

    if ifndefIndex != -1:
        tempFailed = False
        for _index, character in enumerate(guardName, (ifndefIndex + 8)):
            if character != line[_index]:
                tempFailed = True
                break
        failed[0] = tempFailed

    elif defineIndex != -1:
        tempFailed = False
        for _index, character in enumerate(guardName, (defineIndex + 8)):
            if character != line[_index]:
                tempFailed = True
                break
        failed[1] = tempFailed

    elif endifIndex != -1:
        tempFailed = False
        guardName = '// ' + guardName
        for _index, character in enumerate(guardName, (endifIndex + 7)):
            if character != line[_index]:
                tempFailed = True
                break
        failed[2] = tempFailed

    return failed
