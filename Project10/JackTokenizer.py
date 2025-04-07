import os
multiComment=False
lineList=[]

tokenClassDict = {
    'class':'keyword',
    'constructor' : 'keyword',
    'function':'keyword',
    'method': 'keyword',
    'field' : 'keyword',
    'static': 'keyword',
    'var': 'keyword',
    'int': 'keyword',
    'char': 'keyword',
    'boolean': 'keyword',
    'void': 'keyword',
    'true': 'keyword',
    'false': 'keyword',
    'null': 'keyword',
    'this': 'keyword',
    'let': 'keyword',
    'do': 'keyword',
    'if': 'keyword',
    'else': 'keyword',
    'while': 'keyword',
    'return': 'keyword',
    '{':'symbol',
    '}':'symbol',
    '(':'symbol',
    ')':'symbol',
    '[':'symbol',
    ']':'symbol',
    '.':'symbol',
    ',':'symbol',
    ';':'symbol',
    '+':'symbol',
    '-':'symbol',
    '/':'symbol',
    '*':'symbol',
    '&':'symbol',
    '|':'symbol',
    '<':'symbol',
    '>':'symbol',
    '=':'symbol',
    '~':'symbol',
}

jackProgram="/mnt/c/Users/Faraz/nand2tetris/Project10/ExpressionLessSquare/Main.jack"
tokenizedOutxml= os.path.dirname(jackProgram) + '/'+ os.path.basename(jackProgram).split('.')[0]+'TT.xml'
print (tokenizedOutxml)

with open(jackProgram,"r+") as f:
    for line in f:
        x=line.split('//')
        x=x[0].split('/*')
        codeLine=x[0].strip()
        if len(codeLine) != 0:
            if (codeLine.startswith('/**')):
                multiComment=True
                if multiComment and codeLine.endswith("*/"):
                    multiComment=False
                    continue;
            if codeLine.endswith('*/'):
                multiComment=False
                continue
            lineList.append(codeLine)

with open (tokenizedOutxml,'w') as f :
    f.write("<tokens>\n")
    for line in lineList:
        localTknlist=line.split(' ')
        lenthLocalTknList=len(localTknlist)
        for token in localTknlist:
            print(token)

            if token in tokenClassDict:
                f.write('<' + tokenClassDict[token] + "> " + token + " </" + tokenClassDict[token]+'>\n')
            elif token.isnumeric():
                number=int(token)
                if number >= 0 and number <= 32767:
                    f.write('<' + 'integerConstant' + "> " + token + " </" +'integerConstant'  +'>\n')
                else:
                    print ("Out of bound number used")
                    exit(-1)
            elif '"' in token:
                firstDoubleQuotePosition = token.find('"')
                secondDoubleQuotePosition = token.rfind('"')
                stringConstant = token[firstDoubleQuotePosition +
                                       1: secondDoubleQuotePosition]
                f.write('<' + 'stringConstant' + "> " +
                        stringConstant + " </" + 'stringConstant' + '>\n')
            elif '=' in token:
                equalPosition = token.find('=')
                idVar = token[:equalPosition]
                f.write('<' + 'identifier' + "> " +
                        idVar + " </" + 'identifier' + '>\n')
                f.write('<' + tokenClassDict[token[equalPosition]] + "> " +
                        token[equalPosition] + " </" + tokenClassDict[token[equalPosition]] + '>\n')
                if token[equalPosition+1] == ' ':
                    equalPosition += 1
            elif (token.find('.') != -1):
                print(token)
                dotPosition = token.find('.')
                semiColonPosition = token.find(';')
                openBraceketPosition = token.find('(')
                closeBraceketPosition = token.find(')')
                identifier1 = token[:dotPosition]
                identifier2=""
                if (openBraceketPosition != -1):
                    identifier2 = token[dotPosition+1:openBraceketPosition].strip()



                f.write('<' + 'identifier' + "> " +
                        identifier1 + " </" + 'identifier' + '>\n')
                f.write('<' + tokenClassDict[token[dotPosition]] + "> " +
                        token[dotPosition] + " </" + tokenClassDict[token[dotPosition]] + '>\n')
                f.write('<' + 'identifier' + "> " +
                        identifier2 + " </" + 'identifier' + '>\n')
                f.write('<' + tokenClassDict[token[semiColonPosition]] + "> " +
                        token[semiColonPosition] + " </" + tokenClassDict[token[semiColonPosition]] + '>\n')

            elif ';' in token:
                semiColonPosition = token.find(';')
                idVar = token[:semiColonPosition]
                f.write('<' + 'identifier' + "> " +
                        idVar + " </" + 'identifier' + '>\n')
                f.write('<' + tokenClassDict[token[semiColonPosition]] + "> " +
                        token[semiColonPosition] + " </" + tokenClassDict[token[semiColonPosition]] + '>\n')

            elif '(' in token:
                # print(token)
                leftBracketPosition = token.find('(')
                rightBracketPosition = token.find(')')
                idVar = token[:leftBracketPosition]
                f.write('<' + 'identifier' + "> " +
                        idVar + " </" + 'identifier' + '>\n')
                f.write('<' + tokenClassDict[token[leftBracketPosition]] + "> " +
                        token[leftBracketPosition] + " </" + tokenClassDict[token[leftBracketPosition]] + '>\n')

                f.write('<' + tokenClassDict[token[rightBracketPosition]] + "> " +
                        token[rightBracketPosition] + " </" + tokenClassDict[token[rightBracketPosition]] + '>\n')

            elif not token in tokenClassDict:
                if token != '':
                    f.write('<' + 'identifier' + "> " +
                            token + " </" + 'identifier' + '>\n')


