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
    f.write("<token>\n");
    for line in lineList:
        localTknlist=line.split(' ')
        lenthLocalTknList=len(localTknlist)
        for token in localTknlist:
            if token in tokenClassDict:
                f.write('<' + tokenClassDict[token] + "> " + token + " </" + tokenClassDict[token]+'>\n')
            elif token.isnumeric():
                number=int(token)
                if number >= 0 and number <= 32767:
                    f.write('<' + 'integerConstant' + "> " + token + " </" +'integerConstant'  +'>\n')
                else:
                    print ("Out of bound number used")
                    exit(-1)

