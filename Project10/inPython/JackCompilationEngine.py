import JackTokenizer as JT


class JackCompilationEngine:

    def __init__(self, tokens, output_file):
        self.tokens = tokens
        self.output_file = output_file
        self.tokenLength = len(self.tokens)
        self.completeTokens = []
        self.cur_loc = 0
        self.classVarDec = ['static', 'field']
        self.subroutineDec = ['constructor', 'function', 'method']
        self.statements = ['let', 'if', 'while', 'do', 'return']
        self.binary_op = ['+', '-', '*', '/', '&', '|', '<', '>', '=']
        self.unary_op = ['-', '~']

    def run(self):
        self.completeTokens = []
        self.cur_loc = 0
        self.CompileClass()

    def nextToken(self):
        if self.cur_loc >= self.tokenLength:
            return None
        nextt = self.tokens[self.cur_loc]
        self.cur_loc += 1
        return nextt

    def peekToken(self):
        if self.cur_loc >= self.tokenLength:
            return None
        nextt = self.tokens[self.cur_loc]
        return nextt


    def CompileClass(self):
        self.completeTokens.append(
            JT.NonTerminalToken('class', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        while self.peekToken().value in self.classVarDec:
            self.CompileClassVarDec()
        while self.peekToken().value in self.subroutineDec:
            self.CompileSubroutine()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(
            JT.NonTerminalToken('class', False))

    def CompileClassVarDec(self):
        self.completeTokens.append(JT.NonTerminalToken('classVarDec', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        while self.peekToken().value == ',':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('classVarDec', False))

    def CompileSubroutine(self):
        self.completeTokens.append(JT.NonTerminalToken('subroutineDec', True))

        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileParameterList()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('subroutineBody', True))
        self.completeTokens.append(self.nextToken())
        while self.peekToken().value == 'var':
            self.CompileVarDec()
        self.CompileStatements()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(
            JT.NonTerminalToken('subroutineBody', False))
        self.completeTokens.append(JT.NonTerminalToken('subroutineDec', False))

    def CompileParameterList(self):
        self.completeTokens.append(JT.NonTerminalToken('parameterList', True))
        if self.peekToken().value != ')':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
        while self.peekToken().value == ',':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('parameterList', False))

    def CompileVarDec(self):
        self.completeTokens.append(JT.NonTerminalToken('varDec', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        while self.peekToken().value == ',':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('varDec', False))

    def CompileStatements(self):
        self.completeTokens.append(JT.NonTerminalToken('statements', True))
        while self.peekToken().value in self.statements:
            v = self.peekToken().value
            if v == 'if':
                self.CompileIf()
            elif v == 'let':
                self.CompileLet()
            elif v == 'while':
                self.CompileWhile()
            elif v == 'do':
                self.CompileDo()
            else:
                self.CompileReturn()
        self.completeTokens.append(JT.NonTerminalToken('statements', False))

    def CompileDo(self):
        self.completeTokens.append(JT.NonTerminalToken('doStatement', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        if self.peekToken().value == '(':
            self.completeTokens.append(self.nextToken())
            self.CompileExpressionList()
            self.completeTokens.append(self.nextToken())
        elif self.peekToken().value == '.':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
            self.CompileExpressionList()
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('doStatement', False))

    def CompileLet(self):
        self.completeTokens.append(JT.NonTerminalToken('letStatement', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        if self.peekToken().value == '[':
            self.completeTokens.append(self.nextToken())
            self.CompileExpression()
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileExpression()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('letStatement', False))

    def CompileWhile(self):
        self.completeTokens.append(JT.NonTerminalToken('whileStatement', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileExpression()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileStatements()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(
            JT.NonTerminalToken('whileStatement', False))

    def CompileReturn(self):
        self.completeTokens.append(
            JT.NonTerminalToken('returnStatement', True))
        self.completeTokens.append(self.nextToken())
        if self.peekToken().value != ';':
            self.CompileExpression()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(
            JT.NonTerminalToken('returnStatement', False))

    def CompileIf(self):
        self.completeTokens.append(JT.NonTerminalToken('ifStatement', True))
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileExpression()
        self.completeTokens.append(self.nextToken())
        self.completeTokens.append(self.nextToken())
        self.CompileStatements()
        self.completeTokens.append(self.nextToken())
        if self.peekToken().value == 'else':
            self.completeTokens.append(self.nextToken())
            self.completeTokens.append(self.nextToken())
            self.CompileStatements()
            self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('ifStatement', False))

    def CompileExpression(self):
        self.completeTokens.append(JT.NonTerminalToken('expression', True))
        self.CompileTerm()
        while self.peekToken().value in self.binary_op:
            self.completeTokens.append(self.nextToken())
            self.CompileTerm()
        self.completeTokens.append(JT.NonTerminalToken('expression', False))

    def CompileTerm(self):
        self.completeTokens.append(JT.NonTerminalToken('term', True))
        if self.peekToken().value == '(':
            self.completeTokens.append(self.nextToken())
            self.CompileExpression()
            self.completeTokens.append(self.nextToken())
        elif self.peekToken().value in self.unary_op:
            self.completeTokens.append(self.nextToken())
            self.CompileTerm()
        else:
            self.completeTokens.append(self.nextToken())
            if self.peekToken().value == '[':
                self.completeTokens.append(self.nextToken())
                self.CompileExpression()
                self.completeTokens.append(self.nextToken())
            elif self.peekToken().value == '(':
                self.completeTokens.append(self.nextToken())
                self.CompileExpressionList()
                self.completeTokens.append(self.nextToken())
            elif self.peekToken().value == '.':
                self.completeTokens.append(self.nextToken())
                self.completeTokens.append(self.nextToken())
                self.completeTokens.append(self.nextToken())
                self.CompileExpressionList()
                self.completeTokens.append(self.nextToken())
        self.completeTokens.append(JT.NonTerminalToken('term', False))

    def CompileExpressionList(self):
        self.completeTokens.append(JT.NonTerminalToken('expressionList', True))
        if self.peekToken().value != ')':
            self.CompileExpression()
            while self.peekToken().value == ',':
                self.completeTokens.append(self.nextToken())
                self.CompileExpression()
        self.completeTokens.append(
            JT.NonTerminalToken('expressionList', False))

    def save_file(self):
        with open(self.output_file, 'w') as file:
            for x in self.completeTokens:
                file.write(x.form + '\n')
