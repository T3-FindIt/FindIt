import os
from ..logger.logger import *

supported_extensions = [".pu", ".puml", ".plantuml", ".iuml", ".wsd"]

def CheckFile(currentFilePath:str):
    '''Check a PlantUML file for errors.'''
    failed = False

    fileName = os.path.basename(currentFilePath)
    fileName, fileExtension = os.path.splitext(fileName)

    if fileName.count(' ') > 0:
        PrintFileError(currentFilePath, 'File name cannot contain whitespace.', 'File name: ' + fileName)
        failed = True

    if fileExtension != '.wsd':
        PrintFileError(currentFilePath, 'File extension is not .wsd.', 'File name: ' + fileName + fileExtension)
        failed = True

    return failed
