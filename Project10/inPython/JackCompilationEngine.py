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
        self.op = ['+', '-', '*', '/', '&', '|', '<', '>', '=']
        self.unary_op = ['-', '~']

    def run(self):
        self.completeTokens = []
        self.cur_loc = 0
        self.CompileClass()

    def next(self):
        if self.cur_loc >= self.tokenLength:
            return None
        nextt = self.tokens[self.cur_loc]
        self.cur_loc += 1
        return nextt

    def peek(self):
        if self.cur_loc >= self.tokenLength:
            return None
        nextt = self.tokens[self.cur_loc]
        return nextt

    def add(self, token):
        self.completeTokens.append(token)

    def CompileClass(self):
        self.add(JT.NonTerminalToken('class', True))  # <class>
        self.add(self.next())  # class
        self.add(self.next())  # className
        self.add(self.next())  # {
        while self.peek().value in self.classVarDec:
            self.CompileClassVarDec()
        while self.peek().value in self.subroutineDec:
            self.CompileSubroutine()
        self.add(self.next())  # }
        self.add(JT.NonTerminalToken('class', False))  # </class>

    def CompileClassVarDec(self):
        self.add(JT.NonTerminalToken('classVarDec', True))
        self.add(self.next())  # (static | field)
        self.add(self.next())  # type
        self.add(self.next())  # varName
        while self.peek().value == ',':
            self.add(self.next())  # ,
            self.add(self.next())  # varName
        self.add(self.next())  # ;
        self.add(JT.NonTerminalToken('classVarDec', False))

    def CompileSubroutine(self):
        self.add(JT.NonTerminalToken('subroutineDec', True))
        self.add(self.next())  # (constructor | function | method)
        self.add(self.next())  # (void | type)
        self.add(self.next())  # subroutineName
        self.add(self.next())  # '('
        self.CompileParameterList()
        self.add(self.next())  # ')'
        self.add(JT.NonTerminalToken('subroutineBody', True))
        self.add(self.next())  # '{'
        while self.peek().value == 'var':
            self.CompileVarDec()
        self.CompileStatements()
        self.add(self.next())  # '}'
        self.add(JT.NonTerminalToken('subroutineBody', False))
        self.add(JT.NonTerminalToken('subroutineDec', False))

    def CompileParameterList(self):
        self.add(JT.NonTerminalToken('parameterList', True))
        if self.peek().value != ')':
            self.add(self.next())  # type
            self.add(self.next())  # varName
        while self.peek().value == ',':
            self.add(self.next())  # ,
            self.add(self.next())  # type
            self.add(self.next())  # varName
        self.add(JT.NonTerminalToken('parameterList', False))

    def CompileVarDec(self):
        self.add(JT.NonTerminalToken('varDec', True))
        self.add(self.next())  # var
        self.add(self.next())  # type
        self.add(self.next())  # varName
        while self.peek().value == ',':
            self.add(self.next())  # ,
            self.add(self.next())  # varName
        self.add(self.next())  # ;
        self.add(JT.NonTerminalToken('varDec', False))

    def CompileStatements(self):
        self.add(JT.NonTerminalToken('statements', True))
        while self.peek().value in self.statements:
            v = self.peek().value
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
        self.add(self.next())  # do
        self.add(self.next())  # subroutineCall
        if self.peek().value == '(':
            self.add(self.next())  # '('
            self.CompileExpressionList()
            self.add(self.next())  # ')'
        elif self.peek().value == '.':
            self.add(self.next())  # '.'
            self.add(self.next())  # subroutineName
            self.add(self.next())  # (
            self.CompileExpressionList()
            self.add(self.next())  # )
        self.add(self.next())  # ;
        self.add(JT.NonTerminalToken('doStatement', False))

    def CompileLet(self):
        self.add(JT.NonTerminalToken('letStatement', True))
        self.add(self.next())  # let
        self.add(self.next())  # varName
        if self.peek().value == '[':
            self.add(self.next())  # '['
            self.CompileExpression()
            self.add(self.next())  # ']'
        self.add(self.next())  # =
        self.CompileExpression()
        self.add(self.next())  # ;
        self.add(JT.NonTerminalToken('letStatement', False))

    def CompileWhile(self):
        self.add(JT.NonTerminalToken('whileStatement', True))
        self.add(self.next())  # while
        self.add(self.next())  # (
        self.CompileExpression()
        self.add(self.next())  # )
        self.add(self.next())  # {
        self.CompileStatements()
        self.add(self.next())  # }
        self.add(JT.NonTerminalToken('whileStatement', False))

    def CompileReturn(self):
        self.add(JT.NonTerminalToken('returnStatement', True))
        self.add(self.next())  # return
        if self.peek().value != ';':
            self.CompileExpression()
        self.add(self.next())  # ;
        self.add(JT.NonTerminalToken('returnStatement', False))

    def CompileIf(self):
        self.add(JT.NonTerminalToken('ifStatement', True))
        self.add(self.next())  # if
        self.add(self.next())  # (
        self.CompileExpression()
        self.add(self.next())  # )
        self.add(self.next())  # {
        self.CompileStatements()
        self.add(self.next())  # }
        if self.peek().value == 'else':
            self.add(self.next())  # else
            self.add(self.next())  # {
            self.CompileStatements()
            self.add(self.next())  # }
        self.add(JT.NonTerminalToken('ifStatement', False))

    def CompileExpression(self):
        self.add(JT.NonTerminalToken('expression', True))
        self.CompileTerm()
        while self.peek().value in self.op:
            self.add(self.next())  # op
            self.CompileTerm()
        self.add(JT.NonTerminalToken('expression', False))

    def CompileTerm(self):
        self.add(JT.NonTerminalToken('term', True))
        if self.peek().value == '(':
            self.add(self.next())  # (
            self.CompileExpression()
            self.add(self.next())  # )
        elif self.peek().value in self.unary_op:
            self.add(self.next())  # - or ~
            self.CompileTerm()
        else:
            self.add(self.next())
            if self.peek().value == '[':
                self.add(self.next())  # '['
                self.CompileExpression()
                self.add(self.next())  # ']'
            elif self.peek().value == '(':
                self.add(self.next())  # '('
                self.CompileExpressionList()
                self.add(self.next())  # ')'
            elif self.peek().value == '.':
                self.add(self.next())  # '.'
                self.add(self.next())  # subroutineName
                self.add(self.next())  # (
                self.CompileExpressionList()
                self.add(self.next())  # )
        self.add(JT.NonTerminalToken('term', False))

    def CompileExpressionList(self):
        self.add(JT.NonTerminalToken('expressionList', True))
        if self.peek().value != ')':
            self.CompileExpression()
            while self.peek().value == ',':
                self.add(self.next())  # ,
                self.CompileExpression()
        self.add(JT.NonTerminalToken('expressionList', False))

    def save_file(self):
        with open(self.output_file, 'w') as file:
            for x in self.completeTokens:
                file.write(x.form + '\n')
