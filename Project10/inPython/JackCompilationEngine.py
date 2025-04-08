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

    def add(self, token):
        self.completeTokens.append(token)

    def CompileClass(self):
        self.add(JT.NonTerminalToken('class', True))  # <class>
        self.add(self.nextToken())  # class
        self.add(self.nextToken())  # className
        self.add(self.nextToken())  # {
        while self.peekToken().value in self.classVarDec:
            self.CompileClassVarDec()
        while self.peekToken().value in self.subroutineDec:
            self.CompileSubroutine()
        self.add(self.nextToken())  # }
        self.add(JT.NonTerminalToken('class', False))  # </class>

    def CompileClassVarDec(self):
        self.add(JT.NonTerminalToken('classVarDec', True))
        self.add(self.nextToken())  # (static | field)
        self.add(self.nextToken())  # type
        self.add(self.nextToken())  # varName
        while self.peekToken().value == ',':
            self.add(self.nextToken())  # ,
            self.add(self.nextToken())  # varName
        self.add(self.nextToken())  # ;
        self.add(JT.NonTerminalToken('classVarDec', False))

    def CompileSubroutine(self):
        self.add(JT.NonTerminalToken('subroutineDec', True))
        self.add(self.nextToken())  # (constructor | function | method)
        self.add(self.nextToken())  # (void | type)
        self.add(self.nextToken())  # subroutineName
        self.add(self.nextToken())  # '('
        self.CompileParameterList()
        self.add(self.nextToken())  # ')'
        self.add(JT.NonTerminalToken('subroutineBody', True))
        self.add(self.nextToken())  # '{'
        while self.peekToken().value == 'var':
            self.CompileVarDec()
        self.CompileStatements()
        self.add(self.nextToken())  # '}'
        self.add(JT.NonTerminalToken('subroutineBody', False))
        self.add(JT.NonTerminalToken('subroutineDec', False))

    def CompileParameterList(self):
        self.add(JT.NonTerminalToken('parameterList', True))
        if self.peekToken().value != ')':
            self.add(self.nextToken())  # type
            self.add(self.nextToken())  # varName
        while self.peekToken().value == ',':
            self.add(self.nextToken())  # ,
            self.add(self.nextToken())  # type
            self.add(self.nextToken())  # varName
        self.add(JT.NonTerminalToken('parameterList', False))

    def CompileVarDec(self):
        self.add(JT.NonTerminalToken('varDec', True))
        self.add(self.nextToken())  # var
        self.add(self.nextToken())  # type
        self.add(self.nextToken())  # varName
        while self.peekToken().value == ',':
            self.add(self.nextToken())  # ,
            self.add(self.nextToken())  # varName
        self.add(self.nextToken())  # ;
        self.add(JT.NonTerminalToken('varDec', False))

    def CompileStatements(self):
        self.add(JT.NonTerminalToken('statements', True))
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
        self.add(JT.NonTerminalToken('statements', False))

    def CompileDo(self):
        self.add(JT.NonTerminalToken('doStatement', True))
        self.add(self.nextToken())  # do
        self.add(self.nextToken())  # subroutineCall
        if self.peekToken().value == '(':
            self.add(self.nextToken())  # '('
            self.CompileExpressionList()
            self.add(self.nextToken())  # ')'
        elif self.peekToken().value == '.':
            self.add(self.nextToken())  # '.'
            self.add(self.nextToken())  # subroutineName
            self.add(self.nextToken())  # (
            self.CompileExpressionList()
            self.add(self.nextToken())  # )
        self.add(self.nextToken())  # ;
        self.add(JT.NonTerminalToken('doStatement', False))

    def CompileLet(self):
        self.add(JT.NonTerminalToken('letStatement', True))
        self.add(self.nextToken())  # let
        self.add(self.nextToken())  # varName
        if self.peekToken().value == '[':
            self.add(self.nextToken())  # '['
            self.CompileExpression()
            self.add(self.nextToken())  # ']'
        self.add(self.nextToken())  # =
        self.CompileExpression()
        self.add(self.nextToken())  # ;
        self.add(JT.NonTerminalToken('letStatement', False))

    def CompileWhile(self):
        self.add(JT.NonTerminalToken('whileStatement', True))
        self.add(self.nextToken())  # while
        self.add(self.nextToken())  # (
        self.CompileExpression()
        self.add(self.nextToken())  # )
        self.add(self.nextToken())  # {
        self.CompileStatements()
        self.add(self.nextToken())  # }
        self.add(JT.NonTerminalToken('whileStatement', False))

    def CompileReturn(self):
        self.add(JT.NonTerminalToken('returnStatement', True))
        self.add(self.nextToken())  # return
        if self.peekToken().value != ';':
            self.CompileExpression()
        self.add(self.nextToken())  # ;
        self.add(JT.NonTerminalToken('returnStatement', False))

    def CompileIf(self):
        self.add(JT.NonTerminalToken('ifStatement', True))
        self.add(self.nextToken())  # if
        self.add(self.nextToken())  # (
        self.CompileExpression()
        self.add(self.nextToken())  # )
        self.add(self.nextToken())  # {
        self.CompileStatements()
        self.add(self.nextToken())  # }
        if self.peekToken().value == 'else':
            self.add(self.nextToken())  # else
            self.add(self.nextToken())  # {
            self.CompileStatements()
            self.add(self.nextToken())  # }
        self.add(JT.NonTerminalToken('ifStatement', False))

    def CompileExpression(self):
        self.add(JT.NonTerminalToken('expression', True))
        self.CompileTerm()
        while self.peekToken().value in self.binary_op:
            self.add(self.nextToken())  # op
            self.CompileTerm()
        self.add(JT.NonTerminalToken('expression', False))

    def CompileTerm(self):
        self.add(JT.NonTerminalToken('term', True))
        if self.peekToken().value == '(':
            self.add(self.nextToken())  # (
            self.CompileExpression()
            self.add(self.nextToken())  # )
        elif self.peekToken().value in self.unary_op:
            self.add(self.nextToken())  # - or ~
            self.CompileTerm()
        else:
            self.add(self.nextToken())
            if self.peekToken().value == '[':
                self.add(self.nextToken())  # '['
                self.CompileExpression()
                self.add(self.nextToken())  # ']'
            elif self.peekToken().value == '(':
                self.add(self.nextToken())  # '('
                self.CompileExpressionList()
                self.add(self.nextToken())  # ')'
            elif self.peekToken().value == '.':
                self.add(self.nextToken())  # '.'
                self.add(self.nextToken())  # subroutineName
                self.add(self.nextToken())  # (
                self.CompileExpressionList()
                self.add(self.nextToken())  # )
        self.add(JT.NonTerminalToken('term', False))

    def CompileExpressionList(self):
        self.add(JT.NonTerminalToken('expressionList', True))
        if self.peekToken().value != ')':
            self.CompileExpression()
            while self.peekToken().value == ',':
                self.add(self.nextToken())  # ,
                self.CompileExpression()
        self.add(JT.NonTerminalToken('expressionList', False))

    def save_file(self):
        with open(self.output_file, 'w') as file:
            for x in self.completeTokens:
                file.write(x.form + '\n')
